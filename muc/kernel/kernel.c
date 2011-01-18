/*
    kernel.c
    Copyright (C) 2008  

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

/* 
	The kernel does the following things:
	- It initializes a special system timer interrupt (timer0) to provide a system wide tick (currently every 10 ms=100 Hz).
	- The global system time (counted in ticks) is regularily incremented.
	- A scheduler is included in the kernel, which schedules tasks to be executed.
		The scheduler has a built-in table with tasks which have to be started for signals which are set.
		These are the so called event tasks. They have a higher priority than normal tasks.
		Normal tasks can be added and deleted to the scheduler task list. They are executed after the event tasks.
*/


#include "lpc2220.h"
#include "global.h"
#include "irq.h"
#include "isr.h"
#include "rf.h"
#include "timerirq.h"
#include "keyboard.h"
#include "kernel.h"

// TODO ISRs need to be fast. We should put them in the fast code section.
// But this only works nicely if we put signal_set and signal_clr also there.
// Then slow functions have to call those signal_set and signal_clr routines by an indirect function call,
// which should be doable with a MACRO

/* The following variables may be changed by interrupt service routines */
volatile static unsigned int sys_time;		// monotonically increasing system time (time ticks since boot)
volatile unsigned int signals;			// signals 


// Only for debugging.
// Implements a kind of poor mans watchdog timer
#ifdef TRACE
volatile unsigned int wdt_cnt = 10000;
volatile int cur_task;
volatile unsigned char trace_flags = 0;
#endif

/* Setting or clearing of signals constitutes a critical section, because it is a read-modify-write operation 
*/

/* Sets a signal while disabling interrupts */
void signal_set(int taskid){
	ARM_INT_KEY_TYPE int_lock_key;
	ARM_INT_LOCK(int_lock_key);
	signals |= (1<<taskid);
	ARM_INT_UNLOCK(int_lock_key);	
};

/* Clears a signal while disabling interrupts */
void signal_clr(int taskid){
	ARM_INT_KEY_TYPE int_lock_key;
	ARM_INT_LOCK(int_lock_key);
	signals &= ~(1<<taskid);
	ARM_INT_UNLOCK(int_lock_key);	
};

/* This is used to catch spurious interrupts.
	It is save to ignore the signal SIG_NONE.
*/
void  defaultIRQ(void){
	signal_set(SIG_NONE);
};

/* Returns <> 0 iff the given signal is set */
#define signal_is_set(sigid) ( signals & (1<<(sigid)) )


/* Short delay routine. Uses at least 1 "nop" per given n. 
	We run at 30 MHz or 60 MHz, so I guess n/30 or n/60 gives us the minimum number of microsecondes
	The loop counting uses cycles itself, so we are probably at n/10 microseconds
	TODO do a real timing of this routine
	TODO check where this is needed
*/
void 
delay(unsigned int n){
	while(n) {
		asm volatile("nop" : :);
		n--;
	}	
};

/* Returns the current system time in ticks (currently 1/100th of a second) since boot) */
unsigned int 
system_time(){
	return (sys_time);
};


/* This routine is called when an interrupt for timer0 channel0 match occurs 
	It incremnts the system tick counter, which holds global system time since boot
	This routine sets signal SIG_TIMER.
*/
void 
timerIRQ(void){
	/* Clear interrupt for Channel 0 Match */
	T0IR = 1;
	signal_set(SIG_TIMER);
	sys_time++;			/* increment the system time */
	
#ifdef TRACE
	if (wdt_cnt == 0){
		// Watchdog triggered, something bad happened.
		// Now we can spend some time in IRQ, it does not matter.

		serial_flush_output();
		serial_outs("\nWDT ");
		serial_out_hex(cur_task);
		serial_out_hex(trace_flags);
		serial_outs("\n");
		wdt_cnt = 200;
	} else wdt_cnt--;
#endif
		
}


#ifdef TRACE  
void feed_wdt(){
  	ARM_INT_KEY_TYPE int_lock_key;
	ARM_INT_LOCK(int_lock_key);
	wdt_cnt = 200;
	ARM_INT_UNLOCK(int_lock_key);
};	
#endif

/* Frequency, with which timer0 shall count 
   Here: 1 MHz
*/
#define T0FREQ	1000000

/* Start timer0. To be called once at startup.
	The timer0 interrupt routine timerIRQ() is bound to VIC slot 15 (lowest priority)
	and enabled
	
	See kernel.h for actual timing data.
	We define the symbols TICKS_PER_SEC and TICKS_PER_TENTH_SEC to let applications
	specify real world time information.
*/
static void 
startTimerIRQ(){
	
	VICIntEnClr = INT_TIMER0;
	
	T0TCR = 0x02;		// Stop the timer, reset the counter and prescaler to 0
	
	T0CTCR = 0x00;		// Timer mode, count every rising PCLK edge
	
	T0TC = 0;		// Starting counter value
	/* We set the prescale register so that we increment our timer every microsecond. Its simply easier to calculate this way. 
		We assume PCLK is higher than 1 MHz (it is 15 MHz in our case	
	*/
	T0PR = (PCLK/T0FREQ)-1; // We calculate PR so that we get a counting period of 1 microsecond. (15 MHz / 1MHZ = 15)
	
	T0MR0 = T0PERIOD;	// Match register 0
	T0MCR = 0x03;		// Interrupt on match register 0 and reset counter

	VICIntSelect &= ~INT_TIMER0;
	VICVectAddr15 = (unsigned long)&(timerIRQ);
	VICVectCntl15 = VIC_SLOT_EN | INT_SRC_TIMER0;
	VICIntEnable = INT_TIMER0;
	
	T0TCR = 0x01;		// Start the timer
}

/*
	We have two kind of routines which the kernel calls.
	- An event handler which is called whenever a specified signal is set.
		This is like the bottom half of an interrupt handler. 

	- A task, which is a protothread function with associated protothread data structure
*/

/* Maximum number of tasks */
#define TASK_LIM 50
static struct task task_list[TASK_LIM];
static int num_tasks;
static int task_next;	// next free slot in task_list	
	
/* To be called once at startup */
void 
kernel_init(){
	int i;
	signals = 0;
	sys_time = 0;	
	num_tasks = 0;
	task_next = 0;	
	for (i=0; i<TASK_LIM; i++)
		task_list[i].state = FINISHED;

	VICDefVectAddr = (unsigned long) &(defaultIRQ);
	
	timer_init();
	
	startTimerIRQ();
};


/* Returns NULL iff the task could NOT be successfully added to the system task list 
	else returns a pointer to the initialized task structure which can later be used to delete the task
	Initializes the given thread.
*/
task_id
task_add(char (*func) (struct pt *pt)){
	struct task *t;
	
	if (task_next == TASK_LIM){
		debug_out("All Tasks Used", 0);
		return -1;
	};

	t = &(task_list[task_next++]);
	t->thread = func;
	t->state = RUN;
	PT_INIT(&(t->thread_pt));
	num_tasks++;

	return (task_next - 1);
}

/* 
	Delete a task from our task list.
	It is safe to delete a running task.
*/
void 
task_del(task_id id){
	if (task_next > 0){
		task_next--;	
		task_list[id].thread_pt = task_list[task_next].thread_pt;
		task_list[id].thread = task_list[task_next].thread;
		task_list[id].state = task_list[task_next].state;
		num_tasks--;
	};
};


// TODO do not use signal ids, simply use signal bits (1<<x)
/* Semantik: Event tasks are functions, that need to be called when a signal is set.
  In most cases the signal is set by an IRQ handler and the event task is the bottom half.
  Events therefore have a higher priority than normal tasks. Before a normal task is allowed to run,
  all pending events are handled.
  TODO priority within events is not implemented and fast recurring events are not handled here.
  TODO we could add some functions to attach/detach a handler to/from each signal
*/
static void 
handle_events(){

	if (signal_is_set(SIG_TIMER))
		timer_tick();
#if 0			
	if (signal_is_set(SIG_NEW_TIME))
		NewTimeTask();
#endif		
#if 0		
	if (signal_is_set(SIG_RTC))
		RtcTask();
#endif		
	
#if 0
	if (pending_tasks & (1<<5)){
			RTCget_busy = 0;
			RTCtest(task3_dat);
		};
#endif 
			

	if (signal_is_set(SIG_RX_PACKET)) {
		rfRcvPacket();
	};
#if 0		
		
		if (signal_is_set(SIG_NEW_PACKET)) {
		signal_clr(SIG_NEW_PACKET);
		pkt_cnt++;
		packet[packet_length] = 0;
		process_packet((char *)packet, packet_length);
	};
#endif
				
#if 0
	if (signal_is_set(SIG_TX)){
		do_tx();
	};
	
#endif

#if 0		
	if (signal_is_set(SIG_KEYSCAN)) {
		key_scan();
	};
#endif				

#if 1	
	if (signal_is_set(SIG_KEY_CHG)) {
		key_change();
	};
#endif	
#if 0
	if (signal_is_set(SIG_RTC_INT)) {
		Task30();
	};
#endif
#if 0
	if (signal_is_set(SIG_RTC_UNLOCK)) {
		rtc_unlock();
	};
#endif 
			
#if 1	
	// SIG_NONE is used, when the API needs a signal, but no handler needs to be invoked
	if (signal_is_set(SIG_NONE)) {
		signal_clr(SIG_NONE);
	};
#endif					
}

/*
	Simple scheduler.
	Run the normal tasks, running all pending events before each one.
*/

void schedule(){
	int i, running;
	struct task *t;

#ifdef TRACE  
	feed_wdt();
#endif
		
	for (i=0; i<num_tasks;){

#ifdef TRACE
		cur_task = 99;
#endif  
	
		handle_events();

#ifdef TRACE
		cur_task = i;
#endif
		
		t=&(task_list[i]);
		/* Run the next thread with its own thread structure as argument */
		if (t->state == RUN) {
			running = PT_SCHEDULE( t->thread(&(t->thread_pt)) );
			i++;
		} else {
			task_del(i);
			// do not increment i, we probably have a new task at this index
		}
  	};
};


