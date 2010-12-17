/*
    ir_sirc.h - sony sirc protocoll encoder
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

#ifndef IR_SIRC_H
#define IR_SIRC_H

void SIRC_Encode (void) __attribute__ ((section(".text.fastcode")));

void SIRC_Init(unsigned char map);
void SIRC_Send(unsigned long cmd);
void SIRC_Repeat(void);
void SIRC_Stop(void);

#endif
