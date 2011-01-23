/*
    screen_tracklist.c - User Interface

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

#include "screen.h"
#include "window.h"
#include "keyboard.h"
#include "lcd.h"
#include "model.h"
#include "mpd.h"


/* We have an externally maintained cache (by model.c) with the track names of 20 or 30 tracks.
	We are informed about new information when mpd calls our view_tracklist_changed() routine.
	We only have a limited number of windows to show the track list.
	So we only show a small part of the current track list.
	A good guess is to show the last two tracks, the current track and then the next tracks as far as our windows
	allow and as far as the tracklist goes.
	The user can scroll within the shown list.
	
	We tell the model what tracks we show with tracklist_range_set(start,end)
	Whenever the contents of the given tracklist range have changed, we are informed 
	by calling our view_tracklist_change. We could be given parameters telling us which range has changed
	to allow efficient uodating of our windows.
*/	


/* 
	We should implement a generic scroll list here, because we need it for the playlist window also.
	
	We assume a list of information, which can be much larger than the number of windows shown on screen.
	We need a function which gives us this information as a string when given an index. 
	
	On screen we show a consecutive subsection of this list.
	
	
	A scroll list consists of a list of windows shown on screen.
	Each window shows a string.
	We associate an index with each window. This index points to a list where all strings are stored.
	
	The Windows are ordered from top to bottom.
	One of the windows is the currently selected one. It is shown in a bigger font and with a border.
	Pressing OK returns the index associated with this window.

	We have to deal with 2 kind of indices here, which complicates matters.
	One index is simply numbering the windows from 0 up to WL_SIZE-1.
	The other index points to the information which shall be shown.
	We call the first the win_idx and the second info_idx.
	The correspondence between the two indices varies according to the subsection shown on screen.
	
*/
	


/* ------------------------------------- Track List Menu ------------------------------------------ */
/* The row where the window list starts */
#define WL_START_ROW 0

/* The width of each window in pixels */
#define WL_WIDTH 128

/* The height of a small window */
#define WL_SMALL_HEIGHT 10

/* The height of a non-highlighted window in the window list */
#define WL_NORMAL_HEIGHT 13

/* The height of a highlighted window in the window list */
#define WL_HIGH_HEIGHT 18

/* Number of entries in the window list 
	We have 160 pixels, so 10 normal windows and 1 big window will fit as well as a title windows
*/
#define WL_SIZE 12

/* Number of lines in the track scroll list */
#define TL_SIZE (WL_SIZE - 1) 

/* The window list */
static struct Window win[WL_SIZE];

#define WIN_TXT_SIZE 64
/* Text buffer for the windows */
static char win_txt[WL_SIZE][WIN_TXT_SIZE];

static scroll_list track_list;


void
view_tracklist_changed(){
	scroll_list_changed(&track_list);
};


void
view_pl_length_changed(int len, int added){
	char str[100];
	char num_string[12];
	
	strlcpy(str, "",100);
	
	if (added != 0){
		strlcat(str, get_digits(added, num_string, 0), 100);
		strlcat(str, " songs added.\n\n", 100);	
	}
	if (len >= 0){
		if (0 == len)
			strlcat(str, " Playlist is\n now empty.\n", 100); 
		else {
			strlcat(str, get_digits(len, num_string, 0), 100);
			strlcat(str, " songs\n in playlist\n", 100);
		};
	};
	if ( (added != 0) || (len >= 0))
		view_message(str, 4 * TICKS_PER_SEC);
	
	scroll_list_total_len(&track_list, len);

}

static void
screen_enter(){
	int cur_song;
	int start_sel;
	
	/* We try to get the index of the current song, if there is any 
		because we want to adjust our track list range screen with the current song in the selected window.
	*/
	cur_song = max (0, mpd_get_pos());

// utterly complicated
#if 0	
	/* We like to start our list with the last 2 songs before the current one. */
	if (cur_song >= 2){
		track_list.first_info_idx = cur_song - 2;
		start_sel = 2;
	} else {
		track_list.first_info_idx = cur_song;
		start_sel = 0;
	};
	
	/* If we are near the end of the playlist, don't show mostly empty windows.
		Adjust start.
	*/
	if (mpd_tracklist_last() >= 0){
		if ((mpd_tracklist_last() - track_list.first_info_idx + 1) < track_list.num_windows){
			track_list.first_info_idx =  max( 0, mpd_tracklist_last() - track_list.num_windows + 1);
			start_sel = max(0, cur_song - track_list.first_info_idx);
		};
	};

	
	/* We always want to startup showing the current song in the selected window */
	select(&track_list, start_sel);

	// Tell model which range we want
	tracklist_range_set( track_list.first_info_idx, last_info_idx(&track_list) );
	
	/* Redraw the scroll list */
	scroll_list_changed(&track_list);
#endif	
	
	/* We always want to start with the first playlist name, because the user might have got lost previously. */
	scroll_list_start(&track_list, 0);	
};


static void 
screen_exit(){
};

// Forward declarations
static int keypress(Screen *this_screen, int cur_key)	;
static int keypress_popup(Screen *this_screen, int cur_key);

void
tracklist_screen_init(Screen *this_screen){
	int cur_start_row;
	
	this_screen->wl_size = WL_SIZE;
	this_screen->win_list = win;
	this_screen->screen_enter = screen_enter;
	this_screen->screen_exit = screen_exit;
	this_screen->keypress = keypress;
	
	cur_start_row = WL_START_ROW;	
	
	/* The first window is a help window */
	win_init(&win[0], cur_start_row, 0, WL_SMALL_HEIGHT, 128,0, win_txt[0]);
	win[0].txt = win_txt[0];
	win[0].font = SMALLFONT;
	win[0].fg_color = WHITE;
	win[0].bg_color = BLACK;
	win[0].flags |= WINFLG_CENTER;
	cur_start_row += WL_SMALL_HEIGHT;
	
	init_scroll_list(&track_list, &(win[1]), win_txt[1], WIN_TXT_SIZE, TL_SIZE, &track_info, cur_start_row, &tracklist_range_set);
	
	win_new_text(&win[0], "Current playlist");
	

};	

static int
keypress_popup(Screen *this_screen, int cur_key){
	switch (cur_key) {
		case KEY_Exit:
		case KEY_Betty:
		case KEY_Menu:
		case KEY_OK:
			popup_end();
			return NO_KEY;
			
		case KEY_A:
		case KEY_C:
		case KEY_D:
		case KEY_Info:
			popup_end();
			break;						// give this key to normal screen handler
			
		default:
			return NO_KEY;
			break;
	};
	return cur_key;
};

static int
keypress_info_popup(Screen *this_screen, int cur_key){
	switch (cur_key) {
		
		case KEY_Exit:
		case KEY_Betty:
		case KEY_Menu:
		case KEY_Info:
			popup_end();
			return NO_KEY;
			
		default:
			popup_end();
			break;						// give this key to normal screen handler
	};
	return cur_key;
};

/* Handle user input (key presses) 
	We have 4 possible responses:
	- Close this screen and switch to another one.
	- Change some of the visible elements on screen
	- Generate user_cmd and user_arg to change something in mpd
	- pass the key through to PLAYING screen
*/
static int
keypress(Screen *track_screen, int cur_key){
		
	switch (cur_key) {

		case KEY_Betty:
		case KEY_Menu:
			popup("A  All\n   playlists\n\nB\n\nC  Search\n\nD  Now playing\n\ni  Info", 0, keypress_popup);
			break;
			
		case KEY_Exit:	
			show_screen(PLAYLIST_SCREEN);
			break;
			
		case KEY_A:	
			show_screen(PLAYLIST_SCREEN);
			break;
			
		case KEY_C:	
			show_screen(SEARCH_SCREEN);
			break;

		case KEY_D:	
			show_screen(PLAYING_SCREEN);
			break;
			
		case KEY_Info:
			popup("OK = Play Song\n"
				"\xB1 = Page Back\n"
				"\xB0 = Page FWD\n"
				"\xB2 = Down\n"
				"\xB3 = Up\n"				
				"\xAD = Clear\n     playlist", 
				  0, keypress_info_popup);
			break;
																				
		case KEY_OK:
			// NOTE info-idx could return -1 if invalid, user_wants_song can handle that 
			user_wants_song ( scroll_list_selected(&track_list) );
			show_screen(PLAYING_SCREEN);
			break;
		
		case KEY_Pplus:	
		case KEY_Up:
			scroll_list_up(&track_list);
			break;
			
		case KEY_Pminus:	
		case KEY_Down:
			scroll_list_down(&track_list);
			break;
			
		case KEY_Left:
			scroll_list_back(&track_list);
			break;
					
		case KEY_Right:
			scroll_list_fwd(&track_list);
			break;
			
		// This clears the current tracklist
		case KEY_VTX2:
			user_tracklist_clr();	
			break;
			

		default:
			return cur_key;
	};
	return NO_KEY;
};




