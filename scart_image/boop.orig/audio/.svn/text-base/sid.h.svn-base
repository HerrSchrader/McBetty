/*
    sid.h - tone encoder routines
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

#ifndef SID_H
#define SID_H

// SID voice controll register bits
#define GATE		(1<<0)
#define SYNC 		(1<<1)
#define RINGMOD 	(1<<2)
#define TEST 		(1<<3)
#define TRIANGLE 	(1<<4)
#define SAWTOOTH 	(1<<5)
#define RECTANGLE 	(1<<6)
#define NOISE 		(1<<7)

// SID RES/FILT ( reg.23 )
#define FILT1 		(1<<0)
#define FILT2 		(1<<1)
#define FILT3 		(1<<2)
// SID MODE/VOL ( reg.24 )  
#define VOICE3OFF 	(1<<7)

// SID registers
#define REG_FREQlo			00
#define REG_FREQhi			01
#define REG_PWlo			02
#define REG_PWhi			03
#define REG_Control			04
#define REG_AttackDecay		05
#define REG_SustainRelease	06

#define REG_FClo			21
#define REG_FChi			22
#define REG_ResFilt			23
#define REG_ModeVol			24

#define max_sid_registers	25
#define max_level			40000
#define voices				3
#define sustainstep			2666

#define ATTACK				1
#define SIDenable			(1<<3)
#define phaseoverflow		(1<<4)

#define note_c		1
#define note_cis	2
#define note_d		3
#define note_dis	4
#define note_e		5
#define note_f		6
#define note_fis	7
#define note_g		8
#define note_gis	9
#define note_a		10 
#define note_ais	11
#define note_h		12
#define note_0		13

extern const unsigned char song2[];
extern const unsigned char song3[];
extern const unsigned char song4[];

extern const unsigned short notes[];


const unsigned short AttackRate[16];
const unsigned short DecayReleaseRate[16];

struct voice_ {
	unsigned short envelope;
	unsigned short phase;
};

struct SID_ {
	struct voice_ voice[3];
	unsigned short noise;
	unsigned char flags;
	unsigned char reg[max_sid_registers];
	
};

struct SID_ SID;

unsigned int playtone_cb;
unsigned char *playtone[3];
unsigned char playstate;
unsigned char tonelen[3];
unsigned short playcounter;
unsigned short playcountermax;

void SIDsetfrq(unsigned char voice, unsigned short frq);
void SIDsetadsr(unsigned char voice,unsigned char attack, unsigned char decay, unsigned char sustain, unsigned char release);
void SIDsetwave(unsigned char voice,unsigned char wave);
void SIDplaytone(void);
void SIDplaydump(void);

#endif
