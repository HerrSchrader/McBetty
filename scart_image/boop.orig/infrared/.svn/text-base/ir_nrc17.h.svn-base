/*
    ir_nrc17.h - nokia remote control 17 encoder
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

#ifndef IR_NRC17_H
#define IR_NRC17_H

void NRC17_Encode (void) __attribute__ ((section(".text.fastcode")));

void NRC17_Init(unsigned char map);
void NRC17_Send(unsigned long cmd);
void NRC17_Repeat(void);
void NRC17_Stop(void);

#endif
