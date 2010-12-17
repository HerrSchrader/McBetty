/*
    soundcore.c - sound core routines (output, ipol, mixing, ...)
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

/* Initializes and serves the PWM IRQ. This IRQ is used for events which are too fast for normal tasks.
	The backlight task influences the working of the PWM IRQ.
*/

#include "lpc2220.h"
#include "global.h"
#include "pwmirq.h"
#include "timerirq.h"
#include "irq.h"
#include "kernel.h"

#define PWMMR0I (1<<0)
#define PWMMR0R (1<<1)
#define PWMMR0S (1<<2)

#define PWMM0IRQ (1<<0)
#define PWMM1IRQ (1<<1)
#define PWMM2IRQ (1<<2)
#define PWMM3IRQ (1<<3)
#define PWMM4IRQ (1<<8)
#define PWMM5IRQ (1<<9)
#define PWMM6IRQ (1<<10)


#define PWMENA2 (1<<10)


/* This value is changed by the PWMirq routine */
static volatile uint8 bl_cnt;

/* The current backlight brightness (0..63) */
static uint8 bl_brightness;

/* The current backlight brightness multiplied by 16 to give 4 digits after the comma  (0 .. 63 * 16) */
static int bl_bright_16;

/* Inactivity counter 
	Necessary for automatic dimming of backlight. 
	Can potentially be changed by many different sources.
*/
unsigned int inactivity_cnt;


void __attribute__ ((section(".text.fastcode"))) pwmIRQ (void);
void __attribute__ ((section(".text.fastcode"))) pwmM0IRQ (void);
void __attribute__ ((section(".text.fastcode"))) pwmM2IRQ (void);

/*
	The PWM IRQ is used for fast actions that cannot be handled through normal tasks 
*/

void 
startPWMIRQ(void)
{
	VICIntEnClr = INT_PWM;		// Disable PWM interrupt, just to make sure
	
	bl_brightness = 0;
	bl_bright_16 = bl_brightness * 16;
	bl_cnt = 0;	
	
	PWMTC = 0;
	
	PWMPR = 7;			/* Prescale register: PWM timer is incremented every 8 PCLK cycles */
					// => PWM clk freqency = 15 MHz / 8 = 1.875 MHz
	
	PWMMR0 = 0x1E6;			// pwm rate (=486)  => Reset Rate of 3.858 kHz
	
	PWMMR2 = 0x00;			// pwm value == always off
	
	PWMLER = 0x05;			// Enable PWM Match 0 and Match 2 latch 
	PWMMCR = PWMMR0I | PWMMR0R;	// Generate interrupt on match0 and reset to 0 
	PWMPCR = PWMENA2; 		// Enable PWM2 output

	PWMTCR = 0x03;		// Counter enable, counter reset
	PWMTCR = 0x09;		// Counter enable, reset disabled, PWM enabled
	
	/* Use this instead if you want FIQ for this interrupt */
//	VICIntSelect |= INT_PWM;
	VICIntSelect &= ~INT_PWM;
	VICVectAddr7 = (unsigned long)&(pwmIRQ);
	VICVectCntl7 = VIC_SLOT_EN | INT_SRC_PWM;
	VICIntEnable = INT_PWM;
}


void __attribute__ ((section(".text.fastcode"))) 
pwmIRQ (void)
{
	while (PWMIR & (PWMM0IRQ | PWMM2IRQ) ){
		
		if (PWMIR & PWMM0IRQ)
			pwmM0IRQ();
		if (PWMIR & PWMM2IRQ)
			pwmM2IRQ();
	};
};


/* P0.4 is used as a general purpose I/O pin to control the backlight level
	If it is 0, the backlight is off, if it is 1, the backlight is on.
	So by generating a PWM on this pin, we can dim the backlight to any value we want
	if we change the duty cycle of the PWM.
*/ 

/* This routine is called ca. 3800 times per second.
	Each time we hit this routine we increment the counter bl_count.
	If bl_count is above a certain threshold (bl_val/4), the backlight pin will be set,
	else it will be cleared.
	bl_count is reset to 0 when it reaches 64
	So counter bl_count rolls over 3800/64 = 60 times a second.
	We get a flicker frequency for the backlight of 60 Hz, which is very good.
*/
void __attribute__ ((section(".text.fastcode"))) 
pwmM0IRQ (void){
	PWMIR = PWMM0IRQ;	
	if (bl_cnt >= bl_brightness){
		FIOSET0	= BACKLIGHT_PWM_PIN;			
	}
	else { 
		FIOCLR0	= BACKLIGHT_PWM_PIN;
	};
	
	bl_cnt = (bl_cnt + 1) & 0x3f;	/* roll over from 63 to 0 */
};

void __attribute__ ((section(".text.fastcode"))) 
pwmM2IRQ (void){
	PWMIR = PWMM2IRQ;
};


/* This is the auto-off backlight task 
	The backlight is switched off after a period of inactivity.
	We keep a global inactivity counter, which is reset to 0 by any user activity
	This task increases the inactivity counter and switches the backlight off, if it reaches a certain threshold
	We do not switch off the backlight immediately, but dim it slowly to 0
	Likewise we turn it on fast again if activity occured
*/

/* Backlight task.
	Periodically checks if there was some kind of user activity.
	If user activity was detected, the backlight is raised to maximum value.
	If no user activity occured for some time, the backlight is slowly switched off.
*/
/* ticks of inactivity, after which backlight is switched off */
#define max_inactivity  (5 * TICKS_PER_SEC)

/* Normal brigntness */
#define BL_NORM_BRIGHT	48


PT_THREAD (backl_proc(struct pt *pt)) {
	static struct timer tmr;
	
	PT_BEGIN(pt);
	timer_add(&tmr, 1*TICKS_PER_TENTH_SEC, 1*TICKS_PER_TENTH_SEC);
	
	do {
		PT_WAIT_UNTIL(pt, timer_expired(&tmr));
		tmr.expired = 0;
			
		inactivity_cnt++;
		if (inactivity_cnt >= max_inactivity){
			inactivity_cnt = max_inactivity;
			if (bl_bright_16 >= 1) 
				bl_bright_16 -= 1;
		} else {
			if (bl_bright_16 < BL_NORM_BRIGHT * 16) 
				bl_bright_16 += 40;		
		};
		bl_brightness = bl_bright_16 >> 4;
	} while (1);	
	PT_END(pt);
};	


