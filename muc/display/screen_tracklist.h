/* screen.h */

#ifndef SCREEN_TRACKLIST_H
#define SCREEN_TRACKLIST_H

#include "global.h"
#include "kernel.h"

void tracklist_screen_init(Screen *this_screen);
void tracklist_screen_enter();
void tracklist_keypress(Screen *track_screen, int cur_key, UserReq *req);
enum USER_CMD tracklist_user_request(void);
int tracklist_arg(void);
void tracklist_screen_exit();
void view_tracklist_changed();

#endif

