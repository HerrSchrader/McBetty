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
	User can enter search strings in this screen
*/

#include "global.h"
#include "kernel.h"
#include "timerirq.h"
#include "screen.h"
#include "window.h"
#include "keyboard.h"
#include "model.h"

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

static scroll_list result_list;

static task_id auto_search_task;

/* ### Automatic search task ###  */
/* This task is started when the search screen is entered and stopped on exit.
	It will check the value of the current search input window for any changes.
	If we have a stable change (i.e. search string is new and unchanged for 2 second,
	we start a search with this string.
*/
PT_THREAD (auto_search(struct pt *pt)){
	static struct timer tmr;
	static char search_string[WIN_TXT_SIZE];
	static int is_new;
	static int len;	// not used, but needed by strn_cpy_cmp()
	 
	PT_BEGIN(pt);
	
	search_string[0] = 0;
	is_new = 0;
	timer_add(&tmr, 10 * TICKS_PER_TENTH_SEC, 0);
	
	while(1){
		PT_WAIT_UNTIL(pt, ( timer_expired(&tmr) ));
		
		if (1 == strn_cpy_cmp(search_string, input_win.txt, WIN_TXT_SIZE -1, &len) ){
			// input string has not changed
			if (is_new) {
				user_set_search_string(input_win.txt);
			};
			is_new = 0;
		} else {
			is_new = 1;	
		};	
		timer_set(&tmr, 10 * TICKS_PER_TENTH_SEC, 0);
	};
	
	PT_END(pt);
};


static void
set_title_win(int t){
	if (t == FIND_TYPE_ARTIST)
			win_new_text(&title_win, "Search Artist");	
	
	if (t == FIND_TYPE_TITLE)
			win_new_text(&title_win, "Search Title");
		
	if (t == FIND_TYPE_ALBUM)
			win_new_text(&title_win, "Search Album");
};

static void 
screen_enter(){
	set_title_win(mpd_find_type());
	screen_redraw(SEARCH_SCREEN);	
	win_cursor_set(&input_win, WIN_TXT_SIZE-1);
	auto_search_task = task_add(&auto_search);
};

static void 
screen_exit(){
	task_del(auto_search_task);
	win_cursor_set(NULL, WIN_TXT_SIZE-1);
};

/* The height of a small window */
#define WL_SMALL_HEIGHT 10

/* The height of a non-selected window in the scroll list */
#define WL_NORMAL_HEIGHT 13

/* The height of a selected window in the scroll list */
#define WL_HIGH_HEIGHT 18


// Forward declaration
static int keypress(Screen *this_screen, int cur_key, UserReq *req)	;
static int keypress_popup(Screen *this_screen, int cur_key, UserReq *req)	;
	
/* Initialize the search screen 
*/
void 
search_screen_init(Screen *this_screen) {
	int cur_start_row = 0;
	
	this_screen->wl_size = WL_SIZE;
	this_screen->win_list = win;
	this_screen->screen_enter = screen_enter;
	this_screen->screen_exit = screen_exit;
	this_screen->keypress = keypress;
	
	win_init(&title_win, cur_start_row, 0, WL_SMALL_HEIGHT, 128, 1, win_txt[0]);
	title_win.font = SMALLFONT;	
	title_win.flags |= WINFLG_CENTER;
	title_win.fg_color = WHITE;
	title_win.bg_color = BLACK;
	win_new_text(&title_win, "Search Artist");	
	cur_start_row += WL_SMALL_HEIGHT;
	
	win_init(&input_win, cur_start_row, 0, 20, 128, 1, win_txt[1]);
	input_win.font = BIGFONT;	
	input_win.txt_offs_row = 3;
	win_new_text(&input_win, "");
	cur_start_row += 20;
	
	win_init(&num_results_win, cur_start_row, 0, WL_SMALL_HEIGHT, 128, 0, win_txt[2]);
	num_results_win.font = SMALLFONT;	
	num_results_win.fg_color = WHITE;
	num_results_win.bg_color = DARK_GREY;	
	win_new_text(&num_results_win, "");
	cur_start_row += WL_SMALL_HEIGHT;
	
	init_scroll_list(&result_list, &(win[3]), win_txt[3], WIN_TXT_SIZE, RL_SIZE, &mpd_result_info, cur_start_row, resultlist_range_set);

}

/* Number of results has changed */
void 
view_results_changed(int num){
	char newstr[50];
	char number[20];
	char *numstr;
	
	strlcpy (newstr, "Results:  ", sizeof(newstr));
	if (num > 50){
		strlcat(newstr, "> 50\n", sizeof(newstr));
		num_results_win.bg_color = LIGHT_GREY;	
	} else {
		numstr = get_digits(num, number, 0); 
		strlcat(newstr, numstr, sizeof(newstr));
		strlcat(newstr, "\n", sizeof(newstr));
		num_results_win.bg_color = DARK_GREY;
	};
	
	win_new_text(&num_results_win, newstr);	
	scroll_list_total_len(&result_list, num);
};


void
view_resultnames_changed(){
	// new results should show the cursor reset to the first position
	scroll_list_start(&result_list, 0);		// also calls scroll_list_changed()
};

static int
keypress_popup(Screen *this_screen, int cur_key, UserReq *req){
	switch (cur_key) {
		case KEY_Exit:
		case KEY_Betty:
		case KEY_Menu:
		case KEY_OK:
			popup_end(this_screen);
			return NO_KEY;
			
		case KEY_A:
		case KEY_B:
		case KEY_C:
		case KEY_D:
		case KEY_Info:
			popup_end(this_screen);
			break;						// give this key to normal screen handler
			
		default:
			return NO_KEY;
			break;
	};
	return cur_key;
};


static int
keypress_info_popup(Screen *this_screen, int cur_key, UserReq *req){
	switch (cur_key) {
		
		case KEY_Exit:
		case KEY_OK:
		case KEY_Info:
		case KEY_Menu:
		case KEY_Betty:
			popup_end();
			return NO_KEY;
			
		default:
			popup_end();
			break;						// give this key to normal screen handler
	};
	return cur_key;
};


static int 
keypress(Screen *this_screen, int cur_key, UserReq *req){
	switch (cur_key) {
		
		case KEY_Betty:
		case KEY_Menu:			
		case KEY_OK:
			popup("A Append to\n   playlist\n\nB Begin new\n   playlist\n\nC Clear search\n\nD Now playing\n\ni Info", 0, keypress_popup);
			break;
					
		case KEY_Exit:	
			show_screen(TRACKLIST_SCREEN);
			break;
			
			
		case KEY_A:
			user_find_add(scroll_list_selected(&result_list) );
			break;
			
		case KEY_B:
			user_tracklist_clr();	
			user_find_add(scroll_list_selected(&result_list) );
			break;	
								
		case KEY_C:	
			win_cursor_clr();
			break;
			
		case KEY_D:	
			show_screen(PLAYING_SCREEN);
			break;
			
		case KEY_Info:
			popup("\xB1 = Left\n"
				"\xB0 = Right\n"
				"\xB2 = Up\n"
				"\xB3 = Down\n"
				"0 .. 9 = Input\n"
				"-/-- = delete\n"
				"Red = Artist\n"
				"Green = Title\n"
				"Yellow = Album\n"
				"OK = Load\n"
				"      results", 
				  0, keypress_info_popup);
			break;
								
		case KEY_Pplus:	
		case KEY_Up:
			scroll_list_up(&result_list);
			break;
			
		case KEY_Pminus:	
		case KEY_Down:
			scroll_list_down(&result_list);
			break;
									
		case KEY_Red:
			if (mpd_find_type() != FIND_TYPE_ARTIST){
				mpd_set_find_type(FIND_TYPE_ARTIST);
				set_title_win(FIND_TYPE_ARTIST);
				user_set_search_string(input_win.txt);	// doing a new search resets the results !
			};		
			break;
			
		case KEY_Green:
			if (mpd_find_type() != FIND_TYPE_TITLE){
				mpd_set_find_type(FIND_TYPE_TITLE);
				set_title_win(FIND_TYPE_TITLE);
				user_set_search_string(input_win.txt);
			}
			break;
			
		case KEY_Yellow:
			if (mpd_find_type() != FIND_TYPE_ALBUM){
				mpd_set_find_type(FIND_TYPE_ALBUM);
				set_title_win(FIND_TYPE_ALBUM);
				user_set_search_string(input_win.txt);
			}
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
			return cur_key;
	};
	return NO_KEY;
};	
