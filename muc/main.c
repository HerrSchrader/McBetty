/*
    main.c - main program routines
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
#include "global.h"
#include "lpc2220.h"
#include "kernel.h"
#include "lcd.h"
#include "irq.h"
#include "keyboard.h"
#include "fonty.h"
#include "timerirq.h"
#include "serial.h"
#include "pwmirq.h"
#include "rf.h"
#include "cc1100.h"
#include "isr.h"
#include "mpd.h"
#include "window.h"
#include "screen.h"


/* ---------- Global Variables ------------- */

void setSpeed(unsigned char sp)
{
	ARM_INT_KEY_TYPE int_lock_key;
	ARM_INT_LOCK(int_lock_key);	
	switch(sp)
	{
		case SPEED_60:		// turbo speed (60 mhz)
			BCFG0	= 0x100005EF;
			PLLCFG	= 0x45;		// 0100.0101	= PSEL=10=4	MSEL=00101=6 = 240mhz Fcco
			PLLFEED = 0xAA;
			PLLFEED = 0x55;

			VPBDIV = 0x00;

			BCFG0	= 0x100004A0;
			BCFG2	= 0x100004A0;
			BCFG1	= 0x00000C21; // 2 idle cycles, 5 cycles per read, 8 bit wide = LCD controller
			break;

		case SPEED_30:	// normal speed (30 mhz)
		default:
			BCFG0	= 0x100005EF;
			PLLCFG	= 0x42;		// 0100.0010	= PSEL=10=4	MSEL=00010=3
			PLLFEED = 0xAA;
			PLLFEED = 0x55;

			VPBDIV = 0x02;

			BCFG0	= 0x10000420;
			BCFG2	= 0x10000420;
			BCFG1	= 0x00000400;	// 1 idle cycle, 3 cycles per read, 8 bit wide = LCD controller
			break;
	};
	ARM_INT_UNLOCK(int_lock_key);
}


int 
main(void){
	
	SCS |= 0x01;
	
	FIODIR0 = 0;	/* Just to make sure that all pins on Port0 are input. */
	
	/* The following pins are output: */
	FIODIR0 |= ((1<<21) | BACKLIGHT_PWM_PIN | SOUND_ENABLE_PIN | (1<<6));
	
	/* TODO what is this pin for ? */
	FIODIR0 |= (1<<12);
	FIOSET0 = (1<<12);

	setSpeed(SPEED_30);
		
	rand_seed(13); 				// initialize the random number generator
	
	/* Initialize the kernel. Kernel is not running yet */
	kernel_init();

	/* Initialize the display */
	lcd_init(0);
	
	/* Initialize the serial port */
	serial_init(38400);
	task_add(&serial_out);					// Task 0
	
	/* Enable IRQs. Now IRQ service routines start to run */
	enableIRQ();
	
	/* Enable radio communication */
	RF_init();								// Task 1 == produce_send_token)

	/* Initialize backlight task */
	inactivity_cnt = 0;		// consider power on to be a user activity
	task_add(&backl_proc);					// Task 2
	startPWMIRQ();

	/* Initialize keyboard handling */
	key_init();								// Task 3 == key_scan()

	/* Build an initial model of the internal and external state of the world */
	model_init();							// Task 4 == update_playtime()
	
	/* Show something to the user depending on model */
	mainscreen_init();						// Task 5 == win_scroll_all()
	
	dbg("RESET");
	
	/* Start the controller task 
		- keeps model up to date
		- starts actions according to model
		- shows relevant information on the screen
	*/
	task_add(&controller);					// Task 6
											// Task 7 == assemble_line()
	
	/* Start the kernel scheduler */
	while(1){
		schedule();
	};

	return 0;
}


