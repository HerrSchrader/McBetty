/* screen.h */

#ifndef SCREEN_PLAYLIST_H
#define SCREEN_PLAYLIST_H

#include "global.h"
#include "kernel.h"


void playlist_screen_init(Screen *this_screen);
void playlist_screen_enter();
enum USER_CMD playlist_user_request();
int playlist_arg(void);
void playlist_screen_exit();
void view_playlist_changed();
void view_playlists_changed();
#endif

