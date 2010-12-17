/*
    keyboard.h - button handling
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

#ifndef KEYBOARD_H
#define KEYBOARD_H

/*
#define KEY_5		(keys[0] & 0x00000001)
#define KEY_6		(keys[0] & 0x00000002)
#define KEY_C		(keys[0] & 0x00000004)
#define KEY_B		(keys[0] & 0x00000008)
#define KEY_A		(keys[0] & 0x00000010)
#define KEY_1		(keys[0] & 0x00000020)
#define KEY_8		(keys[0] & 0x00000040)
#define KEY_9		(keys[0] & 0x00000080)
#define KEY_D		(keys[0] & 0x00000100)
#define KEY_Up		(keys[0] & 0x00000200)
#define KEY_Betty	(keys[0] & 0x00000400)
#define KEY_4		(keys[0] & 0x00000800)
#define KEY_0		(keys[0] & 0x00001000)
#define KEY_AV		(keys[0] & 0x00002000)
#define KEY_Right	(keys[0] & 0x00004000)
#define KEY_Left	(keys[0] & 0x00008000)
#define KEY_Vplus	(keys[0] & 0x00010000)
#define KEY_7		(keys[0] & 0x00020000)
#define KEY_AB		(keys[0] & 0x00040000)
#define KEY_16_9	(keys[0] & 0x00080000)
#define KEY_Exit	(keys[0] & 0x00100000)
#define KEY_OK		(keys[0] & 0x00200000)
#define KEY_Vminus	(keys[0] & 0x00400000)
#define KEY_Minus	(keys[0] & 0x00800000)
#define KEY_VTX2	(keys[0] & 0x01000000)
#define KEY_VTX3	(keys[0] & 0x02000000)
#define KEY_Pplus	(keys[0] & 0x04000000)
#define KEY_Down	(keys[0] & 0x08000000)
#define KEY_Menu	(keys[0] & 0x10000000)
#define KEY_PiP		(keys[0] & 0x20000000)

#define KEY_Yellow	(keys[1] & 0x00000001)
#define KEY_Blue	(keys[1] & 0x00000002)
#define KEY_Pminus	(keys[1] & 0x00000004)
#define KEY_Mute	(keys[1] & 0x00000008)
#define KEY_Info	(keys[1] & 0x00000010)
#define KEY_VTX1	(keys[1] & 0x00000020)
#define KEY_TV		(keys[1] & 0x00000040)
#define KEY_Power	(keys[1] & 0x00000080)
#define KEY_3		(keys[1] & 0x00000100)
#define KEY_2		(keys[1] & 0x00000200)
#define KEY_Red		(keys[1] & 0x00000400)
#define KEY_Green	(keys[1] & 0x00000800)
*/


#define KEY_OK		(keys[0] & 0x00000001)
#define KEY_7		(keys[0] & 0x00000002)
#define KEY_Vplus	(keys[0] & 0x00000004)
#define KEY_4		(keys[0] & 0x00000008)
#define KEY_Betty	(keys[0] & 0x00000010)
#define KEY_1		(keys[0] & 0x00000020)
#define KEY_A		(keys[0] & 0x00000040)
#define KEY_Left	(keys[0] & 0x00000080)
#define KEY_Exit	(keys[0] & 0x00000100)
#define KEY_Up		(keys[0] & 0x00000200)
#define KEY_B		(keys[0] & 0x00000400)
#define KEY_16_9	(keys[0] & 0x00000800)
#define KEY_AB		(keys[0] & 0x00001000)
#define KEY_D		(keys[0] & 0x00002000)
#define KEY_Right	(keys[0] & 0x00004000)
#define KEY_C		(keys[0] & 0x00008000)
#define KEY_AV		(keys[0] & 0x00010000)
#define KEY_0		(keys[0] & 0x00020000)
#define KEY_9		(keys[0] & 0x00040000)
#define KEY_8		(keys[0] & 0x00080000)
#define KEY_6		(keys[0] & 0x00100000)

#define KEY_Green	(keys[1] & 0x00000001)
#define KEY_Red		(keys[1] & 0x00000002)
#define KEY_VTX1	(keys[1] & 0x00000004)
#define KEY_Info	(keys[1] & 0x00000008)
#define KEY_PiP		(keys[1] & 0x00000010)
#define KEY_Menu	(keys[1] & 0x00000020)
#define KEY_2		(keys[1] & 0x00000040)
#define KEY_3		(keys[1] & 0x00000080)
#define KEY_Mute	(keys[1] & 0x00000100)
#define KEY_Pminus	(keys[1] & 0x00000200)
#define KEY_Down	(keys[1] & 0x00000400)
#define KEY_Pplus	(keys[1] & 0x00000800)
#define KEY_Power	(keys[1] & 0x00001000)
#define KEY_TV		(keys[1] & 0x00002000)
#define KEY_Blue	(keys[1] & 0x00004000)
#define KEY_Yellow	(keys[1] & 0x00008000)
#define KEY_VTX3	(keys[1] & 0x00010000)
#define KEY_VTX2	(keys[1] & 0x00020000)
#define KEY_5		(keys[1] & 0x00040000)
#define KEY_Minus	(keys[1] & 0x00080000)
#define KEY_Vminus	(keys[1] & 0x00100000)

#define AUTO_TIMEOUT	5

#define		KEY_IDLE	0x00
#define		KEY_PRESS	0x01
#define		KEY_HOLD	0x02
#define		KEY_RELEASE	0x03

#define ANYKEY		(!(FIOPIN0 & (1<<30)))

#define testLine(_c) \
{ \
	if(!(_c)) \
	{ \
		keys[half] |= keytest; \
	} \
 	keytest <<= 1; \
}

void initKeys(void);
void scanKeys(void);
void waitKeyUp(void);
void waitKeyDown(void);
signed char getNumKeyValue(void);
unsigned char getKeynum(void);

#define waitKeyUpDown() { waitKeyUp(); waitKeyDown(); }

#endif
