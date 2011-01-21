/*
    lcd.h - lcd control & graphics primitives
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

#ifndef LCD_H
#define LCD_H

#include "global.h"

// the lcd data & command ports
#define LCD_CMD		(*((volatile unsigned char *) 0x81000000))
#define LCD_DATA	(*((volatile unsigned char *) 0x81000001))


// The available colors 
#define WHITE 0
#define LIGHT_GREY 1
#define DARK_GREY 2
#define BLACK 3

#define LCD_SIZE_X	128
#define LCD_SIZE_Y	160

// Number of pages that our popup buffer can store
#define POPUP_PAGES	16

// Start page of our popup buffer
#define POPUP_STARTPAGE 2

void lcd_init(unsigned char s);
void lcd_enable(unsigned char e);
void lcd_fill(unsigned char f);
void trianglebar_border(uint8 row, uint8 col, uint8 width, uint8 height, uint8 fg_col, uint8 bg_col);
void draw_trianglebar(uint8 row, uint8 col, uint8 width, uint8 height, uint8 fg_col, uint8 bg_col, uint8 oldval, uint8 newval);
void draw_progressbar(uint8 row, uint8 col, uint8 height, uint8 fg_col, uint8 bg_col, uint8 oldval, uint8 newval);
void draw_rect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char lw, unsigned char fg_col);

extern void (*draw_block) (uint8 row, uint8 col, uint8 w, uint8 h, uint8 fg_col);
extern void (*write_buf) (uint8 row, uint8 col, uint8 w, uint8 h);

void lcd_scroll(uint8 row, uint8 col, uint8 width, int height, int offset);

void store_buf (uint8 pos, uint16 pattern, uint8 fg_col, uint8 bg_col);

void popup_save();
void popup_restore();

#endif

