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

#define NO_KEY		-1

/* First row: */
#define KEY_5		0	
#define KEY_6		1
#define KEY_C		2
#define KEY_B		3
#define KEY_A		4
#define KEY_1		5

/* Second row: */
#define KEY_8		6
#define KEY_9		7
#define KEY_D		8
#define KEY_Up		9
#define KEY_Betty	10
#define KEY_4		11

/* Third row */
#define KEY_0		12
#define KEY_AV		13
#define KEY_Right	14
#define KEY_Left	15
#define KEY_Vplus	16
#define KEY_7		17

/* Fourth row */
#define KEY_AB		18
#define KEY_16_9	19
#define KEY_Exit	20
#define KEY_OK		21
#define KEY_Vminus	22
#define KEY_Minus	23

/* Fifth row */
#define KEY_VTX2	24
#define KEY_VTX3	25
#define KEY_Pplus	26
#define KEY_Down	27
#define KEY_Menu	28
#define KEY_PiP		29

/* Sixth row */
#define KEY_Yellow	30
#define KEY_Blue	31
#define KEY_Pminus	32
#define KEY_Mute	33
#define KEY_Info	34
#define KEY_VTX1	35

#define KEY_TV		36
#define KEY_Power	37
#define KEY_3		38
#define KEY_2		39
#define KEY_Red		40
#define KEY_Green	41


char key_scan(struct pt *pt);
void key_init ();
void key_change ();
void set_keypress_handler (void (*new_handler) (int cur_key));

#endif
