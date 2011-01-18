/* screen.h */

#ifndef SCREEN_PLAYING_H
#define SCREEN_PLAYING_H

#include "global.h"
#include "kernel.h"
#include "model.h"
#include "mpd.h"


void playing_screen_init(Screen *this_screen);
void playing_screen_enter();
enum USER_CMD playing_user_request();
void playing_screen_exit();
int playing_arg(void);
void view_state_changed(enum PLAYSTATE state);
void view_single_changed(int sgl);
void view_repeat_changed(int rpt);
void view_random_changed(int rnd);

#endif

