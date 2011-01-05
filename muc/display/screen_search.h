/* screen_search.h */

#ifndef SCREEN_SEARCH_H
#define SCREEN_SEARCH_H

#include "global.h"
#include "kernel.h"

void view_results_changed(int num);
void view_resultnames_changed();
void search_keypress(Screen *this_screen, int cur_key, UserReq *req);
void search_screen_init(Screen *this_screen);

#endif

