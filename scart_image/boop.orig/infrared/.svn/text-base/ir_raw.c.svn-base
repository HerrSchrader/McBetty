/*
    ir_raw.c - raw ir encoder
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
#include "ir_raw.h"
#include "encoders.h"
#include "codes.h"
#include "ir_capture.h"
#include "fonty.h"
#include "lcd.h"

#include "global.h"
#include "bfs.h"

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

struct RAWcmd_* RAWcmd;

#define RAW_IDLE	0x00
#define RAW_HI	0x01
#define RAW_LO	0x02

const struct irModule RAW_Module =
{ 
	RAW_Encode,
	RAW_Send,
	RAW_Repeat,
	RAW_Stop,
	76,	// carrier 
	91,	// carrier 
	1,	// carrier-on
	3,	// carrier-off
};

void __attribute__ ((section(".text.fastcode"))) RAW_Encode (void)
{	
	
	switch(ir_state)
	{	unsigned char* x;
	
		case RAW_IDLE:
			mod_enable = 0;
			break;

		case RAW_HI:
			mod_enable = 1;

			x=(unsigned char*)&(RAWcmd->pulselen[(RAWcmd->data[RAWcmd->CMDstart[ir_actcmd]+ir_bit])>>4]);
			cycles = x[0] + (x[1]<<8);
			ir_state++;
			break;

		case RAW_LO:
			mod_enable = 0;
			x=(unsigned char*)&(RAWcmd->pulselen[(RAWcmd->data[RAWcmd->CMDstart[ir_actcmd]+ir_bit])&0x0f]);
			cycles = x[0] + (x[1]<<8);
			ir_bit++;
			if (ir_bit >= RAWcmd->CMDlen[ir_actcmd]) {
				ir_bit = 0;
				if (ir_trail) {
					ir_state = RAW_HI;
					ir_actcmd = 1;
				}
				else
					ir_state = RAW_IDLE;
			}
			else
				ir_state--;
			break;


	}
}

void RAW_Init(unsigned char map)
{
	if(map < RAW.num_tables)
	{
		struct RAWset_ *RAWset;
		unsigned long RAWcmdbase;
		
		int x;
		for(x=0;x<42;x++) {
			keyMap[x] = (unsigned long)&ir_cmd;
		}
		ir_actcmd	= 0;
		ir_bit	= 0;
		ir_state	= RAW_IDLE;
		setIR(RAW_Module);
		ir_cmd	= 0xff;
		
		RAWcmdbase = (unsigned long)BFS_LoadFileAddr(BFS_ID_RAWslot0 + map);
		if(!RAWcmdbase)
			return;
		
		RAWset = (struct RAWset_ *)RAWcmdbase;
				
		x = memcmp(RAWset,"RC01",4);
		if(x == 0) {
			for(x=0;x<38;x++) {
				keyMap[x+4] = (unsigned long)&RAWset->RAWcmd[x];
			}	
		}
		
	}
}

void RAW_Send(unsigned long cmd)
{	
	RAWcmd = (struct RAWcmd_*)cmd;
	ir_actcmd = RAWcmd->tval;
	if(RAWcmd->tval != 0xff) {
		setIR(RAW_Module);
		T1MR0 = RAWcmd->tval;
		hi_border = (RAWcmd->borders)>>4;
		lo_border = (RAWcmd->borders)&0x0f;
		ir_actcmd = 0;
		ir_trail = 0x01;
		sysInfo |= 0x20;
		if(ir_state == RAW_IDLE)
		{
			ir_bit	= 0;
			ir_state++;
			runIR();
		}
	}
}

void RAW_Repeat(void)
{	

}

void RAW_Stop(void)
{	
	if(RAWcmd->tval != 0xff) {
		ir_trail = 0;
		while(ir_state != RAW_IDLE);
		ir_actcmd = 2;
		if(RAWcmd->CMDstart[1] != RAWcmd->CMDstart[2])
			ir_state++;
	
		while(ir_state != RAW_IDLE);
		stopIR();
		sysInfo &= ~0x20;
	}
}
