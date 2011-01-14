/* screen.h */

#ifndef SCREEN_H
#define SCREEN_H

#include "global.h"
#include "kernel.h"

#include "mpd.h"


/* Screens are enumerated so that we do not have to use their addresses */
enum SCREEN {PLAYLIST_SCREEN, TRACKLIST_SCREEN, PLAYING_SCREEN, INFO_SCREEN, SEARCH_SCREEN };

/* We do some object oriented design here.
	A screen is a number of data structures with some externally callable functions
	Each screen has an array of windows
	The memory for that array is handled by the screen itself (currently statically allocated)
	Before using a screen you have to call a special init function for each screen which fills 
	this Screen structure.
*/
typedef struct Screen {
	int	wl_size;					// number of windows in this screen
	struct Window *win_list;		// pointer to array with the windows
	void (*screen_enter)(void);		// function called when screen is entered
	void (*screen_exit)(void);		// function called when screen is exited
	void (*keypress) (struct Screen *, int, UserReq *);		// function called for screen specific key handler
	enum USER_CMD user_req_cmd;		// ?
	uint8_t popup_active;			// is TRUE iff this screen is overlayed with a popup
									// other windows must not draw into screen
} Screen;

void screen_visible(enum SCREEN screen, int v);
void screen_redraw(enum SCREEN screen);
void mainscreen_init(void);
void user_pressed_a_key(int cur_key);
void switch_screen(enum SCREEN oldscreen, enum SCREEN newscreen);
void get_user_request(UserReq *req);
void show_new_screen(enum SCREEN screen);

#endif

