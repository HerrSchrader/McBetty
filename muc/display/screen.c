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
	( Not all input is handled directly by the current screen, for example the "MUTE" key is given to
	  the playing screen to handle. It only makes sense there. )
	
	TODO Maybe handle error messages here. Any info/warning/error message means that we must interrupt the normal view on screen.
	We must switch to the error screen to show the error window and react to some keys.
	After handling of the error we should resume the previous screen or switch to a completely different screen depending
	on the kind of error. So this could be handled here. 
*/	

#include "screen.h"
#include "window.h"
#include "keyboard.h"
#include "screen_playing.h"
#include "screen_tracklist.h"
#include "screen_playlist.h"
#include "screen_info.h"
#include "screen_search.h"
#include "model.h"
#include "mpd.h"

/* We have a global variable cur_screen which tells us which screen is currently running. 
	We decide which screen will be shown and how we react to key presses depending on this variable.
*/
static enum SCREEN cur_screen;

/* This is an array of all our screens
	We can index them with enum SCREEN
	See screen.h for more documentation
*/
static Screen screen_list[5];

/* Here we store the action wanted by a user keypress */
static UserReq user_request;

void mainscreen_keypress(int cur_key);
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


/* The current screen has been exited.
	Find a new screen !
*/
static void
screen_exit(enum SCREEN screen){
	screen_list[screen].screen_exit();
};

void
screen_enter(enum SCREEN screen){
	screen_list[screen].screen_enter();
	cur_screen = screen;
};
	
void 
switch_screen(enum SCREEN oldscreen, enum SCREEN newscreen){
	screen_exit(oldscreen);
	screen_enter(newscreen);
};


/* ------------------------------------------------------------------------------------------------- */

/* Initialize the main screen.
	Start with top menu system.
	Initialize all screens.
	We clear the whole screen.
	We start the PLAYING screen.
	// TODO We could check mpd status to see if we have a current song.
	// Only then is the PLAYING screen useful.
*/
void 
mainscreen_init(void) {
	
	init_font();
	set_font(MEDIUMFONT);	
		
	win_scroll_init();
	
	win_cursor_init();
	
	playing_screen_init( &(screen_list[PLAYING_SCREEN]) );
	playlist_screen_init( &(screen_list[PLAYLIST_SCREEN]) );
	tracklist_screen_init( &(screen_list[TRACKLIST_SCREEN]) );
	info_screen_init( &(screen_list[INFO_SCREEN]) );
	search_screen_init( &(screen_list[SEARCH_SCREEN]) );
	
	screen_enter(PLAYING_SCREEN);
	
	user_request.cmd = NO_CMD;

	set_keypress_handler(mainscreen_keypress);
};
	
/* This is called when the current task does not know which screen is shown, 
	so that switch_screen() can not be used.
	We simply exit the current screen and enter the new one.
*/
void
show_new_screen(enum SCREEN screen){
	screen_exit(cur_screen);
	screen_enter(screen);
};


/* This is the main key press handler.
	After a key press has been detected by the keyboard driver,
	the kernel calls this routine.
*/
void
mainscreen_keypress(int cur_key){
	
	user_request.cmd = NO_CMD;
	
	/* Some keys are handled independently of the current screen */	
	switch (cur_key){
		
	/* BETTY always shows the playlist screen */
		case KEY_Betty:
			switch_screen(cur_screen, PLAYLIST_SCREEN);
			return;

	/* The BLUE key enters the search screen */		
		case KEY_Blue:
			switch_screen(cur_screen, SEARCH_SCREEN);
			return;

	/* The EXIT key switches through the screens */
		case KEY_Exit:
			switch (cur_screen){
				case PLAYING_SCREEN:
					switch_screen(cur_screen, TRACKLIST_SCREEN);
					break;
				case TRACKLIST_SCREEN:
					switch_screen(cur_screen, PLAYLIST_SCREEN);
					break;
				case PLAYLIST_SCREEN:
					switch_screen(cur_screen, PLAYING_SCREEN);
					break;
				case INFO_SCREEN:
					switch_screen(cur_screen, PLAYLIST_SCREEN);
					break;
				default:
					switch_screen(cur_screen, PLAYLIST_SCREEN);
					break;
			};
			return;
			
		case KEY_Vplus:	
			/* Inform the controller that the user wants a volume increase */
			user_wants_volume_add(+5);
			break;
			
		case KEY_Vminus:
			/* Inform the controller that the user wants a volume decrease */
			user_wants_volume_add(-5);
			break;
		
	/* The first video text key toggles the serial debug output. */
		case KEY_VTX1:
			fDebug ^= 1;
			return;
		
		case KEY_Mute:
			user_toggle_mute();
			return;
	};
	
	switch(cur_screen){	

		case PLAYING_SCREEN:
			playing_keypress( &(screen_list[PLAYING_SCREEN]), cur_key, &user_request);	
			break;
			
		case PLAYLIST_SCREEN:
			playlist_keypress( &(screen_list[PLAYLIST_SCREEN]), cur_key, &user_request);
			break;
			
		case TRACKLIST_SCREEN:
			tracklist_keypress( &(screen_list[TRACKLIST_SCREEN]), cur_key, &user_request);
			break;
			
		case INFO_SCREEN:
			info_keypress( &(screen_list[INFO_SCREEN]), cur_key, &user_request);
			break;	
			
		case SEARCH_SCREEN:
			search_keypress( &(screen_list[INFO_SCREEN]), cur_key, &user_request);
			break;
				
		default:
			break;
	}
};
