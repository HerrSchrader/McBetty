/* screen.h */

#ifndef INFO_PLAYING_H
#define INFO_PLAYING_H

#include "global.h"
#include "kernel.h"
#include "model.h"
#include "mpd.h"


void info_screen_init(Screen *this_screen);
void info_screen_enter();
void info_keypress(Screen *this_screen, int cur_key, UserReq *req);
enum USER_CMD info_user_request();

int info_arg(void);

#endif

