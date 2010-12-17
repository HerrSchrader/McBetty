/*
    lcd.c - lcd control & graphics primitives
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

#include "lpc2220.h"
#include "lcd.h"
#include "fonty.h"
#include "boop_logo"
#include "adc.h"

unsigned int is_drawing;

void _do_rcu(unsigned char x, unsigned char y, unsigned char l, unsigned char m);
void _do_rcuS(unsigned char x, unsigned char y, unsigned char m);
void _draw_block(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char c, unsigned char m);
void _draw_hline(unsigned char x, unsigned char y, unsigned char l, unsigned char c, unsigned char m);
void _draw_vline(unsigned char x, unsigned char y, unsigned char l, unsigned char c, unsigned char m);

/*
typedef void(*rcu_fn)(unsigned char x, unsigned char y, unsigned char l, unsigned char m);
typedef void(*rcus_fn)(unsigned char x, unsigned char y, unsigned char m);
typedef void(*bl_fn)(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char c, unsigned char m);
typedef void(*ln_fn)(unsigned char x, unsigned char y, unsigned char l, unsigned char c, unsigned char m);
*/

rcu_fn do_rcu;
rcus_fn do_rcuS;
bl_fn draw_block;
ln_fn draw_hline;
ln_fn draw_vline;

void pause(unsigned int ptime)
{
	unsigned long xx;
	xx = ptime * 1500;
	while(xx)
	{
		xx--;
	};
}

void lcd_set(unsigned char s)
{
	is_drawing++;
	if(s == 0)
	{
		LCD_CMD = 0xA0;		// set segment remap (00H mapped to seg0)
		LCD_CMD = 0xC8;		// set com output scan direction (remapped mode)
	}
	else
	{
		LCD_CMD = 0xA1;		// set segment remap (7FH mapped to seg0)
		LCD_CMD = 0xC0;		// set com output scan direction (normal mode)
	}
	is_drawing--;
}

void lcd_init(unsigned char s)
{
	is_drawing = 1;
	LCD_CMD = 0xE1;		// exit power save mode
	LCD_CMD = 0xE2;		// software reset
	pause(0x32);
	LCD_CMD = 0xAB;		// start internal oscillator
	LCD_CMD = 0x27;		// set internal regulator resistor ratio (8.1)
	LCD_CMD = 0x81;		// volume cmd
	LCD_CMD = 0x3A; 	// volume cmd value
	LCD_CMD = 0x65;		// set DC-DC converter factor (4x)
	LCD_CMD = 0x60;		// set upper window corner ax cmd
	LCD_CMD = 0x1C;		// set upper window corner ax value
	LCD_CMD = 0x61;		// set upper window corner ay cmd
	LCD_CMD = 0x0A;		// set upper window corner ay value
	LCD_CMD = 0x62;		// set lower window corner bx cmd
	LCD_CMD = 0x75;		// set lower window corner bx value
	LCD_CMD = 0x63;		// set lower window corner by cmd
	LCD_CMD = 0x81;		// set lower window corner by value
	LCD_CMD = 0x90;		// set PWM and FRC (4-frames)
	LCD_CMD = 0x88;		// set white mode frame  2nd and 1st cmd
	LCD_CMD = 0x00;		// set white mode frame  2nd and 1st value
	LCD_CMD = 0x89;		// set white mode frame  4th and 3rd cmd
	LCD_CMD = 0x00;		// set white mode frame  4th and 3rd value
	LCD_CMD = 0x8A;		// set light gray mode frame  2nd and 1st cmd
	LCD_CMD = 0x36;		// set light gray mode frame  2nd and 1st value
	LCD_CMD = 0x8B;		// set light gray mode frame  4th and 3rd cmd
	LCD_CMD = 0x63;		// set light gray mode frame  4th and 3rd vakue
	LCD_CMD = 0x8C;		// set dark gray mode frame  2nd and 1st cmd
	LCD_CMD = 0x67;		// set dark gray mode frame  2nd and 1st value
	LCD_CMD = 0x8D;		// set dark gray mode frame  4th and 3rd cmd
	LCD_CMD = 0x76;		// set dark gray mode frame  4th and 3rd vakue
	LCD_CMD = 0x8E;		// set black mode frame  2nd and 1st cmd
	LCD_CMD = 0xAC;		// set black mode frame  2nd and 1st value
	LCD_CMD = 0x8F;		// set black mode frame  4th and 3rd cmd
	LCD_CMD = 0xCA;		// set black mode frame  4th and 3rd value
	LCD_CMD = 0x66;		// set DC-DC converter factor (5x)
	lcd_set(s);

//	LCD_CMD = 0x2E;		// set power control register (boost on, reg on, buffer off)

//	pause(0x32);

	LCD_CMD = 0x2F;		// set power control register (boost on, reg on, buffer on)
	LCD_CMD = 0xA4;		// set entire display on, normal mode)
	LCD_CMD = 0xA6;		// set normal display mode, inverse = 0xA7

do_rcu = _do_rcu;
do_rcuS = _do_rcuS;
draw_block = _draw_block;
draw_hline = _draw_hline;
draw_vline = _draw_vline;

	lcd_fill(0x00);

	LCD_CMD = 0xAF;

	set_font(SMALLFONT);
	
	is_drawing--;
}

void lcd_enable(unsigned char e)
{
	is_drawing++;
	if(e)
	{
		LCD_CMD = 0xAF;		// set display on
	}
	else
	{
		LCD_CMD = 0xAE;		// set display off
	}
	is_drawing--;
}

void lcd_fill(unsigned char f)
{
	unsigned char x,y;
	is_drawing++;
	for(y=0;y<20;y++)
	{
		LCD_CMD = 0xB0;
		LCD_CMD = y;
		LCD_CMD = 0x10;
		LCD_CMD = 0x00;
		for(x=0;x<128;x++)
		{
			LCD_DATA = f;
			LCD_DATA = f;
		}
	}
	is_drawing--;
	showBat();
}

void draw_logo(void)
{
	unsigned short f;
	unsigned char x,y;
	is_drawing++;
	f=0;
	for(y=0;y<b_h>>3;y++)
	{
		LCD_CMD = 0xB0;
		LCD_CMD = y;
		LCD_CMD = 0x10;
		LCD_CMD = 0x00;
		for(x=0;x<b_w;x++)
		{
			LCD_DATA = b_data[f++];
			LCD_DATA = b_data[f++];
		}
	}
	is_drawing--;
}

unsigned char rcubuf[2][128];
unsigned char drawbuf[2][128];

void __attribute__ ((section(".text.fastcode"))) _do_rcu(unsigned char x, unsigned char y, unsigned char l, unsigned char m)
{
	unsigned char q;
	is_drawing++;

	if(m != DRAW_NORCU)
	{
		LCD_CMD = 0xB0;
		LCD_CMD = (y >> 3);
		LCD_CMD = (0x10 + (x >> 4));
		LCD_CMD = (x & 0x0F);
	
		q = LCD_DATA;
		q = l;
		
		while(q--)
		{
			rcubuf[0][q] = LCD_DATA;
			rcubuf[1][q] = LCD_DATA;
		}
	
		q = LCD_DATA;
	}

	LCD_CMD = 0xB0;
	LCD_CMD = (y >> 3);
	LCD_CMD = (0x10 + (x >> 4));
	LCD_CMD = (x & 0x0F);

	q = l;

	switch(m)
	{
		case DRAW_PUT:		// draw put
			while(q--)
			{
				LCD_DATA = rcubuf[0][q] | drawbuf[0][q];
				LCD_DATA = rcubuf[1][q] | drawbuf[1][q];
			}
			break;
		case DRAW_XOR:		// draw xor
			while(q--)
			{
				LCD_DATA = rcubuf[0][q] ^ drawbuf[0][q];
				LCD_DATA = rcubuf[1][q] ^ drawbuf[1][q];
			}
			break;
		case DRAW_ERASE:		// draw erase
			while(q--)
			{
				LCD_DATA = rcubuf[0][q] & ~drawbuf[0][q];
				LCD_DATA = rcubuf[1][q] & ~drawbuf[1][q];
			}
			break;
		case DRAW_NORCU:		// draw without rcu
			while(q--)
			{
				LCD_DATA = drawbuf[0][q];
				LCD_DATA = drawbuf[1][q];
			}
			break;
	}
	is_drawing--;
}

void __attribute__ ((section(".text.fastcode"))) _do_rcuS(unsigned char x, unsigned char y, unsigned char m)
{
	unsigned char q;
	is_drawing++;

	if(m != DRAW_NORCU)
	{
		LCD_CMD = 0xB0;
		LCD_CMD = (y >> 3);
		LCD_CMD = (0x10 + (x >> 4));
		LCD_CMD = (x & 0x0F);
	
		q = LCD_DATA;
		rcubuf[0][0] = LCD_DATA;
		rcubuf[1][0] = LCD_DATA;
	
		q = LCD_DATA;
	}

	LCD_CMD = 0xB0;
	LCD_CMD = (y >> 3);
	LCD_CMD = (0x10 + (x >> 4));
	LCD_CMD = (x & 0x0F);

	switch(m)
	{
		case DRAW_PUT:		// draw put
			LCD_DATA = rcubuf[0][0] | drawbuf[0][0];
			LCD_DATA = rcubuf[1][0] | drawbuf[1][0];
			break;
		case DRAW_XOR:		// draw xor
			LCD_DATA = rcubuf[0][0] ^ drawbuf[0][0];
			LCD_DATA = rcubuf[1][0] ^ drawbuf[1][0];
			break;
		case DRAW_ERASE:		// draw erase
			LCD_DATA = rcubuf[0][0] & ~drawbuf[0][0];
			LCD_DATA = rcubuf[1][0] & ~drawbuf[1][0];
			break;
		case DRAW_NORCU:		// draw erase
			LCD_DATA = drawbuf[0][0];
			LCD_DATA = drawbuf[1][0];
			break;
	}
	is_drawing--;
}

void __attribute__ ((section(".text.fastcode"))) _draw_block(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char c, unsigned char m)
{
	unsigned char q,s;
	unsigned char yy,ss,vv,ww,xx;
	unsigned char segm;
	is_drawing++;

	s = y;

	if(h)
	{
		
		
		
		yy = h;
		ss = y >> 3;
		vv = y & 0x07;
	
		ww = yy & 0x07;

		segm = 0xFF;

		xx = 8;
		if(yy < 8)
		{
			segm >>= (8-ww);
			xx -= (8-ww);
			if(vv > (8-xx))
			{
				xx = yy -(vv-(8-xx));
			}
			else
			{
				xx = yy;
			}
		}
		else
		{
			xx -= vv;
		}

		segm <<= vv;

		q = w;
		while(q--)
		{
			if(c & 0x02)
				drawbuf[0][q] = segm;
			else
				drawbuf[0][q] = 0;
	
			if(c & 0x01)
				drawbuf[1][q] = segm;
			else
				drawbuf[1][q] = 0;
		}
		do_rcu(x,s,w,m);

		yy -= xx;

		while(yy > 8)
		{
			s += 8;
			yy -= 8;
			q = w;
			while(q--)
			{
				if(c & 0x02)
					drawbuf[0][q] = 0xFF;
				else
					drawbuf[0][q] = 0;
		
				if(c & 0x01)
					drawbuf[1][q] = 0xFF;
				else
					drawbuf[1][q] = 0;
			}
			do_rcu(x,s,w,m);
		}

		if(yy)
		{
			s += 8;
			q = w;
			while(q--)
			{
				if(c & 0x02)
					drawbuf[0][q] = 0xFF >> (8-yy);
				else
					drawbuf[0][q] = 0;
		
				if(c & 0x01)
					drawbuf[1][q] = 0xFF >> (8-yy);
				else
					drawbuf[1][q] = 0;
			}
			do_rcu(x,s,w,m);
		}
	}
	is_drawing--;
}

void __attribute__ ((section(".text.fastcode"))) _draw_hline(unsigned char x, unsigned char y, unsigned char l, unsigned char c, unsigned char m)
{
	unsigned char p,q;
	is_drawing++;

	q = l;
	p = 0x01 << (y & 0x07);

	while(q--)
	{
		if(c & 0x02)
			drawbuf[0][q] = p;
		else
			drawbuf[0][q] = 0;

		if(c & 0x01)
			drawbuf[1][q] = p;
		else
			drawbuf[1][q] = 0;
	}
	do_rcu(x,y,l,m);
	is_drawing--;
}

void __attribute__ ((section(".text.fastcode"))) _draw_vline(unsigned char x, unsigned char y, unsigned char l, unsigned char c, unsigned char m)
{
	unsigned char s;
	unsigned char yy,ss,vv,ww,xx;
	unsigned char segm;
	is_drawing++;

	s = y;

	if(l)
	{
		
		
		
		yy = l;
		ss = y >> 3;
		vv = y & 0x07;
	
		ww = yy & 0x07;

		segm = 0xFF;

		xx = 8;
		if(yy < 8)
		{
			segm >>= (8-ww);
			xx -= (8-ww);
			if(vv > (8-xx))
			{
				xx = yy -(vv-(8-xx));
			}
			else
			{
				xx = yy;
			}
		}
		else
		{
			xx -= vv;
		}

		segm <<= vv;

		if(c & 0x02)
			drawbuf[0][0] = segm;
		else
			drawbuf[0][0] = 0;

		if(c & 0x01)
			drawbuf[1][0] = segm;
		else
			drawbuf[1][0] = 0;

		do_rcuS(x,s,m);

		yy -= xx;

		while(yy > 8)
		{
			s += 8;
			yy -= 8;
			if(c & 0x02)
				drawbuf[0][0] = 0xFF;
			else
				drawbuf[0][0] = 0;
		
			if(c & 0x01)
				drawbuf[1][0] = 0xFF;
			else
				drawbuf[1][0] = 0;
			do_rcuS(x,s,m);
		}

		if(yy)
		{
			s += 8;
			if(c & 0x02)
				drawbuf[0][0] = 0xFF >> (8-yy);
			else
				drawbuf[0][0] = 0;
	
			if(c & 0x01)
				drawbuf[1][0] = 0xFF >> (8-yy);
			else
				drawbuf[1][0] = 0;
			do_rcuS(x,s,m);
		}
	}
	is_drawing--;
}

void draw_rect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char lw, unsigned char c, unsigned char m)
{
	is_drawing++;
	if(lw)
	{
		if(lw == 1)
		{
			draw_hline(x,     y,     w,   c, m);
			draw_hline(x,     y+h-1, w,   c, m);

			draw_vline(x,     y+1,   h-2, c, m);
			draw_vline(x+w-1, y+1,   h-2, c, m);
		}
		else
		{
			draw_block(x,      y,      w,  lw,       c, m);
			draw_block(x,      y+h-lw, w,  lw,       c, m);
			draw_block(x,      y+lw,   lw, h-(2*lw), c, m);
			draw_block(x+w-lw, y+lw,   lw, h-(2*lw), c, m);
		}
	}
	is_drawing--;
}

void draw_pixel(unsigned char x, unsigned char y, unsigned char c, unsigned char m)
{
	unsigned char p;
	is_drawing++;

	p = 0x01 << (y & 0x07);

	if(c & 0x02)
		drawbuf[0][0] = p;
	else
		drawbuf[0][0] = 0;

	if(c & 0x01)
		drawbuf[1][0] = p;
	else
		drawbuf[1][0] = 0;

	do_rcuS(x,y,m);
	is_drawing--;
}

void draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char c, unsigned char m)
{
	unsigned char i,dx,dy,px,py;
	unsigned int x,y;
	is_drawing++;

	if (y2 > y1)                    // incl. POSITIVE
	{
		dx=x2-x1;
		dy=y2-y1;
		px=x1;
		py=y1;
		draw_pixel(x1, y1, c, m);           // first pixel
		if (dx>=dy)                        // incl. is more horizontal than vertical
		{
			y = y1;
			for(i=0;i<dx;i++)
			{
				y+=dy;
				if (y>=dx)
				{
					y-=dx;
					py+=1;
				}
				px+=1;
				draw_pixel(px, py, c, m);
			}
		}
		else                               // incl. is more vertical than horizontal
		{
			x = x1;
			for(i=0;i<dy;i++)
			{
				x+=dx;
				if (x>=dy)
				{
					x-=dy;
					px+=1;
				}
				py+=1;
				draw_pixel(px, py, c, m);        //??????
			}
		};
	}
	else if (y2 < y1)                    // incl. NEGATIVE
	{
		dx=x2-x1;
		dy=y1-y2;                          // inverted to produce a positive value
		dx=dx;
		dy=dy;
		px=x1;
		py=y2;                             // Note: due to inverted "dy",
		draw_pixel(x2, y2, c, m);           // the first pixel became the last one
		if (dx>=dy)                        // incl. is more horizontal than vertical
		{
			y = y1;
			py=y1;
			for(i=0;i<dx;i++)
			{
				y+=dy;
				if (y>=dx)
				{
					y-=dx;
					py-=1;
				}
				px+=1;
				draw_pixel(px, py, c, m);
			}
		}
		else                               // incl. is more vertical than horizontal
		{
			x = x1;
			px=x2;
			for(i=0;i<dy;i++)
			{
				x+=dx;
				if (x>=dy)
				{
					x-=dy;
					px-=1;
				}
				py+=1;
				draw_pixel(px, py, c, m);
			}
		};
	}
	is_drawing--;
}

void draw_circle(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char c, unsigned char m)
{
	signed int tswitch;
	unsigned char y, x = 0;
	unsigned char d;
	is_drawing++;
	
	d = cy - cx;
	y = radius;
	tswitch = 3 - 2 * radius;
	while (x <= y)
	{
		if(x != 0)
		{
			draw_pixel(cx + x,     cy + y, c, m);
			draw_pixel(cx + x,     cy - y, c, m);
		}
		draw_pixel(cx - x,     cy + y, c, m);
		draw_pixel(cx - x,     cy - y, c, m);

		if(x != y)
		{
			if(x != 0)
			{
				draw_pixel(cy + y - d, cy + x, c, m);
				draw_pixel(cy - y - d, cy + x, c, m);
			}
			draw_pixel(cy + y - d, cy - x, c, m);
			draw_pixel(cy - y - d, cy - x, c, m);
		}

		if (tswitch < 0)
		{ 
			tswitch += (4 * x + 6);
		}
		else
		{
			tswitch += (4 * (x - y) + 10);
			y--;
		}
		x++;
	}
	is_drawing--;
}

void draw_qcircle(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char quarter, unsigned char c, unsigned char m)
{
	signed int tswitch;
	unsigned char y, x = 0;
	unsigned char d;
	is_drawing++;
	
	d = cy - cx;
	y = radius;
	tswitch = 3 - 2 * radius;

	switch(quarter)
	{
		case TOP_LEFT:
		while (x <= y)
		{
			draw_pixel(cx - x,     cy - y, c, m);
	
			if(x != y)
			{
				draw_pixel(cy - y - d, cy - x, c, m);
			}
	
			if (tswitch < 0)
			{ 
				tswitch += (4 * x + 6);
			}
			else
			{
				tswitch += (4 * (x - y) + 10);
				y--;
			}
			x++;
		}
		break;

		case TOP_RIGHT:
		while (x <= y)
		{
			draw_pixel(cx + x,     cy - y, c, m);
	
			if(x != y)
			{
				draw_pixel(cy + y - d, cy - x, c, m);
			}
	
			if (tswitch < 0)
			{ 
				tswitch += (4 * x + 6);
			}
			else
			{
				tswitch += (4 * (x - y) + 10);
				y--;
			}
			x++;
		}
		break;

		case BOTTOM_LEFT:
		while (x <= y)
		{
			draw_pixel(cx - x,     cy + y, c, m);
	
			if(x != y)
			{
				draw_pixel(cy - y - d, cy + x, c, m);
			}
	
			if (tswitch < 0)
			{ 
				tswitch += (4 * x + 6);
			}
			else
			{
				tswitch += (4 * (x - y) + 10);
				y--;
			}
			x++;
		}
		break;

		case BOTTOM_RIGHT:
		while (x <= y)
		{
			draw_pixel(cx + x,     cy + y, c, m);
	
			if(x != y)
			{
				draw_pixel(cy + y - d, cy + x, c, m);
			}
	
			if (tswitch < 0)
			{ 
				tswitch += (4 * x + 6);
			}
			else
			{
				tswitch += (4 * (x - y) + 10);
				y--;
			}
			x++;
		}
		break;
	}
	is_drawing--;
}

void draw_disc(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char c, unsigned char m)
{
	signed int tswitch;
	unsigned char y, x = 0;
	unsigned char d;
	is_drawing++;
	
	d = cy - cx;
	y = radius;
	tswitch = 3 - 2 * radius;
	while (x <= y)
	{
		if(x != y)
		{
			if(x != 0)
			{
				draw_hline(cy - y - d, cy + x, (y<<1)+1, c, m);
			}
			draw_hline(cy - y - d, cy - x, (y<<1)+1, c, m);
		}

		if (tswitch < 0)
		{ 
			tswitch += (4 * x + 6);
		}
		else
		{
			draw_hline(cx-x, cy + y, (x<<1)+1, c, m);
			draw_hline(cx-x, cy - y, (x<<1)+1, c, m);
			tswitch += (4 * (x - y) + 10);
			y--;
		}
		x++;
	}
	is_drawing--;
}

void draw_qdisc(unsigned char cx, unsigned char cy, unsigned char radius, unsigned char quarter, unsigned char c, unsigned char m)
{
	signed int tswitch;
	unsigned char y, x = 0;
	unsigned char d;
	is_drawing++;
	
	d = cy - cx;
	y = radius;
	tswitch = 3 - 2 * radius;
	switch(quarter)
	{
		case TOP_LEFT:
		while (x <= y)
		{
			if(x != y)
			{
				draw_hline(cy - y - d, cy - x, y+1, c, m);
			}
	
			if (tswitch < 0)
			{ 
				tswitch += (4 * x + 6);
			}
			else
			{
				draw_hline(cx-x, cy - y, x+1, c, m);
				tswitch += (4 * (x - y) + 10);
				y--;
			}
			x++;
		}
		break;

		case TOP_RIGHT:
		while (x <= y)
		{
			if(x != y)
			{
				draw_hline(cx, cy - x, y+1, c, m);
			}
	
			if (tswitch < 0)
			{ 
				tswitch += (4 * x + 6);
			}
			else
			{
				draw_hline(cx, cy - y, x+1, c, m);
				tswitch += (4 * (x - y) + 10);
				y--;
			}
			x++;
		}
		break;

		case BOTTOM_LEFT:
		while (x <= y)
		{
			if(x != y)
			{
				draw_hline(cy - y - d, cy + x, y+1, c, m);
			}
	
			if (tswitch < 0)
			{ 
				tswitch += (4 * x + 6);
			}
			else
			{
				draw_hline(cx-x, cy + y, x+1, c, m);
				tswitch += (4 * (x - y) + 10);
				y--;
			}
			x++;
		}
		break;

		case BOTTOM_RIGHT:
		while (x <= y)
		{
			if(x != y)
			{
				draw_hline(cx, cy + x, y+1, c, m);
			}
	
			if (tswitch < 0)
			{ 
				tswitch += (4 * x + 6);
			}
			else
			{
				draw_hline(cx, cy + y, x+1, c, m);
				tswitch += (4 * (x - y) + 10);
				y--;
			}
			x++;
		}
		break;
	}
	is_drawing--;
}


unsigned char draw_arrow(int x, int y, unsigned char ArrowDir, int ArrowLen, unsigned char Color, unsigned char Mode)
{
	int i;
	int Len;
	is_drawing++;

	switch(ArrowDir)
	{
	case ARROW_UP:
		for(i=0; i<ArrowLen; i++)
		{
			Len = 1 + (i*2);
			draw_hline(x + ((ArrowLen-1)-i), y + i, Len, Color, Mode);
		}
		break;
	case ARROW_DOWN:
		for(i=0; i<ArrowLen; i++)
		{
			Len = 1 + (i*2);
			draw_hline(x + ((ArrowLen-1)-i), y + ((ArrowLen-1)-i), Len, Color, Mode);
		}
		break;
	case ARROW_LEFT:
		for(i=0; i<ArrowLen; i++)
		{
			Len = 1 + (i*2);
			draw_vline(x + i, y + ((ArrowLen-1)-i), Len, Color, Mode);
		}
		break;
	case ARROW_RIGHT:
		for(i=0; i<ArrowLen; i++)
		{
			Len = 1 + (i*2);
			draw_vline(x + ((ArrowLen-1)-i), y + ((ArrowLen-1)-i), Len, Color, Mode);
		}
		break;
	default:
		is_drawing--;
		return 0;//Error
		break;
	}
	is_drawing--;

	return 1;//OK
}

void draw_frame(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char t, unsigned char r, unsigned char c, unsigned char m)
{
	is_drawing++;

	if(r == 0)
	{
		draw_rect(x,y,w,h,t,c,m);
	}
	else
	{
		if(t == 1)
		{
			draw_qcircle(x+r, y+r, r, TOP_LEFT, c, m);
			draw_qcircle(x+w-r-1, y+r, r, TOP_RIGHT, c, m);
			draw_qcircle(x+r, y+h-r-1, r, BOTTOM_LEFT, c, m);
			draw_qcircle(x+w-r-1, y+h-r-1, r, BOTTOM_RIGHT, c, m);
	
			draw_hline(x+r+1,y,w-(r<<1)-2,c,m);
			draw_hline(x+r+1,y+h-1,w-(r<<1)-2,c,m);
	
			draw_vline(x, y+r+1, h-(r<<1)-2,c,m);
			draw_vline(x+w-1, y+r+1, h-(r<<1)-2,c,m);
		}
		else
		{
			draw_qdisc(x+r, y+r, r, TOP_LEFT, c, m);
			draw_qdisc(x+w-r-1, y+r, r, TOP_RIGHT, c, m);
			draw_qdisc(x+r, y+h-r-1, r, BOTTOM_LEFT, c, m);
			draw_qdisc(x+w-r-1, y+h-r-1, r, BOTTOM_RIGHT, c, m);

			if(t > r)
			{
				draw_block(x,     y+r+1, r+1, h-(r<<1)-2,c,m);
				draw_block(x+r+1,     y+t, t-r-1, h-(t<<1),c,m);
				draw_block(x+w-r-1, y+r+1, r+1, h-(r<<1)-2,c,m);
				draw_block(x+w-t,     y+t, t-r-1, h-(t<<1),c,m);
			}
			else
			{
				if(r > t)
				{
					draw_qdisc(x+r, 	y+r, 		r-t, TOP_LEFT, 	c, DRAW_XOR);
					draw_qdisc(x+w-r-1, 	y+r, 		r-t, TOP_RIGHT, 	c, DRAW_XOR);
					draw_qdisc(x+r, 	y+h-r-1, 	r-t, BOTTOM_LEFT, 	c, DRAW_XOR);
					draw_qdisc(x+w-r-1, 	y+h-r-1, 	r-t, BOTTOM_RIGHT, 	c, DRAW_XOR);
				}
				draw_block(x,     y+r+1, t, h-(r<<1)-2,c,m);
				draw_block(x+w-t, y+r+1, t, h-(r<<1)-2,c,m);
			}
			draw_block(x+r+1, y,     w-(r<<1)-2, t, c, m);
			draw_block(x+r+1, y+h-t, w-(r<<1)-2, t, c, m);
		}
	}
	is_drawing--;
}
