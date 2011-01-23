/* model.c */

/* ------------------------------- Our MODEL ----------------------------------------------

	We keep 2 models here.
	mpd_model keeps track of the state of mpd (current song etc.)
	user_model keeps track of the user intentions (which song shall be current song etc.)


  Information about status is stored in this structure. 
	We need to indicate that we do not know some things in here.
	For volume, song and song_id we use -1 to indicate UNKNOWN.
	The state has its own UNKNOWN value.
	cur_artist ard cur_title are NULL if UNKNOWN.
	
	We make sure that the information here is consistent, i.e. if for example songid is set,
	the variables pos, time_total etc. reflect the information about that particular song and not
	about some earlier song.
	
*/

/* Functions starting with user reflect what the user wants.
	We reflect the necessary changes in the user_model variable.
	We return very quickly from theses routines, because they are called from the GUI-Handlers.
	Later we generate the necessary commands to communicate our wishes to MPD (see model_needs_action() ) 
*/


#include "global.h"
#include "kernel.h"
#include "timerirq.h"

#include "fonty.h"
#include "mpd.h"
#include "screen.h"
#include "model.h"

/* The currently known state of MPD. */
static struct MODEL mpd_model;

/* The world of MPD as the user wants it to be */
static struct MODEL user_model;

static void mpd_set_state(enum PLAYSTATE newstate);
static void mpd_set_title(char *s);
static void mpd_set_artist(char *s);
static void mpd_set_pos(int newpos);
static void user_song_unknown();
static char *mpd_result_string(int pos);

/* ===================== Info about changes ====================================== */

static uint32 model_changed_flags;			// Flags which are set when (part of) the model has changed

int
model_get_changed(){
	return model_changed_flags;
};

/* Setting flags indicating something has changed */
static inline void
model_changed(uint32 change){
	model_changed_flags |= change;	
};

void
model_reset_changed(){
	model_changed_flags = 0;
};


/* ================ Playlist variables, keeping info about all known playlists ========================= */
static STR_CACHE playlists;

/* ============== Tracklist variables, keeping info about songs in the current playlist. ==============
* Track lists can be very long (100's of songs). So we only cache a few of them and request
	more information from MPD if we need it.
	And of course we only keep information for one track list (the current one),
	so after loading a new track list we have to empty the cache and refill it.
*/
static STR_CACHE tracklist;

/* ================ This cache holds results from searches ========================= */
static STR_CACHE resultlist;




/* Returns TRUE iff there are one or more variables in mpd_model that are unknown and which we can get
	be issuing a status command.
	We also issue a STATUS command after some time, just to make sure everything is as we expect it
	
	TODO
	We should get more information from status command.
	Not only is the information valuable that we get, but also when certain information is missing.
	For example when there is no current song, we do not get song:, songid:, time:, bitrate: and audio: info.
	Even if we assume that there might be communication problems and some lines are missing (occurs rarely),
	we can deduce that there is no current song if all of that info is missing but other infos are present.
	
*/
static int
need_status(){

	/* If the following variables are unknown, we must definitely issue a STATUS command */
	if ( (mpd_model.playlistlength == -1) || (mpd_model.volume == -1) || (mpd_model.state == UNKNOWN) ) 
		return 1;
	
	/* We use status when we want to know the current song number */	
	if ( mpd_model.pos == SONG_UNKNOWN) { 
		/* Makes only sense if we have a current playlist with some songs in it */
		if (mpd_model.playlistlength > 0) 
			/* TODO We will not always get the pos when mpd has stopped 
				If there is a current song, we will get it.
				But if the playlist has finished, there is no current song and we do not get that information
			*/
			if (mpd_model.state != STOP) 
				return 1;
	};

	if ((mpd_model.random == -1) || (mpd_model.repeat == -1) )
		return 1;

	/* The remaining values are not given to us, when there is no current song */
	if (mpd_model.pos == NO_SONG) return 0;
		
	/* MPD gives us no time information, if the state is stopped. Stupid, but true. */
	if (mpd_model.state == STOP) return 0;
	

	return ( (mpd_model.time_elapsed == -1)
			|| (mpd_model.time_total == -1)
		  );		
};


/* Here we determine if user_model and mpd_model agree and if we need any information from mpd or need to send some command to mpd
	We return a command (NO_CMD if no action is needed).
	If the command has an argument, we store it in req->arg.
	The if-clauses here are in a specific order, because some information is necessary for other actions to be useful.
*/
static enum USER_CMD
model_needs_action(UserReq *req){	
	int pos;
	
	/* Most of the following commands only work if we have loaded the correct playlist.
		So check that first.
	*/ 
	if (mpd_model.num_playlists == -1)
		return PLAYLISTCOUNT_CMD;
	
	if (user_model.cur_playlist != -1){
		req->str = mpd_playlistname_info(user_model.cur_playlist);
		return LOAD_CMD;
	};
	
	/* mpd playing state and the user state should match */
	if ( (user_model.state != UNKNOWN) && (mpd_model.state != user_model.state) ){
		switch(user_model.state){
			case PLAY:
				/* The user definitively wants to play something */
				if (mpd_model.state == PAUSE)
					return PAUSE_OFF;

				/* Only issue a play command if the playlist is not empty 
					We might just have issued a CLEAR command, which stopped MPD, but user wants playing.
					Inform user about empty playlist.
				*/
				if (mpd_model.playlistlength > 0){
					return PLAY_CMD;
				} else
					if (mpd_model.playlistlength == 0){
						model_changed(PLAYLIST_EMPTY);
						user_model.state = -1;
					};
				break;
				
			case PAUSE:
				return PAUSE_ON;
				
			case STOP:
				return STOP_CMD;
				
			default:
				break;
		};
	};

	/* Does the user want a specific volume that is not currently set ? */
	if ( (user_model.volume != -1) && (user_model.volume != mpd_model.volume)){
		req->arg = user_model.volume;
		return VOLUME_NEW;
	};
				 
	/* Maybe the user wants to change something about the current song */
	if ( (user_model.pos != SONG_UNKNOWN) && (user_model.pos != mpd_model.pos) ){
		/* A specific song from the playlist? */ 
		if (user_model.pos >= 0) {
			req->arg = user_model.pos;
			return SEL_SONG;
		};
		if (user_model.pos == PREV_SONG)
			return PREV_CMD;
		if (user_model.pos == NEXT_SONG)
			return NEXT_CMD;
	};
	
	if ((user_model.random != -1) && (mpd_model.random != user_model.random) ){
		req->arg = user_model.random;
		return RANDOM_CMD;
	};

	if ((user_model.repeat != -1) && (mpd_model.repeat != user_model.repeat) ){
		req->arg = user_model.repeat;
		return REPEAT_CMD;
	};
	
	if ((user_model.single != -1) && (mpd_model.single != user_model.single) ){
		req->arg = user_model.single;
		return SINGLE_CMD;
	};

	/* We ask for status before asking for current song, because only after status do we know if there is any current song */
	if (need_status())
		return STATUS_CMD;

	if  ( (mpd_model.playlistlength > 0) && ((mpd_model.artist == NULL) || (mpd_model.title == NULL)) ){
		return CUR_SONG_CMD;
	};
	
	// Seeking only makes sense if we have a current song
	if ( (user_model.time_elapsed > mpd_model.time_elapsed ) && (mpd_model.pos >= 0) ) {
			req->arg = mpd_model.pos;
			req->arg2 = user_model.time_elapsed;
			return FORWARD_CMD;
	};
	
	if ( (user_model.time_elapsed >= 0) && (user_model.time_elapsed < mpd_model.time_elapsed )
			&& (mpd_model.pos >= 0) ){
		req->arg = mpd_model.pos;
		req->arg2 = user_model.time_elapsed;
		return REWIND_CMD;	
	};
	
	if ((0 == user_model.playlistlength) && (mpd_model.playlistlength != 0) ){
		return CLEAR_CMD;
	};
		
	pos = cache_find_unknown(&tracklist);
	if ( (pos >= 0) && (pos < mpd_model.playlistlength) ) {
		req->arg = pos;
		return PLINFO_CMD;
	};
	
	/* Something to add to the playlist ? */
	if (user_model.find_add != -1){
		req->str = mpd_result_string(user_model.find_add);
		return FINDADD_CMD;
	};
	
	/* Something to search ? */
	if (user_model.search_string != NULL){
		req->str = mpd_get_search_string();
		return SEARCH_CMD;
	};
	/* TODO the available playlists may change by an outside action (another client created/deleted one etc.)
			So we should regularily (every 5 minutes or so) reread this information
	*/ 

	/* Find the first unknown playlist name */
	pos = cache_find_unknown(&playlists);
	if ( (pos >= 0) && (pos < mpd_model.num_playlists) ) {
		req->arg = pos;
		return PLAYLISTNAME_CMD;
	};

	/* Find the first unknown result */
	if (mpd_model.num_results > 0){
		pos = cache_find_unknown(&resultlist);
		if ( (pos >= 0) && (pos < mpd_model.num_results) ) {
			req->arg = pos;
			return RESULT_CMD;
		};
	};

	/* Maybe the user wants some script to be executed */
	if (user_model.script != -1){
		req->arg = user_model.script;
		return SCRIPT_CMD;
	};

	/* Regular Synchronization */
	if ( (system_time() - mpd_model.last_status) > STATUS_SYNC_TIME * TICKS_PER_SEC )
		return STATUS_CMD; 
	
	return NO_CMD;
};

/* This function returns TRUE (<> 0) iff the model needs some interaction with mpd,
	i.e. some commands have to be sent to mpd and the response has to be interpreted.
	In effect, this routine checks if user model and mpd model disagree.
	If they disagree, we store the needed action in *request.
*/
int
action_needed(UserReq *request){

	request->cmd = model_needs_action(request);

	return (request->cmd != NO_CMD);
}

/* ------------------------------------ Scripts --------------------------------------- */
void
user_wants_script(int script_no){
	user_model.script = script_no;
}

void
mpd_script_ok(struct MODEL *a){
	user_model.script = -1;
};

/* ------------------------------------- Tracklist -------------------------------------- */

/* 
	The cache containing track info has to follow the information that we show on screen.
	Here we tell the cache which positions we want to show,
	namely all infos between start_pos and end_pos inclusive.
*/
void
tracklist_range_set(int start_pos, int end_pos){	
	cache_range_set(&tracklist, start_pos, end_pos);
};

/* The tracklist screen will ask us for the track entry of a specific track_no. 
	We return it if available, else we return "" 
	The track numbering starts with 0 as MPD does.
*/
char *
track_info(int track_pos){
	return cache_info(&tracklist, track_pos);
};

/* 
	Given title, artist and track number, store this info in our tracklist cache (if it fits) 
*/
static void
model_store_track(char *title, char *artist, int track_pos){
	char tmp[64];
	
	strlcpy(tmp, title, sizeof(tmp)); 
	strlcat(tmp, " \xB7 ", sizeof(tmp));	// dot icon == b7
	strlcat(tmp, artist, sizeof(tmp));
	cache_store(&tracklist, track_pos, tmp);
	model_changed(TRACKLIST_CHANGED);
};

void
mpd_playlistinfo_ok(struct MODEL *a){
	if (a->pos == a->request.arg)
		model_store_track(a->title_buf, a->artist_buf, a->pos);
}

/*
 NOTE It happens not too infrequently, that mpd cannot find a song in the tracklist (database changed).
		We could issue a delete command or inform the user if this is the case.
		For now we just set the info to a value of "- not - found -" so that we do not repeatedly ask mpd about it.
*/
void
mpd_playlistinfo_ack(struct MODEL *a){
	if (a->pos == a->request.arg)
		model_store_track( "- not", "found -", a->pos);
}

/* 
	Returns pos of the last track in the current playlist
	Is < 0 if the playlistlength is unknown
	or playlist is empty.
*/
int 
mpd_tracklist_last(){
	return mpd_model.playlistlength - 1;
};

int 
mpd_get_pl_length(){
	return mpd_model.playlistlength;
};

int
mpd_get_pl_added(){
	return mpd_model.pl_added;
};
	
/* 
	The length of our playlist has changed.
*/
static void
set_playlistlength(int n){
	n = max(-1 ,n);	
	if (mpd_model.playlistlength == n)
		return;								// no change

	/* Were songs added? */
	if ( (mpd_model.playlistlength >= 0) && (n > mpd_model.playlistlength) )
		mpd_model.pl_added = max(0, n - mpd_model.playlistlength);
	else 
		mpd_model.pl_added = 0;
	
	mpd_model.playlistlength = n;	
	cache_set_limit(&tracklist, n);				// new limit for the cache
	model_changed(PL_LENGTH_CHANGED);
	
	
	//	tracklist_range_set(0, CACHE_MAX);	
	/* If the playlist is empty, there can be no current song.*/
	if (n == 0)
		mpd_set_pos(NO_SONG);

	/* NOTE We do NOT set TRACKLIST_CHANGED flag here, because only our information about the
		total number of tracks has changed, but not the tracks itself.
		The tracks itself can currently only be changed after a LOAD, CLEAR or FINDADD command.
	*/
};

/* Called after a successful LOAD command. 
	Currently we interpret the LOAD as a CLEAR_AND_LOAD, i.e. the old playlist is cleared and the new one loaded.
	We assume the user wish has been fulfilled 
	We do not care what playlist was loaded.
	But we assume the user wants to play the first song of the playlist.
	mpdtool automatically added a status command for us, so that we get
	the new status of MPD
*/
void
mpd_load_ok(struct MODEL *a){
	user_model.cur_playlist = -1;			// wish fulfilled
	user_model.playlistlength = -1;	

	set_playlistlength(0);					// old playlist was cleared
	model_changed(TRACKLIST_CHANGED);
	mpd_status_ok(a);						// here we set the new real playlistlength
	
	// now we should know the playlistlength and we can begin to play
	user_wants_song(0);						// user wants first song ... 
	user_model.state = PLAY;				// ... to be played
};

/* 
	The user wants to clear the current tracklist.
	We note that by setting user_model.playlistlength to 0.
	There are a lot of other things that happen when the playlist is cleared,
	but this is the only thing the user explicitly wishes.
*/
void
user_tracklist_clr(){
	user_model.playlistlength = 0;	
};

/* We got an "OK" for our CLEAR command */
void
mpd_clear_ok(struct MODEL *a){
	user_model.playlistlength = -1;	
	user_model.cur_playlist = -1;			// wish fulfilled
	set_playlistlength(0);	
//	cache_clear(&tracklist, 0);
//	tracklist_range_set(0, 0);
	model_changed(TRACKLIST_CHANGED);
	mpd_set_state(STOP);				// mpd changes its state to STOP after a CLEAR command!
	mpd_set_pos(NO_SONG);				// there is no current song
};

/* 
	User wants a specific song pos in the playlist if pos >= 0
	pos == -1 means user error, do not select any song
	pos == NEXT_SONG or pos == PREV_SONG trigger the corresponding action
*/
void
user_wants_song(int pos){
	if (pos >= 0) {			// a specific song, we make sure that the wish makes sense.
		if ((mpd_model.playlistlength >= 0) && (pos > mpd_model.playlistlength - 1)){
			return;
		} else {
			user_model.pos = pos;
			return;
		}
	} else {				// pos < 0
		if (pos == -1) return;	// invalid, do nothing
		user_model.pos = pos;	// either NEXT_SONG or PREV_SONG
	}
};


/* ------------------------------------- Playlists -------------------------------------- */

/* Given an index starting from 0, we return the corresponding playlist name entry.
	If we have no info or the playlist does not exist we return "".
	Does return a non-NULL string.
*/
char *
mpd_playlistname_info(int pos){
	return cache_info(&playlists, pos);
};

/* 
	The cache containing playlists info has to follow the information that we show on screen.
	Here we tell the cache which positions we want to show,
	namely all infos between start_pos and end_pos inclusive.
*/
void
playlists_range_set(int start_pos, int end_pos){
	cache_range_set(&playlists, start_pos, end_pos);
};

void
mpd_set_playlistcount(int n){
	mpd_model.num_playlists = n;
	model_changed(NUM_PL_CHANGED);
};

void
mpd_store_playlistname(char *name, int playlist_pos){
	cache_store(&playlists, playlist_pos, name);
	model_changed(PL_NAMES_CHANGED);
};

/* 
	Returns number playlists known to MPD
	Is < 0 if the total number of playlists is unknown
*/
int 
mpd_get_num_pl(){
	return mpd_model.num_playlists;
};


/* The user wants a new playlist. 
	We assume the user really wants three things:
	First he wants the current playlist to be emptied.
	Then he wants to load the new playlist.
	And then he wants to play the first song in this playlist.
	This is different than the way MPD interprets the LOAD command.
	We set the CLEAR_AND_LOAD wish here and when that command succeeded, we set the PLAY 0 wish.
	The need_action() routine above must make sure to handle the CLEAR and LOAD request first.
*/
void 
user_wants_playlist(int idx){
	if ((mpd_model.num_playlists < 0) || (idx < 0) || (idx >= mpd_model.num_playlists))
		return;
	
	user_model.playlistlength = -1;		// User doesn't care about length of list. */

	user_model.cur_playlist = idx;		// We want to load this new list 
	user_model.state = UNKNOWN;			// user accepts that state changes while doing this command
};

/* --------------------------------------- Search results ------------------------------- */

/* Given an index starting from 0, we return the corresponding resultlist name entry.
	If we have no info or the result does not exist we return "".
	Does return a non-NULL string
*/
static char *
mpd_result_string(int pos){
	static char string[200];
	if (mpd_model.find_type == FIND_TYPE_ARTIST)
		strlcpy(string, "artist \"", sizeof(string));
	if (mpd_model.find_type == FIND_TYPE_TITLE)
		strlcpy(string, "title \"", sizeof(string));
	if (mpd_model.find_type == FIND_TYPE_ALBUM)
		strlcpy(string, "album \"", sizeof(string));

	strlcat(string,	cache_info(&resultlist, pos), sizeof(string));
	strlcat(string, "\"",sizeof(string));	
	
	return string;
};

char *
mpd_result_info(int pos){
	return cache_info(&resultlist, pos);
};
	
/* 
	The cache containing result info has to follow the information that we show on screen.
	Here we tell the cache which positions we want to show,
	namely all infos between start_pos and end_pos inclusive.

	This routine is called by the scroll_list routines whenever the
	range that the scroll_list shows is changed
*/
void
resultlist_range_set(int start_pos, int end_pos){
	cache_range_set(&resultlist, start_pos, end_pos);
};


void
mpd_store_resultname(char *name, int result_pos){
	cache_store(&resultlist, result_pos, name);
	model_changed(RESULT_NAMES_CHANGED);
};

/* 
	Returns pos of the last result known to MPD
	Is < 0 if the total number of results is unknown
	or if no result at all.
*/
int 
mpd_resultlist_last(){
	return mpd_model.num_results - 1;
};

void
mpd_result_ack(struct MODEL *a){
		mpd_store_resultname("", a->request.arg);	
};


/* -------------------------------------- Searching ----------------------------------------------------------- */

/* Our search command returned the number of results */
void
mpd_store_num_results(int n){
	mpd_model.num_results = n;
	cache_set_limit(&resultlist, n);	
	cache_unknown(&resultlist, 0);			// all results in cache are unknown	
	model_changed(RESULTS_CHANGED);
	model_changed(RESULT_NAMES_CHANGED);
};

int
mpd_get_num_results(){
	return	(mpd_model.num_results);
}

char *
mpd_get_search_string(){
	static char string[200];
	if (mpd_model.find_type == FIND_TYPE_ARTIST)
		strlcpy(string, "artist \"", sizeof(string));
	if (mpd_model.find_type == FIND_TYPE_TITLE)
		strlcpy(string, "title \"", sizeof(string));
	if (mpd_model.find_type == FIND_TYPE_ALBUM)
		strlcpy(string, "album \"", sizeof(string));
	strlcat(string,	user_model.search_string, sizeof(string));
	strlcat(string, "\"",sizeof(string));
	return string;
};

/* A search command has been successfully executed. */
void
mpd_search_ok(struct MODEL *a){
	user_model.search_string = NULL;	// wish fulfilled
};

/* A search command returned an error. */
void
mpd_search_ack(struct MODEL *a){
	user_model.search_string = NULL;	// abort this search
	mpd_store_num_results(0);
};

/* Maybe the user has changed his search string 
	We do not copy the string, but keep a pointer to it.
	It may again change before we are really doing the search command.
	But that is ok, because that way our search string is always up to date.
*/
void
user_set_search_string(char * s){
	user_model.search_string = s;
};

void
mpd_findadd_ok(struct MODEL *a){
	user_model.find_add = -1;				// wish fulfilled
	user_model.playlistlength = -1;
	model_changed(TRACKLIST_CHANGED);
	mpd_status_ok(a);	
};

/* 
	User has found what he wanted.
	He wants to add the result to the current playlist
	idx is the chosen position in the result list 
*/
void
user_find_add(int pos){
	user_model.find_add = pos;
};

int
mpd_find_type(){
	return mpd_model.find_type;
};

void
mpd_set_find_type(int t){
	mpd_model.find_type = t;
};

/* ------------------ Elapsed and total time ------------------------------- */
int
mpd_get_time_total(){
	return mpd_model.time_total;
};

int
mpd_get_time_elapsed(){
	return mpd_model.time_elapsed;
};

/* We have (new) timing information from mpd */
static void
mpd_set_time(int elapsed, int total){
	if ( (mpd_model.time_elapsed != elapsed) || (mpd_model.time_total != total) )
		model_changed(TIME_CHANGED);
	mpd_model.time_elapsed = elapsed;
	mpd_model.time_total = total;
	
	// There are songs with incorrect timing information.
	// It can happen that elapsed time is greater than total time.
	// This is clearly wrong.
	// We temporarily adjust the total time and ask again in a few seconds
	if ((mpd_model.time_total >= 0) && (mpd_model.time_elapsed > mpd_model.time_total))
		mpd_model.time_total = mpd_model.time_elapsed + 3;

	/* Here we check if MPD is close to the specific time wanted by the user.
		If it is closely within the real time, we set users wish to don't care,
		else we leave it as it is (and later try to skip forward or backward)
	*/
	if (abs(user_model.time_elapsed - mpd_model.time_elapsed) < 3)
		user_model.time_elapsed = -1;		// wish fulfilled
};

/* The user wants to skip forward or backward in time a few seconds. */
void
user_wants_time_add(int offs){
	int tmp;
	if (mpd_model.time_elapsed == -1) return;
	tmp = mpd_model.time_elapsed + offs;
	if (tmp < 0) tmp = 0;
	if (mpd_model.time_total != -1)
		if (tmp > mpd_model.time_total) 
			tmp = mpd_model.time_total;
	user_model.time_elapsed = tmp;
};

/* Time flies by when you're the driver of a train :-)
	We increment the elapsed time by 1 second if possible.
	This is done because we expect mpd to behave nicely.
	When we issue the next status command, time is synchronized with MPD
*/
static void
mpd_inc_time(){
	/* Only increase time if we are playing something */
	if (mpd_model.state == PLAY){
		if (mpd_model.time_elapsed >= 0){		// We have a time that we can increment 

			/* If time is up, we set the mpd values to -1 to tell controller that we need info */
			if (mpd_model.time_elapsed >= mpd_model.time_total){
				/* We assume the current song has ended playing.
					We then don't know very much, because playlist might have ended. */
				mpd_set_pos(SONG_UNKNOWN);
			} else {
				/* Time is not up. We can increment elapsed time */
				mpd_model.time_elapsed++;
				model_changed(TIME_CHANGED);
			};
		};
	};
};

/* This thread keeps record of the remaining time of a currently playing song. 
	We could issue a status command every second and get the playing time exactly as seen by mpd.
	But this generates quite some traffic over the air, which we want to avoid.
	So we update the time here once per second.
*/
PT_THREAD (update_playtime(struct pt *pt)){
	static struct timer sec_tmr;
	
	PT_BEGIN(pt);
	
	/* We want to be noticed once per second. */
	timer_add(&sec_tmr, 1*TICKS_PER_SEC, 1*TICKS_PER_SEC);
	
	while (1){
		PT_WAIT_UNTIL(pt, timer_expired(&sec_tmr));
		sec_tmr.expired = 0;
		mpd_inc_time();
	};
	PT_END(pt);
};


/* ------------------------------ Playing state (play, pause, stop) -------------------------- */
/* Used by view_state_changed to show the current playing state */
int
mpd_get_state(){
	return mpd_model.state;
};

/* Sets mpd to a certain playing state (i.e. UNKNOWN,PLAY,PAUSE,STOP) 
	MPD handles the STOP state somewhat strangely.
	The STOP state automatically resets the playing time of the current song to 0!
	And after the playlist is finished, the STOP state is entered and there is no current song.
	But MPD reacts in that situation to a PLAY command with OK and plays the first song.
	We reflect that here in our model.
	Called after a successful state changing command 
*/
static void
mpd_set_state(enum PLAYSTATE newstate){
	if (newstate == mpd_model.state) return;	/* Not new */
	
	mpd_model.state = newstate;
	
	/* STOP resets elapsed time (if there is a song) */
	if (newstate == STOP)
		mpd_set_time(0, mpd_model.time_total);
	
	/* A successful PLAY command even if there was no current song ? Current song is now unknown ! */
	if ((newstate == PLAY) && (mpd_model.pos == NO_SONG))
		mpd_set_pos(SONG_UNKNOWN);
					
	/* This is important to do, because sometimes mpd changes state itself, for example when the playlist finished.
		Then we do not want to repeatedly issue a useless "PLAY" command because the user some time back wanted to play 
		TODO well maybe better to issue a notification in this case so the user knows why playing stopped !
	*/
	if (mpd_model.state == user_model.state)		// wish fulfilled
		user_model.state = -1;
	
	/* We only set the model_changed flags when we know the state, because else we have nothing to show the user.
		We will get a definitive playstate later with a status command (we hope so).
	*/
	if (newstate != UNKNOWN)
		model_changed(STATE_CHANGED);
};

/* We got an OK answer after a state changing command */
void
mpd_state_ok(struct MODEL *a){
	mpd_set_state(user_model.state);
};

/* We got an ACK answer after a state changing command */
void
mpd_state_ack(struct MODEL *a){
		user_wants_state(-1);	// don't try to issue the same command again and again
		// TODO here some error handling, maybe message to the user
};

void
user_wants_state(enum PLAYSTATE s){
	user_model.state = s;
};

/* The user has hit the pause toggle key.
	If we know the current state of mpd, we assume the user wants to toggle that.
 	If current mpd playing state is unknown, we simply do nothing.
	User has to press the key again when mpd_model.state is known.
	Almost all of the time mpd_model.state should be known!
*/
void
user_toggle_pause(){
	if ( (mpd_model.state == PAUSE) || (mpd_model.state == STOP) )
		user_model.state = PLAY;
	else if (mpd_model.state == PLAY)
		user_model.state = PAUSE;
};


/* ---------------------------- Random mode ------------------------------- */
int
mpd_get_random(){
	return mpd_model.random;
};

/* This function is called when we know about the random status by a "status" command or similar.
*/
static void
mpd_set_random(int rnd){
	if (mpd_model.random != rnd){
		mpd_model.random = rnd;
		model_changed(RANDOM_CHANGED);
	};
};

/* This function is called if we successfully issued a "random x" command and got "OK".
	We can then safely assume that random is set the same as the user_model wanted it.
*/
void
mpd_random_ok(struct MODEL *a){
	mpd_set_random(user_model.random);
	user_model.random = -1;
};

void
user_toggle_random(){
	if (mpd_model.random == 0)
		user_model.random = 1;
	else if (mpd_model.random == 1)
		user_model.random = 0;
};


/* ---------------------------- Repeat mode ------------------------------------------ */
int
mpd_get_repeat(){
	return mpd_model.repeat;
};

static void
mpd_set_repeat(int rpt){
	if (mpd_model.repeat != rpt){
		mpd_model.repeat = rpt;
		model_changed(REPEAT_CHANGED);
	};
};

void
mpd_repeat_ok(struct MODEL *a){
	mpd_set_repeat (user_model.repeat);
	user_model.repeat = -1;
};

void
user_toggle_repeat(){
	if (mpd_model.repeat == 0)
		user_model.repeat = 1;
	else if (mpd_model.repeat == 1)
		user_model.repeat = 0;
};


/* ------------------------------------- Single mode -------------------------------------------- */

int
mpd_get_single(){
	return mpd_model.single;
};

static void
mpd_set_single(int sgl){
	if (mpd_model.single != sgl){
		mpd_model.single = sgl;
		model_changed(SINGLE_CHANGED);
	};	
};

void
mpd_single_ok(struct MODEL *a){
	mpd_set_single (user_model.single);
	user_model.single = -1;
};

// NOTE this routine does nothing, if mpd_model.single is == -1
//		this happens when MPD does not support the single command
void
user_toggle_single(){
	if (mpd_model.single == 0)
		user_model.single = 1;
	else if (mpd_model.single == 1)
		user_model.single = 0;
};


/* ------------------------------------------ Current song information ------------------------------- */

/* 
	NOTE here we will return -1 or -2 if there is no current song (-1) or it is unknown (-2).
		caller has to handle those values
*/
int
mpd_get_pos(){
	return mpd_model.pos;
};

/* We have got (maybe new) information about the current song pos */ 
static void
mpd_set_pos(int newpos){
	if (mpd_model.pos == user_model.pos)		// wish fulfilled
		user_model.pos = -1;
	if (mpd_model.pos == newpos) return;		// nothing new

	mpd_model.pos = newpos;

	if (newpos == NO_SONG){ 
		mpd_set_artist("");
		mpd_set_title("");
		mpd_model.songid = NO_SONG;
		mpd_set_state(STOP);						// can set time, but not always correct
		mpd_set_time(-1, -1);	

		if (user_model.state == PLAY){				// user wanted to play something
			user_model.state = STOP;				// not possible, change his wish
		};
	} else { 
		mpd_set_artist(NULL);						// We do not yet know which artist and title we have
		mpd_set_title(NULL);
		mpd_model.songid = UNKNOWN;				// NOTE songid is currently not used
		mpd_set_time(0, -1);						// The 0 is just a guess. But we have to get total time anyway
	};
//	model_changed(POS_CHANGED);						// this is (maybe) important to know for track list handling
													// POS_CHANGED is currently not used
};

/* We sent a "PLAY xxx" command and got "OK". */
void 
mpd_select_ok(struct MODEL *a){
	mpd_set_pos(a->request.arg);
	user_song_unknown();				// wish fulfilled
	mpd_set_state(PLAY);				// MPD starts playing
};

/* Our play xxx command did not succeed 
	Maybe MPD could not find that song (database too old)
	TODO we should inform the user about that
*/
void 
mpd_select_ack(struct MODEL *a){
	user_wants_state(-1);
	user_model.pos = SONG_UNKNOWN;			// ignore user wish
};

/* Called after a succesful NEXT or PREVIOUS command. */
void
mpd_newpos_ok(struct MODEL *a){
	/* We do not really know which song is the current one. There may not even be one or it is random.
		Mark mpd.pos as UNKNOWN to read the info soon.
	*/
	mpd_set_pos(SONG_UNKNOWN);
	user_song_unknown();				// wish fulfilled
};

// TODO not fully implemented, we should change most references of pos to id
/* We have got (maybe new) information about the current song id */ 
static void
mpd_set_id(int newid){
	if (mpd_model.songid == user_model.songid)		// wish fulfilled
		user_model.songid = -1;
	if (mpd_model.songid == newid) return;		// nothing new

	mpd_model.songid = newid;

};

/* Returns a string with the current title or "" if unknown. */
char *
mpd_get_title(){
	if (NULL == mpd_model.title)
		return "";
	return mpd_model.title;
};

/* We are given either NULL or a string with the title tag, NULL meaning title unknown */
static void
mpd_set_title(char *s){
	int length;
	
	if (s == NULL){
		if (mpd_model.title != NULL){
			mpd_model.title = NULL;
			model_changed(TITLE_CHANGED);
		};
	} else {
		if (strn_cpy_cmp(mpd_model.title_buf, s, TITLE_LEN, &length) == 0)	
			model_changed(TITLE_CHANGED);
		if (mpd_model.title == NULL)
			model_changed(TITLE_CHANGED);
		mpd_model.title = mpd_model.title_buf;
	}; 
};

/* Returns a string with the current artist or "" if unknown. */
char *
mpd_get_artist(){
	if (NULL == mpd_model.artist)
		return "";
	return mpd_model.artist;
};

/* We are given either NULL or a string with the artist tag, NULL meaning artist unknown 
	Copies artist information to mpd_model
*/
static void
mpd_set_artist(char *s){
	int length;
	
	if ( (s == NULL) ){
		if (mpd_model.artist != NULL){
			mpd_model.artist = NULL;		
			model_changed(ARTIST_CHANGED);
		};
	} else {
		if (strn_cpy_cmp(mpd_model.artist_buf, s, TITLE_LEN, &length) == 0)	
			model_changed(ARTIST_CHANGED);
		if (mpd_model.artist == NULL)
			model_changed(ARTIST_CHANGED);
		mpd_model.artist = mpd_model.artist_buf;
	}; 
};

/* We got an "OK" for "current song" command */
void
mpd_currentsong_ok(struct MODEL *a){
	
	/* Sometimes songs don't have a tag for title and/or artist.
		We then show a "?" to the user, so he knows that and can maybe correct the tags.
		For empty titles mpdtool substitutes the last part of the filename so that should not happen
		Empty artist can make sense, if you really can not figure out who the hell sings that song.
		We want to avoid showing a "?" if soemhow the radio reception was corrupted and some
		lines were lost. So first we check if we got any information at all.
		If not, we conclude that the command failed and avoid updating the info.
	*/
	if ( (a->pos == SONG_UNKNOWN) || (a->songid == -1) )
		return;
	
	mpd_set_pos(a->pos);
	mpd_set_id(a->songid);
	
	if (a->title == NULL)
		mpd_set_title("?");		// should not occur
	else
		mpd_set_title(a->title);
	
	if (a->artist == NULL)
		mpd_set_artist("?");
	else
		mpd_set_artist(a->artist);
}


static void
user_song_unknown(){
	user_model.pos = SONG_UNKNOWN;
	user_model.state = UNKNOWN;
	user_model.artist=NULL;
	user_model.title=NULL;
	user_model.songid = -1;
	user_model.time_elapsed = -1;
	user_model.time_total = -1;
};


/* -------------------------------------- Volume and Mute ---------------------------------------- */
int	
mpd_get_volume(){
	return mpd_model.volume;
};

/* We have some information about the current mpd volume.
	Update mpd_model and if necessary user_model
*/
static void
mpd_set_volume(int vol){
	if (vol != mpd_model.volume){
		mpd_model.volume = vol;
		if (user_model.volume == mpd_model.volume)
			user_model.volume = -1;					// wish fulfilled
		model_changed(VOLUME_CHANGED);
	}
};

/* We sent a "setvol" command and got "OK" */
void 
mpd_volume_ok(struct MODEL *a){
	mpd_set_volume(a->request.arg);	
};

/* Changes the internal user volume by adding a value to mpd_model.volume 
	If mpd_model.volume is unknown at this point, wished volume will not be set.
*/
void
user_wants_volume_add(int chg){
	if (mpd_model.volume == -1) return;
	user_model.volume = max(0, min(100, mpd_model.volume + chg));
};

/* User wants to toggle the mute status, i.e. if volume was 0, reset it to old value, else set it to 0. 
	volume == 0 is treated as MUTE 
*/
void
user_toggle_mute(){
	if ( mpd_model.volume != 0 ){
		mpd_model.old_volume = mpd_model.volume;		// remember the current volume
		user_model.volume = 0;
	} else
		if (mpd_model.old_volume > 0)
			user_model.volume = mpd_model.old_volume;
		else
			user_model.volume = 15;			// we use a default here in case we have no idea of old volume
};


/* -------------------------------------- Other information -------------------------------------------------- */

/* Set the time that mpd last communicated with us */
void
model_set_last_response(unsigned int t){
	mpd_model.last_response = t;
};

/* TODO
	If we let the SCART software and mpdtool each send regular radio packets to us, we can detect much better
	what kind of communication problem we have.
	But that does not fit the current communication protocol. Every communication is initiated by Betty.
	Possible solution:
	Betty sends status commands in regular intervals (already implemented).
	If they go through to MPD and we get an answer back, everything is ok.
	If MPD is not responding, mpdtool does detect that.
	In this case mpdtool sends an "ACK" answer with an appropriate error number.
	If mpdtool does not respond anymore, the scart adapter has to send something back.
	(Question: Can the scart adapter even recognize a dead mpdtool ?)
	And if we get nothing back after some time, either our own radio link is dead or
	the scart adapter is hung up.
*/

void
model_check_mpd_dead(){
	if ((system_time() - mpd_model.last_response) > MAX_MPD_TIMEOUT * TICKS_PER_SEC){
		model_changed(MPD_DEAD);
		// will time-out again after 10 seconds
		mpd_model.last_response = system_time() - (MAX_MPD_TIMEOUT - MPD_RETRY_TIMEOUT) * TICKS_PER_SEC;
	};
}


/* -------------------------------------- Status -------------------------------------------------- */

/* We got a valid response to a "status" command. */
void
mpd_status_ok(struct MODEL *a){
	mpd_set_volume (a->volume);
	mpd_set_repeat (a->repeat);
	mpd_set_random (a->random);
	mpd_set_single (a->single);
	set_playlistlength(a->playlistlength);
	mpd_set_state(a->state);
	
	// We got "OK" but no pos info. So there definately is no current song.
	if (a->pos == SONG_UNKNOWN)
			mpd_set_pos (NO_SONG);
	else 
		mpd_set_pos (a->pos);
	mpd_set_id (a->songid);

	/* If the state is stopped and there is a current song, MPD does not give us any timing information.
		But the internal elapsed time of MPD is reset to 00:00 
	*/
	if ((mpd_model.state == STOP) && (mpd_model.songid >= 0))
		mpd_set_time (0, mpd_model.time_total);
	else
		mpd_set_time (a->time_elapsed, a->time_total);
	
	/*	Remember the time so we can wait with out next status command.*/	
	mpd_model.last_status = system_time();	
};


/* ----------------------------------------------- Initialization -------------------------------------------------- */
/* sets all values in st to UNKNOWN */
void
model_reset(struct MODEL *m){
	m->num_playlists = -1;
	m->playlistlength = -1;
	m->old_volume = -1;
	m->volume = -1;
	m->state = UNKNOWN;
	m->cur_playlist = -1;
	m->time_elapsed = -1;
	m->time_total = -1;
	m->last_response = 0;
	m->last_status = 0;
	m->pos = SONG_UNKNOWN;
	m->songid = -1;
	m->title = NULL;
	m->artist = NULL;
	m->artist_buf[0] = '\0';
	m->title_buf[0] = '\0';
	m->random = -1;
	m->repeat = -1;
	m->single = -1;
	m->find_type = FIND_TYPE_ARTIST;
	m->search_string = NULL;
	m->find_add = -1;
	m->num_results = -1;
	m->script = -1;
	m->pl_added = 0;
	m->errmsg = NULL;
	m->errmsg_buf[0] = '\0';
};

/* Initialize our model
	We assume that we know nothing at the start
	Must be called only once !
*/
void 
model_init(){
	model_reset(&mpd_model);
	model_reset(&user_model);
	user_song_unknown();
		
	/* We assume that the user wants to play immideately */
	user_model.state = PLAY;
	
	/* We have just reset all infos. So the view has to be updated. */
	model_changed_flags = VOLUME_CHANGED | TITLE_CHANGED | ARTIST_CHANGED | TIME_CHANGED | RANDOM_CHANGED
							| REPEAT_CHANGED | SINGLE_CHANGED | PL_NAMES_CHANGED;
	
	cache_init(&tracklist);
	cache_init(&playlists);
	cache_init(&resultlist);

	/* 
		This task updates the models notion of playtime once per second.
	*/
	task_add(&update_playtime);	
	
};
