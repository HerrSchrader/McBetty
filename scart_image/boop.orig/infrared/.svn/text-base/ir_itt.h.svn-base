/*
    ir_itt.h - itt protocol ir encoder
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

#ifndef IR_ITT_H
#define IR_ITT_H

void ITT_Encode (void) __attribute__ ((section(".text.fastcode")));

void ITT_Init(unsigned char map);
void ITT_Send(unsigned long cmd);
void ITT_Repeat(void);
void ITT_Stop(void);

#endif
