/*
    fonty.c - drawing text & font handling
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

#include "lcd.h"
#include "fonty.h"
#include "global.h"

#include "fonts/charset.info"
#include "fonts/charset.bits"

#include "fonts/smooth.info"
#include "fonts/smooth.bits"

extern unsigned char drawbuf[2][128];
extern unsigned int is_drawing;

static unsigned char tx, ty, coff, dbl;
unsigned char *font_bits, *font_info, font_dlines;
unsigned char *font_bitsUM, *font_bitsUL, *font_bitsMM, *font_bitsML, *font_bitsLM, *font_bitsLL; 
unsigned char activefont;
 
void set_font(unsigned char f)
{	
	activefont = f;
	switch(f)
	{
                case SMOOTHFONT:
                        coff = 0;
                        dbl = 16;
                        font_info = (unsigned char*) smooth_info;
                        font_bitsUM = (unsigned char*) smooth_mt_bits;
                        font_bitsUL = (unsigned char*) smooth_lt_bits;
                        font_bitsLM = (unsigned char*) smooth_mb_bits;
                        font_bitsLL = (unsigned char*) smooth_lb_bits;
                        font_dlines = smooth_dlines;
                        break;
                case BOLDDOUBLEFONT:
                        coff = 96;
                        dbl = 1;
                        font_bits = (unsigned char*) charset_bits;
                        font_info = (unsigned char*) charset_info;
                        font_dlines = charset_dlines;
                        break;
                case BOLDFONT:
                        coff = 96;
                        dbl = 0;
                        font_bits = (unsigned char*) charset_bits;
                        font_info = (unsigned char*) charset_info;
                        font_dlines = charset_dlines;
                        break;
                case DOUBLEFONT:
                        coff = 0;
                        dbl = 1;
                        font_bits = (unsigned char*) charset_bits;
                        font_info = (unsigned char*) charset_info;
                        font_dlines = charset_dlines;
                        break;
                default:
                        coff = 0;
                        dbl = 0;
                        font_bits = (unsigned char*) charset_bits;
                        font_info = (unsigned char*) charset_info;
                        font_dlines = charset_dlines;
                        break;
	}
}

void _draw_char(unsigned int ch, unsigned char c, unsigned char m)
{
	unsigned char p,q,r,s,t;
	unsigned int cpos, cnt;
	is_drawing++;

	cpos = 0;

	for(cnt=0; cnt<ch; cnt++)
	{
		if(cnt & 0x01)
		{
			cpos += (font_info[cnt>>1] & 0x0F);
		}
		else
		{
			cpos += (font_info[cnt>>1] >> 4);
		}
	}

	p = font_info[ch>>1];
	if(ch & 0x01)
	{
		p &= 0x0F;
	}
	else
	{
		p >>= 4;
	}

	if(dbl == 1)
	{
		if(ty & 0x07)
		{
			t = ty & 0x07;
			p <<= 1;

			for(cnt=0;cnt<p;cnt+=2)
			{
				r = p-cnt-1;
				s = font_bits[cpos+(cnt>>1)];
				q = 0x00;

				if(s & 0x01)
					q += 0x03;;
				if(s & 0x02)
					q += 0x0C;;
				if(s & 0x04)
					q += 0x30;;
				if(s & 0x08)
					q += 0xC0;;

				if(c & 0x02)
					s = q << t;
				else
					s = 0x00;
				drawbuf[0][r] = s;
				drawbuf[0][r-1] = s;

				if(c & 0x01)
					s = q << t;
				else
					s = 0x00;
				drawbuf[1][r] = s;
				drawbuf[1][r-1] = s;
			}
			do_rcu(tx,ty,p,m);

			for(cnt=0;cnt<p;cnt+=2)
			{
				r = p-cnt-1;
				s = font_bits[cpos+(cnt>>1)];
				q = 0x00;

				if(s & 0x01)
					q += 0x03;;
				if(s & 0x02)
					q += 0x0C;;
				if(s & 0x04)
					q += 0x30;;
				if(s & 0x08)
					q += 0xC0;;

				if(c & 0x02)
					s = q >> (8-t);
				else
					s = 0x00;
				drawbuf[0][r] = s;
				drawbuf[0][r-1] = s;

				if(c & 0x01)
					s = q >> (8-t);
				else
					s = 0x00;
				drawbuf[1][r] = s;
				drawbuf[1][r-1] = s;
			}
			do_rcu(tx,ty+8,p,m);

			for(cnt=0;cnt<p;cnt+=2)
			{
				r = p-cnt-1;
				s = font_bits[cpos+(cnt>>1)];
				q = 0x00;

				if(s & 0x10)
					q += 0x03;;
				if(s & 0x20)
					q += 0x0C;;
				if(s & 0x40)
					q += 0x30;;
				if(s & 0x80)
					q += 0xC0;;

				if(c & 0x02)
					s = q << t;
				else
					s = 0x00;
				drawbuf[0][r] = s;
				drawbuf[0][r-1] = s;

				if(c & 0x01)
					s = q << t;
				else
					s = 0x00;
				drawbuf[1][r] = s;
				drawbuf[1][r-1] = s;
			}
			do_rcu(tx,ty+8,p,m);

			for(cnt=0;cnt<p;cnt+=2)
			{
				r = p-cnt-1;
				s = font_bits[cpos+(cnt>>1)];
				q = 0x00;

				if(s & 0x10)
					q += 0x03;;
				if(s & 0x20)
					q += 0x0C;;
				if(s & 0x40)
					q += 0x30;;
				if(s & 0x80)
					q += 0xC0;;

				if(c & 0x02)
					s = q >> (8-t);
				else
					s = 0x00;
				drawbuf[0][r] = s;
				drawbuf[0][r-1] = s;

				if(c & 0x01)
					s = q >> (8-t);
				else
					s = 0x00;
				drawbuf[1][r] = s;
				drawbuf[1][r-1] = s;
			}
			do_rcu(tx,ty+16,p,m);

		}
		else
		{
			p <<= 1;
			for(cnt=0;cnt<p;cnt+=2)
			{
				r = p-cnt-1;
				s = font_bits[cpos+(cnt>>1)];
				q = 0x00;

				if(s & 0x01)
					q += 0x03;;
				if(s & 0x02)
					q += 0x0C;;
				if(s & 0x04)
					q += 0x30;;
				if(s & 0x08)
					q += 0xC0;;

				if(c & 0x02)
					s = q;
				else
					s = 0x00;
				drawbuf[0][r] = s;
				drawbuf[0][r-1] = s;

				if(c & 0x01)
					s = q;
				else
					s = 0x00;
				drawbuf[1][r] = s;
				drawbuf[1][r-1] = s;
			}
			do_rcu(tx,ty,p,m);
			for(cnt=0;cnt<p;cnt+=2)
			{
				r = p-cnt-1;
				s = font_bits[cpos+(cnt>>1)];
				q = 0x00;

				if(s & 0x10)
					q += 0x03;;
				if(s & 0x20)
					q += 0x0C;;
				if(s & 0x40)
					q += 0x30;;
				if(s & 0x80)
					q += 0xC0;;

				if(c & 0x02)
					s = q;
				else
					s = 0x00;
				drawbuf[0][r] = s;
				drawbuf[0][r-1] = s;

				if(c & 0x01)
					s = q;
				else
					s = 0x00;
				drawbuf[1][r] = s;
				drawbuf[1][r-1] = s;
			}
			do_rcu(tx,ty+8,p,m);
		}
		tx += p+2;
	}
	else
	{
	
		if(ty & 0x07)
		{
			s = ty & 0x07;
	
			for(cnt=0;cnt<p;cnt++)
			{
				if(c & 0x02)
					drawbuf[0][p-cnt-1] = font_bits[cpos+cnt] << s;
				else
					drawbuf[0][p-cnt-1] = 0x00;

				if(c & 0x01)
					drawbuf[1][p-cnt-1] = font_bits[cpos+cnt] << s;
				else
					drawbuf[1][p-cnt-1] = 0x00;
			}
			do_rcu(tx,ty,p,m);
	
			for(cnt=0;cnt<p;cnt++)
			{
				if(c & 0x02)
					drawbuf[0][p-cnt-1] = font_bits[cpos+cnt] >> (8-s);
				else
					drawbuf[0][p-cnt-1] = 0x00;

				if(c & 0x01)
					drawbuf[1][p-cnt-1] = font_bits[cpos+cnt] >> (8-s);
				else
					drawbuf[1][p-cnt-1] = 0x00;
			}
			do_rcu(tx,ty+8,p,m);
		}
		else
		{
			for(cnt=0;cnt<p;cnt++)
			{
				if(c & 0x02)
					drawbuf[0][p-cnt-1] = font_bits[cpos+cnt];
				else
					drawbuf[0][p-cnt-1] = 0x00;

				if(c & 0x01)
					drawbuf[1][p-cnt-1] = font_bits[cpos+cnt];
				else
					drawbuf[1][p-cnt-1] = 0x00;
			}
			do_rcu(tx,ty,p,m);
		}
		tx += (p+1);
	}
	is_drawing--;
}

void _draw_charB(unsigned int ch, unsigned char c, unsigned char m)
{
	unsigned char p;
	unsigned int cpos, cnt;
	is_drawing++;

	cpos = 0;

	for(cnt=0; cnt<ch; cnt++)
	{
		cpos += font_info[cnt];
	}

	p = font_info[ch];

	if(dbl == 16)
	{
		for(cnt=0;cnt<p;cnt++)
		{
			drawbuf[0][p-cnt-1] = font_bitsUM[cpos+cnt];
			drawbuf[1][p-cnt-1] = font_bitsUL[cpos+cnt];
		}
		do_rcu(tx,ty,p,m);
		for(cnt=0;cnt<p;cnt++)
		{
			drawbuf[0][p-cnt-1] = font_bitsLM[cpos+cnt];
			drawbuf[1][p-cnt-1] = font_bitsLL[cpos+cnt];
		}
		do_rcu(tx,ty+8,p,m);
		tx += (p+1);
	}
	is_drawing--;
}

void draw_char(unsigned char x, unsigned char y, unsigned char ch, unsigned char c, unsigned char m)
{
	is_drawing++;
	tx = x;
	ty = y;
	_draw_char(ch+coff, c, m);
	is_drawing--;

}

void draw_string(unsigned char x, unsigned char y, char *st, unsigned char c, unsigned char m)
{
	unsigned char cp;
	is_drawing++;
	tx = x;
	ty = y;
	cp = 0;
	
	while(st[cp] != 0)
	{
		if(st[cp] == '\n') {
			tx = x;
			if(!dbl)
				ty += 9;
			else
				ty += 18;
		}
		else if(st[cp] == '\t') {
			tx=((tx/tabstep)+1)*tabstep;
		}
		else {
			if(dbl < 15)
				_draw_char(st[cp]+coff, c, m);
			else
				_draw_charB(st[cp]+coff, c, m);
		}
		cp++;
		
	}
	is_drawing--;
}

unsigned short get_stringwidth(char *st)
{
	unsigned char cp;
	unsigned char p;
	unsigned short width;
	unsigned short prevwidth;
	
	width = 0;
	prevwidth = 0;
	
	cp = 0;
	
	while(st[cp] != 0)
	{
		p=0;
		if(st[cp] == '\n') {
			prevwidth = max(width,prevwidth);
			width = 0;
		}
		else if(st[cp] == '\t') {
			
		}
		else {
			if(dbl < 15) {
				p = font_info[(st[cp]+coff)>>1];
				if((st[cp]+coff) & 0x01)
				{
					p &= 0x0F;
				}
				else
				{
					p >>= 4;
				}
				p +=1;
				if(dbl == 1)
					p <<= 1;
			}
			else {
				p = font_info[(unsigned char)(st[cp]+coff)];
				p+=1;
			}
		}
		width += p;
		cp++;
	}
	width=max(width,prevwidth);
	if (width)
		width--;
	return width;
}

unsigned short get_stringheight(char *st)
{
	unsigned char cp;
	unsigned short height;
	
	if(!dbl)
		height = 8;
	else
		height = 16;
	
	cp = 0;
	
	while(st[cp] != 0) {
		if(st[cp] == '\n') {
			if(!dbl)
				height += 9;
			else
				height += 18;
		}
		cp++;
	}

	return height;
}

extern char hval[16];

void draw_hexC(unsigned char x, unsigned char y, const unsigned char v, unsigned char c, unsigned char m)
{
	is_drawing++;
	tx = x;
	ty = y;
	_draw_char(hval[v>>4]+coff, c, m);
	_draw_char(hval[v & 0x0F]+coff, c, m);
	is_drawing--;
}

void draw_hexS(unsigned char x, unsigned char y, const unsigned short v, unsigned char c, unsigned char m)
{
	is_drawing++;
	tx = x;
	ty = y;
	draw_hexC(x, y, v >> 8, c, m);
	draw_hexC(tx, ty, v & 0xFF, c, m);
	is_drawing--;
}

void draw_hexW(unsigned char x, unsigned char y, const unsigned long v, unsigned char c, unsigned char m)
{
	is_drawing++;
	draw_hexS(x, y, v >> 16, c, m);
	draw_hexS(tx, ty, v & 0xFFFF, c, m);
	is_drawing--;
}

static unsigned long u_longval;
static unsigned char pos, inc;
static unsigned char num[12];

void get_digit(unsigned long tval)
{
	unsigned char count;
	count = 0;
	while(u_longval >= tval)
	{
		count++;
		u_longval -= tval;
	}

	num[pos] = '0' + count;
	if(count)
		inc = 1;
	pos += inc;
}

void draw_numU(unsigned char x, unsigned char y, const unsigned long v, unsigned char z, unsigned char c, unsigned char m)
{
	is_drawing++;
	pos = 0;
	u_longval = v;

	if(z)
		inc = 1;
	else
		inc = 0;
	
	get_digit(1000000000);
	get_digit(100000000);
	get_digit(10000000);
	get_digit(1000000);
	get_digit(100000);
	get_digit(10000);
	get_digit(1000);
	get_digit(100);
	get_digit(10);
	inc = 1;
	get_digit(1);
	num[pos] = 0x00;
	draw_string(x, y, (char*)num, c, m);
	is_drawing--;
}
