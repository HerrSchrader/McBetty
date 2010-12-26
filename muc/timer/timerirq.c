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
#include "global.h"
#include "timerirq.h"
#include "kernel.h"

/* We keep an array of currently used timers. */
#define NUM_TIMERS 50
static struct timer *timer_list[NUM_TIMERS];
/* The next free entry in timer_list */
static int timer_next;

/* Reset the timer list so that no timer is used. */
void 
timer_init(){
  timer_next = 0;
};


/* 
	Given a pointer to a timer structure, add this timer to the timer list.
	The timer is automatically started
	Returns 0 iff the timer could be successfully added to the system timer list 
	Start time may be 0, in which case the timer expires immediately
	The time information is given in TICKS, applications should use symbols TICKS_PER_SEC and TICKS_PER_TENTH_SEC
*/
// TODO this interface seems to be a bit strange.
// The application has to allocate and free a timer structure itself
// We did this for efficiency reasons, so that one timer can be used frequently without allocation overhead.
// TODO better to insert fucntions here to allocate and free a timer structure
// TODO the following routine should then be used to start the timer with given parameters
//		Reason for the current API is that we do not have malloc() yet!
int 
timer_add(struct timer *t, int start_time, int period){
	if (timer_next == NUM_TIMERS){
		debug_out("All Timers Used", 0);
		return -1;
  };
  t->remaining = start_time;
  t->period = period;
  if (t->remaining)
	t->expired = 0;
  else
	t->expired=1;
  timer_list[timer_next++] = t;
  return 0;
}


/* Remove a timer from the system timer list. 
	It is safe to remove a running timer.
*/
void 
timer_del(struct timer *t){
  int i;

  for (i=0; i<timer_next; i++){
    if (timer_list[i] == t){
      timer_list[i] = timer_list[timer_next - 1];
      timer_next--;
    };
  };
};


/* Returns TRUE iff the given timer has expired. */
int 
timer_expired(struct timer *t){
  return t->expired;
};


/* This will be called whenever a new time tick signal has been received. 
	Bottom half of timer irq.
	Event handler for SIG_TIMER
*/
void 
timer_tick(){
	int i;
	struct timer *t;

	signal_clr(SIG_TIMER);
	for (i=0; i<timer_next; i++){
		t = timer_list[i];
		if (t->remaining > 0){
			t->remaining--;
			if (t->remaining == 0){
				t->expired = 1;
				if (t->period > 0)
					t->remaining = t->period;
			}
		}
	}
};
