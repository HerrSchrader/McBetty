/*
    screen.c - User Interface

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
	This modules manages the different screens.
	It keeps track of the screen which is currently displayed.
	It receives input from the user (key presses) and sends them to the appropriate screen handlers.
	( Not all input is handled directly by the current screen, for example the "MUTE" key is acted upon
	  directly. It is useful in all screens. )
	
	Error messages are handled here with popups. Any info/warning/error message means that we must interrupt the normal view on screen.
	We must popup the error window and react to some keys.
	After handling of the error we should resume the previous screen. 
*/	
#include "kernel.h"
#include "timerirq.h"
#include "screen.h"
#include "window.h"
#include "keyboard.h"
#include "screen_playing.h"
#include "screen_tracklist.h"
#include "screen_playlist.h"
#include "screen_search.h"
#include "model.h"
#include "mpd.h"

/* We have a global variable cur_screen which tells us which screen is currently shown.
 	cur_screen must never be NO_SCREEN. This is taken care of.
	We decide which screen will be active and how we react to key presses depending on this variable.
	
	The variable deferred_screen is for one very special purpose:
	When a popup is active and the application wants to switch to a new screen, we do not do this
	immediately. It would confuse the user visually. We set deferred_screen to the new screen 
	and when the popup ends, it automatically switches to the new screen.
	All of this should not happen very often, it really irritates the user.

	When there is no deferred next screen, it will be set to NO_SCREEN.
*/
static enum SCREEN cur_screen, deferred_screen;

/* This is an array of all our screens
	We can index them with enum SCREEN
	See screen.h for more documentation
*/
static Screen screen_list[5];


#define POPUP_TXT_SIZE	200
/* The popup window belongs to all screens 
	It is handled here.
*/
static struct Window popup_win;
static char popup_txt[POPUP_TXT_SIZE];


/* Forward declarations */
static void mainscreen_keypress(int cur_key);

static task_id close_popup_task;
static struct timer popup_tmr;
static uint8_t timed_popup;	
static uint8_t popup_active;			// is TRUE iff screen is overlayed with a popup
static int (*popup_keypress_handler) (struct Screen *, int);		// popup specific key handler
/* ------------------------------------------------------------------------------------------------- */

/* Functions which are the same for all screens */

/* Sets all windows in a specific screen to be either visible or not */
void
screen_visible(enum SCREEN screen, int v){
	Screen *scr = &(screen_list[screen]);
	int i;
	
	for (i=0; i < scr->wl_size; i++)
		if (v)
			(scr->win_list)[i].flags |= WINFLG_VISIBLE;
		else
			(scr->win_list)[i].flags &= ~WINFLG_VISIBLE;
};

/* Redraw all the windows of the screen */
void 
screen_redraw(enum SCREEN screen){
	Screen *scr = &(screen_list[screen]);
	int i;
	
	for (i=0; i < scr->wl_size; i++)
		win_redraw( &( (scr->win_list)[i] ) );
};


/* 
	The current screen has exited.
	- call the screen specific exit function to end tasks and timers etc.
	- avoid updating the screens windows
	- keystrokes are still sent to the screen until another one opens. 
*/
static void
screen_exit(enum SCREEN screen){
	screen_list[screen].screen_exit();
	screen_visible(screen, 0);
};

/*
	Enter a new screen

	- completely clear the display
	- allow updating the screens windows
	- call the screen specific enter function to start tasks and timers etc.
	- draw contents of screens windows
	- allow keystrokes to be sent to the screen 
*/
static void
screen_enter(enum SCREEN screen){
	lcd_fill(0x00);
	cur_screen = screen;
	deferred_screen = NO_SCREEN;
	screen_visible(screen, 1);
	screen_list[screen].screen_enter();
	screen_redraw(screen);
};


/* This is called when the current task does not know which screen is shown, 
	so that switch_screen() can not be used.
	We simply exit the current screen and enter the new one.
	
	if a popup is active, do not switch screens,
		set deferred_screen instead
*/
void
show_screen(enum SCREEN newscreen){
	screen_exit(cur_screen);
	if (popup_active)
		deferred_screen = newscreen;
	else
		screen_enter(newscreen);	
};

/* ------------------------------------------------------------------------------------------------- */

/* Initialize the main screen.
	Start with top menu system.
	Initialize all screens.
	We clear the whole screen.
	We start the PLAYING screen.
*/
void 
mainscreen_init(void) {
	
	init_font();
	set_font(MEDIUMFONT);	
		
	win_scroll_init();
	
	win_cursor_init();
	
	// popup window
	win_init(&popup_win, POPUP_STARTPAGE * 8, 8, POPUP_PAGES * 8, 112, 1, popup_txt);
	popup_win.font = SMALLFONT;	
	popup_win.flags |= WINFLG_CENTER | WINFLG_LFTADJ;
	popup_win.buffer_size = POPUP_TXT_SIZE;
	popup_active = 0;
	deferred_screen = NO_SCREEN;
	
	playing_screen_init( &(screen_list[PLAYING_SCREEN]) );
	playlist_screen_init( &(screen_list[PLAYLIST_SCREEN]) );
	tracklist_screen_init( &(screen_list[TRACKLIST_SCREEN]) );
	search_screen_init( &(screen_list[SEARCH_SCREEN]) );
	
	set_keypress_handler(mainscreen_keypress);
	screen_enter(PLAYING_SCREEN);

};

/*
	This is a standard keyboard input handler for simple messages.
	Any key that is pressed closes the popup.
	Normal exiting keys are consumed, other keys are returned to the screen.
*/
static int
keypress_msg_popup(Screen *this_screen, int cur_key){
	switch (cur_key) {
		
		case KEY_Exit:
		case KEY_OK:
		case KEY_Info:
		case KEY_Betty:
		case KEY_Menu:
			popup_end();
			return NO_KEY;
			
		default:
			popup_end();
			break;						// give all other keys to normal screen handler
	};
	return cur_key;
};

/* 
	Pop up a simple message on screen.
	Used for error messages and notifications
*/
void
view_message(char *m, int time){
	popup(m, time, keypress_msg_popup);
};

/* This is the popup key press dispatcher.
	When a popup is open, keys are first given to this routine.
	If a screen specific popup keypress handler is available,
	the key is given to that.
	If the key is still noc processed after that, it is given to
	the normal keypress handler.
*/
static void
popup_keypress_dispatch(int cur_key){
	/* First we call the popup keypress handler */
	if ( NULL != popup_keypress_handler )
		cur_key = popup_keypress_handler( &(screen_list[cur_screen]), cur_key);
	
	/* It returns NO_KEY, if it has completely handled the key */
	if (NO_KEY == cur_key)
		return;
	
	/* If there is still a key to be handled, we try the main keypress routine */
	mainscreen_keypress(cur_key);
};

/* ### Automatic popup ending task ###  */
/* This task is started when the popup function is called with a time argument.
	It will wait until time out and then close the popup window
*/
PT_THREAD (close_popup(struct pt *pt)){

	PT_BEGIN(pt);
	PT_WAIT_UNTIL(pt, ( timer_expired(&popup_tmr) ));
	
	popup_end();		// deletes this task
	
	PT_WAIT_WHILE(pt, 1);
	PT_END(pt);
};

void 
popup(char *text, int time_out, int (*keypress_handler) (struct Screen *, int)){
	if 	( ! popup_active ){
		popup_active = 1;
		screen_visible(cur_screen, 0);
		popup_save();
		popup_win.flags |= WINFLG_VISIBLE;
		if (keypress_handler != NULL)
				popup_keypress_handler = keypress_handler;

		set_keypress_handler(popup_keypress_dispatch);

		if (time_out > 0){
			timer_add(&popup_tmr, time_out, 0);
			close_popup_task = task_add(&close_popup);
		} 
	} else {						// already a popup active
		if (time_out > 0){
			if (timed_popup)
				timer_set(&popup_tmr, time_out, 0);
			else {
				timer_add(&popup_tmr, time_out, 0);
				close_popup_task = task_add(close_popup);
			};
		} else {
			if (timed_popup) {
				task_del(close_popup_task);
				timer_del(&popup_tmr);
			};
		};
	};
	timed_popup = (time_out > 0);
	win_new_text(&popup_win, text);
	win_redraw(&popup_win);	
};

void 
popup_end(){
	if (timed_popup) {
		task_del(close_popup_task);
		timer_del(&popup_tmr);	
	};

	popup_active = 0;
	set_keypress_handler(mainscreen_keypress);
	
	if (deferred_screen != NO_SCREEN)
		screen_enter(deferred_screen);
	else {
		popup_restore();
		screen_visible(cur_screen, 1);
		screen_redraw(cur_screen);
	};
};


/* This is the main key press handler.
	After a key press has been detected by the keyboard driver,
	the kernel calls this routine.
*/
static void
mainscreen_keypress(int cur_key){
	
	/* First we call the screen specific key handler if there is one */
	if (cur_screen != NO_SCREEN)
		cur_key = screen_list[cur_screen].keypress( &(screen_list[cur_screen]), cur_key);
	
	/* If the current screen has completely handled the key, it returns NO_KEY and we are finished */
	if (NO_KEY == cur_key)
		return;
		
	/* We can handle some keys on our own */	
	switch (cur_key){
		
		case KEY_Vplus:	
			/* Inform the controller that the user wants a volume increase */
			user_wants_volume_add(+5);
			return;
			
		case KEY_Vminus:
			/* Inform the controller that the user wants a volume decrease */
			user_wants_volume_add(-5);
			return;
		
		case KEY_Mute:
			user_toggle_mute();
			return;
			
	};
	return;
};
