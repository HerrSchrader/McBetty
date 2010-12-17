/*
    ir_rc6.h - phillips rc6 protocoll encoder
    Copyright (C) 2008  p.c.squirrel <pcs@gmx.at>

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

#ifndef IR_RC6_H
#define IR_RC6_H

void RC6_Encode (void) __attribute__ ((section(".text.fastcode")));

void RC6_Init(unsigned char map);
void RC6_Send(unsigned long cmd);
void RC6_Repeat(void);
void RC6_Stop(void);

#endif
