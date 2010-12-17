/*
    controls.h - GUI controls
    Copyright (C) 2008  <telekatz@gmx.de>

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

#ifndef CONTROLS_H
#define CONTROLS_H

#define BC_OKOnly			0
#define BC_YesNo			1
#define BC_nobuttons		2
#define BC_DefaultButton1	0
#define BC_DefaultButton2	4
#define BC_DefaultButton3	8

#define BC_OK				1
#define BC_Yes				2
#define BC_No				3

#define BC_NoTabStop		0x80
#define BC_Disabled			0x40

#define BC_Label			1
#define BC_Labelnotab		(BC_Label | BC_NoTabStop)
#define BC_Button			2
#define BC_Numbox			3
#define BC_Txtbox			4


extern struct FORM dummyform;
extern struct CONTROL dummycontrol;

typedef void(*event)(struct FORM* form, struct CONTROL* control);
typedef void(*fevent)(struct FORM* form);

typedef struct CONTROL {
	unsigned char	type;
	unsigned char	left;
	unsigned char	top;
	unsigned char	width;
	unsigned char	height;
	unsigned char	font;
	unsigned char	tag1;
	unsigned char	tag2;
	char*			caption;
	event			ONclick;
} CONTROL;

typedef struct FORM {
		char*			caption;
		char* 			footer;
		fevent			load;
		fevent			close;
		unsigned long	tag;
		CONTROL*		*controls;
		unsigned char	sel;
		unsigned char	controlcount;
}FORM;
	
typedef struct LABEL {
	unsigned char	type;
	unsigned char	left;
	unsigned char	top;
	unsigned char	width;
	unsigned char	height;
	unsigned char	font;
	unsigned short	tag;
	char*			caption;
	event			ONclick;
} LABEL;

typedef struct BUTTON {
	unsigned char	type;
	unsigned char	left;
	unsigned char	top;
	unsigned char	width;
	unsigned char	height;
	unsigned char	font;
	unsigned short	tag;
	char*			caption;
	event			ONclick;
} BUTTON;

typedef struct NUMBOX {
	unsigned char	type;
	unsigned char	left;
	unsigned char	top;
	unsigned char	width;
	unsigned char	height;
	unsigned char	font;
	unsigned short	value;
	event			ONchange;
	event			ONclick;
	unsigned short	min;
	unsigned short	max;
} NUMBOX;

typedef struct TXTBOX {
	unsigned char	type;
	unsigned char	left;
	unsigned char	top;
	unsigned char	width;
	unsigned char	height;
	unsigned char	font;
	unsigned char	maxlen;
	unsigned char	tag;
	char*			text;
	event			ONclick;
} TXTBOX;

unsigned char msgbox(unsigned char x,unsigned short flags, char *st);
void form_draw(FORM* form);
void form_exec(FORM* form);

#endif
