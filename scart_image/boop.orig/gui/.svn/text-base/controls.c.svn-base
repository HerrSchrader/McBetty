/*
    controls.c - GUI controls
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

#include "controls.h"
#include "lcd.h"
#include "fonty.h"
#include "global.h"
#include "lpc2220.h"
#include "keyboard.h"
#include "backlight.h"
#include "timerfuncs.h"

#define Cursor_timeout	150

extern volatile unsigned int keys[2];

const char string_yes[] = "JA";
const char string_no[] = "NEIN";
const char string_ok[] = "OK";

const char TXTKey1[] = ".,-+_1@*;:'&";
const char TXTKey2[] = "abc2ABC";
const char TXTKey3[] = "def3DEF";
const char TXTKey4[] = "ghi4GHI";
const char TXTKey5[] = "jkl5JKL";
const char TXTKey6[] = "mno6MNO";
const char TXTKey7[] = "pqrs7PQRS";
const char TXTKey8[] = "tuv8TUV";
const char TXTKey9[] = "wxyz9WXYZ";
const char TXTKey0[] = " 0";

const char* TXTKeys[10] = {TXTKey1,TXTKey2,TXTKey3,TXTKey4,TXTKey5,TXTKey6,TXTKey7,TXTKey8,TXTKey9,TXTKey0};

unsigned char msgbox(unsigned char y,unsigned short flags, char *st) {

	unsigned short textwidth;
	unsigned short textheight;
	unsigned char width;
	unsigned char height;
	unsigned char left;
	unsigned char x;
	unsigned char sel;
	
	struct bt_ {
		unsigned char cnt;
		unsigned char left[3];
		unsigned char top;
		unsigned char ret[3];
		char* text[3];
	} button;
	
	textwidth = get_stringwidth(st);
	textheight = get_stringheight(st);
	
	if(textwidth > 120)
		width = 124;
	else if(textwidth < 54)
		width = 60;
	else
		width = (textwidth | 0x01) + 6;
		
	if((flags & 0x03) == BC_YesNo) {
		button.cnt = 2;
		button.left[0] = 38;
		button.ret[0]=BC_Yes;
		button.text[0] = (char*)string_yes;
		button.left[1] = 65;
		button.ret[1]=BC_No;
		button.text[1] = (char*)string_no;
		button.top = y + textheight +5;
	}
	else if((flags & 0x03) == BC_nobuttons) {
		button.cnt = 0;
		button.top = 0;
	}
	else {
		button.cnt = 1;
		button.left[0] = 51;
		button.ret[0]=BC_OK;
		button.text[0] = (char*)string_ok;
		button.top = y + textheight +5;
		
	}
	
	if(button.cnt)
		height = textheight + 20;
	else
		height = textheight + 6;
	
	left = (128-width)>>1;
	
	draw_block(left,y,width,height,3,DRAW_ERASE);
	draw_rect(left,y,width,height,1,3,DRAW_PUT);
	draw_rect(left+1,y+1,width-2,height-2,1,2,DRAW_PUT);
	draw_string(left+3,y+3,st,3,DRAW_PUT);
	
	for(x=0;x<button.cnt;x++) {
		draw_rect(button.left[x],button.top,25,11,1,3,DRAW_PUT);
		draw_string(button.left[x]+12-(get_stringwidth(button.text[x])>>1),button.top+2,button.text[x],3,DRAW_PUT);
	}
	
	sel = 0;
	if((flags & 0x0C) == BC_DefaultButton2) {
		if(button.cnt>1)
			sel=1;
	}
	else if((flags & 0x0C) == BC_DefaultButton3) {
		if(button.cnt>2)
			sel=2;
	}
	
	setBacklight(BL_AUTO);	
	while(KEY_OK) {};
	while(!KEY_OK && button.cnt)
	{
		draw_block(button.left[sel]+1,button.top+1,23, 9, 3, DRAW_XOR);
		waitKeyUpDown();
		draw_block(button.left[sel]+1,button.top+1,23, 9, 3, DRAW_XOR);
		if(KEY_Left)
		{
			if(sel>0)
				sel--;
		}
		if(KEY_Right)
		{
			if(sel<button.cnt-1)
				sel++;
		}
	}
	
	if((flags & 0x03) != BC_nobuttons)
		draw_block(left,y,width,height,3,DRAW_ERASE);
	return button.ret[sel];
	
}
	
void label_draw(LABEL* label, unsigned char sel) {
	
	unsigned char c;
	unsigned char m;
	
	if(label->type & BC_Disabled)
		c=2;
	else
		c=3;
	
	if(sel) {
		draw_block(label->left,label->top,label->width,label->height,3,DRAW_PUT);
		m=DRAW_ERASE;
	}
	else
		m=DRAW_PUT;
	draw_string(label->left+1,label->top+1,label->caption, c, m);
	
}

void button_draw(BUTTON* button, unsigned char sel) {
	
	unsigned char c;
	unsigned char cleft;
	unsigned char ctop;
	
	if(button->type & BC_Disabled)
		c=2;
	else
		c=3;
	
	draw_block(button->left,button->top,button->width,button->height,3,DRAW_ERASE);
	draw_rect(button->left,button->top,button->width,button->height,1,c,DRAW_PUT);
	draw_rect(button->left+1,button->top+1,button->width-2,button->height-2,1,c-1,DRAW_PUT);
		
	cleft = ((button->width - get_stringwidth(button->caption))>>1) +button->left;
	ctop = ((button->height - get_stringheight(button->caption)+1)>>1) +button->top;
		
	if(sel == 2) {
		draw_block(button->left+2,button->top+2,button->width-4,button->height-4,3,DRAW_PUT);
		draw_string(cleft,ctop,button->caption, c, DRAW_ERASE);
	}
	else if (sel) {
		draw_block(button->left+2,button->top+2,button->width-4,button->height-4,1,DRAW_PUT);
		draw_string(cleft,ctop,button->caption, c, DRAW_PUT);
	}
	else
		draw_string(cleft,ctop,button->caption, c, DRAW_PUT);
	
}

void numbox_draw(NUMBOX* numbox, unsigned char sel) {
	
	unsigned char c;
	unsigned char m;
	
	if(numbox->type & BC_Disabled)
		c=2;
	else
		c=3;
	
	if(sel) {
		draw_block(numbox->left,numbox->top,numbox->width,numbox->height,3,DRAW_PUT);
		m=DRAW_ERASE;
	}
	else
		m=DRAW_PUT;
	draw_numU(numbox->left+1,numbox->top+1,numbox->value, 0, c, m);
	
	
}

void txtbox_draw(TXTBOX* txtbox, unsigned char sel) {
	
	unsigned char c;
	unsigned char m;
	
	if(txtbox->type & BC_Disabled)
		c=2;
	else
		c=3;
		
	if(sel) {
		draw_block(txtbox->left,txtbox->top,txtbox->width,txtbox->height,3,DRAW_PUT);
		m=DRAW_ERASE;
	}
	else
		m=DRAW_PUT;
	draw_string(txtbox->left+1,txtbox->top+1,txtbox->text, c, m);
	
}

void control_draw(CONTROL* control, unsigned char select) {
	
	draw_block(control->left,control->top,control->width,control->height,3,DRAW_ERASE);
	set_font(control->font);
	switch ((control->type) & 0x1F) {
		case BC_Label:
			label_draw((LABEL*)control,select);
			break;
		case BC_Button:
			button_draw((BUTTON*)control,select);
			break;
		case BC_Numbox:
			numbox_draw((NUMBOX*)control,select);
			break;
		case BC_Txtbox:
			txtbox_draw((TXTBOX*)control,select);
			break;
	}
}

void form_draw(FORM* form) {
	
	unsigned char x;
	
	lcd_fill(0);
	set_font(BOLDFONT);
	draw_string(0, 0, form->caption, 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	
	if(form->footer) {
		set_font(SMALLFONT);
		draw_block(0,149,128,2,3,DRAW_PUT);
		draw_string(0,152, form->footer, 3, DRAW_PUT);
	}
	for(x=0;x<form->controlcount;x++) {
		control_draw(form->controls[x],(form->sel == x));
	}	
}

unsigned char get_prev_control(FORM* form) {
	
	signed short x;
	unsigned char type;
	
	for(x=form->sel-1;x>-1;x--) {
		type = (form->controls[x])->type;
		if(!(type & 0xe0))
			return x;
	}
	for(x=form->controlcount-1;x>form->sel;x--) {
		type = (form->controls[x])->type;
		if(!(type & 0xe0))
			return x;
	}
	return form->sel;
}

unsigned char get_next_control(FORM* form) {
	
	unsigned char x;
	unsigned char type;
	
	for(x=form->sel+1;x<form->controlcount;x++) {
		type = (form->controls[x])->type;
		if(!(type & 0xe0))
			return x;
	}
	for(x=0;x<form->sel;x++) {
		type = (form->controls[x])->type;
		if(!(type & 0xe0))
			return x;
	}
	return form->sel;
}

void numbox_entervalue(NUMBOX* numbox) {
	
	unsigned char digit[5];
	unsigned char len;
	unsigned char maxlen;
	unsigned char cpos;
	unsigned char width;
	unsigned char space;
	unsigned char k;
	unsigned short value;
	unsigned short x;
	unsigned char h;
	
	set_font(numbox->font);
	width = get_stringwidth("0");
	space = get_stringwidth("00") - 2 * width;
	h=get_stringheight("0");
	
	cpos = 0;
	len = 0;
	x=numbox->max;
	for(maxlen = 1;x > 9;maxlen++)
		x /= 10;
		
	while(!(KEY_Exit || KEY_OK)) {
		k=getKeynum();
		if((k > 15) && (k < 26)) {
			digit[cpos] = k-15;
			if(digit[cpos] > 9)
				digit[cpos] = 0;
			if(len < maxlen)
				len++;
			cpos++;
			if(cpos == maxlen)
				cpos--;
		}
		if(KEY_Left) {
			if(len)
				len--;
			cpos = len;
		}
		draw_block(numbox->left,numbox->top,numbox->width,numbox->height,3,DRAW_ERASE);
		for(x=0; x<len; x++) 
			draw_char((numbox->left+1)+(width+space)*x ,numbox->top+1, digit[x] + 0x30,3, DRAW_PUT);
		draw_hline((numbox->left+1)+(width+space)*cpos,numbox->top+h,width,3,DRAW_PUT);
		waitKeyUpDown();
	}
	if((KEY_OK) && len) {
		x=1;
		value = 0;
		for(k=len;k;k--) {
			value += x*digit[k-1];
			x *=10;
		}
		if(value > numbox->max)
			numbox->value = numbox->max;
		else if(value < numbox->min)
			numbox->value = numbox->min;
		else
			numbox->value = value;
	}	
	waitKeyUp();
}

void txtbox_entervalue(TXTBOX* txtbox) {

	unsigned char k;
	unsigned char kpos;
	unsigned char kold;
	unsigned char cpos;
	unsigned char x;
	unsigned char y;
	unsigned char h;
	char buffer[txtbox->maxlen];
	
	memcpy(buffer,txtbox->text,txtbox->maxlen);
	
	if(KEY_Right) {
		for(cpos=0;buffer[cpos] !=0;cpos++);
	}
	else {
		cpos = 0;
		buffer[cpos] = 0;
	}
	if(cpos == txtbox->maxlen-1)
		cpos--;
	kpos = 0;
	
	h=get_stringheight("0");
	kold = 10;
	while(!(KEY_Exit || KEY_OK)) {
		k=getKeynum();
		if((k > 15) && (k < 26)) {
			k = k-16;
			if(k != kold) {
				kpos = 0;
				if((cpos < txtbox->maxlen-2) && (buffer[cpos]))
					cpos++;
			}
			else {
				kpos++;
				if(TXTKeys[k][kpos] == 0)
					kpos = 0;
			}
			buffer[cpos] = TXTKeys[k][kpos];
			buffer[cpos+1] = 0;
			kold = k;
		}
		if(KEY_Right) {
			if((cpos < txtbox->maxlen-2) && (buffer[cpos])) {
				cpos++;
				kold = 10;
			}
		}
		if(KEY_Left) {
			if((cpos > 0) && (!buffer[cpos]))
				cpos--;
			buffer[cpos] = 0;
		}
		draw_block(txtbox->left,txtbox->top,txtbox->width,txtbox->height,3,DRAW_ERASE);
		draw_string(txtbox->left+1,txtbox->top+1,buffer, 3, DRAW_PUT);
		x=get_stringwidth(buffer);
		y=get_stringwidth(&buffer[cpos]);
		if(!y){
			y=get_stringwidth("A");
			x+=y+1;
		}
		
		draw_block(txtbox->left+x-y,txtbox->top,y+2,h+2,3,DRAW_XOR);
		
		waitKeyUp();
		if((buffer[cpos]) && (cpos < txtbox->maxlen-2)) {
			unsigned short cnt;
			cnt = Cursor_timeout;
			while(((keys[0] == 0) && (keys[1] == 0)) && --cnt)	{
				wait5ms();
			}
			setBacklight(BL_AUTO);
			if(!cnt) {
				if((cpos < txtbox->maxlen-2) && (buffer[cpos]))
					cpos++;
					buffer[cpos] = 0;
					kold = 10;
			}
		}
		else
			waitKeyDown();
		
	}
	if(KEY_OK) {
		memcpy(txtbox->text,buffer,txtbox->maxlen);
	}	
	waitKeyUp();
}

void control_onkey(FORM* form, CONTROL* control) {
	unsigned short x;
	
	if(KEY_OK) {
		switch ((control->type) & 0x1F) {
			case BC_Label:
			case BC_Txtbox:
			case BC_Numbox:	
				if (control->ONclick)
					control->ONclick(form,control);
				waitKeyUp();
				break;
			case BC_Button:
				button_draw((BUTTON*)control,2);
				if (control->ONclick)
					control->ONclick(form,control);
				waitKeyUp();
				button_draw((BUTTON*)control,1);
				break;
		}
		return;
	}
	switch ((control->type) & 0x1F) {
		case BC_Label:
		case BC_Button:
			break;
		case BC_Numbox:
			x=((NUMBOX*)control)->value;
			if((KEY_Left) && ((NUMBOX*)control)->value > ((NUMBOX*)control)->min)
					((NUMBOX*)control)->value--;
			if((KEY_Right) && ((NUMBOX*)control)->value < ((NUMBOX*)control)->max)
					((NUMBOX*)control)->value++;
			if((getKeynum() > 15) && (getKeynum() < 26))
				numbox_entervalue((NUMBOX*) control);
			numbox_draw((NUMBOX*)control,1);
			if((x!=((NUMBOX*)control)->value) && (((NUMBOX*)control)->ONchange))
				((NUMBOX*)control)->ONchange(form,control);
			break;
		case BC_Txtbox:
			if(((getKeynum() > 15) && (getKeynum() < 26)) | KEY_Right)
				txtbox_entervalue((TXTBOX*) control);
			txtbox_draw((TXTBOX*)control,1);
			break;
	}
}

void form_exec(FORM* form) {
	
	unsigned char x;
	
	if(form->load)
		form->load(form);
	form_draw(form);
	
	while(KEY_Exit) {};
	while(!KEY_Exit)
	{
		waitKeyUpDown();
		if(KEY_Up | KEY_Down) {
			if(KEY_Up)
				x = get_prev_control(form);
			else
				x = get_next_control(form);
			control_draw(form->controls[form->sel],0);
			form->sel = x;
			control_draw(form->controls[form->sel],1);
		}
		else
			control_onkey(form,form->controls[form->sel]);
	}
	
	if(form->close)
		form->close(form);
		
	while(KEY_Exit);
}
