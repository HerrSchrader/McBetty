/*
    sound.h - sound core routines
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

#ifndef SOUND_H
#define SOUND_H

#define SOUND_OFF	0x00
#define SOUND_ON	0x01

#define MODE_STOP	0x01
#define MODE_START	0x02
#define MODE_ONESHOT	0x04
#define MODE_LOOP	0x08
#define MODE_SAMPLE	0x10
#define MODE_SYNTH	0x20
#define MODE_FREE	0x40

#define MAXVOICES	16

typedef void(*callback_fn)(void *voice);

struct t_voice
{
	callback_fn	callback;
	unsigned char	*buffer;
	unsigned char	mode; 
	unsigned int	len;
	unsigned int	pos;
	short		speed;
};

void startSoundIRQ(void);

void initSound(void);
void switchSound(unsigned char onoff);
void playSound(unsigned char *sound, unsigned long len);
void stopSound(void);
void waitSound(void);
void initVoices(void);


#endif
