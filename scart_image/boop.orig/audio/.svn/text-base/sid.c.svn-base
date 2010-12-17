/*
    sid.c - tone encoder routines
    Copyright (C) 2008  Telekatz <telekatz@gmx.de>

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
#include "sid.h"
#include "sound.h"
#include "soundirq.h"
#include "lpc2220.h"
#include "timerfuncs.h"

unsigned int playtone_cb;
unsigned char *playtone[3];
unsigned char playstate;
unsigned char tonelen[3];
unsigned short playcounter;
unsigned short playcountermax;

const unsigned short AttackRate[16] = {19531,9766,2441,1628,1028,673,574,488,391,156,78,49,39,13,8,5};
const unsigned short DecayReleaseRate[16] = {6510,1628,814,543,343,233,191,163,130,52,16,13,4,3,2};

const unsigned char song2[] = {	note_d,2, note_e,2, note_f,2, note_e,2, note_d,2, note_g,2, note_f,2, note_e,2,
									note_d,2, note_e,2, note_f,2, note_g,2, note_a,2, note_g,2, note_f,2, note_e,2,
									note_d,2, note_e,2, note_f,2, note_e,2, note_d,2, note_g,2, note_f,2, note_e,2,
									note_cis,2, note_d,2, note_e,2, note_f,2, note_g,2, note_a,2, note_f,2, note_e,2,0};

const unsigned char song3[] = {	note_0,1, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2,
									note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2,
									note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2,
									note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,2, note_c,1,0};

const unsigned short notes[] = {0,4455,4720,5001,5298,5613,5947,6301,6676,7072,7493,7939,8411,0};


void SIDsetfrq(unsigned char voice, unsigned short frq) {

	if (voice < voices) {
		SID.reg[(voice * 7) + REG_FREQhi] = frq>>8;
		SID.reg[(voice * 7) + REG_FREQlo] = frq & 0xFF;
	}
}

void SIDsetadsr(unsigned char voice,unsigned char attack, unsigned char decay, unsigned char sustain, unsigned char release) {

	if (voice < voices) {
		SID.reg[(voice * 7) + REG_AttackDecay] = (attack<<4) + (decay&0x0f);
		SID.reg[(voice * 7) + REG_SustainRelease] = (sustain<<4) + (release&0x0f);
	}
}

void SIDsetwave(unsigned char voice,unsigned char wave) {

	if (voice < voices) {
		SID.reg[(voice * 7)+REG_Control] &= 0x0f;
		SID.reg[(voice * 7)+REG_Control] |= wave;
	}	
}

void SIDplaytone(void) {
	unsigned char tone;
	unsigned char x;
	
	switch (playstate) {
		case 0x01:
			playstate++;
			setCBIntervall(playtone_cb,48);
			for (x=0; x<3; x++) {
				if (playtone[x]) {
					if (!tonelen[x]) {
						tone = *playtone[x];
						if (tone) {
							playtone[x]++;
							tonelen[x] = *playtone[x];
							playtone[x]++;
							SIDsetfrq(x,notes[tone]);
							SID.reg[(x*7)+REG_Control] |= GATE;
						}
						else {
							playstate = 0x00;
							playtone[0] = 0;
							playtone[1] = 0;
							playtone[2] = 0;
							removeTimerCB(playtone_cb);
						}
					}
				}
			}
			break;
		case 0x02:
			playstate = 0x01;
			for (x=0; x<3; x++) {
				if (playtone[x]) {
					tonelen[x]--;
					if (!tonelen[x])
						SID.reg[(x*7)+REG_Control] &= ~GATE;
				}
			}
			setCBIntervall(playtone_cb,2);
			break;
		default:
			playstate = 0x00;
			removeTimerCB(playtone_cb);
	}
}

void SIDplaydump(void) {
	unsigned char x;
	
	if (playcounter++ < playcountermax) {
		for	(x=0;x<14;x++) {
			SID.reg[x] = *playtone[0];
			playtone[0]++;
		}
	}
	else {
		SID.reg[REG_Control] &= ~GATE;
		SID.reg[7+REG_Control] &= ~GATE;
		SID.reg[14+REG_Control] &= ~GATE;
		playstate = 0x00;
		playtone[0] =0;
		removeTimerCB(playtone_cb);
	}
}

