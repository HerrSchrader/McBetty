/*
    keyirq.h - keyboard scanning interrupt
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

#ifndef KEYIRQ_H
#define KEYIRQ_H

#define AUTO_TIMEOUT	5

#define testLine(_c) \
{ \
	if(!(_c)) \
	{ \
		keys[half] |= keytest; \
	} \
 	keytest <<= 1; \
}

void __attribute__ ((section(".text.fastcode"))) keyScanner (void);

#endif
