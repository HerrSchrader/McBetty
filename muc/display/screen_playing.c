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


/* Number of entries in the window list */
#define WL_SIZE 12

/* The window list */
static struct Window win[WL_SIZE];

#define WIN_TXT_SIZE 80
/* Text buffer for the windows */
static char win_txt[WL_SIZE][WIN_TXT_SIZE];

/* Some definitions to refer to specific windows by name */

/* Window to show the current title */
#define title_win (win[0])

/* Window to show the current artist */
#define artist_win (win[1])

/* Window to show running time and total time */
#define time_win (win[2])

/* Window to show a progress bar for running time */
#define time_prog_win (win[3])

/* Window to show volume ramp */
#define vol_ramp_win (win[4])

/* Window to show the current volume in precent */
#define volume_win (win[5])

/* Window to show speaker on/off */
#define speaker_win (win[6])

/* Window to show play/stop/pause status */
#define state_win (win[7])

/* Window to show random state */
#define rnd_win (win[8])

/* Window to show repeat state */
#define rpt_win (win[9])

/* Window to show single state */
#define single_win (win[10])

#define version_win (win[11])


/* Window to show the current album TODO maybe later */

static void 
screen_enter(){
	lcd_fill(0x00);
	screen_visible(PLAYING_SCREEN, 1);
	version_win.flags &= ~WINFLG_VISIBLE;
	screen_redraw(PLAYING_SCREEN);
};


static void 
screen_exit(){
	screen_visible(PLAYING_SCREEN, 0);
};

// Forward declaration
static void keypress(Screen *this_screen, int cur_key, UserReq *req)	;

/* Initialize the playing screen 
	We clear the whole screen.
*/
void 
playing_screen_init(Screen *this_screen) {
	this_screen->wl_size = WL_SIZE;
	this_screen->win_list = win;
	this_screen->screen_enter = screen_enter;
	this_screen->screen_exit = screen_exit;
	this_screen->keypress = keypress;
	
	win_init(&title_win, 0, 0, 50, 128, 1, win_txt[0]);
	title_win.font = MEDIUMFONT;	
	title_win.flags |= WINFLG_CENTER;
	
	win_init(&artist_win, 49, 0, 30, 128, 1, win_txt[1]);
	artist_win.font = MEDIUMFONT;	
	artist_win.flags |= WINFLG_CENTER;
	
	win_init(&time_win, 84, 22, 10, 128-22, 0, win_txt[2]);
	time_win.font = SMALLFONT;

	/* The state window, on the same line as the time progress */
	win_init(&state_win, 89, 1, 16, 18, 0, win_txt[3]);

	state_win.font = BIGFONT;
	win_new_text(&state_win, "\xB3"); 	// play icon == b3
	
	/* This is our progressbar */
	win_init(&time_prog_win, 95, 22, 8, 102, 1, win_txt[4]);
	time_prog_win.font = SMALLFONT;
	time_prog_win.flags = WINFLG_BAR;
	time_prog_win.fg_color = DARK_GREY;
	time_prog_win.cur_char = 0;		/* current value */
	
#define VOL_ROW 114	
	/* The speaker indicator is on the same line as volume text */
	win_init(&speaker_win, VOL_ROW, 15, 18, 15, 0, win_txt[5]);
	speaker_win.font = BIGFONT;
	win_new_text(&speaker_win, "\xB8");	// speaker icon on
	
	/* Our indicator for the volume (triangle progress bar) */
	win_init(&vol_ramp_win, VOL_ROW, 31, 16, 50, 0, win_txt[6]);
	vol_ramp_win.flags = WINFLG_RAMP;
	vol_ramp_win.fg_color = BLACK;
	vol_ramp_win.cur_char = 100/2;		/* We initialize the volume with 100 % */

	/* The text portion of the volume follows on the same line as the ramp */
	win_init(&volume_win, VOL_ROW, 88, 16, 128-88, 0, win_txt[7]);
	win_new_text(&volume_win, "100 %");

	win_init(&rnd_win, 134, 0, 14, 33, 1, win_txt[8]);
	rnd_win.font = SMALLFONT;
	rnd_win.flags |= WINFLG_CENTER | WINFLG_FRAME | WINFLG_HIDE;
	rnd_win.fg_color = WHITE;
	rnd_win.bg_color = BLACK;
	win_new_text(&rnd_win, "RND" );
	
	win_init(&rpt_win, 134, 43, 14, 33, 1, win_txt[9]);
	rpt_win.font = SMALLFONT;
	rpt_win.flags |= WINFLG_CENTER | WINFLG_FRAME | WINFLG_HIDE;
	rpt_win.fg_color = WHITE;
	rpt_win.bg_color = BLACK;
	win_new_text(&rpt_win, "RPT" );
	
	win_init(&single_win, 134, 86, 14, 33, 1, win_txt[10]);
	single_win.font = SMALLFONT;
	single_win.flags |= WINFLG_CENTER | WINFLG_FRAME | WINFLG_HIDE;
	single_win.fg_color = WHITE;
	single_win.bg_color = BLACK;
	win_new_text(&single_win, "1" );
	
	win_init(&version_win, 150, 0, 10, 128, 0, win_txt[11]); 
	version_win.font = SMALLFONT;	
	version_win.flags |= WINFLG_CENTER;
	version_win.fg_color = WHITE;
	version_win.bg_color = BLACK;
	win_new_text(&version_win, "Version " VERSION);

}

#define TIMESTR_LEN 30
void
view_time_changed(int te, int tt){
	char newstr[TIMESTR_LEN];
	int newval;
	
	sec2hms(newstr, te);
	strlcat(newstr, " / ", sizeof(newstr));	
	sec2hms(newstr+8, tt);
	win_new_text(&time_win, newstr);
		
	newval = (te * 100) / tt;
	
	if ( (win->flags & WINFLG_VISIBLE))
		draw_progressbar(time_prog_win.start_row+1, time_prog_win.start_col, time_prog_win.height-2, 
							time_prog_win.fg_color, time_prog_win.bg_color, time_prog_win.cur_char, newval);
	time_prog_win.cur_char = newval;
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

static void
view_version_changed(){
	if ( (version_win.flags & WINFLG_VISIBLE)){
		version_win.bg_color = WHITE;
		win_clear(&version_win, 1); 
		version_win.flags &= ~WINFLG_VISIBLE;
		version_win.bg_color = BLACK;
	} else {
		version_win.flags |= WINFLG_VISIBLE;
		win_redraw(&version_win);
	}; 	
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

void 
keypress(Screen *this_screen, int cur_key, UserReq *req){
	switch (cur_key) {

		case KEY_Yellow:
			user_toggle_pause();
			break;
			
		case KEY_Green:
			user_wants_state(PLAY);
			break;
			
		case KEY_Left:
			user_wants_time_add(-10);	
			break;
			
		case KEY_Right:
			user_wants_time_add(+10);	
			break;
			
		case KEY_Red:
			user_wants_state(STOP);
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

			/* Inform the controller that the user wants the next song */

		case KEY_Down:
		case KEY_Pplus:
			user_wants_song(NEXT_SONG);
			break;
			
			/* Inform the controller that the user wants the previous song */
		case KEY_Up:
		case KEY_Pminus:
			user_wants_song(PREV_SONG);	
			break;
			
			/* Show/hide version info */
		case KEY_Info:
			view_version_changed();
			break;
			
		// TODO debug only
		case KEY_VTX3:
			switch_screen(PLAYING_SCREEN, INFO_SCREEN);	
			break;

		case KEY_A:
			user_wants_script(1);
			break;
			
		case KEY_B:
			user_wants_script(2);
			break;
				
		default:
			break;
	};
};	


