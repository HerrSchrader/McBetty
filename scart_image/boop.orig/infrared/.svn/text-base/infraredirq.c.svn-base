/*
    infraredirq.c - ir interrupt
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

#include "infrared.h"
#include "infraredirq.h"
#include "irq.h"
#include "lpc2220.h"
#include "keyboard.h"

static unsigned int c_cnt = 0;
static unsigned int b_len = 0;
extern volatile unsigned char mod_enable;
extern volatile unsigned char hi_border;
extern volatile unsigned char lo_border;
extern volatile unsigned int cycles;

extern ir_fn	irEncoder;

void __attribute__ ((section(".text.fastcode"))) irIRQ(void)
{
	c_cnt++;
	if(c_cnt <= hi_border)
	{
		FIOSET0 = (mod_enable<<21);
	}
	else
	{
		FIOCLR0 = (1<<21);
		if(c_cnt >= lo_border)
		{
			c_cnt = 0;
			b_len++;
			if(b_len >= cycles)
			{
				irEncoder();
				b_len = 0;
			}
		}
	}

	T1IR = 1;
//	VICVectAddr = 0;
}
