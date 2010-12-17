/*
    ir_rc6.c - phillips rc6 protocoll encoder
    Copyright (C) 2008  p.c.squirrel <pcs@gmx.at>

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
#include "ir_rc6.h"
#include "encoders.h"
#include "codes.h"

const struct irModule RC6_Module =
{
	RC6_Encode,
	RC6_Send,
	RC6_Repeat,
	RC6_Stop,
	84,	// carrier normal mode
	101,	// carrier turbo mode
	1,	// carrier-on
	4	// carrier-off
};

extern volatile unsigned char mod_enable;
extern volatile unsigned int cycles;
extern volatile unsigned long keyMap[42];

extern unsigned int	ir_cmd;
extern unsigned int	ir_header_cmd;
extern unsigned int	ir_actcmd;
extern unsigned int	ir_startstop;
extern unsigned int	ir_state;
extern unsigned int	ir_bit;
extern unsigned int	ir_header_bit;
extern unsigned int	ir_numbits;
extern unsigned int	ir_wait;
extern unsigned int	ir_pause;
extern unsigned int	ir_leadtime;
extern unsigned int	ir_bittime0;
extern unsigned int	ir_bittime1;
extern unsigned int	ir_toggle;


extern char sysInfo;

#define RC6_IDLE	        0x00
#define RC6_LEADER_A	    0x01
#define RC6_LEADER_B	    0x02
#define RC6_HEADER_BIT_A	0x03
#define RC6_HEADER_BIT_B	0x04
#define RC6_TRAILER_BIT_A	0x05
#define RC6_TRAILER_BIT_B	0x06
#define RC6_BIT_A	        0x07
#define RC6_BIT_B	        0x08
#define RC6_WAIT	        0x09


#define RC6_BITTIME	    16
#define RC6_WAITTIME	188  //~80 ms

void __attribute__ ((section(".text.fastcode"))) RC6_Encode (void)
{
	cycles = RC6_BITTIME; //?
	switch(ir_state)
	{
		case RC6_IDLE:
			mod_enable = 0;
			break;

		case RC6_LEADER_A:
			mod_enable = 1;
			cycles = RC6_BITTIME*6;
			ir_state++;
			break;

		case RC6_LEADER_B:
			mod_enable = 0;
			cycles = RC6_BITTIME*2;
			ir_state++;
			break;

		case RC6_HEADER_BIT_A:
			if(ir_header_cmd & 0x8) //0b1000 4. bit  SB, mb2, mb1, mb0
				mod_enable = 1;
			else
				mod_enable = 0;
			ir_header_cmd <<= 1;
			cycles = RC6_BITTIME;
			ir_state++;
			break;

		case RC6_HEADER_BIT_B:
			if(mod_enable == 0)
				mod_enable = 1;
			else
				mod_enable = 0;
			ir_header_bit++;
			cycles = RC6_BITTIME;
			if(ir_header_bit == 4)
				ir_state++;
			else
				ir_state--;
			break;

		case RC6_TRAILER_BIT_A:
            if(ir_toggle & 0x01)
                mod_enable = 1;
            else
                mod_enable = 0;
			cycles = RC6_BITTIME*2;
			ir_state++;
			break;

		case RC6_TRAILER_BIT_B:
			if(mod_enable == 0)
				mod_enable = 1;
			else
				mod_enable = 0;
            cycles = RC6_BITTIME*2;
			ir_state++;
			break;

		case RC6_BIT_A:
			if(ir_cmd & 0x8000)
				mod_enable = 1;
			else
				mod_enable = 0;
			ir_cmd <<= 1;
			ir_state++;
			break;

		case RC6_BIT_B:
			if(mod_enable == 0)
				mod_enable = 1;
			else
				mod_enable = 0;
			ir_bit++;
			if(ir_bit == 16)
				ir_state++;
			else
				ir_state--;
			break;

		case RC6_WAIT:
			mod_enable = 0;
			ir_wait++;
			if(ir_wait >= RC6_WAITTIME)
			{
				ir_bit = 0;
				ir_header_bit = 0;
				ir_wait = 0;
				ir_state = RC6_IDLE;
			}
			break;
	}
}

void RC6_Init(unsigned char map)
{
	if(map < RC6.num_tables)
	{
		ir_state	= RC6_IDLE;
		ir_cmd		= 0x0000;
		ir_actcmd	= 0x0000;
		ir_wait	= 0;
		copyMapS((unsigned short*)RC6.table[map].codes);
		setIR(RC6_Module);
		cycles = RC6_BITTIME;
	}
}

void RC6_Send(unsigned long cmd)
{
	ir_actcmd = cmd;

	setIRspeed(RC6_Module);

//	if(ir_toggle & 0x01)
	//	ir_actcmd &= 0xF7FF;
	RC6_Repeat();
}

void RC6_Repeat(void)
{
	if(ir_actcmd != 0x0000)
	{
		if(ir_state == RC6_IDLE)
		{
			ir_cmd = ir_actcmd;
            ir_header_cmd = 0x08;  //0b1000;
            ir_header_bit = 0;
			ir_state++;
			runIR();
		}
	}
}

void RC6_Stop(void)
{
    if(ir_actcmd != 0x00000000)
	{
waitend:
		if(ir_state != RC6_IDLE)
			goto waitend;
	}

	ir_toggle++;
	ir_actcmd = 0x0000;
	stopIR();
}


