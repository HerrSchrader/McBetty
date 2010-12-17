/*
    cc1100.c - 
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

#include "lpc2220.h"
#include "cc1100.h"
#include "irq.h"

//setting 6_WOR
const unsigned char conf[0x2F] = 
{
0x29 , 0x2E , 0x06 , 0x47 , 0xD3 , 0x91 , 0x3E , 0x1A , 
0x45 , 0x01 , 0x01 , 0x06 , 0x00 , 0x10 , 0x0B , 0xDA , 
0x8A , 0x75 , 0x13 , 0x22 , 0xC1 , 0x35 , 0x04 , 0x0C , 
0x38 , 0x16 , 0x6C , 0x43 , 0x40 , 0x91 , 0x46 , 0x50 , 
0x78 , 0x56 , 0x10 , 0xA9 , 0x0A , 0x00 , 0x11 , 0x41 , 
0x00 , 0x57 , 0x7F , 0x3F , 0x98 , 0x31 , 0x0B
};

/*const unsigned char conf[0x2F] = 
{
	0x29 , 0x2E , 0x07 , 0x00 , 0xD3 , 0x91 , 0xFF , 0x0C , 
	0x05 , 0x00 , 0x01 , 0x06 , 0x00 , 0x10 , 0x0B , 0xDA , 
	0x8A , 0x83 , 0x13 , 0x22 , 0xC1 , 0x35 , 0x07 , 0x00 , 
	0x18 , 0x16 , 0x6C , 0x43 , 0x40 , 0x91 , 0x87 , 0x6B , 
	0xF8 , 0x56 , 0x10 , 0xE9 , 0x2A , 0x00 , 0x1F , 0x41 , 
	0x00 , 0x59 , 0x7F , 0x3F , 0x81 , 0x35 , 0x09
};*/


void cc1100_init(void) {
	
	unsigned long xx = 0x200;
	
	
	PINSEL1 &= 0xffffff57;
	PCONP &= 0xfffffbff;
	PCONP |= (1<<21);	
	FIOSET0 = SCK1;
	FIOCLR0 = MOSI1;
	FIOCLR0 = CS1;
	while(xx) {
		asm volatile("nop" : :);
		xx--;
	}
	FIOSET0 = CS1;
	xx=0x200;	
	while(xx) {
		asm volatile("nop" : :);
		xx--;
	}
	FIOCLR0 = CS1;
	FIOCLR0 = SCK1;	
	while (FIOPIN0 & MISO1);
	
	PINSEL1 |= 0x000002A8;
	SSPCR0 = 0x0007;
	SSPCPSR = 0x02;
	SSPCR1 = 0x02;
			
	SSPDR = SRES;
	while (FIOPIN0 & MISO1);	
	while (SSPSR & (1<<4));
	xx = SSPDR;
		
	cc1100_write((0x00 | BURST ),(unsigned char*)conf,0x2f);
	cc1100_strobe(SIDLE);
	cc1100_strobe(SPWD);
		
}


unsigned char cc1100_write(unsigned char addr,unsigned char* data, unsigned char length) {

	unsigned short i;
	unsigned char status;
	unsigned char x;
	
	unsigned long VICInt;
	VICInt = VICIntEnable;
	VICIntEnClr = INT_EINT0;
	
	FIOCLR0 = CS1;
	while (FIOPIN0 & MISO1);
	SSPDR = (addr | WRITE);
	while ((SSPSR & (1<<4)));
	status = SSPDR;
	for (i=0; i < length; i++) {
		SSPDR = data[i];
		while ((SSPSR & (1<<4)));
		x=SSPDR;
	}
	FIOSET0 = CS1;
	
	VICIntEnable = VICInt;
	
	return(status);
}

unsigned char cc1100_write1(unsigned char addr,unsigned char data) {

	unsigned char status;
	unsigned char x;
	
	unsigned long VICInt;
	VICInt = VICIntEnable;
	VICIntEnClr = INT_EINT0;
	
	FIOCLR0 = CS1;
	while (FIOPIN0 & MISO1);
	SSPDR = (addr | WRITE);
	while ((SSPSR & (1<<4)));
	status = SSPDR;
	
	SSPDR = data;
	while ((SSPSR & (1<<4)));
	x=SSPDR;
	
	FIOSET0 = CS1;
	
	VICIntEnable = VICInt;
	
	return(status);
}


unsigned char cc1100_read(unsigned char addr, unsigned char* data, unsigned char length) {

	unsigned short i;
	unsigned char status;
	
	unsigned long VICInt;
	VICInt = VICIntEnable;
	VICIntEnClr = INT_EINT0;
		
	FIOCLR0 = CS1;
	
	while (FIOPIN0 & MISO1);
	SSPDR = (addr | READ);
	while ((SSPSR & (1<<4)));
	status = SSPDR;
	for (i=0; i < length; i++) {
		SSPDR = 0x00;
		while ((SSPSR & (1<<4)));
		data[i]=SSPDR;
	}
	FIOSET0 = CS1;
	
	VICIntEnable = VICInt;
	
	return(status);
}

unsigned char cc1100_read1(unsigned char addr) {

	unsigned char r;
	
	unsigned long VICInt;
	VICInt = VICIntEnable;
	VICIntEnClr = INT_EINT0;
	
	FIOCLR0 = CS1;
	while (FIOPIN0 & MISO1);
	SSPDR = (addr | READ);
	while ((SSPSR & (1<<4)));
	r = SSPDR;
	SSPDR = 0x00;
	while ((SSPSR & (1<<4)));
	r=SSPDR;
	FIOSET0 = CS1;
	
	VICIntEnable = VICInt;
	
	return(r);
}

unsigned char cc1100_strobe(unsigned char cmd) {

	unsigned short status;
	
	unsigned long VICInt;
	VICInt = VICIntEnable;
	VICIntEnClr = INT_EINT0;
	
	FIOCLR0 = CS1;
	while (FIOPIN0 & MISO1);
	SSPDR = cmd;
	while ((SSPSR & (1<<4)));
	status = SSPDR;
	FIOSET0 = CS1;
	
	VICIntEnable = VICInt;
	
	return(status);
}


