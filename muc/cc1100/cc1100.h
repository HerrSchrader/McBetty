/*
    cc1100.h - 
    Copyright (C) 2007  

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

#ifndef cc1100_H
#define cc1100_H

void cc1100_init(void);
int cc1100_write(unsigned char addr, unsigned char* data, unsigned char length);
int cc1100_write1(unsigned char addr,unsigned char data);
int cc1100_read(unsigned char addr, unsigned char* data, unsigned char length);
unsigned char cc1100_read1(unsigned char addr);

uint8 cc1100_read_status_reg_otf(uint8 reg);
int cc1100_strobe(uint8 cmd);

void startcc1100IRQ(void);

#define cc1100_read_status_reg(addr)   cc1100_read1( (addr) | BURST) 
#define cc1100_read_fifo(buf,len)     cc1100_read( RX_fifo | BURST, (buf), (len))
#define cc1100_write_reg   cc1100_write1


/* Set to 2 if sender (Betty) appends status bytes, else 0 */
#define TX_USE_STATUS	2

/* Set to 1 if we send the device address, else 0 */
#define TX_USE_ADDR 1

/* 
	Number of actual data bytes that we can send at once over the radio link.
	This number does not include the length byte, the address byte and the appended status bytes.
*/
#define MAX_PACKET_SIZE (64 - 1 - TX_USE_ADDR - TX_USE_STATUS) 

#endif
