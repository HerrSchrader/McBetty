/*
    keyirq.c - keyboard scanning interrupt
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

#include "keyboard.h"
#include "keyirq.h"
#include "lpc2220.h"
#include "timerfuncs.h"

//volatile unsigned int keys[2];
unsigned int keys[2];

static unsigned char scannerTimeout;
static unsigned char repeatTimeout;
static unsigned char fastToggle;
static unsigned char scannerState = 0;
extern char sysInfo;

#define S_IDLE		0x00
#define S_PRESSED	0x01
#define S_HOLD		0x02
#define S_REPEAT	0x03

#define S_REPEATDELAY	6
#define S_REPEATRATE	4

#define MASK0 0x18402000
#define MASK3 0x00300000

void __attribute__ ((section(".text.fastcode"))) keyScanner(void)
{
	static unsigned int temp;

	keys[0] = 0;
	keys[1] = 0;

	if(!(FIOPIN0 & (1<<30)))
	{

		FIOMASK0 |= ~MASK0;
		IOSET2=((1<<19)|(1<<20)|(1<<21)|(1<<22)|(1<<23)|(1<<24));

		keys[0] = FIOPIN0>>1;
		keys[0] |= (IOPIN3 & MASK3) >> 14;

		IOSET2 = (1<<18);
		IOCLR2 = (1<<19);

		keys[0] |= FIOPIN0>> 3;
		keys[0] |= (IOPIN3 & MASK3) >> 16;

		IOSET2 = (1<<19);
		IOCLR2 = (1<<20);

		keys[0] |= FIOPIN0>> 5;
		keys[0] |= (IOPIN3 & MASK3) >> 18;

		IOSET2 = (1<<20);
		IOCLR2 = (1<<21);

		keys[0] |= FIOPIN0 >> 13;
		keys[0] |= (IOPIN3 & MASK3) << 8;

		keys[0] = ~keys[0];
		keys[0] &= 0x3FEAD7FD;

		IOSET2 = (1<<21);
		IOCLR2 = (1<<22);

		keys[1] = FIOPIN0>> 3;
		keys[1] |= (IOPIN3 & MASK3) >> 16;

		IOSET2 = (1<<22);
		IOCLR2 = (1<<23);

		keys[1] |= FIOPIN0>> 5;
		keys[1] |= (IOPIN3 & MASK3) >> 18;

		IOSET2 = (1<<23);
		IOCLR2 = (1<<24);

		keys[1] |= FIOPIN0>> 7;
		keys[1] |= (IOPIN3 & MASK3) >> 20;
	
		keys[1] = ~keys[1];
		keys[1] &= 0x3FA857F;

		temp = (keys[0] & 0x3FE80000)>>3;
		keys[0] &= 0x0002D7FD;
		keys[0] |= temp;
	
		temp = (keys[0] & 0x07FFD000)>>1;
		keys[0] &= 0x000007FD;
		keys[0] |= temp;
	
		temp = keys[0]>>1;
		keys[0] &= 0x00000001;
		keys[0] |= temp;
	
		temp = (keys[0] & 0x1FFF000)>>1;
		keys[0] &= 0x7FF;
		keys[0] |= temp;

		temp = (keys[1] & 0x03FA8000) >> 8;
		keys[1] &= 0x0000057F;
		keys[1] |= temp;

		temp = keys[0] & 0x00E00000;
		keys[0] &= 0x1FFFFF;
		keys[1] |= (temp>>3);

		FIOMASK0 &= MASK0;
		IOCLR2=((1<<18)|(1<<19)|(1<<20)|(1<<21)|(1<<22)|(1<<23));
	}

	if((keys[0] == 0) && (keys[1] == 0))
	{
		scannerTimeout = 0;
		repeatTimeout = S_REPEATDELAY;
		scannerState = S_IDLE;
	}
	else if(scannerState == S_IDLE)
	{
		scannerState = S_PRESSED;
	}

	switch(scannerState)
	{
		case S_PRESSED:
				scannerTimeout++;
				if((scannerTimeout > repeatTimeout) && (repeatTimeout != 0)  && !(sysInfo & 0x20))
				{
					repeatTimeout = S_REPEATRATE;
					scannerState++;
				}
				break;
		case S_HOLD:
				keys[0] = 0;
				keys[1] = 0;
				scannerState++;
				break;

		case S_REPEAT:
				scannerTimeout++;
				if(scannerTimeout > repeatTimeout)
				{
					keys[0] = 0;
					keys[1] = 0;
					scannerTimeout = 0;
					fastToggle++;
					if((fastToggle > 4) && (repeatTimeout > 1))
					{
						repeatTimeout >>= 1;
						fastToggle = 0;
					}
				}
				break;
	}
}
