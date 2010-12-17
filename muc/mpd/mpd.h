/*
    mpd.h - 
    Copyright (C) 2010 <raap@ewetel.net>  

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

#ifndef MPD_H
#define MPD_H

// the possible actions / commands
// This enum plays two roles:
// It serves as a data type to record the user input which wants to affect the model in some way.
// And it is the return value of the model function action_needed, which tells the communicator what
// kind of interaction with MPD it has to start 
// TODO maybe eliminate the first role.
// We already have a user model where we can reflect all changes that the user wants!
enum USER_CMD {
	NO_CMD,
	SEL_SONG,
	VOLUME_UP,
	VOLUME_DOWN,
	MUTE_CMD,
	VOLUME_NEW,
	CUR_SONG_CMD,
	PREV_CMD,
	NEXT_CMD,
    PAUSE_ON,
	PAUSE_OFF,
	PLAY_CMD,
	STOP_CMD,
	FORWARD_CMD,
	REWIND_CMD,
	STATUS_CMD,
 	PLINFO_CMD,
	LSINFO_CMD,
	LOAD_CMD,
	RANDOM_CMD,
	REPEAT_CMD,
	SINGLE_CMD
};

extern int playlist_changed;

/* this should be a real function. just for testing */
#define pl_changed (model_changed_flags & (1<<5))

void tx_init();
void do_tx();
	
PT_THREAD (controller(struct pt *pt));

char *track_info(int no);

/* See rf.c */
#define CMDSTR_LEN 254

int mpd_get_song();
void model_init();

#endif
