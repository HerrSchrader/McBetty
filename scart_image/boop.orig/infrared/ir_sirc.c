/*
    ir_sirc.c - sony sirc protocoll encoder
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

#include "lpc2220.h"
#include "infrared.h"
#include "ir_sirc.h"
#include "encoders.h"
#include "codes.h"

extern volatile unsigned char mod_enable;
extern volatile unsigned int cycles;
extern volatile unsigned long keyMap[42];

extern unsigned int	ir_cmd;
extern unsigned int	ir_actcmd;
extern unsigned int	ir_startstop;
extern unsigned int	ir_state;
extern unsigned int	ir_bit;
extern unsigned int	ir_numbits;
extern unsigned int	ir_wait;
extern unsigned int	ir_pause;
extern unsigned int	ir_leadtime;
extern unsigned int	ir_bittime0;
extern unsigned int	ir_bittime1;
extern unsigned int	ir_toggle;
extern unsigned int	ir_len;
extern unsigned int	ir_trail;
extern unsigned int	ir_preamble;

extern char sysInfo;

#define SIRC_IDLE	0x00
#define SIRC_START_A	0x01
#define SIRC_START_B	0x02
#define SIRC_BIT_A	0x03
#define SIRC_BIT_B	0x04
#define SIRC_WAIT	0x05

#define SIRC_STARTLEN	 96
#define SIRC_BITLEN_0	 24
#define SIRC_BITLEN_1	 48
#define SIRC_BITOFF	 24
#define SIRC_GTIME	 45

const struct irModule SIRC_Module =
{ 
	SIRC_Encode,
	SIRC_Send,
	SIRC_Repeat,
	SIRC_Stop,
	76,	// carrier 
	91,	// carrier 
	1,	// carrier-on
	4,	// carrier-off
};

void __attribute__ ((section(".text.fastcode"))) SIRC_Encode (void)
{
	switch(ir_state)
	{
		case SIRC_IDLE:
			mod_enable = 0;
			break;

		case SIRC_START_A:
			mod_enable = 1;
			cycles = SIRC_STARTLEN;
			ir_state++;
			break;

		case SIRC_START_B:
			mod_enable = 0;
			cycles = SIRC_BITOFF;
			ir_state++;
			break;

		case SIRC_BIT_A:
			mod_enable = 1;
			if(ir_cmd & 0x01)
				cycles = SIRC_BITLEN_1;
			else
				cycles = SIRC_BITLEN_0;
			ir_cmd >>= 1;
			ir_state++;
			break;

		case SIRC_BIT_B:
			mod_enable = 0;
			cycles = SIRC_BITOFF;
			ir_bit++;
			if(ir_bit == ir_numbits)
				ir_state++;
			else
				ir_state--;
			break;

		case SIRC_WAIT:
			mod_enable = 0;
			ir_wait++;
			cycles = SIRC_BITOFF;		// 1 ms
			if(ir_wait >= ir_pause)
			{
				ir_bit = 0;
				ir_wait = 0;
				ir_state = SIRC_IDLE;
			}
			break;

	}
}

void SIRC_Init(unsigned char map)
{
	if(map < SIRC.num_tables)
	{
		ir_cmd	= 0x00000000;
		ir_actcmd	= 0x00000000;
		ir_bit	= 0;
		ir_numbits	= 12;
		ir_wait	= 0;
		ir_pause	= SIRC_GTIME;
		ir_state	= SIRC_IDLE;
		copyMapS((unsigned short*)SIRC.table[map].codes);
		setIR(SIRC_Module);
	}
}

void SIRC_Send(unsigned long cmd)
{
	ir_actcmd = cmd;

	if(ir_actcmd & 0x800000)
		ir_numbits = 20;
	else if(ir_actcmd & 0x8000)
		ir_numbits = 15;
	else
		ir_numbits = 12;
	setIRspeed(SIRC_Module);
	SIRC_Repeat();
}

void SIRC_Repeat(void)
{
	if(ir_actcmd != 0x00000000)
	{
		if(ir_state == SIRC_IDLE)
		{
			ir_cmd = ir_actcmd;
			ir_state++;
			runIR();
		}
	}
}

void SIRC_Stop(void)
{
	if(ir_actcmd != 0x00000000)
	{
waitend:
		if(ir_state != SIRC_IDLE)
			goto waitend;
	}
	ir_actcmd = 0x0000;
	stopIR();
}
