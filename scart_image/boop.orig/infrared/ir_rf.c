/*
    ir_rf.c - RF IR encoder
    Copyright (C) 2008  <telekatz@gmx.de>

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
#include "ir_rf.h"
#include "encoders.h"
#include "codes.h"
#include "rf.h"

const struct irModule IRRF_Module =
{ 
	IRRF_Encode,
	IRRF_Send,
	IRRF_Repeat,
	IRRF_Stop,
	84,	// carrier normal mode
	75,	// carrier turbo mode 
	1,	// carrier-on
	4	// carrier-off
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

extern char sysInfo;

#define IRRF_IDLE	0x00
#define IRRF_WAIT	0x01

#define IRRF_BITTIME	 40
#define IRRF_WAITTIME	125

void __attribute__ ((section(".text.fastcode"))) IRRF_Encode (void)
{
	switch(ir_state)
	{
		case IRRF_IDLE:
			mod_enable = 0;
			break;

		case IRRF_WAIT:
			mod_enable = 0;
			ir_wait++;
			if(ir_wait >= IRRF_WAITTIME)
			{
				ir_bit = 0;
				ir_wait = 0;
				ir_state = IRRF_IDLE;
			}
			break;
	}
}

void IRRF_Init(unsigned char map)
{
	if(map < IRRF.num_tables)
	{
		ir_state	= IRRF_IDLE;
		ir_cmd		= 0x0000;
		ir_actcmd	= 0x0000;
		ir_bit		= 0x00;
		ir_wait	= 0;
		copyMapS((unsigned short*)IRRF.table[map].codes);
		setIR(IRRF_Module);
		cycles = IRRF_BITTIME;
	}
}

void IRRF_Send(unsigned long cmd)
{
	ir_actcmd = cmd;

	setIRspeed(IRRF_Module);

	if((ir_toggle & 0x01) &&(ir_actcmd != 0x0000))
		ir_actcmd |= 0x0080;
	IRRF_Repeat();
}

void IRRF_Repeat(void)
{
	if(ir_actcmd != 0x0000)
	{
		if(ir_state == IRRF_IDLE)
		{
			unsigned char xmit[5];
			
			ir_cmd = ir_actcmd;
			
			xmit[0] = 4;
			xmit[1] = (ir_actcmd & 0xff00) >> 8;
			xmit[2] = cc1100Addr;
			xmit[3] = 2;
			xmit[4] = (ir_actcmd & 0x00ff);
			RF_send(xmit);

			ir_state++;
			runIR();
		}
	}
}

void IRRF_Stop(void)
{
	if(ir_actcmd != 0x00000000)
	{
waitend:
		if(ir_state != IRRF_IDLE)
			goto waitend;
	}

	ir_toggle++;
	ir_actcmd = 0x0000;
	stopIR();
}


