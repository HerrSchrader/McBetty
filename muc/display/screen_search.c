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

#include "screen.h"
#include "window.h"
#include "keyboard.h"

#include "screen_search.h"

/* Number of entries in the window list 
	We have 160 pixels, so 1 title window, 10 normal windows and 1 search window will fit 
*/
#define WL_SIZE 12

/* Number of lines in scrolling window list */
#define RL_SIZE (WL_SIZE -3)

/* The window list */
static struct Window win[WL_SIZE];

#define WIN_TXT_SIZE 80
/* Text buffer for the windows */
static char win_txt[WL_SIZE][WIN_TXT_SIZE];

/* Some definitions to refer to specific windows by name */

/* Window to search an album */
#define title_win (win[0])

/* Window to enter the search string */
#define input_win (win[1])

/* Window showing the number of search reseults */
#define num_results_win (win[2])

static scroll_list results_list;


static void 
screen_enter(){
	lcd_fill(0x00);
	screen_visible(SEARCH_SCREEN, 1);
	screen_redraw(SEARCH_SCREEN);	
	win_cursor_set(&input_win, WIN_TXT_SIZE-1);
};

static void 
screen_exit(){
	win_cursor_set(NULL, WIN_TXT_SIZE-1);
	screen_visible(SEARCH_SCREEN, 0);
};

/* The height of a small window */
#define WL_SMALL_HEIGHT 10

/* The height of a non-selected window in the scroll list */
#define WL_NORMAL_HEIGHT 13

/* The height of a selected window in the scroll list */
#define WL_HIGH_HEIGHT 18

static char * 
get_result(int i){
	return "";
};
		
/* Initialize the search screen 
*/
void 
search_screen_init(Screen *this_screen) {
	int cur_start_row = 0;
	
	this_screen->wl_size = WL_SIZE;
	this_screen->win_list = win;
	this_screen->screen_enter = screen_enter;
	this_screen->screen_exit = screen_exit;

	win_init(&title_win, cur_start_row, 0, WL_SMALL_HEIGHT, 128, 1, win_txt[0]);
	title_win.font = SMALLFONT;	
	title_win.flags |= WINFLG_CENTER;
	title_win.fg_color = WHITE;
	title_win.bg_color = BLACK;
	win_new_text(&title_win, "Search Album");	
	cur_start_row += WL_SMALL_HEIGHT;
	
	win_init(&input_win, cur_start_row, 0, 20, 128, 1, win_txt[1]);
	input_win.font = MEDIUMFONT;	
	input_win.txt_offs_row = 3;
	win_new_text(&input_win, " ");
	cur_start_row += 20;
	
	win_init(&num_results_win, cur_start_row, 0, WL_SMALL_HEIGHT, 128, 0, win_txt[2]);
	num_results_win.font = SMALLFONT;	
	num_results_win.flags |= WINFLG_CENTER;
	num_results_win.fg_color = BLACK;
	num_results_win.bg_color = DARK_GREY;	
	win_new_text(&num_results_win, " ");
	cur_start_row += WL_SMALL_HEIGHT;
	
	init_scroll_list(&results_list, &(win[3]), win_txt[3], WIN_TXT_SIZE, RL_SIZE, &get_result, cur_start_row);

}

void 
view_results_changed(int num){
	win_new_text(&num_results_win, " results:");
};


void 
search_keypress(Screen *this_screen, int cur_key, UserReq *req){
	switch (cur_key) {
		case KEY_OK:
			user_set_search_string(input_win.txt);
			
			/* The user wants to leave this screen */
//			switch_screen(SEARCH_SCREEN, PLAYLIST_SCREEN);
			break;
			
		case KEY_Left:	
			win_cursor_input(CURSOR_LEFT);
			break;
			
		case KEY_Right:
			win_cursor_input(CURSOR_RIGHT);
			break;
			
		case KEY_Minus:
			win_cursor_input(CURSOR_BACKSPACE);
			break;
			
		case KEY_0:
			win_cursor_input(0);	
			break;
					
		case KEY_1:
			win_cursor_input(1);	
			break;
			
		case KEY_2:
			win_cursor_input(2);	
			break;
			
		case KEY_3:
			win_cursor_input(3);	
			break;
			
		case KEY_4:
			win_cursor_input(4);	
			break;
			
		case KEY_5:
			win_cursor_input(5);	
			break;
			
		case KEY_6:
			win_cursor_input(6);	
			break;				
					
		case KEY_7:
			win_cursor_input(7);	
			break;
			
		case KEY_8:
			win_cursor_input(8);	
			break;
			
		case KEY_9:
			win_cursor_input(9);	
			break;
				
		default:
			break;
	};
};	
