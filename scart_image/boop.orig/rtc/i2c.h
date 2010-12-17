/*
    i2c.h - 
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

#ifndef I2C_H
#define I2C_H

#define STA		0x20
#define SIC		0x08
#define SI		0x08
#define STO		0x10
#define STAC	0x20
#define AA		0x04

int i2c_start(int addr);
int i2c_write(unsigned char *buf, unsigned count);
int i2c_write1(unsigned char d);
int i2c_read(unsigned char ack);
void i2c_stop(void);

#endif
