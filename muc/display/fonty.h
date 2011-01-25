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

#include "global.h"


// The possible fonts
enum FONT {
	NO_FONT,
	SMALLFONT,
	MEDIUMFONT,
	BIGFONT
};

void init_font();
void set_font(enum FONT f);
uint8_t char_start_col(char *txt, int pos);
uint8 draw_char(uint8 start_row, uint8 start_col, unsigned char ch, uint8 fg_col, uint8 bg_col, uint8 width);
int draw_text_space (char *s, int start_row, int start_col, int width, int height, int fg_col, int bg_col, int cnt, int lftadj);
void scroll(int start_col, int start_row, int width, int offset);

#endif
