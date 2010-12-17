/*
    ir_rca.c - space modulated protocoll encoder
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
#include "ir_rca.h"
#include "encoders.h"
#include "codes.h"

const struct irModule RCA_Module =
{ 
	RCA_Encode,
	RCA_Send,
	RCA_Repeat,
	RCA_Stop,
	74,	// carrier normal mode
	134,	// carrier turbo mode
	1,	// carrier-on
	2	// carrier-off
};

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
extern unsigned int	ir_trail;

extern char sysInfo;

#define RCA_IDLE	0x00
#define RCA_PRE_A	0x01
#define RCA_PRE_B	0x02
#define RCA_BIT_A	0x03
#define RCA_BIT_B	0x04
#define RCA_IBIT_A	0x05
#define RCA_IBIT_B	0x06
#define RCA_STOP	0x07
#define RCA_WAIT	0x08

#define RCA_NUMBITS	 12
#define RCA_BITTIME	 28
#define RCA_BIT0	 2*RCA_BITTIME
#define RCA_BIT1	 4*RCA_BITTIME
#define RCA_PRETIME	 8*RCA_BITTIME
#define RCA_WAITTIME	 16

void __attribute__ ((section(".text.fastcode"))) RCA_Encode (void)
{
	switch(ir_state)
	{
		case RCA_IDLE:
			mod_enable = 0;
			break;

		case RCA_PRE_A:
			cycles = RCA_PRETIME;
			mod_enable = 1;
			ir_state++;
			break;

		case RCA_PRE_B:
			mod_enable = 0;
			ir_cmd = ir_actcmd;
			ir_state++;
			break;

		case RCA_BIT_A:
		case RCA_IBIT_A:
		case RCA_STOP:
			cycles = RCA_BITTIME;
			mod_enable = 1;
			ir_state++;
			break;

		case RCA_BIT_B:
		case RCA_IBIT_B:
			mod_enable = 0;
			if(ir_cmd & 0x0800)
				cycles = RCA_BIT1;
			else
				cycles = RCA_BIT0;
			ir_cmd <<= 1;
			ir_bit++;
			if(ir_bit == RCA_NUMBITS)
			{
				if(ir_state == RCA_BIT_B)
				{
					ir_cmd = ~ir_actcmd;
					ir_bit = 0;
				}
				ir_state++;
			}
			else
				ir_state--;
			break;

		case RCA_WAIT:
			mod_enable = 0;
			cycles = RCA_BITTIME;
			ir_wait++;
			if(ir_wait >= RCA_WAITTIME)
			{
				ir_bit = 0;
				ir_wait = 0;
				if (ir_trail)
					ir_state = RCA_PRE_A;
				else
					ir_state = RCA_IDLE;
			}
			break;
	}
}

void RCA_LoadMap(unsigned char map)
{
	switch(map)
	{
		default:
			copyMapPC(0x0500, (unsigned char*)RCA.table[map].codes);
			break;
	}
}
void RCA_Init(unsigned char map)
{
	if(map < RCA.num_tables)
	{
		ir_state		= RCA_IDLE;
		ir_cmd			= 0x0000;
		ir_actcmd		= 0x0000;
		ir_bit			= 0x00;
		ir_wait		= 0;
		RCA_LoadMap(map);
		setIR(RCA_Module);
		cycles = RCA_BITTIME;
	}
}

void RCA_Send(unsigned long cmd)
{
	ir_actcmd = cmd;
	if(ir_actcmd != 0x0000) {
		setIRspeed(RCA_Module);
		ir_trail = 0x01;
		sysInfo |= 0x20;
		if (ir_state != RCA_IDLE)
			while(ir_state != RCA_IDLE);
		ir_state = RCA_PRE_A;
		runIR();
	}
}

void RCA_Repeat(void)
{
	if(ir_actcmd != 0x0000)
	{
		
		if(ir_state == RCA_IDLE)
		{
			//ir_state++;
			//runIR();
		}
	}
}

void RCA_Stop(void)
{	
	if(ir_actcmd != 0x00000000)
	{
		ir_trail = 0;
		sysInfo &= ~0x20;
waitend:
		if(ir_state != RCA_IDLE)
			goto waitend;
	}
	stopIR();
}


