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

#include "screen.h"
#include "window.h"
#include "keyboard.h"
#include "lcd.h"
#include "model.h"
#include "mpd.h"

/* ------------------------------------- Track List Menu ------------------------------------------ */
/* The row where the window list starts */
#define WL_START_ROW 0

/* The width of each window in pixels */
#define WL_WIDTH 128

/* The height of a small window */
#define WL_SMALL_HEIGHT 10

/* Number of entries in the window list 
	We have 160 pixels, so 1 title window, 10 normal windows and 1 big window will fit 
*/
#define WL_SIZE 12

/* Number of lines in scrolling window list */
#define PL_SIZE (WL_SIZE -1)

/* The window list */
static struct Window win[WL_SIZE];

#define WIN_TXT_SIZE 32
/* Text buffer for the windows */
static char win_txt[WL_SIZE][WIN_TXT_SIZE];

static scroll_list playlist_list;

/* The height of a non-selected window in the scroll list */
#define WL_NORMAL_HEIGHT 13

/* The height of a selected window in the scroll list */
#define WL_HIGH_HEIGHT 18


/* We are called when the playlist names have potentially changed (i.e they have been read in from MPD) */
void
view_playlists_changed(){
	scroll_list_changed(&playlist_list);
};


static void
screen_enter(){
	
	/* We always want to start with the first playlist name, because the user might have got lost previously. */
	scroll_list_start(&playlist_list, 0);

};

static void 
screen_exit(){
};

// Forward declarations
static int keypress(Screen *this_screen, int cur_key)	;
static int keypress_popup(Screen *this_screen, int cur_key);

void
playlist_screen_init(Screen *this_screen){
	int cur_start_row;
	
	this_screen->wl_size = WL_SIZE;
	this_screen->win_list = win;
	this_screen->screen_enter = screen_enter;
	this_screen->screen_exit = screen_exit;
	this_screen->keypress = keypress;

	cur_start_row = WL_START_ROW;	
	
	/* The first window is the title window */
	win_init(&win[0], cur_start_row, 0, WL_SMALL_HEIGHT, 128,0, win_txt[0]);
	win[0].font = SMALLFONT;
	win[0].fg_color = WHITE;
	win[0].bg_color = BLACK;
	win[0].flags |= WINFLG_CENTER;
	cur_start_row += WL_SMALL_HEIGHT;
	
	init_scroll_list(&playlist_list, &(win[1]), win_txt[1], WIN_TXT_SIZE, PL_SIZE, &mpd_playlistname_info, cur_start_row, playlists_range_set);
	
	win_new_text(&win[0], "All playlists");
};	

void
view_num_pl_changed(int new_num_pl){
	scroll_list_total_len(&playlist_list, new_num_pl);	
};

/* Betty popup key handler */
static int
keypress_popup(Screen *this_screen, int cur_key){
	switch (cur_key) {
		case KEY_Exit:
		case KEY_Betty:
		case KEY_Menu:
		case KEY_OK:
			popup_end();
			return NO_KEY;
			
		case KEY_B:
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
		case KEY_OK:
		case KEY_Info:
		case KEY_Betty:
		case KEY_Menu:
			popup_end();
			return NO_KEY;
			
		default:
			popup_end();
			break;						// give this key to normal screen handler
	};
	return cur_key;
};


static int
keypress(Screen *pl_screen, int cur_key){
		
	switch (cur_key) {

		case KEY_Betty:
		case KEY_Menu:
			popup("A\n\n"
				"B  Current\n   playlist\n\n"
				"C  Search\n\n"
				"D  Main Screen\n\n"
				"i  Info",
					0, keypress_popup);
			break;
			
		case KEY_Exit:	
			show_screen(PLAYING_SCREEN);
			break;
			
		case KEY_B:	
			show_screen(TRACKLIST_SCREEN);
			break;
			
		case KEY_C:	
			show_screen(SEARCH_SCREEN);
			break;

		case KEY_D:	
			show_screen(PLAYING_SCREEN);
			break;
									
		case KEY_Info:
			popup("\xB1 = Page Back\n"
				"\xB0 = Page FWD\n"
				"\xB2 = Up\n"
				"\xB3 = Down\n"
				"OK = Load\n"
				"      playlist", 
				  0, keypress_info_popup);
			break;
															
		case KEY_OK:
			user_wants_playlist(scroll_list_selected(&playlist_list) );
			show_screen(PLAYING_SCREEN);
			break;
			
		case KEY_Pplus:					
		case KEY_Up:
			scroll_list_up(&playlist_list);
			break;
			
		case KEY_Pminus:				
		case KEY_Down:
			scroll_list_down(&playlist_list);
			break;
			
		case KEY_Left:
			scroll_list_back(&playlist_list);
			break;
					
		case KEY_Right:
			scroll_list_fwd(&playlist_list);
			break;
			

		default:
			return cur_key;
	};
	return NO_KEY;
};

