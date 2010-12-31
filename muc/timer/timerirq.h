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

/* A timer */
struct timer {
  int remaining;	/* Number of ticks before the timer will trigger */
  int period;		/* Value to reload the field 'remaining' with after timer expires. */
  char expired;		/* True, if the timer has expired */
};

void timer_init();
void timer_set(struct timer *t, int start_time, int period);
void timer_stop (struct timer *t);
int timer_add(struct timer *t, int start_time, int period);
void timer_del(struct timer *t);
int timer_expired(struct timer *t);
void timer_tick();
#endif
