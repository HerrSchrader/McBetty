/*
    soundcore.c - sound core routines (output, ipol, mixing, ...)
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

#include "irq.h"
#include "soundirq.h"
#include "sound.h"
#include "sid.h"
#include "lpc2220.h"

#define AUTO_OFF_TIME	0x4000

/*
extern volatile unsigned int tval;
extern volatile unsigned char bl_val, cmp_val;
extern volatile unsigned int slen;
extern volatile unsigned char *sdata;
extern volatile unsigned char sact;
extern volatile char out1;
extern volatile char last_sample;
*/

unsigned int tval;
unsigned char bl_val, cmp_val;
unsigned int slen;
unsigned char *sdata;
unsigned char sact;
unsigned char out1;
unsigned char last_sample;

const unsigned char prevvoice[3] = {2,0,1};

void __attribute__ ((section(".text.fastcode"))) soundIRQ (void)
{
	PWMIR = 0x01;
	if(sact != 0)
	{
		out1++;
		if(out1 == 2)		// quik & dirty anti-aliasing filter
		{			// (2 point linear interpolated 2x upsampling)
			
			PWMMR2 = (sdata[tval] + last_sample) >> 1;
			PWMLER = 0x04;
		}
		else
		if(out1 == 4)	// prepare next sample
		{
			out1 = 0;
			last_sample = sdata[tval];
			PWMMR2 = last_sample;
			tval++;
			if(tval >= slen)
			{
				tval = 0;
				FIOCLR0 = (1<<11);
				sact = 0;
				PWMPR = 7;
			}
			PWMLER = 0x04;
		}
	}
	else if(SID.flags & SIDenable) {
		unsigned short tempphase;
		unsigned char x;
		unsigned short sig[3];
		signed short temp;
		static unsigned char PWMcounter;
		unsigned short noise8;
		signed char wave[3];
		
		temp = 243;
		noise8=SID.noise>>6;
		for (x=0; x<3; x++) {
			volatile unsigned char phase8;
			unsigned char tempwave;
			
			if ((SID.reg[(x * 7) + REG_Control] & SYNC) && (SID.flags & phaseoverflow))
				SID.voice[x].phase = 0;
						
			
			tempphase=SID.voice[x].phase + ((SID.reg[(x * 7) + REG_FREQlo] + (SID.reg[(x * 7) + REG_FREQhi]<<8))>>4);
			phase8 = (SID.voice[x].phase)>>8;
			
			switch (SID.reg[(x * 7) + REG_Control] & 0xf0) {
				case TRIANGLE:
					
					tempwave = phase8<<1;
					if (phase8 & 0x80)
						tempwave ^= 0xff;
					
					if ((SID.reg[(x * 7) + REG_Control] & RINGMOD) && (SID.voice[prevvoice[x]].phase & 0x8000))
						tempwave ^= 0xff;
					
					wave[x] = tempwave - 128;
					break;
					
					
				case SAWTOOTH:
					wave[x] = phase8 - 0x80;
					break;
					
				case RECTANGLE:
					if(phase8 > ((SID.reg[(x * 7) + REG_PWlo] + (SID.reg[(x * 7) + REG_PWhi]<<8)) >> 4))
						wave[x] = 127;
					else
						wave[x] = -128;
					break;
				
				case NOISE:
					if(SID.voice[x].phase & 0x4000)
						wave[x]=noise8;
					break;
					
				default:
					wave[x] = 0;
			}
			sig[x]=((SID.voice[x].envelope)>>8) * (wave[x]);
			
			if (SID.voice[x].phase > tempphase)
				SID.flags |= phaseoverflow;
			else
				SID.flags &= ~phaseoverflow;
			
			SID.voice[x].phase=tempphase;
			
			if (!PWMcounter) {
				signed int envelope;
				if (SID.reg[(x * 7) + REG_Control] & GATE) {
					if (!(SID.flags & (ATTACK<<x))) {
						envelope = SID.voice[x].envelope + AttackRate[SID.reg[(x * 7) + REG_AttackDecay]>>4];
						if (envelope > max_level) {
							SID.voice[x].envelope = max_level;
							SID.flags |= (ATTACK<<x);
						}
						else
							SID.voice[x].envelope = envelope;
					}
					else {
						envelope = SID.voice[x].envelope - DecayReleaseRate[SID.reg[(x * 7) + REG_AttackDecay] & 0x0f];
						if (envelope <= (sustainstep * (SID.reg[(x * 7) + REG_SustainRelease]>>4)))
							SID.voice[x].envelope = sustainstep * (SID.reg[(x * 7) + REG_SustainRelease]>>4);
						else
							SID.voice[x].envelope = envelope;
					}	
				}
				else {
					SID.flags &= ~(ATTACK<<x);
					envelope = SID.voice[x].envelope - DecayReleaseRate[SID.reg[(x * 7) + REG_SustainRelease] & 0x0f];
					if (envelope < 0)
						SID.voice[x].envelope = 0;
					else
						SID.voice[x].envelope = envelope;
				}
			}
			if (!((x == 3) && (SID.reg[REG_ModeVol] & VOICE3OFF)))
				temp+=(signed char)(sig[x]>>8);
		}
		
		PWMMR2 = temp; 
		PWMLER = 0x04;
		
		if (!(PWMcounter--))
			PWMcounter = 29;
		
		temp = SID.noise;
		SID.noise=SID.noise << 1;

		temp ^= SID.noise;
		if ( ( temp & 0x4000 ) == 0x4000 ) 
		{ 
			SID.noise |= 1;
		}
			
	}

	cmp_val++;
	cmp_val &= 0x3F;

	if(bl_val > cmp_val)
		FIOCLR0 |= (1<<4);
	else
		FIOSET0 |= (1<<4);
}
