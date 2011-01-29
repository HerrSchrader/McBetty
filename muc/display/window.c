 /*
    window.c - window handling and drawing

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

#include "window.h"
#include "fonty.h"
#include "keyboard.h"
#include "lcd.h"
#include "timerirq.h"
#include "mpd.h"
#include "screen.h"

/* 
TODO comment no longer valid
These are the window manager functions.
The window manager knows all about the current layout of the screen and handles it. 

The windows are organized in a list (an array).
So we may do some operations for all windows by iterating through this list.
Might as well be a linked list or something.
*/

#define WIN_DEFAULT_TXT_SIZE 80

/* Draw a 1 pixel wide black border at the edge of a window. */
void
win_draw_border(struct Window *win){
	if ( (win->border) && (win->flags & WINFLG_VISIBLE) )
		draw_rect(win->start_col, win->start_row, win->width, win->height, 1, BLACK);	
};
	

/* Initializes a window.  
	Each window is NOT visible by default and is assumed to be a TEXT type window
	This routine just sets some sensible defaults for a window.
*/
void 
win_init(struct Window *win, uint8 row, uint8 col, uint8 height, uint8 width, uint8 border, char *txt){
	win->start_row = row;
	win->start_col = col;
	win->height = height;
	win->width = width;
	win->fg_color = BLACK;
	win->bg_color = WHITE;
	win->border = border;
	win->flags = WINFLG_TEXT;
	win->fits = 1;
	win->scroll = 0;
	win->font = MEDIUMFONT;
	win->buffer_size = WIN_DEFAULT_TXT_SIZE;
	win->cur_char = 0;
	win->txt = txt;
	txt[0] = '\0';
	win->txt_offs_row = 1;
	win->txt_offs_col = 2;
	win->cur_col = win_txt_col(win);
};


/* Clears visible content of window to background color.
	Normally this means to set the window to the background color.
	But if window type is WINFLG_FRAME and WINFLG_HIDE is set, the foreground color is used to clear the window.
	If clr_border is 1, the border (if any) will also be cleared.
*/
void 
win_clear(struct Window *win, int clr_border){
	int border = clr_border ? 0 : win->border;
	int frame = (win->flags & WINFLG_FRAME) ? 1 : 0;
	int color = ((win->flags & WINFLG_FRAME) && (win->flags & WINFLG_HIDE)) ? win->fg_color : win->bg_color;
	
	if (! (win->flags & WINFLG_VISIBLE)) return;
	
	draw_block(win->start_row+border+frame, win->start_col+border+frame, 
			  win->width-2*border-2*frame, win->height-2*border-2*frame, color);
};


/* Draw the current character at the current text position in a window 
	Increaes cur_char and cur_col if character did fit.
	After end of text draws 4 spaces and then resets cur_char to start of text.
	Returns 1 if the character did fit into the window, else 0.
	Assumes that the font is already set.
*/
static uint8_t 
draw_cur_char(struct Window *win){
	uint8_t res;
	char ch;
	
	/* If we are past the text length, draw space */
	if (win->cur_char >= win->text_len) ch = ' ';
	else ch = win->txt[win->cur_char];
	
	res = draw_char(win_txt_row(win), win->cur_col, ch, win->fg_color, win->bg_color,  txt_col_lim(win) - win->cur_col ); 
	
	// if it does not fit completely, return 0.
	if (res == 0) return 0;
	
	// if it fits completely in remaining window space,
	// increase current character column
	win->cur_col += res;
	
	/* select next character */
	/* After we have drawn 4 spaces beyond the current length, we continue with the beginning of the text */
	if (++win->cur_char >= win->text_len + 4)
		win->cur_char = 0;
	
	return 1;
};

/* 	Clear window (except border) and draw the contents completely (except border) new. Use text in win->txt
	Sets win->fits to 1 if the text fits in window, else sets it to 0.
	Text is centered in window (horizontally as well as vertically)
*/
static void
win_center_text(struct Window *win){
	int txt_height, txt_offs, cur_row, col_offs;
		
	if (win->flags & WINFLG_LFTADJ) 
		col_offs = 2;
	else
		col_offs = 0;

	/* center vertically */
	txt_height = draw_text_space(win->txt, win_txt_row(win), win->start_col + win->border + col_offs, 
								 win->width - 2*win->border - 2*col_offs, win->height - 2*win->border,
		  						win->fg_color, win->bg_color, 1, (win->flags & WINFLG_LFTADJ) );

	txt_offs = ( win_txt_height(win) - txt_height) / 2;
	
	if (txt_offs >= 0)
		cur_row  = win_txt_row(win) + txt_offs;
	else 
		cur_row  = win_txt_row(win);
	
	 draw_text_space(win->txt, cur_row, win->start_col + win->border + col_offs, 
					 win->width - 2*win->border - 2*col_offs, win->height - 2*win->border,
	   				win->fg_color, win->bg_color, 0, (win->flags & WINFLG_LFTADJ) );

};

/* 	Clear window (except border) and draw the contents completely (except border) new. Use text in win->txt
	Sets win->fits to 1 if the text fits in window, else sets it to 0.
	Honors WINFLG_CENTER, WINFLG_VISIBLE and WINFLG_HIDE.
*/
void 
win_draw_text(struct Window *win){
	if (! (win->flags & WINFLG_VISIBLE)) return;
	if (win->txt == NULL) return; 	
	win_clear(win, 0);
	if (win->flags & WINFLG_HIDE) return;
	
	set_font(win->font);
	
	if (win->flags & WINFLG_CENTER)
		return win_center_text(win);
	
	win->cur_char = 0;
	win->cur_col = win_txt_col(win);
	win->fits = 1;
	while (win->cur_char < win->text_len){
		if (draw_cur_char(win) == 0) {
			win->fits = 0;
			break;
		};
	};
};
		

/* We are given a new text 
	If it is different from current text in window, 
	copy text to window and redraw window
*/
void 
win_new_text(struct Window *win, char *s){
	/* We copy the new text to our internal buffer with clipping if necessary
		We also check if the text currently in buffer is identical
	*/
	if ( strn_cpy_cmp(win->txt, s, win->buffer_size - 1, &(win->text_len)) != 0)
		return;			// the two strings are identical, do nothing

	win_draw_text(win);
};

void
win_redraw(struct Window *win){
	if (! (win->flags & WINFLG_VISIBLE)) return;
	win_draw_border(win);
	if (win->flags & WINFLG_TEXT)
		win_draw_text(win);
	else if (win->flags & WINFLG_BAR){
		draw_progressbar(win->start_row+1, win->start_col, win->height-2, win->fg_color, win->bg_color, 100, 0);
		draw_progressbar(win->start_row+1, win->start_col, win->height-2, win->fg_color, win->bg_color, 0, win->cur_char);
	} else if (win->flags & WINFLG_RAMP){			
			trianglebar_border(win->start_row+1, win->start_col+1, win->width-2, win->height-2, win->fg_color, win->bg_color);
			draw_trianglebar(win->start_row+1, win->start_col+1, win->width-2, win->height-2, win->fg_color, win->bg_color, 0, win->cur_char);
	};
};

/* ------------------------------ Windows with horizontally scrolling text ------------------------------------------------------- */

/* ### Scroll Task ###  
	Some windows might need scrolling to show their full content.
	This task regularily (after a timer has expired) checks each window in turn if it needs scrolling.
	If so, the window is scrolled and the task yields.
	If not, we wait for next timer expiration.
	So we make sure that at most 1 window is scrolled before we are yielding to minimize the time spent in this task.

	TODO If the font of a window has changed, check again if the text fits!

	We do not check all available windows, that would be a logistic nightmare.
	It would also look bad on screen.
	We keep a small list (say 10) of windows to check.
	If you want a window to scroll, add it with win_scroll() and remove it with win_unscroll().

	TODO scrolling currently looks blurred.
	TODO improve that (maybe scrolling single characters at a time ?)
*/
#define SCROLL_LIST_MAX 10
static struct Window *hor_scroll_list[SCROLL_LIST_MAX + 1];

#define SCROLL_OFFSET 8
static void 
win_do_scroll(struct Window *win){ 	
	int res;
	
	set_font(win->font);
	
	scroll(win_txt_row(win), win_txt_col(win), win_txt_width(win), SCROLL_OFFSET);
	
	if (win->cur_col - SCROLL_OFFSET >= 0)
		 win->cur_col -= SCROLL_OFFSET;
	else
		win->cur_col = 0;
	
	do {
		res = draw_cur_char(win);
	} while (res);
};	

/* Insert a window into the scroll list (if a place is free) */
void 
win_scroll(struct Window *win){
	int i;
	for (i=0; i < SCROLL_LIST_MAX; i++){
		if (hor_scroll_list[i] == NULL){
			hor_scroll_list[i] = win;
			return;
		};
	};
};

/* Remove a window from the scroll list 
	It is safe to try to remove a window which is not in the list
*/	
void 
win_unscroll(struct Window *win){
	int i;
	for (i=0; i < SCROLL_LIST_MAX; i++){
		if (hor_scroll_list[i] == win){
			hor_scroll_list[i] = NULL;
			return;
		};
	};
};

#define SCROLL_PERIOD (8 * (TICKS_PER_TENTH_SEC))

/* ### Scroll Task ###  */
PT_THREAD (win_scroll_all(struct pt *pt)){
	static int i;
	static struct timer scroll_tmr;
	
	PT_BEGIN(pt);
		
	timer_add(&scroll_tmr, SCROLL_PERIOD, SCROLL_PERIOD);
	while(1){
		for (i=0; i < SCROLL_LIST_MAX; i++){
			PT_WAIT_UNTIL(pt, timer_expired(&scroll_tmr));
			if (hor_scroll_list[i] == NULL) continue;
			if (! (hor_scroll_list[i]->flags & WINFLG_VISIBLE)) continue; 
			if ( ! hor_scroll_list[i]->fits ){
				win_do_scroll(hor_scroll_list[i]);
				PT_YIELD(pt);
			};
		};
		/* We have checked (and potentially scrolled each window). Timer can expire again. */
		scroll_tmr.expired=0;
	};
	PT_END(pt);
};

void
win_scroll_init(){
	int i;
	for (i=0; i < SCROLL_LIST_MAX; i++)
		hor_scroll_list[i] = NULL;
	
	task_add(&win_scroll_all);
};	

/* ---------------------------------------------- Vertical Scroll List ---------------------------------------- */
/* 
	An array of windows can form a scroll list, i.e. the user can scroll in this list.
	The list is showing strings.
	The idea is as follows:
	The model has a large sequential list of strings (which can change).
	A scroll list can show a small portion of these strings.
	The user can choose which portion to show.
	Indices into the list of the model are called positions (or pos) for clarity.
	
	We need the following interaction with the model here:
	- a function (info_text()) which returns the string correspondinvoid init_scroll_list(scroll_list *sl, struct Window *pwl, char *win_txt, int win_txt_len, int nw, char * (*info_text) (), int start_row );g to a given pos
	- a function (update_info() ) which is called by the model if the text of a given pos
		has changed, currently only implemented in scroll_list_update()
	- a function which tells us the end of the model list, because the user should not
		scroll past this end 
	- a function which tells us the start of the model list, because the user must not
		scroll before this start
		We do not implement this function, because for all our current lists we have an absolute start
		value of 0!
	- a function which tells us if the start or end of the model list have changed 
	- a function range_set which tells the model the begin and end pos of the info 
		that we show on screen, so that the model can get those infos for us	
*/

/* The height of a non-selected window in the scroll list */
#define WL_NORMAL_HEIGHT 13

/* The height of a selected window in the scroll list */
#define WL_HIGH_HEIGHT 18

/* The given window has its properties set so that it is selected */
static void 
select_win(struct Window *w ){
	w->font = BIGFONT;
	w->height = WL_HIGH_HEIGHT;
	w->border = 1;
	win_scroll(w);		// enable horizontal scrolling
};

/* The given window has its properties set so that it is not selected */
static void
unselect_win(struct Window *w ){
	w->font = MEDIUMFONT;
	w->height = WL_NORMAL_HEIGHT;
	w->border = 0;
	win_unscroll(w);		// disable horizontal scrolling
};

/* The selected window is moved one line up */
static void
sel_win_up(scroll_list *sl){
	if (sl->sel_win >= 1){
		unselect_win( & (sl->wl[sl->sel_win]) );
		sl->wl[sl->sel_win].start_row = sl->wl[sl->sel_win - 1].start_row + WL_HIGH_HEIGHT;
		sl->sel_win--;
		
		select_win( & (sl->wl[sl->sel_win]) );
						
		win_redraw(& (sl->wl[sl->sel_win]) );
		win_redraw(& (sl->wl[sl->sel_win + 1]) );
	};
};

/* The selected window is moved one line down */
static void
sel_win_down(scroll_list *sl){
	if (sl->sel_win < (sl->num_windows - 1) ){
		
		unselect_win( & (sl->wl[sl->sel_win]) );
		
		sl->sel_win++;
		select_win( & (sl->wl[sl->sel_win]) );
		
		sl->wl[sl->sel_win].start_row = sl->wl[sl->sel_win -1].start_row + WL_NORMAL_HEIGHT;
		win_redraw(& (sl->wl[sl->sel_win]) );
		win_redraw(& (sl->wl[sl->sel_win - 1]) );
	};
};

/* Set a specific window as the selected one */
static void 
select(scroll_list *sl, int sel){
	while ( sl->sel_win < sel)
		sel_win_down(sl);
		
	while ( sl->sel_win > sel)
		sel_win_up(sl);
};


	
/* Index of last info text that we currently show in our list */
static inline int 
last_shown_pos(scroll_list *sl){
	return (sl->first_shown_pos + sl->num_windows -1); 
};	


/* Gets the info pos corresponding to a given window index 
	win_idx must be from 0 ... num_windows -1 
*/
static inline int
info_pos(scroll_list *sl, int win_idx){	
	return (sl->first_shown_pos + win_idx);
};

	
/* We are called when the scroll list has potentially changed 
	Updates the info in all windows of the scroll list (sets new text in window).
*/
void
scroll_list_changed(scroll_list *sl){
	int i;
	char *info;
	struct Window *pwin;

	for (i=0, pwin = sl->wl; i < sl->num_windows; pwin++, i++){
		info = sl->info_text( info_pos(sl, i) );
		win_new_text( pwin, info);
	}
};

/* Returns the positional index corresponding to the selected window
	Used by the view to tell the controller which item the user selected
*/
int 
scroll_list_selected(scroll_list *sl){
	return info_pos(sl, sl->sel_win);
};

/*
	The position of the currently selected info is decremented,
	i. e. the selected window is moved up towards the top of the screen
	If we are already at the top of the window list, we must move
	the text lines down! 
*/
void 
scroll_list_up(scroll_list *sl){
	if (sl->sel_win > 0)
		sel_win_up(sl);
	else {					// We want to scroll past the upper end of the window list
		if ( info_pos(sl, 0) > sl->first_pos){		// valid pos
			sl->first_shown_pos--;
			sl->range_set(sl->first_shown_pos, last_shown_pos(sl) );
			scroll_list_changed(sl);
		};
	};
};

/*
	The position of the currently selected info is incremented,
	i. e. the selected window is moved down towards the bottom of the screen
	If we are already at the bottom of the window list, we must move
	the text lines up! 
*/
void
scroll_list_down(scroll_list *sl){
	if (sl->sel_win < (sl->num_windows - 1)) {
		
		// here we must check if we have reached the absolute end of the infos
		if ( (sl->last_pos < 0) || (scroll_list_selected(sl) < sl->last_pos) )
			sel_win_down(sl);

	} else {					// We want to scroll past the lower end of the window list
		// here we must check if we have reached the absolute end of the infos
		if ( (sl->last_pos < 0) || (scroll_list_selected(sl) < sl->last_pos) ){
			sl->first_shown_pos++;
			sl->range_set(sl->first_shown_pos, last_shown_pos(sl) );
			scroll_list_changed(sl);
		}
	};	
};

/* We want to show a completely new "page" of infos,
	i.e. the scroll list first position is incremented by the number of windows in it
*/
void
scroll_list_fwd(scroll_list *sl){
	int new_first;
	
	// This will be our new first shown position
	new_first = sl->first_shown_pos+sl->num_windows;
	// Maybe it is too high (past total end of information)
	if (sl->last_pos >= 0){
		if (new_first > sl->last_pos)
			return;
	};
	
	sl->first_shown_pos = new_first;
	sl->range_set(sl->first_shown_pos, last_shown_pos(sl) );
	
	// Now maybe our selected window is too high
	if (sl->last_pos >= 0){
		if (scroll_list_selected(sl) > sl->last_pos)
			select(sl, (sl->last_pos - sl->first_shown_pos));	
	}
	scroll_list_changed(sl);
};

/* We want to show a completely new "page" of infos,
	i.e. the scroll list first position is decremented by the number of windows in it
*/
void
scroll_list_back(scroll_list *sl){
	int new_first;
	
	// This will be our new first shown position
	new_first = sl->first_shown_pos - sl->num_windows;	
	// Maybe it is too low (before start of information)
	if (new_first < sl->first_pos){
		new_first = sl->first_pos;
		select(sl, 0);				// visual clue to the user that we are at top of list
	};
		
	sl->first_shown_pos = new_first; 
	sl->range_set(sl->first_shown_pos, last_shown_pos(sl) );
	scroll_list_changed(sl);	
};

/*
	We tell the scroll list that we want pos to be the first shown info
	We assume that all infos might have changed and update the windows
*/
void
scroll_list_start(scroll_list *sl, int pos){
	sl->first_shown_pos = max(0,pos);				// pos below 0 makes no sense
	sl->range_set(pos, pos + sl->num_windows - 1);
	select(sl, 0);	
	scroll_list_changed(sl);	
};

/* 
	set the total length of the given scroll list 
	-1 means unknown
	0 is the empty list. We simply pretend to have one element,
		otherwise it would make no sense to display the list
*/
void
scroll_list_total_len(scroll_list *sl, int length){
	if (length <= 0)
		sl->last_pos = length;
	else
		sl->last_pos = length -1;
	
	// We should not have our starting value outside of the limit
	if ( (sl->last_pos >= 0) && (sl->first_shown_pos > sl->last_pos) )
		scroll_list_start(sl, sl->last_pos);							// this will call scroll_list_changed(sl)
	else
		scroll_list_changed(sl);
};

 
 /*
Parameters:
	sl = pointer to scroll_list structure
	pwl = pointer to the array of windows belonging to the scroll_list
	win_txt = pointer to array with space for window texts
	win_txt_len = size of each win_txt entry
	nw = number of windows in the scroll_list
	(*info_text)() = function which returns the text of a specified info index
	start_row = where shall the scroll_list start on screen
*/
void 
init_scroll_list(scroll_list *sl, struct Window *pwl, char *win_txt, int win_txt_len, int nw, 
				 char * (*info_text) (), int start_row, void (*range_set) ()  ) {
	int i;
	struct Window *pwin;

	sl->wl = pwl;
	sl->num_windows = nw;
	sl->sel_win= 0;
	sl->info_text = info_text;
	sl->range_set = range_set;
	sl->first_pos = 0;
	sl->last_pos = -1;
	sl->first_shown_pos = 0;
	
	/* These windows are scroll list lines */
	for (i=0, pwin = sl->wl; i < sl->num_windows; pwin++, i++){

		if (i==sl->sel_win){
			/* The selected window is bigger than usual */ 	
			win_init(pwin, start_row, 0, WL_HIGH_HEIGHT, 128, 1, &(win_txt[i*win_txt_len]));
			select_win(pwin);
		} else {
			win_init(pwin, start_row, 0, WL_NORMAL_HEIGHT, 128, 0, &(win_txt[i*win_txt_len]));
			unselect_win(pwin);
		};
		start_row += pwin->height;
		/* Every second window has a different background color */
		if (0 == (i & 1)) pwin->bg_color = LIGHT_GREY;
	};
};

/* --------------------------------------------- Text Input Window ------------------------------------- */
/*
We need a blink task. Cursor blinks in selected window at 1|2 HZ
*/
#define BLINK_PERIOD (5 * TICKS_PER_TENTH_SEC)

#define WAIT_KEY_TIME (25 * TICKS_PER_TENTH_SEC)

/* We have at most one window where the cursor blinks */
static struct Window *pcursor_win = NULL;

/* Maximum length of text for this window that we allow 
	There is at least room for another '\0' character in the array 
*/
static int max_txt_len;

static int cursor_pos = 0;				// position of cursor within text string (0 == first character)
static int cursor_on = 0;				// is cursor currently on (1 = black) or off

static int last_key = -1;
static int key_cnt = -1;

#define cursor_col(p) (char_start_col(pcursor_win->txt, p))

/* 
	When this timer expires, the user has not pressed a key for some time
	Used to move the cursor to the next position automatically.
*/ 
static struct timer char_tmr;

/* 
	Clear the input window. Reset cursor to start position. 
	Stop char_tmr.
*/
void
win_cursor_clr(){
	if (NULL == pcursor_win) 
		return; 
	pcursor_win->txt[0] = '\0';
	pcursor_win->text_len = 0;
	cursor_pos = 0;
	key_cnt = -1;
	last_key = -1;
	timer_stop(&char_tmr);
	win_draw_text(pcursor_win);	
}

/* 
	We do not want to redraw our input window every time the cursor blinks.
	We only redraw the character under the cursor.
	For that we have to known the position in the text at which the cursor is.
	If the cursor is past the last character in the window text, we draw a space.
*/	
static uint8_t
draw_cursor(struct Window *win, uint8_t cursor_pos, uint8_t reversed){
	char ch;
	int cc;
	
	set_font(win->font);			// necessary !
	cc = cursor_col(cursor_pos);
	
	/* If we are past the text length, draw space */
	if (cursor_pos >= win->text_len) ch = ' ';
	else ch = win->txt[cursor_pos];
	
	if (reversed)
		draw_char(win_txt_row(win), win_txt_col(win)+ cc, ch, win->bg_color, win->fg_color, txt_col_lim(win) - cc ); 
	else	
		draw_char(win_txt_row(win), win_txt_col(win)+ cc, ch, win->fg_color, win->bg_color, txt_col_lim(win) - cc ); 
	
	return 1;
};

/* Returns 1 if cursor position could be decremented */
static int
dec_cursor(){
	if (cursor_pos <= 0) 
		return 0;
	
	/* Clear old cursor */
	draw_cursor(pcursor_win, cursor_pos, 0);
	
	/* Draw new cursor at new position */
	cursor_on = 1;
	cursor_pos--;
	draw_cursor(pcursor_win, cursor_pos, cursor_on);
	return 1;	
};

/* Returns 1 if cursor could be advanced */
static int
advance_cursor(){
	/* We must not move beyond end of string */
	if ('\0' == pcursor_win->txt[cursor_pos])
		return 0;
	
	/* Clear old cursor */
	draw_cursor(pcursor_win, cursor_pos, 0);
	
	/* Draw new cursor at new position */
	cursor_on = 1;
	cursor_pos++;
	draw_cursor(pcursor_win, cursor_pos, cursor_on);
	return 1;
};



/* 
	Store a character at pos p in the window text 
	If p is beyond the current text, the character is appended at the
	end (if possible)
*/
static void
store_char(struct Window *win, int p, char c){
	if (p >= max_txt_len)
		return;
	
	/* Clear old cursor */
	cursor_on = 0;
	draw_cursor(win, cursor_pos, 0);
			
	if (p >= win->text_len){ 
		win->txt[win->text_len++] = c;
		win->txt[win->text_len] = '\0';
	} else {
		win->txt[p] = c;
	};
	
	win_draw_text(win);	
};

/*
	Delete a character at position p in the window text.
*/
static void
del_char(struct Window *win, int p){
	// No text ==> nothing to delete 
	if ( (0 == win->text_len) || (p > win->text_len) )
		return;
	
	/* Clear old cursor */
	cursor_on = 0;
	draw_cursor(win, cursor_pos, 0);
				
	if (p == win->text_len) {
		win->text_len = str_del(win->txt, win->text_len - 1);
	} else {	
		win->text_len = str_del(win->txt, p);	
	};
	win_draw_text(win);	
};


/*
	This thread does 2 things.
	- It regularily blinks the cursor at the current position.
	- It advances the cursor if no new input via keyboard has arrived.

*/

/* ### Cursor Blinking Task ###  */
PT_THREAD (win_cursor_blink(struct pt *pt)){
	static struct timer tmr;
	static uint8_t cursor_on=0;
	 
	PT_BEGIN(pt);
		
	timer_add(&tmr, BLINK_PERIOD, BLINK_PERIOD);
	
	while(1){
		tmr.expired = 0;
		cursor_on ^= 1;	
		
		PT_WAIT_UNTIL(pt, ( timer_expired(&tmr) || timer_expired(&char_tmr) ) );
		
		if (pcursor_win == NULL) {
			timer_stop(&char_tmr);
			continue;
		};
		
		if (! (pcursor_win->flags & WINFLG_VISIBLE)) continue;
		if (pcursor_win->txt == NULL) continue; 	
		if (pcursor_win->flags & WINFLG_HIDE) continue;
		
		/* Check if we should advance the cursor because user did not press a key again within time */
		if ( timer_expired(&char_tmr)) {	
			advance_cursor();
			timer_stop(&char_tmr);
			key_cnt = -1;
		} else
			draw_cursor(pcursor_win, cursor_pos, cursor_on);
	};
	PT_END(pt);
};

/* 
	Sets a window to have a blinking cursor.
	At most one window at a time can have a cursor.
	Giving NULL as parameter stops cursor blinking for the current cursor window. 
*/
void
win_cursor_set(struct Window *pwin, int size){
	pcursor_win = pwin;
	max_txt_len = size;
	if (pwin != NULL){
		cursor_pos = max(0, (strlen(pcursor_win->txt) - 1));
//		draw_cursor_win(pcursor_win);
	};
	last_key = -1;
	key_cnt = -1;
};

void
win_cursor_init(){
	timer_add(&char_tmr, 0, 0);
	timer_stop(&char_tmr);
	task_add(&win_cursor_blink);
	cursor_pos = 0;
	key_cnt = -1;
	last_key = -1;
};	


static char
key2char(int key, int cnt){
//	const char key_table[10][5] = {" ", ".-@", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
	const char key_table[10][5] = {" ", ".-@", "ABC", "DEF", "GHI", "JKL", "MNO", "PQRS", "TUV", "WXYZ"};
	cnt = cnt % strlen(key_table[key]) ;
	return key_table[key][cnt];
};

/* 
	Our window with cursor has got some input
	
	key_cnt == -1 means no input has been taking place at this cursor position
	key_cnt >= 0 means the same key has been pressed one or more times here
*/
void
win_cursor_input(int new_key){
	char c;
	
	if (new_key == CURSOR_LEFT){
		dec_cursor();
		key_cnt = -1;
		last_key = -1;
		timer_stop(&char_tmr);
		return;
	};
	
	if (new_key == CURSOR_RIGHT){
		advance_cursor();
		key_cnt = -1;
		last_key = -1;
		timer_stop(&char_tmr);
		return;
	};
	
	/* A bit tricky. It makes sense to implement this as a kind of delete, i.e. delete the character 
		under the cursor. This only fails if the cursor is at end of text. Then delete the last character.
	*/
	if (new_key == CURSOR_BACKSPACE){
		key_cnt = -1;
		last_key = -1;
		timer_stop(&char_tmr);
		
		del_char(pcursor_win, cursor_pos);
		cursor_pos = min(cursor_pos, pcursor_win->text_len);
		return;	
	};
	
	timer_set(&char_tmr, WAIT_KEY_TIME, 0);				// start auto advance timer	
	/* This is the first time a key is pressed here */
	if (key_cnt < 0){
		key_cnt = 0;
		c = key2char(new_key, 0);
		store_char(pcursor_win, cursor_pos, c);
	} else {
		/* We already entered text at this cursor position */	
		if (new_key == last_key){			// User pressed the same key twice before a time out occured 
			key_cnt++;
			c = key2char(new_key, key_cnt);
			store_char(pcursor_win, cursor_pos, c);
		} else {
		// User pressed a different key, advance to the next cursor position, store new char
			key_cnt = 0;
			c = key2char(new_key, 0);
			advance_cursor();
			store_char(pcursor_win, cursor_pos, c);
		};
	};
	last_key = new_key;		
};

