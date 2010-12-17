/*
    ir_rc5.h - phillips rc5 protocoll encoder
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

#ifndef IR_RC5_H
#define IR_RC5_H

void RC5_Encode (void) __attribute__ ((section(".text.fastcode")));

void RC5_Init(unsigned char map);
void RC5_Send(unsigned long cmd);
void RC5_Repeat(void);
void RC5_Stop(void);

#endif
