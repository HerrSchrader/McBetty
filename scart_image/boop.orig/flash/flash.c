/*
    flash.c - writing/erasing flash
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

#include "flash.h"
#include "serial.h"
#include "lcd.h"

const unsigned long secaddr[19] = 
{ 0x00000,
  0x02000,
  0x03000,
  0x04000,
  0x08000,
  0x10000,
  0x18000,
  0x20000,
  0x28000,
  0x30000,
  0x38000,
  0x40000,
  0x48000,
  0x50000,
  0x58000,
  0x60000,
  0x68000,
  0x70000,
  0x78000 };

unsigned long flash_base;

int eraseSector(unsigned char chip, unsigned char secnum)
{

	if(chip == 0)
		flash_base = FLASH0_BASE;
	else
		flash_base = FLASH1_BASE;

	serial_puts("Erasing Sector: 0x");
	putHexW(flash_base + (secaddr[secnum]<<1));

	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0xAA;
	*((volatile unsigned short *)(flash_base | 0x554)) = 0x55;
	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0x80;
	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0xAA;
	*((volatile unsigned short *)(flash_base | 0x554)) = 0x55;
	*((volatile unsigned short *)(flash_base + (secaddr[secnum]<<1))) = 0x30;

	if((*((volatile unsigned short *)(flash_base)) & 0x44) == (*((volatile unsigned short *)(flash_base)) & 0x44))
	{
		serial_puts(" -- ERROR\n");
		*((volatile unsigned short *)(flash_base)) = 0xF0;
		return -1;
	}


	while ((*((volatile unsigned short *)(flash_base)) & 0x44) != (*((volatile unsigned short *)(flash_base)) & 0x44))
	{}
	serial_puts(" +> OK\n");
	return 0;
}

int eraseFlash(unsigned char chip)
{
	if(chip == 0)
		flash_base = FLASH0_BASE;
	else
		flash_base = FLASH1_BASE;

	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0xAA;
	*((volatile unsigned short *)(flash_base | 0x554)) = 0x55;
	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0x80;
	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0xAA;
	*((volatile unsigned short *)(flash_base | 0x554)) = 0x55;
	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0x10;

	if((*((volatile unsigned short *)(flash_base)) & 0x44) == (*((volatile unsigned short *)(flash_base)) & 0x44))
	{
		*((volatile unsigned short *)(flash_base)) = 0xF0;
		return -1;
	}


	while ((*((volatile unsigned short *)(flash_base)) & 0x44) != (*((volatile unsigned short *)(flash_base)) & 0x44))
	{}
	return 0;
}

int writeWord(unsigned long addr, unsigned short data)
{
	flash_base = addr & 0xFF000000;
	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0xAA;
	*((volatile unsigned short *)(flash_base | 0x554)) = 0x55;
	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0xA0;
	*((volatile unsigned short *)(addr)) = data;

	if(*((volatile unsigned short *)(addr)) == *((volatile unsigned short *)(addr)))
	{
		serial_puts("NACK");
		*((volatile unsigned short *)(flash_base)) = 0xF0;
		return -1;
	}

	while(*((volatile unsigned short *)(addr)) != *((volatile unsigned short *)(addr)))
	{}
	return 0;
}

void prepareBulk(unsigned long dst)
{
	flash_base = dst & 0xFF000000;

	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0xAA;
	*((volatile unsigned short *)(flash_base | 0x554)) = 0x55;
	*((volatile unsigned short *)(flash_base | 0xAAA)) = 0x20;
}

void endBulk(unsigned long dst)
{
	flash_base = dst & 0xFF000000;

	*((volatile unsigned short *)(flash_base)) = 0x90;
	*((volatile unsigned short *)(flash_base)) = 0x00;
}

int writeBulk(unsigned long src, unsigned long dst, unsigned long cnt)
{
	unsigned short check1, check2;
	while(cnt--)
	{
		*((volatile unsigned short *)dst) = 0xA0;
		*((volatile unsigned short *)dst) = *((volatile unsigned short *)src);

retry:
		check1 = *((volatile unsigned short *)dst);
		check2 = *((volatile unsigned short *)dst);
		if((check1 & 0x40) == (check2 & 0x40))
			goto done;
		if(!(check2 & 0x20))
			goto retry;
		check1 = *((volatile unsigned short *)dst);
		check2 = *((volatile unsigned short *)dst);
		if((check1 & 0x40) == (check2 & 0x40))
			goto done;
		*((volatile unsigned short *)dst) = 0xF0;
		return -1;
		
done:
		check1 = *((volatile unsigned short *)dst);
		check2 = *((volatile unsigned short *)dst);
		if(check1 != check2)
			goto retry;

		src+=2;
		dst+=2;
	}

	return 0;
}
