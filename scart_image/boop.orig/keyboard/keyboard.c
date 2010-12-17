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

#include "keyboard.h"
#include "keyirq.h"
#include "lpc2220.h"
#include "timerfuncs.h"
#include "backlight.h"
#include "irq.h"

extern volatile unsigned int keys[2];
//extern unsigned int keys[2];

void initKeys(void)
{
	keys[0] = 0;
	keys[1] = 0;
	IOCLR2=((1<<18)|(1<<19)|(1<<20)|(1<<21)|(1<<22)|(1<<23)|(1<<24));
	FIODIR0 &= ~(1<<30);
	startCB(addTimerCB(keyScanner, 15));
		
	EXTWAKE |= 0x08;
	PINSEL1 |= (1<<29);	
}

void waitKeyUp(void)
{
	while((keys[0] != 0) || (keys[1] != 0)) {};
}

void waitKeyDown(void)
{
	while((keys[0] == 0) && (keys[1] == 0))	{};
	setBacklight(BL_AUTO);	// pwm value
}

signed char getNumKeyValue(void) {
	signed char x;
	
	switch (keys[0]) {
		case 0x00000002:
			x= 7;
			break;
		case 0x00000008:
			x= 4;
			break;
		case 0x00000020:
			x= 1;
			break;
		case 0x00020000:
			x= 0;
			break;
		case 0x00040000:
			x= 9;
			break;
		case 0x00080000:
			x= 8;
			break;
		case 0x00100000:
			x= 6;
			break;
		default:
			x=-1;
	}
	switch (keys[1]) {
		case 0x00000040:
			x= 2;
			break;
		case 0x00000080:
			x= 3;
			break;
		case 0x00040000:
			x= 5;
			break;
	}
	return(x);
}

#define keynum( _X, _Y ) if(_X) return _Y; else 

unsigned char getKeynum(void)
{
		keynum(KEY_A, 0);
		keynum(KEY_B, 1);
		keynum(KEY_C, 2);
		keynum(KEY_D, 3);
		keynum(KEY_Betty, 4);
		keynum(KEY_Exit, 5);
		keynum(KEY_Up, 6);
		keynum(KEY_Down, 7);
		keynum(KEY_Left, 8);
		keynum(KEY_Right, 9);
		keynum(KEY_OK, 10);
		keynum(KEY_Vplus, 11);
		keynum(KEY_Vminus, 12);
		keynum(KEY_Mute, 13);
		keynum(KEY_Pplus, 14);
		keynum(KEY_Pminus, 15);
		keynum(KEY_1, 16);
		keynum(KEY_2, 17);
		keynum(KEY_3, 18);
		keynum(KEY_4, 19);
		keynum(KEY_5, 20);
		keynum(KEY_6, 21);
		keynum(KEY_7, 22);
		keynum(KEY_8, 23);
		keynum(KEY_9, 24);
		keynum(KEY_0, 25);
		keynum(KEY_Minus, 26);
		keynum(KEY_AV, 27);
		keynum(KEY_Menu, 28);
		keynum(KEY_PiP, 29);
		keynum(KEY_AB, 30);
		keynum(KEY_16_9, 31);
		keynum(KEY_Info, 32);
		keynum(KEY_VTX1, 33);
		keynum(KEY_VTX2, 34);
		keynum(KEY_VTX3, 35);
		keynum(KEY_Blue, 36);
		keynum(KEY_Yellow, 37);
		keynum(KEY_Green, 38);
		keynum(KEY_Red, 39);
		keynum(KEY_TV, 40);
		keynum(KEY_Power, 41);
		return 42;
}

