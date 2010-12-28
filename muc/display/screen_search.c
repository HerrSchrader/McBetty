/*
    screen_search.c - User Interface

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
	User can enter search strings here
*/

#include "screen_search.h"

#include "screen.h"
#include "window.h"

/* Number of entries in the window list */
#define WL_SIZE 3

/* The window list */
static struct Window win[WL_SIZE];

#define WIN_TXT_SIZE 80
/* Text buffer for the windows */
static char win_txt[WL_SIZE][WIN_TXT_SIZE];

/* Some definitions to refer to specific windows by name */

/* Window to search a title */
#define title_win (win[0])

/* Window to search an artist */
#define artist_win (win[1])

/* Window to search an album */
#define album_win (win[2])


static void 
screen_enter(){
	lcd_fill(0x00);
	screen_visible(SEARCH_SCREEN, 1);
	screen_redraw(SEARCH_SCREEN);	
};


/* Initialize the search screen 
*/
void 
search_screen_init(Screen *this_screen) {
	this_screen->wl_size = WL_SIZE;
	this_screen->win_list = win;
	this_screen->screen_enter = screen_enter;
	
	win_init(&title_win, 0, 0, 40, 128, 1, win_txt[0]);
	title_win.font = BIGFONT;	
	title_win.flags |= WINFLG_CENTER;
	title_win.fg_color = WHITE;
	title_win.bg_color = BLACK;
	win_new_text(&title_win, "Title ");	
	
	win_init(&artist_win, 40, 0, 50, 128, 1, win_txt[1]);
	artist_win.font = MEDIUMFONT;	
	win_new_text(&artist_win, "Artist ");
		
	win_init(&album_win, 90, 0, 160-40-50, 128, 1, win_txt[2]);
	album_win.font = MEDIUMFONT;
	win_new_text(&album_win, "Album ");
	win_cursor_set(&album_win);
}

