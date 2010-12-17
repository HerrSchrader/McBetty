/*
    flash.h - writing/erasing flash
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

#ifndef FLASH_H
#define FLASH_H

#define FLASH0_BASE	0x80000000
#define FLASH1_BASE	0x82000000

const unsigned long secaddr[19];

void prepareBulk(unsigned long dst);
void endBulk(unsigned long dst);
int eraseSector(unsigned char chip, unsigned char secnum);
int eraseFlash(unsigned char chip);
int writeWord(unsigned long addr, unsigned short data);
int writeBulk(unsigned long src, unsigned long dst, unsigned long cnt);
void lcd_set(unsigned char s);

#endif
