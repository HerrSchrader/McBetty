/*
    adc.c - adc readout for battery status
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

#include "irq.h"
#include "timerfuncs.h"
#include "adc.h"
#include "adcirq.h"
#include "lcd.h"
#include "fonty.h"
#include "lpc2220.h"

extern volatile unsigned int bat_adc;
extern volatile unsigned int bmax, bmin;
extern unsigned int is_drawing;

static unsigned short bat_max, bat_min;
static unsigned short diffs[4];
static unsigned short diff;
volatile unsigned char bat_state;



void draw_b(unsigned char x, unsigned char y)
{	
	unsigned char w;
	
	if(!(is_drawing))
	{
		if(bat_state & BAT_DEBUG)
		{
			draw_block(4,0,108,24,3,DRAW_ERASE);
			draw_numU(4,0,bat_min>>2,0,3,DRAW_PUT);
			draw_numU(32,0,bat_max>>2,0,3,DRAW_PUT);
			draw_numU(4,8,(bat_max>>2)-(bat_min>>2),0,3,DRAW_PUT);
	
			draw_numU(60,0,bat_min,0,3,DRAW_PUT);
			draw_numU(88,0,bat_max,0,3,DRAW_PUT);
			draw_numU(60,8,bat_max-bat_min,0,3,DRAW_PUT);
			if((bat_state & BAT_NORMAL))
				draw_string(4,16,"Bat.Operation",3,DRAW_PUT);
			if((bat_state & BAT_CHARGE))
				draw_string(4,16,"Charging",3,DRAW_PUT);
			if((bat_state & BAT_CHARGE_DONE))
				draw_string(4,16,"Done Charging",3,DRAW_PUT);
		}

		if (bat_min<0x320)
			w = 0;
		else if (bat_min>0x380)
			w = 12;
		else
			w = (bat_min-0x320)/8;
		
		draw_block(x+1,y+1,12,5,3,DRAW_ERASE);
		draw_rect(x,y,14,7,1,3,DRAW_PUT);
		draw_vline(x+14,y+2,3,3,DRAW_PUT);

		if(w>0)	
			draw_block(x+1,y+1,w,5,2,DRAW_PUT);
	
		if(w<12)
			draw_block(x+1+w,y+1,12-w,5,0,DRAW_PUT);
	}
}

void calcBat(void)
{
	ADCR |= ((1<<21) | (1<<24));
//	ADCR  |= (1<<24);  /* Start A/D Conversion (START:0=1) */
	ADINTEN = 1<<5;
}

void showBat(void)
{
	static int cnt;
	unsigned char x;

	bat_max = bmax;
	bat_min = bmin;

	cnt &= 0x03;
	diffs[cnt++] = bat_max - bat_min;

	diff = 0;
	for(x=0;x<4;x++)
		diff += diffs[x];
	diff >>= 2;

	if(bat_state & BAT_NORMAL)
	{
		if(diff > 15)
		{
			bat_state |= BAT_CHARGE;
			bat_state &= ~BAT_NORMAL;
		}
	}
	else if(bat_state & BAT_CHARGE)
	{
		if(bat_max >= 1020)
		{
			bat_state |= BAT_CHARGE_DONE;
			bat_state &= ~BAT_CHARGE;
		}
	}
	else if(bat_state & BAT_CHARGE_DONE)
	{
		if(bat_max < 1000)
		{
			bat_state |= BAT_CHARGE;
			bat_state &= ~BAT_CHARGE_DONE;
		}
	}

	if((diff < 7) && (bat_min < 1000))
	{
		bat_state |= BAT_NORMAL;
		bat_state &= ~BAT_CHARGE;
		bat_state &= ~BAT_CHARGE_DONE;
	}

	draw_b(113,0);
	bmin = bat_max;
	bmax = bat_min;
}

void startADC(void)
{
	ADCR = ( 1<<5 | ((15-1)<<8) /*| (1UL<<16)*/ | (1UL<<21) );
	ADCR  |= (1UL<<24);  /* Start A/D Conversion (START:0=1) */
	while ((ADDR & (1UL<<31)) == 0); /* Wait for the conversion to complete (DONE=1)*/
	bmin = ((ADDR >> 6) & 0x03FF);	/* Extract the A/D result */
	ADCR &=~(1UL<<21);
	bmax = bmin;
	bat_max = bmin;
	bat_min = bmin;
	bat_state = BAT_NORMAL;
	
	diffs[0] = 0;
	diffs[0] = 1;
	diffs[0] = 2;
	diffs[0] = 3;

	VICVectAddr14 = (unsigned long)&(adcIRQ);
	VICVectCntl14 = VIC_SLOT_EN | INT_SRC_ADC;
	VICIntEnable = INT_ADC;

	startCB(addTimerCB(calcBat, 6));
	startCB(addTimerCB(showBat, 220));
}
