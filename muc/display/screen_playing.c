/*
    screen_playing.c - User Interface

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

//TODO if we have no current song gray out the timing info

#include "screen.h"
#include "window.h"
#include "keyboard.h"
#include "lcd.h"
#include "model.h"
#include "mpd.h"

/* The height of a small window */
#define WL_SMALL_HEIGHT 10

/* Number of entries in the window list */
#define WL_SIZE 12

/* The window list */
static struct Window win[WL_SIZE];

#define WIN_TXT_SIZE 80
/* Text buffer for the windows */
static char win_txt[WL_SIZE][WIN_TXT_SIZE];

// row to start with the windows
#define WL_START_ROW 0

/* Some definitions to refer to specific windows by name */

#define caption_win (win[0])

/* Window to show the current title */
#define title_win (win[1])

/* Window to show the current artist */
#define artist_win (win[2])

/* Window to show running time and total time */
#define time_win (win[3])

/* Window to show a progress bar for running time */
#define time_prog_win (win[4])

/* Window to show volume ramp */
#define vol_ramp_win (win[5])

/* Window to show the current volume in precent */
#define volume_win (win[6])

/* Window to show speaker on/off */
#define speaker_win (win[7])

/* Window to show play/stop/pause status */
#define state_win (win[8])

/* Window to show random state */
#define rnd_win (win[9])

/* Window to show repeat state */
#define rpt_win (win[10])

/* Window to show single state */
#define single_win (win[11])


/* Window to show the current album TODO maybe later */

// Forward declarations
void view_pos_changed();

static void 
screen_enter(){
	view_pos_changed();				// sets caption window title
};

static void 
screen_exit(){
};

// Forward declarations
static int keypress(Screen *this_screen, int cur_key)	;
static int keypress_popup(Screen *this_screen, int cur_key);

/* Initialize the playing screen 
	We clear the whole screen.
*/
void 
playing_screen_init(Screen *this_screen) {
	int cur_start_row;
	
	this_screen->wl_size = WL_SIZE;
	this_screen->win_list = win;
	this_screen->screen_enter = screen_enter;
	this_screen->screen_exit = screen_exit;
	this_screen->keypress = keypress;
	
	cur_start_row = WL_START_ROW;	
	
	win_init(&title_win, cur_start_row, 0, 50, 128, 1, win_txt[1]);
	title_win.font = MEDIUMFONT;	
	title_win.flags |= WINFLG_CENTER;
	
	// The first line overlaps with the last line of the previous window
	cur_start_row += 49;

	win_init(&artist_win, cur_start_row, 0, 30, 128, 1, win_txt[2]);
	artist_win.font = MEDIUMFONT;	
	artist_win.flags |= WINFLG_CENTER;
	
	// leave some room between artist and time window
	cur_start_row += 30 + 5;			// 84
			
	win_init(&time_win, cur_start_row, 22, 10, 128-22, 0, win_txt[3]);
	time_win.font = SMALLFONT;
	
	// state windows lies between time window and progress bar
	cur_start_row +=  5;				// 89

	/* The state window, a bit higher than the time progress */
	win_init(&state_win, cur_start_row - 2, 1, 16, 18, 0, win_txt[4]);

	state_win.font = BIGFONT;
	win_new_text(&state_win, "\xB3"); 	// play icon == b3
	
	cur_start_row +=  5;				// 94
	
	/* This is our progressbar */
	win_init(&time_prog_win, cur_start_row, 22, 8, 102, 1, win_txt[5]);
	time_prog_win.font = SMALLFONT;
	time_prog_win.flags = WINFLG_BAR;
	time_prog_win.fg_color = DARK_GREY;
	time_prog_win.cur_char = 0;		/* current value */
	
	cur_start_row +=  16;				// 110

	/* The speaker indicator is on the same line as volume text */
	win_init(&speaker_win, cur_start_row, 15, 18, 15, 0, win_txt[6]);
	speaker_win.font = BIGFONT;
	win_new_text(&speaker_win, "\xB8");	// speaker icon on
	
	/* Our indicator for the volume (triangle progress bar) */
	win_init(&vol_ramp_win, cur_start_row, 31, 16, 50, 0, win_txt[7]);
	vol_ramp_win.flags = WINFLG_RAMP;
	vol_ramp_win.fg_color = BLACK;
	vol_ramp_win.cur_char = 100/2;		/* We initialize the volume with 100 % */

	/* The text portion of the volume follows on the same line as the ramp */
	win_init(&volume_win, cur_start_row, 88, 16, 128-88, 0, win_txt[8]);
	win_new_text(&volume_win, "100 %");
	
	cur_start_row +=  20;				// 130

	win_init(&rnd_win, cur_start_row, 0, 14, 32, 1, win_txt[9]);
	rnd_win.font = SMALLFONT;
	rnd_win.flags |= WINFLG_CENTER | WINFLG_FRAME | WINFLG_HIDE;
	rnd_win.fg_color = WHITE;
	rnd_win.bg_color = BLACK;
	win_new_text(&rnd_win, "RND" );
	
	win_init(&rpt_win, cur_start_row, 48, 14, 32, 1, win_txt[10]);
	rpt_win.font = SMALLFONT;
	rpt_win.flags |= WINFLG_CENTER | WINFLG_FRAME | WINFLG_HIDE;
	rpt_win.fg_color = WHITE;
	rpt_win.bg_color = BLACK;
	win_new_text(&rpt_win, "RPT" );
	
	win_init(&single_win, cur_start_row, 96, 14, 32, 1, win_txt[11]);
	single_win.font = SMALLFONT;
	single_win.flags |= WINFLG_CENTER | WINFLG_FRAME | WINFLG_HIDE;
	single_win.fg_color = WHITE;
	single_win.bg_color = BLACK;
	win_new_text(&single_win, "1" );
		
	cur_start_row +=  20;				// 150
	
	/* The caption window is the last window */
	win_init(&caption_win, cur_start_row, 0, WL_SMALL_HEIGHT, 128,0, win_txt[0]);
	caption_win.font = SMALLFONT;
	caption_win.fg_color = BLACK;
	caption_win.bg_color = LIGHT_GREY;
	caption_win.flags |= WINFLG_CENTER;
}

#define TIMESTR_LEN 30
void
view_time_changed(int te, int tt){
	char newstr[TIMESTR_LEN];
	int newval;
	
	sec2hms(newstr, te);
	strlcat(newstr, " / ", sizeof(newstr));	
	
	if (0 == tt)				// shoutcast ! 
		strlcat(newstr+8, "--:--", sizeof(newstr)-8);
	else
		sec2hms(newstr+8, tt);
	
	win_new_text(&time_win, newstr);
		
	if ( (te < 0) || (tt < 0) || (0 == tt ) )	// elapsed == UNKNOWN or total == UNKNOWN or Shoutcast
		newval = 0;
	else
		newval = (te * 100) / tt;
	
	if ( (win->flags & WINFLG_VISIBLE))
		draw_progressbar(time_prog_win.start_row+1, time_prog_win.start_col, time_prog_win.height-2, 
							time_prog_win.fg_color, time_prog_win.bg_color, time_prog_win.cur_char, newval);
	time_prog_win.cur_char = newval;
};

/* 
	The current song or the playlistlength (or both) has changed.
	Redraw the caption window
*/
void
view_pos_changed(){
	char newstr[30];
	char number[20];
	char *numstr;
	int cur_song, pl_length;		
	
	cur_song = mpd_get_pos();
	if (cur_song == SONG_UNKNOWN) {
		win_new_text(&caption_win,"");
		return;
	};
	
	if (cur_song == NO_SONG){
		win_new_text(&caption_win,"No current song");
		return;
	};

	if (is_stream() == 1)
		strlcpy(newstr, "Station ", sizeof(newstr));
	else
		strlcpy(newstr, "Song ", sizeof(newstr));
	
	numstr = get_digits(cur_song + 1, number, 0); 			// user readable song pos starts with 1, not 0
	strlcat(newstr, numstr, sizeof(newstr));
	
	pl_length = mpd_get_pl_length();
	if (pl_length >= 0){
		strlcat(newstr, " of ", sizeof(newstr));
		numstr = get_digits(pl_length, number, 0); 
		strlcat(newstr, numstr, sizeof(newstr));
	};
	win_new_text(&caption_win, newstr);
};


void 
view_title_changed(char *s){
	win_new_text(&title_win, s);
};

void 
view_artist_changed(char *s){
	win_new_text(&artist_win, s);
};

void
view_random_changed(int rnd){
	if (rnd == 1) 
		rnd_win.flags &= ~WINFLG_HIDE;	
	else 
		rnd_win.flags |= WINFLG_HIDE;	
	win_redraw(&rnd_win);
};

void
view_repeat_changed(int rpt){
	if (rpt == 1) 
		rpt_win.flags &= ~WINFLG_HIDE;	
	else 
		rpt_win.flags |= WINFLG_HIDE;	
	win_redraw(&rpt_win);
};

void
view_single_changed(int sgl){
	if (sgl == 1) 
		single_win.flags &= ~WINFLG_HIDE;	
	else 
		single_win.flags |= WINFLG_HIDE;	
	win_redraw(&single_win);
};


#define VOLSTR_SIZE 20
/* The volume has changed. Show new value. */
void
view_volume_changed(int vol){
	char newstr[VOLSTR_SIZE];
	char number[20];
	char *numstr;
		
	if (vol > 0)
		win_new_text(&speaker_win, "\xB8");	// speaker icon on
	else
		win_new_text(&speaker_win, "\xB9");	//speaker off	
	
	strlcpy (newstr, "", sizeof(newstr));
	if (vol < 0)
		strlcat(newstr, "?? \n", sizeof(newstr));
	else {
		numstr = get_digits(vol, number, 0); 
		strlcat(newstr, numstr, sizeof(newstr));
		strlcat(newstr, " %\n", sizeof(newstr));
	};
	win_new_text(&volume_win, newstr);	

	if (vol >= 0){
		if ((win->flags & WINFLG_VISIBLE)) 
			draw_trianglebar( vol_ramp_win.start_row+1, vol_ramp_win.start_col+1, vol_ramp_win.width-2,
						  vol_ramp_win.height-2, vol_ramp_win.fg_color, vol_ramp_win.bg_color, vol_ramp_win.cur_char, vol/2 );
			vol_ramp_win.cur_char = vol/2;
	};
};

void
view_state_changed(enum PLAYSTATE state){
	if (state == PLAY)
		win_new_text(&state_win, "\xB3"); 	// play icon == b3
	else
		if (state == PAUSE)
			win_new_text(&state_win, "\xBA"); 	// pause icon == ba
	else
		if (state == STOP)
			win_new_text(&state_win, "\xA0"); 	// pause icon == a0
	
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
		case KEY_B:
		case KEY_C:
		case KEY_Info:
			popup_end();
			break;						// give this key to normal screen handler
			
		default:
			return NO_KEY;
	};
	return cur_key;
};

static int
keypress_info_popup(Screen *this_screen, int cur_key){
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


/*
	Betty always opens a menu
	Exit exits a popup, it also cycles through 3 main screens
	OK may open a popup if there are more than 1 options what to do
	i button can open a color menu or an info popup
	A = add to current playlist (search and playlists screen)
	B = begin new playlist (search and playlists screen)
	C = clear search term (search only)
	D = goto search screen
*/
//TODO add info to title screen track 4 of 17, playlist 1 of 99

static int 
keypress(Screen *this_screen, int cur_key){
	switch (cur_key) {
			
		case KEY_Betty:
		case KEY_Menu:
			popup(
				"A  All\n   playlists\n\n"
				"B  Current\n   playlist\n\n"
				"C  Search\n\n"
				"D \n\n"
				"i  Info",
					0, keypress_popup);
			break;
			
		case KEY_Exit:	
			show_screen(TRACKLIST_SCREEN);
			break;
			
		case KEY_A:	
			show_screen(PLAYLIST_SCREEN);
			break;
			
		case KEY_B:	
			show_screen(TRACKLIST_SCREEN);
			break;
			
		case KEY_C:	
			show_screen(SEARCH_SCREEN);
			break;

		case KEY_Info:
			popup("Red = Stop\n"
				"Green = Play\n"
				"Yellow = Pause\n"
				"PiP = Random\n"
				"A/B = Repeat\n"
				"16:9 = Single\n"
				"\xB1 = Seek Back\n"
				"\xB0 = Seek FWD\n"
				"\xB2 = Next Song\n"
				"\xB3 = Prev. Song\n"
				"TV = Version", 
				  0, keypress_info_popup);
			break;
												
		case KEY_Left:
			user_wants_time_add(-10);	
			break;
			
		case KEY_Right:
			user_wants_time_add(+10);	
			break;
			
		case KEY_Up:
		case KEY_Pminus:
			user_wants_song(PREV_SONG);	
			break;
					
		case KEY_Down:
		case KEY_Pplus:
			user_wants_song(NEXT_SONG);
			break;
			

		case KEY_Red:
			user_wants_state(STOP);
			break;
			
		case KEY_Green:
			user_wants_state(PLAY);
			break;
						
		case KEY_Yellow:
			user_toggle_pause();
			break;

			
		case KEY_PiP:
			user_toggle_random();	
			break;
			
		case KEY_AB:
			user_toggle_repeat();	
			break;
			
		case KEY_16_9:
			user_toggle_single();	
			break;

		case KEY_VTX1:
			user_wants_script(1);
			break;
			
		case KEY_VTX3:
			user_wants_script(2);
			break;
				
		case KEY_TV:
			view_message("   McBetty\n\n"
						"Version "VERSION"\n\n\n"
						"  (c) 2010\n   H. Raap",
	  			5 * TICKS_PER_SEC);
			break;
			
		default:
			return cur_key;				// we could not handle key
	};
	return NO_KEY;
};	


