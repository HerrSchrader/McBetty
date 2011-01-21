/* model.h */

#ifndef MODEL_H
#define MODEL_H

/* mpd can be in one of those states TODO maybe add a state ERROR */
enum PLAYSTATE {UNKNOWN, STOP, PAUSE, PLAY};


/* These are the possible values for model_changed_flags */
#define VOLUME_CHANGED		(1<<0)
#define TITLE_CHANGED		(1<<1)
#define ARTIST_CHANGED		(1<<2)
#define TIME_CHANGED		(1<<3)
#define STATE_CHANGED		(1<<4)
#define PL_NAMES_CHANGED	(1<<5)
#define TRACKLIST_CHANGED	(1<<6)
#define POS_CHANGED			(1<<7)
#define RANDOM_CHANGED		(1<<8)
#define REPEAT_CHANGED		(1<<9)
#define SINGLE_CHANGED		(1<<10)
#define ERROR_FLAG			(1<<11)
#define RESULTS_CHANGED		(1<<12)
#define RESULT_NAMES_CHANGED	(1<<13)
#define PL_LENGTH_CHANGED	(1<<14)
#define NUM_PL_CHANGED		(1<<15)
#define MPD_DEAD			(1<<16)
#define PLAYLIST_EMPTY		(1<<17)

// Length of artist and title strings each, some songs and some albums really have long titles
#define TITLE_LEN 149
// Size of the character array needed to store artist and title strings each
#define TITLE_SIZE	(TITLE_LEN + 1)

// Maximum number of seconds that we wait for MPD to send anything to us before raising an error
// Currently we rely on the fact that we send a "status" command regularily,
// so we should get our regular responses from MPD
#define MAX_MPD_TIMEOUT 34
#define MPD_RETRY_TIMEOUT 10

// status command interval, must be smaller than MAX_MPD_TIMEOUT
#define STATUS_SYNC_TIME 25

// Maximum length of MPD error message that we store
#define ERRMSG_LEN 	63
// Size of the character array needed to store MPD error message
#define ERRMSG_SIZE	(ERRMSG_LEN + 1)

/* MPD gives a 0-based answer on song pos. 
 * We need 4 extra values here:
  -1 for unknown (or don't care)
  -2 for MPD has no current song
  -3 for user wants next song
  -4 for user wants previous song
  NOTE SONG_UNKNOWN has to be -1 
*/
#define PREV_SONG		-4
#define NEXT_SONG		-3
#define NO_SONG			-2
#define SONG_UNKNOWN	-1

struct MODEL {
	int	num_playlists;			// total number of playlists known by MPD (-1 if unknown to us)
	int cur_playlist;			// index to currently loaded playlist (or -1 if unknown)
	int playlistlength;			// length (number of songs) of the current playlist (-1 if unknown)
	int volume;					// -1 if unknown
	int old_volume;				// -1 if unknown, volume before a mute command, used to unmute
	int pos; 					// current song position in the playlist. Starts with 0! For values below 0 see above.
	enum PLAYSTATE state;		// PLAY, PAUSE, STOP or UNKNOWN
	int8_t	random;				// 1 if random mode is on
	int8_t repeat;				// 1 if repeat mode is on
	int8_t single;				// 1 if single mode is on
	int songid;					// MPD's internal id of the current song
	char artist_buf[TITLE_SIZE];	// If cur_artist points here, this is the artist tag, else irrelevant 
	char title_buf[TITLE_SIZE];	// If cur_title points here, this is the title tag, else irrelevant 
	char *artist;				// NULL if not known, else points to info in artist_buf;
	char *title;				// NULL if not known, else points to info in title_buf;
	int time_elapsed;			// in seconds
	int time_total;				// in seconds
	unsigned int last_response;	// system time when we last saw a response line from mpd (for error detection)
	unsigned int last_status;	// system time when we last got a valid status answer
	char *search_string;		// is <> NULL iff the user wants to search for something
	int add;					// is <> -1 iff the user wants to add something to the playlist
	int num_results;			// number of results after a search command
	unsigned int script;		// if the user wants a script to be executed this is >= 0
	int pl_added;				// no. of songs added to the paylist (either + or -), 0 means no change or unknown
	char errmsg_buf[ERRMSG_SIZE];
	char *errmsg;				// error message from MPD, NULL if no error
	UserReq request;			// only used by ans_model, request that this answer is for
};


int model_get_changed(void);
void model_reset_changed();

int action_needed(UserReq *request);

/* ------------------------------------ Scripts --------------------------------------- */
void user_wants_script(int script_no);
void mpd_script_ok(struct MODEL *a);

/* ------------------------------------- Tracklist = current playlist -------------------------------------- */
void tracklist_range_set(int start, int end);
char *track_info(int no);
void mpd_playlistinfo_ok(struct MODEL *a);
void mpd_playlistinfo_ack(struct MODEL *a);
int mpd_tracklist_last();
void user_tracklist_clr();
void mpd_clear_ok(struct MODEL *a);
void mpd_load_ok(struct MODEL *a);
int mpd_get_pl_length();
int mpd_get_pl_added();
/* ------------------------------------- Playlists -------------------------------------- */
void mpd_set_playlistname(char *s);
char *mpd_playlistname_info(int idx);
int mpd_playlists_last();
void user_wants_playlist(int idx);
void playlists_range_set(int start_pos, int end_pos);
void mpd_set_playlistcount(int n);
void mpd_store_playlistname(char *name, int playlist_pos);
int mpd_get_num_pl();
/* --------------------------------------- Search results ------------------------------- */
char *mpd_result_info(int i);
void resultlist_range_set(int start_pos, int end_pos);
int  mpd_resultlist_last();
void mpd_result_ack(struct MODEL *a);
void mpd_store_resultname(char *name, int result_pos);

/* -------------------------------------- Searching ----------------------------------------------------------- */
void mpd_store_num_results(int n);
int mpd_get_num_results();
char *mpd_get_search_string();
void mpd_search_ok(struct MODEL *a);
void mpd_search_ack(struct MODEL *a);
void user_set_search_string(char * str);
void mpd_findadd_ok();
void user_wants_add(int idx);

/* ------------------ Elapsed and total time ------------------------------- */

int mpd_get_time_total();
int mpd_get_time_elapsed();
void user_wants_time_add(int offs);

/* ------------------------------ Playing state (play, pause, stop) -------------------------- */
void user_toggle_pause(void);
void user_set_state(enum PLAYSTATE s);
int mpd_get_state();
void mpd_state_ok(struct MODEL *a);
void mpd_state_ack(struct MODEL *a);
void user_wants_state(enum PLAYSTATE s);

/* ---------------------------- Random mode ------------------------------- */
int mpd_get_random();
void mpd_random_ok(struct MODEL *a);
void user_toggle_random();
void user_toggle_mute(void);

/* ---------------------------- Repeat mode ------------------------------------------ */
int mpd_get_repeat();
void mpd_repeat_ok(struct MODEL *a);
void user_toggle_repeat();

/* ------------------------------------- Single mode -------------------------------------------- */
int mpd_get_single();
void mpd_single_ok(struct MODEL *a);
void user_toggle_single();

/* ------------------------------------------ Current song information ------------------------------- */
int mpd_get_pos();
void mpd_select_ok(struct MODEL *a);
void mpd_select_ack(struct MODEL *a);
void mpd_newpos_ok(struct MODEL *a);
char *mpd_get_title();
char *mpd_get_artist();
void user_wants_song(int pos);
void mpd_currentsong_ok(struct MODEL *a);

/* -------------------------------------- Volume and Mute ---------------------------------------- */
int	mpd_get_volume();
void user_wants_volume_add(int chg);
void mpd_volume_ok(struct MODEL *a);

/* -------------------------------------- Status -------------------------------------------------- */
void mpd_status_ok(struct MODEL *a);

/* -------------------------------------- Other information -------------------------------------------------- */
void model_check_mpd_dead();
void model_set_last_response(unsigned int time);
void model_reset(struct MODEL *m);

int get_comm_error();

#endif

