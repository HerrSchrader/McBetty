/*
    screen_info.c - User Interface

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

/* Gives information to the user, i.e warnings and error messages
*/

#include "screen.h"
#include "window.h"
#include "keyboard.h"
#include "lcd.h"
#include "model.h"
#include "mpd.h"
#include "screen_info.h"

/* Number of entries in the window list */
#define WL_SIZE 3

/* The window list */
static struct Window win[WL_SIZE];

#define WIN_TXT_SIZE 80
/* Text buffer for the windows */
static char win_txt[WL_SIZE][WIN_TXT_SIZE];

/* Some definitions to refer to specific windows by name */

/* Window to show the current title */
#define title_win (win[0])

/* Window to show the current info, warning or error */
#define info_win (win[1])

/* Window to show possible help for user */
#define help_win (win[2])


static void 
screen_enter(){
	lcd_fill(0x00);
	screen_visible(INFO_SCREEN, 1);
	screen_redraw(INFO_SCREEN);	
};


static void 
screen_exit(){
	screen_visible(INFO_SCREEN, 0);
};

// Forward declaration
static void keypress(Screen *this_screen, int cur_key, UserReq *req)	;

/* Initialize the playing screen 
	We clear the whole screen.
*/
void 
info_screen_init(Screen *this_screen) {
	this_screen->wl_size = WL_SIZE;
	this_screen->win_list = win;
	this_screen->screen_enter = screen_enter;
	this_screen->screen_exit = screen_exit;
	this_screen->keypress = keypress;
	
	win_init(&title_win, 0, 0, 40, 128, 1, win_txt[0]);
	title_win.font = BIGFONT;	
	title_win.flags |= WINFLG_CENTER;
	title_win.fg_color = WHITE;
	title_win.bg_color = BLACK;
	win_new_text(&title_win, "Error ");	
	
	win_init(&info_win, 40, 0, 50, 128, 1, win_txt[1]);
	info_win.font = MEDIUMFONT;	
	info_win.flags |= WINFLG_CENTER;
	
	win_init(&help_win, 90, 0, 160-40-50, 128, 1, win_txt[2]);
	help_win.font = MEDIUMFONT;
	help_win.flags |= WINFLG_CENTER;
	win_new_text(&help_win, "Test Message");
	
}


/* Error handling is interesting.
	We detect somewhere (maybe in the model module) that some conditions exist, 
	which we have to tell the user.
	The user can sometimes do some remedial action (like loading a new playlist).
	But until he does that, the conditions still exist.
	We only want to inform him once about that. So we should not react to the level (error flag is TRUE),
	but to the edge (error flag just went high after it was low)!
	
	We make the following assumptions:
	- We do have only a small number of possible errors/warnings/notifications
		So we can use a variable (maybe 32 bits) as a bit vector
	- Errors can occur at different places in the code.
		No central routine can detect all error conditions without too much knowledge about the inner workings of some modules.
		Each module detects possible error conditions itself and sets the corresponding bit in the error variable.
		The same module also resets the bit when the error condition is no longer true.
		A central routine regularily checks the error variable against the previous state.
		When a bit went from 0 to 1 the corresponding error handling routine is called.
		The natural place to do this error checking is in the controller task where model_changed() is checked,
		because new errors are some kind of change in the internal model and they trigger similar actions, namely 
		changing the view (user interface). 
	- If multiple errors are pending, show only the most important.
*/

static uint8_t error_last = 0;
static uint8_t error = 0;

void set_error( uint8_t flag){
	error |= flag;
};
	
void clr_error( uint8_t flag){
	error &= ~flag;
};

static void
info_end_of_playlist(){
	win_new_text(&title_win, "Info");	
	win_new_text(&info_win, "End of playlist reached.");
	win_new_text(&help_win, "Select a new track or a new playlist.");
	show_new_screen(INFO_SCREEN);	
}


static void
info_playlist_empty(){
	win_new_text(&title_win, "Info");	
	win_new_text(&info_win, "Playlist is empty.");
	win_new_text(&help_win, "Select a new playlist or check mpd configuration.");
	show_new_screen(INFO_SCREEN);	
}

static void
info_mpd_dead(){
	win_new_text(&title_win, "Error");	
	win_new_text(&info_win, "Communication with MPD broken.");
	win_new_text(&help_win, "Check if MPD is still running and reachable via LAN.");
	show_new_screen(INFO_SCREEN);		
}

void detect_errors(){
	if ( (error & END_OF_PLAYLIST) && !(error_last & END_OF_PLAYLIST) ) {
		info_end_of_playlist();
	} 
	else if ( (error & PLAYLIST_EMPTY) && !(error_last & PLAYLIST_EMPTY) ) {
		info_playlist_empty();
	} else if ( (error & MPD_DEAD) && !(error_last & MPD_DEAD) ) {
		
		/*We try to solve the communication error by resetting our
		radio reception to a sane state.
		*/
		rx_reset();
		info_mpd_dead();
	} else if ( (error_last & MPD_DEAD) && !(error & MPD_DEAD) ) {
		// MPD recovered from an error
		show_new_screen(PLAYING_SCREEN);
	};
	error_last=error;
};

void 
keypress(Screen *this_screen, int cur_key, UserReq *req){
	switch (cur_key) {
		case KEY_OK:
			/* The user wants to leave this screen 
				If MPD_DEAD error we assume he has taken some steps to repair
				the problem. So we reset the error, so that we can show it again if
				necessary.
			*/
			error &= ~MPD_DEAD; error_last &= ~MPD_DEAD;
			model_set_last_response(system_time() - 10 * TICKS_PER_SEC);
			switch_screen(INFO_SCREEN, PLAYLIST_SCREEN);
			break;
			
		case KEY_VTX1:
			fDebug ^= 1;	
			break;
			
		default:
			break;
	};
};	


