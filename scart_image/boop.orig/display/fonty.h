/*
    fonty.h - drawing text & font handling
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

#ifndef FONTY_H
#define FONTY_H

#define SMALLFONT       0
#define DOUBLEFONT      1
#define BOLDFONT        2
#define BOLDDOUBLEFONT  3
#define SMOOTHFONT      4

#define tabstep			20

unsigned char activefont;

void set_font(unsigned char f);
void draw_char(unsigned char x, unsigned char y, unsigned char ch, unsigned char c, unsigned char m);
void draw_string(unsigned char x, unsigned char y, char *st, unsigned char c, unsigned char m);
void draw_hexC(unsigned char x, unsigned char y, const unsigned char v, unsigned char c, unsigned char m);
void draw_hexS(unsigned char x, unsigned char y, const unsigned short v, unsigned char c, unsigned char m);
void draw_hexW(unsigned char x, unsigned char y, const unsigned long v, unsigned char c, unsigned char m);

void draw_numU(unsigned char x, unsigned char y, const unsigned long v, unsigned char z, unsigned char c, unsigned char m);
unsigned short get_stringwidth(char *st);
unsigned short get_stringheight(char *st);

#endif
