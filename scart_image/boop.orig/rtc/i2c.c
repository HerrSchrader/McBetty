/*
    i2c.c - 
    Copyright (C) 2008  

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

#include "lpc2220.h"
#include "i2c.h"

void wait_SI(void){

	unsigned long t = 1600000;

	I2CONCLR = 8;
	while (t-- && !(I2CONSET & 8));
}

int i2c_start(int addr) {
	I2CONCLR = 0x14;
	I2CONSET = 0x28;
	wait_SI();
	I2CONCLR = 0x20;
	if (I2STAT > 0x10)
		return(-1);
	I2DAT = addr;
	wait_SI();
	return (I2STAT != 0x40 && I2STAT != 0x18);
}

int i2c_write(unsigned char *buf, unsigned count) {
	while (count--)
	{
		I2DAT = *buf++;
		wait_SI();
		if (I2STAT != 0x28) return 1;
	}
	return 0;
}

int i2c_write1(unsigned char d) {
	I2DAT = d;
	wait_SI();
	if (I2STAT != 0x28) 
		return 1;
	return 0;
}

int i2c_read(unsigned char ack) {
	if (ack)
		I2CONSET=AA;
	else
		I2CONCLR=AA;
	wait_SI();
	
	if (((I2STAT!=0x50 && I2STAT!=0x40) && ack) || ((I2STAT!=0x58) && !ack))
		return -1;
	else
		return I2DAT;
}

void i2c_stop(void) {
	I2CONSET = STO;
	I2CONCLR = SIC;
	while((I2CONSET & STO));
	I2CONSET = 0x40;
}

