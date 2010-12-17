/*
    ir_raw.h - raw ir encoder
    Copyright (C) 2008  <telekatz@gmx.de>

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

#ifndef IR_RAW_H
#define IR_RAW_H

void RAW_Encode (void) __attribute__ ((section(".text.fastcode")));

void RAW_Init(unsigned char map);
void RAW_Send(unsigned long cmd);
void RAW_Repeat(void);
void RAW_Stop(void);

#endif
