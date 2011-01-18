/*
    keyboard.c - button handling
    Copyright (C) 2007  Ch. Klippel <ck@mamalala.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "lpc2220.h"
#include "global.h"
#include "kernel.h"
#include "timerirq.h"
#include "irq.h"
#include "screen.h"
#include "keyboard.h"

/* See keyboard.h for a description of the keycodes which correspond to a given key.
	We have excactly 7 rows with 6 columns of keys = 42 keys.
	Each key has a unique code between 0 and 41 with code = column + row * 6
	The symbolic names (and by that the layout on the physical keyboard) are given in keyboard.h

The pins associated with keyboard:
*  - p0.30 : interrupt!
*  - p0.28, p0.27, p3.21, p3.20, p0.22, p0.13 : the columns (6 columns)
*  - p2.18-p2.24 : the rows (7 rows), active low !
*/

/* in scan cycles */
#define DEBOUNCE_PERIOD 2

int last_key;

/* The key currently pressed (-1 means no key pressed)*/
int cur_key;
/* Time when the key was pressed (or time of release if cur_key = -1) */
static unsigned int key_time;

/* How long do we wait between successive scans of the keyboard ? */
#define SCAN_PERIOD	(5 * TICKS_PER_HUNDR_SEC)

/* Initial delay after which auto repeat is turned on (in scan cycles) */
#define RPT_DELAY 10
/* Auto repeat rate in scan cycles (for example 6 means every 6 scan cycles the key is repeated) */
#define RPT_RATE 3

static unsigned int rpt_delay_cnt, rpt_rate_cnt;

/* Port0.30 is the keyboard interrupt pin */
#define KEY_INT	(FIOPIN0 & (1<<30))

/* DRIVE_ROW sets pin corresponding to row no. r as output ( 0 <= r <= 6) */
#define DRIVE_ROW(r) IODIR2|=(1<<(18+(r)))

/* TRIS_ROW sets ping corresponding to row no. r as input ( 0 <= r <= 6) */
#define TRIS_ROW(r) IODIR2&=~(1<<(18+(r)))

#define ROW_MASK ((1<<18)|(1<<19)|(1<<20)|(1<<21)|(1<<22)|(1<<23)|(1<<24))

/* Tristate all rows (all are input) */
#define TRIS_ALL_ROWS() IODIR2&=~ROW_MASK

/*
	The buttons on Betty are configured so that any key press or release can generate an interrupt.
	We use this interrupt to determine when a key has been pressed.
	We then start our key scanning task as a time task.
	
	TODO maybe simpler to do a time task only.
*/

// TODO use interrupt !

#if 0
/* The keyboard ISR 

	We simply signal our internal task no. 29 
	// TODO currently not used!

*/
void  keyIRQ (void) {
	
	EXTINT = EINT3;			/* Clear the interrupt flag */
	signal_set (SIG_KEY_INT);
	//	VICSoftIntClr = INT_EINT3;	/* Clear the VIC interrupt flag */
};


void initKeys(void)
{
	IOCLR2=((1<<18)|(1<<19)|(1<<20)|(1<<21)|(1<<22)|(1<<23)|(1<<24));
	FIODIR0 &= ~(1<<30);			// Pin P0.30 is input
//	startCB(addTimerCB(keyScanner, 15));
		
//	EXTWAKE |= 0x08;
//	PINSEL1 |= (1<<29);	
}
#endif 

/* Initialize keyboard  routines 
	It seems that we do not need to do software debouncing of the keys.
	Maybe this is what vw 3 p9 does (retriggerable monoflop ?)
*/
void key_init (){
	VICIntEnClr = INT_EINT3;	// Disable EINT3 (just to make sure) 
/*	
	DRIVE_ALL_ROWS;
	
	PINSEL1 |= (1<<29);		// Choose EINT3 function for P0.30 	
	EXTWAKE |= EINT3;		// EINT3 wakes processor from power down 
	EXTMODE |= EINT3;		// EINT3 is edge-sensitive 
	EXTPOLAR &= ~EINT3;		// EINT3 is falling edge sensitive 
	EXTINT = EINT3;			// Clear any pending EINT3 
	
	VICIntSelect &= ~INT_EINT3;			// EINT3 is assigned to normal IRQ, not FIQ 
	VICVectAddr13 = (unsigned long)&(keyIRQ);	// Fairly low interrupt priority 13 chosen 
	VICVectCntl13 = VIC_SLOT_EN | INT_SRC_EINT3;	// Enable VIC vector 13 
	VICIntEnable = INT_EINT3;			// Enable EINT3 IRQ
*/
	/* TODO Just to make sure that the necessary pins are GPIO pins PINSEL0 ... */
	
	TRIS_ALL_ROWS();		// At the moment, all rows are input
	IOCLR2 |= ROW_MASK;		// If they are later activated as output, they will output a 0
	
	rpt_delay_cnt = 0; 
	rpt_rate_cnt = 0;
	cur_key = NO_KEY;
	last_key = NO_KEY;
	
	task_add(&key_scan);
};


/* Task to check for keyboard presses.
	This task is selftimed. It creates a timer, waits, checks for key presses and loops.
	Scan the keyboard and determine which key is pressed
	If more than 1 key is pressed, simply ignore it
	If a key is pressed, it is stored in global variable current_key.
	The time of the key press is stored in global variable key_time.
	The signal SIG_KEY_CHG is set if a new key is pressed.
	Long key presses lead to repeated events (auto-repeat)!
	
### Experimentally we implement some software debouncing here.
	A key change is only accepted if it lasts longer than DEBOUNCE_PERIOD
	
*/
PT_THREAD (key_scan(struct pt *pt)) {
	static struct timer tmr;
	static int row, row_offset, cnt;
	static unsigned int port0, port3;
	static unsigned int key;
		
	PT_BEGIN(pt);
	timer_add(&tmr, SCAN_PERIOD, SCAN_PERIOD);
	
	do {
		PT_WAIT_UNTIL(pt, timer_expired(&tmr));
		tmr.expired = 0;
			
		TRIS_ALL_ROWS();
	
		row_offset = 0;
		cnt = 0;		/* how many keys are pressed */
		key = NO_KEY;
		
		for(row=0;row<7;row++) {
		
			/* drive the row */
			DRIVE_ROW(row);
		
			/* scan the columns */
	
			port0=FIOPIN0;
			port3=IOPIN3;
			if(!(port0&(1<<28))) {
				key=row_offset+0;
				cnt++;
			}
	
			if(!(port0&(1<<27))) {
				key=row_offset+1;
				cnt++;	
			}
			if(!(port0&(1<<22))) {
				key=row_offset+2;
				cnt++;	
			}
			if(!(port0&(1<<13))) {
				key=row_offset+3;
				cnt++;	
			}
			if(!(port3&(1<<21))) {
				key=row_offset+4;
				cnt++;	
			}
			if(!(port3&(1<<20))) {
				key=row_offset+5;
				cnt++;	
			}

			TRIS_ROW(row);
			row_offset+=6;
		}	
	
		if ( (cnt == 0) || (cnt > 1)) {
			cur_key = -1;				// no key pressed (or multiple keys simultaneously)
			key_time = system_time();
			last_key = -1;
			rpt_delay_cnt = 0;
		} else {					// we have registered a single key press
			/* Check if we have seen the same key the last time */
			if (key == last_key)
				rpt_delay_cnt++;
			else {
				last_key = key;
				rpt_delay_cnt = 0;
			};
			
			/* If we already accepted this key, we check if it is time to do autorepeat */
			if (key == cur_key){
				/* Auto repeat is only turned on after RPT_DELAY has expired */ 
				if (rpt_delay_cnt < RPT_DELAY)
					rpt_delay_cnt++;
				else {
					/* Now we have expired RPT_DELAY_CNT, use RPT_RATE for autorepeat */
					if (rpt_rate_cnt < RPT_RATE)	// still not long enough
						rpt_rate_cnt++;
					else {
						rpt_rate_cnt = 0;
						signal_set(SIG_KEY_CHG);
					};
				};
			} else {
				/* We have a potentially new key. Count how long it is pressed */
				if (key == last_key)
					rpt_delay_cnt++;
				else {
					/* key is different than last time */
					rpt_delay_cnt = 0;
					last_key = key;
				};
				
				/* If the key was seen long enough, we accept it. */
				if (rpt_delay_cnt >= DEBOUNCE_PERIOD) {
					cur_key = key;
					key_time = system_time();
					signal_set(SIG_KEY_CHG);
					rpt_rate_cnt = 0;
				};
			};
		};
	} while (1);	
	PT_END(pt);	
};

/* Variable which holds the current function to receive key presses */
static void (*keypress_handler) (int cur_key) = NULL;

void 
set_keypress_handler (void (*new_handler) (int cur_key)){
	keypress_handler = new_handler;
};

extern int inactivity_cnt;

/* This will be signalled from keyboard scan routine when a key was pressed / released */
void key_change (){
	signal_clr(SIG_KEY_CHG);

	inactivity_cnt = 0;				/* A user activity has occurred */
	
	/* TODO This is only a test routine.
	  We should do it so that a listener must register with keyboard routines to be called (or signalled)
	  when a key press (or release) occurs. 
	*/
    if (NULL != keypress_handler) (*keypress_handler)(cur_key);
	
};

