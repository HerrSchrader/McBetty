/*
    timerirq.c - timekeeping and calllbacks
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
#include "timerirq.h"
#include "timerfuncs.h"

struct CB callbacks[MAX_CB];

void __attribute__ ((section(".text.fastcode"))) timerIRQ(void)
{
	unsigned int cnt;
	struct CB *cur_cb;

	for(cnt=0; cnt<MAX_CB; cnt++)
	{
		cur_cb = &callbacks[cnt];
		if(cur_cb->state & SLOT_USED)
		{
			if(!(cur_cb->state & SLOT_HALT))
			{
				cur_cb->cur_intervall++;
				if(cur_cb->cur_intervall >= cur_cb->intervall)
				{
					if(cur_cb->exec)
						(cur_cb->exec)();
					cur_cb->cur_intervall = 0;
				}
			}
		}
	}
	T0IR = 1;
}
