/*
    timerirq.h - timekeeping and calllbacks
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

#ifndef TIMERIRQ_H
#define TIMERIRQ_H

#define MAX_CB	32

#define	SLOT_FREE	0x00
#define	SLOT_USED	0x01
#define	SLOT_HALT	0x02

typedef void(*cb_fn)(void);

typedef struct CB
{
	unsigned int	state;
	unsigned short	cur_intervall;
	unsigned short	intervall;
	cb_fn		exec;
} CB;

void __attribute__ ((section(".text.fastcode"))) timerIRQ (void);

#endif
