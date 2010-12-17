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

// the lcd data & command ports
#define LCD_CMD		(*((volatile unsigned char *) 0x81000000))
#define LCD_DATA	(*((volatile unsigned char *) 0x81000001))

// drawmodes 
#define DRAW_PUT	0
#define DRAW_XOR	1
#define DRAW_ERASE	2
#define DRAW_NORCU	3

#define LCD_COLOR_W	0
#define LCD_COLOR_LG	1
#define LCD_COLOR_DG	2
#define LCD_COLOR_B	3

#define LCD_SIZE_X	128
#define LCD_SIZE_Y	160

#define ARROW_UP	1
#define ARROW_DOWN	2
#define ARROW_LEFT	3
#define ARROW_RIGHT	4
#define ARROW_LEN	4

#define TOP_LEFT	0
#define TOP_RIGHT	1
#define	BOTTOM_LEFT	2
#define BOTTOM_RIGHT	3


typedef void(*rcu_fn)(unsigned char x, unsigned char y, unsigned char l, unsigned char m);
typedef void(*rcus_fn)(unsigned char x, unsigned char y, unsigned char m);
typedef void(*bl_fn)(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char c, unsigned char m);
typedef void(*ln_fn)(unsigned char x, unsigned char y, unsigned char l, unsigned char c, unsigned char m);


extern rcu_fn do_rcu;
extern rcus_fn do_rcuS;
extern bl_fn draw_block;
extern ln_fn draw_hline;
extern ln_fn draw_vline;


void lcd_init(unsigned char s);
void lcd_enable(unsigned char e);
//extern void do_rcu(unsigned char x, unsigned char y, unsigned char l, unsigned char m);
//extern void do_rcuS(unsigned char x, unsigned char y, unsigned char m);

void lcd_fill(unsigned char f);
void draw_logo(void);
//void draw_block(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char c, unsigned char m);
//void draw_hline(unsigned char x, unsigned char y, unsigned char l, unsigned char c, unsigned char m);
//void draw_vline(unsigned char x, unsigned char y, unsigned char l, unsigned char c, unsigned char m);
void draw_rect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char lw, unsigned char c, unsigned char m);
void draw_pixel(unsigned char x, unsigned char y, unsigned char c, unsigned char m);
void draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char c, unsigned char m);
void draw_circle(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char c, unsigned char m);
void draw_qcircle(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char quarter, unsigned char c, unsigned char m);
void draw_disc(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char c, unsigned char m);
void draw_qdisc(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char quarter, unsigned char c, unsigned char m);

unsigned char draw_arrow(int x, int y, unsigned char ArrowDir, int ArrowLen, unsigned char Color, unsigned char Mode);

void draw_frame(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char t, unsigned char r, unsigned char c, unsigned char m);

#endif
