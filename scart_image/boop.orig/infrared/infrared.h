/*
    infrared.h - ir carrier generation and encoding core
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

#ifndef INFRARED_H
#define INFRARED_H

volatile unsigned char hi_border;
volatile unsigned char lo_border;

typedef void(*ir_fn)(void);
typedef void(*ir_sfn)(unsigned long cmd);

typedef struct irModule
{
	ir_fn		encoder;
	ir_sfn		sender;
	ir_fn		repeater;
	ir_fn		stopper;
	unsigned char	tval;
	unsigned char	tval1;
	unsigned char	hi_border;
	unsigned char	lo_border;
} irModule;

//#define setIRspeed( _m ) { if(sysInfo & 0x80) T1MR0 = _m.tval1; else T1MR0 = _m.tval; }
#define setIRspeed( _m ) { T1MR0 = _m.tval1; }

void __attribute__ ((section(".text.fastcode")))  defIR(void); 
void defSender(unsigned long cmd);
void defRepeater(void);
void defStopper(void);

void initIR(void);
void startIrIRQ(void);
void setIR(struct irModule module);
void runIR(void);
void stopIR(void);

void setEncoder( unsigned char _x, unsigned char _y );

void copyMapC(unsigned char *map);
void copyMapPC(unsigned int pre, unsigned char *map);
void copyMapS(unsigned short *map);
void copyMapI(unsigned int *map);

unsigned long getCode(void);

#endif

