/*
    window.h - window handling and drawing

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

#ifndef WINDOW_H
#define WINDOW_H

#include "global.h"
#include "kernel.h"
#include "fonty.h"
#include "lcd.h"


/* A text window is a rectangular area on screen.
	Each window has a text buffer which it displays.
	It can do either automatic scrolling or line wrapping and/or clipping.
	It can display a border around itself.
*/

struct Window {
	uint8 start_row, start_col;		// upper left corner of window 
	uint8 width, height;			// total width and height of window (in pixels)
	uint8 fg_color;					// foreground color
	uint8 bg_color;					// background color
	uint8 border;					// 0 if no border, 1 if border TODO would be better if this meant thickness of border in pixels
	uint8 cur_char;					// index into char *txt, next character to be drawn in window
	uint8 cur_col;
	uint8 txt_offs_row;				// distance of text from upper and lower sides of window (rows)
	uint8 txt_offs_col;				// distance of text from left and right side of window (columns)
	int	 text_len;					// == strlen(txt)
	enum FONT font;					// which font to use for text
	uint8 fits;						// is <>0 iff text fits completely in window
	uint8 scroll;					// is <>0 iff ??? TODO
	uint8 flags;					// see defines below
	uint8 buffer_lim;
	char *txt;						// Pointer to the window text buffer
};

/* The flags variable in struct Window has these semantics: */
/* Is the window visible */
#define	WINFLG_VISIBLE	(1<<0)
/* Shall the content be scrolled when necessary */
#define WINFLG_SCROLL	(1<<1)	
/* Shall text be centered in the window */
#define WINFLG_CENTER	(1<<2)
/* Is this a text window */
#define WINFLG_TEXT		(1<<3)
/* Is this a ramp */
#define WINFLG_RAMP		(1<<4)
/* Is this a progress bar */
#define WINFLG_BAR		(1<<5)
/* This is a window with a frame around it */
#define WINFLG_FRAME	(1<<6)
/* Don't show any text in the window (only border) */
#define WINFLG_HIDE		(1<<7)

/* Start of text 
	TODO think about including border here
*/
#define win_txt_row(win) ( (win)->start_row + (win)->txt_offs_row )
#define win_txt_col(win) ( (win)->start_col + (win)->txt_offs_col )
#define win_txt_width(win) ( (win)->width - 2*(win)->txt_offs_col ) 
#define win_txt_height(win) ( (win)->height - 2*(win)->txt_offs_row )

/* Last absolute column + 1 available for text TODO think about including border here */
#define txt_col_lim(win) ((win)->start_col + (win)->width - (win)->txt_offs_col)

/* Codes sent to win_cursor_input */
#define CURSOR_LEFT			-1
#define CURSOR_RIGHT		-2
#define CURSOR_BACKSPACE	-3


/* A scroll list */
typedef struct {
	struct Window *wl;					/* Address of first window showing the scroll list */
	int win_offs;						/* TODO ??? offset from win_txt ???*/
	int num_windows;					/* number of windows (== lines) in the scroll list */
	int first_info_idx;					/* index of first info shown on screen */
	int sel_win;						/* selected window */
	char * (*info_text) (int info_idx);	;	/* pointer to function to get the text corresponding to an info index */
} scroll_list;


void win_changed(int pos, char *s);
void view_title_changed (char *s);
void view_artist_changed (char *s);
void view_volume_changed (int vol);
void view_time_changed(int te, int tt);
void win_new_text(struct Window *win, char *s);
void win_init(struct Window *win, uint8 row, uint8 col, uint8 height, uint8 width, uint8 border, char *txt);
void win_draw_border(struct Window *win);
void win_redraw(struct Window *win);
void win_scroll(struct Window *win);
void win_unscroll(struct Window *win);
void win_scroll_init();
void win_clear(struct Window *win, int clr_border);

void select(scroll_list *sl, int sel);
void select_win(struct Window *w );
void unselect_win(struct Window *w );
int last_info_idx(scroll_list *sl);
int info_idx(scroll_list *sl, int win_idx);
void sel_win_up(scroll_list *sl);
void sel_win_down(scroll_list *sl);
void scroll_list_changed(scroll_list *sl);
void init_scroll_list(scroll_list *sl, struct Window *pwl, char *win_txt, int win_txt_len, int nw, char * (*info_text) (), int start_row );

void win_cursor_set(struct Window *pwin, int size);
void win_cursor_input(int new_key);
void win_cursor_init();

#endif

