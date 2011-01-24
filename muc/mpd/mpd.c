/*
    mpd.c - MPD control functions

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


/*
  We use a model-view-controller architecture here
  
	Our model consists of the status of mpd (including (part of) the current playlist stored in the array play_list[])
	and the status as the user wants it to be.
	If the user status and the mpd status differ, some action is needed (usually commands to mpd).
	
	The view are windows on screen, with 1 window which has the focus
	Changes in the model are reported to the view.
	
	The controller takes input from user and from mpd and changes the model
	accordingly.

*/

#include "global.h"
#include "kernel.h"

#include "timerirq.h"
#include "pt-sem.h"
#include "rf.h"
#include "fonty.h"
#include "window.h"
#include "screen.h"
#include "screen_playlist.h"
#include "screen_playing.h"
#include "screen_tracklist.h"
#include "screen_search.h"
#include "mpd.h"

/* Returns TRUE iff the semaphore count is not zero, i. e. the semaphore is available */
#define PT_SEM_CHECK(s) ((s)->count) 

// Length of response line
#define RESPLEN 1024

static char response[RESPLEN];


/* Somewhat similar to the C function. 
	We don't want to include the whole snprintf function,
	so we include a very reduced version.
	Not very fast.
	Returns 0 iff created string did not fit completely into given size.
*/
int
slprintf(char *dst, const char *src, UserReq *f, int size){
	/* We can fill this array with a number representation */
	char num_string[12];
	int cur_len;			// current length of destination string
	int cur_arg = 1;
	
	if (0 == size)	return 0;
	
	*dst = 0;				// empty destination string
	cur_len = 0;
	
	while (cur_len < size) {
		if (0 == *src) {
			dst[cur_len] = 0;
			return 1;						// finished sucessfully
		};
		
		if ( (*src == '%') && ( *(src + 1) == 'd') ){
			dst[cur_len] = 0;
			if (cur_arg == 1){
				cur_len = strlcat(dst, get_digits(f->arg, num_string, 0), size);
				cur_arg++;
			} else{
				cur_len = strlcat(dst, get_digits(f->arg2, num_string, 0), size);
			};
			src += 2;
		} else if ( (*src == '%') && ( *(src + 1) == 's') ){
			dst[cur_len] = 0;
			cur_len = strlcat(dst, f->str, size);
			src += 2;
		} else {
			dst[cur_len++] = *src++;
		};
	};
	return 0;				// too big
}


/* ------------------------------ The Communicator ----------------------------------------- */


/* ---------------------------- Response collecting handlers ------------------------------------- */
/* Each request from Betty choses one of the following handlers.
	The handler then reads the response line(s) from mpd and tells the model about the results.
*/
 

/* We got a line from mpd after a status command.
	Some information can be handled directly by the model.
	Some information is collected and later transfered to the model.
	We collect that info in the variable ans_status.
	
	Sets response_valid to 1 if we received an "OK" string.
	TODO get more information, namely if some info is not given
*/ 
void 
ans_status_line(char *s, struct MODEL *a){
	int tmp;
	char *s2;
	
	if (strstart(response, "volume: ")){
		a->volume = atoi(response+8);
		return;
	};	
	
	if (strstart(response, "repeat: ")){
		a->repeat = atoi (response+8);
		return;
	};	
			
	if (strstart(response, "random: ")){
		a->random = atoi (response+8);
		return;
	};
	
	if (strstart(response, "single: ")){
		a->single = atoi (response+8);
		return;
	};

	if (strstart(response, "playlistlength: ")){ 
		a->playlistlength = atoi(response+16);
		return;
	};
	
	if (strstart(response, "state: ")){
		tmp = UNKNOWN;
		if (strstart(response+7, "play")) 
			tmp = PLAY;
		else 
			if (strstart(response+7, "pause"))
				tmp = PAUSE;
			else
				if (strstart(response+7, "stop"))
					tmp = STOP;
		a->state = tmp;
		return;
	};
	
	/* Compare with "song: " */
	if (strstart(response, "song: ")){ 
		a->pos = atoi(response+6);
		return;
	};
	
	/* Compare with "Id: " */
	if (strstart(response, "songid: ")){
		a->songid = atoi(response+4);
		return;
	};
	
	if (strstart(response, "time: ")) {
		s2 = strchr(response+6, ':');
		if (s2) {
			a->time_elapsed = atoi(response+6);
			a->time_total = atoi(s2+1);
		};
		return;
	};
};


/* We got a line from mpd after a "current_song" command.
	Gather the information in the variable mpd_status.
*/ 
static void 
ans_currentsong_line(char *s, struct MODEL *a){

	/* Compare with "Artist: " */
	if (strstart(response, "Artist: ")){
		strlcpy(a->artist_buf, response+8, sizeof(a->artist_buf) );
		a->artist = a->artist_buf;
		return;
	};
	
	/* Compare with "Title: " */
	if (strstart(response, "Title: ")){
		strlcpy(a->title_buf, response+7, sizeof(a->title_buf) );
		a->title = a->title_buf;
		return;
	};
	
	/* Compare with "Pos: " */
	if (strstart(response, "Pos: ")){
		a->pos = atoi (response+5);
		return;
	};
	
	/* Compare with "Id: " */
	if (strstart(response, "Id: ")){
		a->songid = atoi (response+4);
	};
};

/*
	The information gathered from playlistinfo response is stored in the variable ans_model.
	It must have been reset to the correct values beforehand.
*/
void 
ans_playlistinfo_line(char *s, struct MODEL *a){

	/* Compare with "Artist: " */
	if (strstart(response, "Artist: ")){
		strlcpy(a->artist_buf, response+8, TITLE_LEN);
		return;
	};
			
	/* Compare with "Title: " */
	if (strstart(response, "Title: ")){
		strlcpy(a->title_buf, response+7, TITLE_LEN);
		return;
	};

	/* Compare with "Pos: " */
	if (strstart(response, "Pos: ")) {
		a->pos = atoi(response+5);
		return;
	};
};

static void 
ans_plcount_line(char *s, struct MODEL *a){
	/* Compare with "playlistcount: " */
	if (strstart(response, "playlistcount: "))
		mpd_set_playlistcount(atoi(response+15));
};


static void
ans_plname_line(char *s, struct MODEL *a){
	/* Compare with "playlist: " */
	if (strstart(response, "playlist: "))
		mpd_store_playlistname(response+10, a->request.arg);
};

/* We sent a "SEARCH xxx xxx" command */
static void
ans_search_line(char *s, struct MODEL *a){
	/* Compare with "results: " */
	if (strstart(response, "results: "))
		mpd_store_num_results(atoi(response+9));
};


static void
ans_result_line(char *s, struct MODEL *a){
	/* Compare with "name: " */
	if (strstart(response, "name: "))
		mpd_store_resultname(response+6, a->request.arg);	
};

 /* ----------------------------------------- End of response gathering functions ------------------------- */ 

/* This semaphore is <> 0 iff a response line from mpd is ready. */
static struct pt_sem line_ready; 


/*  Whenever the radio receives a packet, it is handed to this routine.
	We maintain a buffer (response[] and response_ptr) to store partial payload data.
	In theory, multiple packets should be assembled together to get the full mpd response into the buffer.
	Then we should parse the buffer contents.
	BUT: The data sent by mpd can be very large (for example a complete playlist).
	There are three possible ways around this situation:
	- Only issue commands which we know generate small enough responses.
	- Filter data by sending program mpdtool on the mpd server. 
		We use this approach with the LSINFO command because else we get overwhelmed by responses.
	- Process parts of the response, even when it is not complete and discard them. This works, because 
	  mpd sends responses in the form of small lines with keyword : value pairs and seperated by '\n'.
	
	We have chosen the last approach here.
	The assemble_line() thread assembles payload packets until a complete line has arrived.
	The current line is stored in the variable response[] as a null-terminated string without the newline.
	When one complete line has been assembled, the semaphore line_ready is signaled.
	Then we wait until the line has been processed and line_ready is free again 
	or until a timeout occurs. 

	Thread does never end.
	
	TODO this could be made more efficient. We do not need an extra response buffer.
	The rf module increments a semaphore each time it puts a complete line in its buffer.
	The consuming function(s) (collect_lines) decrement that semaphore.
	The buffer contents need not be copied if the rf module substitutes each "\n" with a '\0' to make C strings.
	But we have to make sure that lines are consumed or discarded within a relatively short time frame, so that the buffer
	is not overflowing.
*/

// ### Line Producing Task ###
//	Started once. Never returns.	
PT_THREAD (assemble_line(struct pt *pt)) {
	static uint8_t c;
	static int response_ptr = 0;
	static struct timer tmr;
	
	PT_BEGIN(pt);
	while (1){
		// Block until we can get data from the receive-buffer.
		PT_WAIT_WHILE(pt, rx_buf_empty());

		/* Copy data to response buffer until we reach '\n' */
		c = get_from_rx_buf();

		/* TODO only for debugging */
		if ( (c < 0x20) && (c != '\n') ) {
			debug_out("Invalid Char ", c);
		};
		 
		if (c != '\n'){
			if (response_ptr < (RESPLEN - 1))
				response[response_ptr++] = c;	
			else debug_out("line too long, character ignored", c);	// if the line is too long, ignore superfluous characters.
		} else {	
			response[response_ptr] = 0;		// Make it a valid C string

			/* Signal a semaphore that there is a line in response[]. */
			PT_SEM_SIGNAL(pt, &line_ready);
			
			dbg(response);
			
			/* Wait until the response buffer has been processed 
				If no one is interested in the line after a short time, we forget it.
			*/
			timer_add(&tmr, 5*TICKS_PER_TENTH_SEC, 0);
			PT_WAIT_UNTIL(pt, !PT_SEM_CHECK(&line_ready) || timer_expired(&tmr));
			timer_del(&tmr);
	
			/* Either way, reset the semaphore */
			PT_SEM_INIT(&line_ready, 0);
			// and we clear the response line
			response_ptr = 0;	
		};
	};
	PT_END(pt);
};


/* We wait for an answer from MPD.
	Process all lines received via radio.
	Detects end of transmission by checking for "OK" and "ACK" lines.
	"OK" lines are processed with function process_ok().
	"ACK" lines are processed with function process_ack().
	All other lines are processed with the function process_line().
	
	Ends with response_finished set to 1 iff we finally received an OK or ACK.
	If ACK or no answer within some time delay exit with response_finished set to 0.
	
	First parameter is the pt structure.
	Second parameter is a function which processes one line of the answer at a time.
		This function interprets the answer and changes the model if appropriate.
	Third parameter is a function which is called when MPD sends an "OK" answer.
		Command succeeded. The function can also update the model.
	Fourth parameter is a function which is called when MPD sends an "ACK" answer.
		Some error happened. Interpret the error, inform the user or ignore it.
	Any of these function parameters can be NULL.

*/
PT_THREAD (collect_lines(struct pt *pt,
			void (*process_line) (char *s, struct MODEL *a),
			void (*process_ok) (struct MODEL *a),
			void (*process_ack) (struct MODEL *a),
			struct MODEL *ans_model,
			int *response_finished
			)) {
	static struct timer tmr;
			
	PT_BEGIN(pt);
	*response_finished = 0;
	
	/* We should receive all the answers in a relatively short time frame, else something went wrong anyway 
		Searching takes somewhat longer, so we are waiting around 2 seconds.
	*/
	timer_add(&tmr, 22 * TICKS_PER_TENTH_SEC, 0);
	
	while (1){

		PT_WAIT_UNTIL(pt, (PT_SEM_CHECK(&line_ready) || timer_expired(&tmr)) );
		
		if (PT_SEM_CHECK(&line_ready) ){
			if (strstart(response, "OK")) {
				if (process_ok) 
					process_ok(ans_model);
				*response_finished = 1;
				break;
			};
		
			if (strstart(response, "ACK")){
				strlcpy(ans_model->errmsg_buf, response + 3, ERRMSG_SIZE);
				ans_model->errmsg=ans_model->errmsg_buf;
				if (process_ack) 
					process_ack(ans_model);
				*response_finished = 1;
				break;
			};
		
			/* gather information from response line by the given function */
			if (process_line) 
				process_line(response, ans_model);
			
			PT_SEM_INIT(&line_ready, 0);	// Tell producer that we consumed the line
		
		} else {							// Time Out	
			dbg("collect_lines() timed out");
			break;
		};	
	};
	
	PT_SEM_INIT(&line_ready, 0);	// Tell producer that we consumed the line if there is one
	
	timer_del(&tmr);
		
	PT_END(pt);
};

/* ---------------------------- End of functions which handle communication with mpd ------------------- */
 	

/* This routine is called when our model has changed in some way. 
	We inform the view about the changed items.
	TODO better make this a seperate task, so that we do not spend too much time in 
		this routine 
*/
void 
inform_view(int model_changed){
	if (model_changed & VOLUME_CHANGED)
		view_volume_changed(mpd_get_volume());
	
	if (model_changed & TITLE_CHANGED)
		view_title_changed(mpd_get_title());

	if (model_changed & ARTIST_CHANGED)
		view_artist_changed(mpd_get_artist());
	
	if (model_changed & TIME_CHANGED)
		view_time_changed(mpd_get_time_elapsed(), mpd_get_time_total());
	
	if (model_changed & STATE_CHANGED)
		view_state_changed(mpd_get_state());
	
	if (model_changed & RANDOM_CHANGED)
		view_random_changed(mpd_get_random());
	
	if (model_changed & REPEAT_CHANGED)
		view_repeat_changed(mpd_get_repeat());
	
	if (model_changed & SINGLE_CHANGED)
		view_single_changed(mpd_get_single());
	
	
	// This should come before TRACKLIST_CHANGED
	if (model_changed & PL_LENGTH_CHANGED)
		view_pl_length_changed(mpd_get_pl_length(), mpd_get_pl_added());
	
	if (model_changed & TRACKLIST_CHANGED)
		view_tracklist_changed();
	
	if ( (model_changed & PL_LENGTH_CHANGED) || (model_changed & POS_CHANGED) )
		view_pos_changed();
	
	if (model_changed & NUM_PL_CHANGED){
		view_num_pl_changed(mpd_get_num_pl());
	};	
	
	if (model_changed & PL_NAMES_CHANGED){
		view_playlists_changed();
	};

	
	if (model_changed & RESULTS_CHANGED){
		view_results_changed(mpd_get_num_results());
	};
	
	if (model_changed & RESULT_NAMES_CHANGED){
		view_resultnames_changed();
	};	
	
	if (model_changed & MPD_DEAD){
		/*We try to solve the communication error by resetting our
		radio reception to a sane state.
		*/
		rx_reset();
		// We show the message for 1 seconds less than the time_out, so if the
		// communication is still broken, the message will soon reappear.
		view_message("     Error\n\n"
				"Communication with MPD is broken.\n\n"
				"Check if MPD is still running and reachable via LAN.",
	 		(MPD_RETRY_TIMEOUT - 1) * TICKS_PER_SEC);
	};
	
	if (model_changed & PLAYLIST_EMPTY){
		view_message("   Playlist\n   is empty!", 4 * TICKS_PER_SEC);
	};
	
	model_reset_changed();
};
	

/* ------------------------------ The Controller ----------------------------------------- */

/* This structure has info about how to process a command */
struct cmd_proc_info {
	char *format_string;								// string sent to mpd with %d and %s parameters substituted
 	void (*process_line) (char *s, struct MODEL *a);	// function to be called for each answer line from MPD
	void (*process_ok) (struct MODEL *a);				// function to be called when MPD has answered with "OK"
	void (*process_ack) (struct MODEL *a);				// function to be called when MPD has answered with "ACK"
};

/* For each possible CMD the necessary cmd_proc_info
	NOTE The order has to be the same as in enum USER_CMD
*/
static const struct cmd_proc_info cmd_info[] = {
	{"", NULL, NULL, NULL},										// NO_CMD,
	{"play %d\n", NULL, mpd_select_ok, mpd_select_ack},			// SEL_SONG,
	{"", NULL, NULL, NULL},										// VOLUME_UP,
	{"", NULL, NULL, NULL},										// VOLUME_DOWN,
	{"", NULL, NULL, NULL},										// MUTE_CMD,
	{"setvol %d\n", NULL, mpd_volume_ok, NULL},					// VOLUME_NEW,
	{"currentsong\n", ans_currentsong_line, mpd_currentsong_ok, NULL},		// CUR_SONG_CMD,
	{"previous\n", NULL, mpd_newpos_ok, NULL}, 				// PREV_CMD,
	{"next\n", NULL, mpd_newpos_ok, NULL},					// NEXT_CMD,
	{"pause 1\n", NULL, mpd_state_ok, mpd_state_ack},		// PAUSE_ON,
	{"pause 0\n", NULL, mpd_state_ok, mpd_state_ack},			// PAUSE_OFF,
	{"play\n", NULL, mpd_state_ok, mpd_state_ack},					// PLAY_CMD,
	{"stop\n", NULL, mpd_state_ok, mpd_state_ack},					// STOP_CMD,
	{"seek %d %d\n", ans_status_line, mpd_status_ok, NULL}, 	//FORWARD_CMD,
	{"seek %d %d\n", ans_status_line, mpd_status_ok, NULL},		//REWIND_CMD,
	{"status\n", ans_status_line, mpd_status_ok, NULL},					// STATUS_CMD,
	{"playlistinfo %d\n",ans_playlistinfo_line, mpd_playlistinfo_ok, mpd_playlistinfo_ack},		//PLINFO_CMD,
	{"loadnew \"%s\"\n", ans_status_line, mpd_load_ok, NULL},			// LOAD_CMD,
	{"random %d\n", NULL, mpd_random_ok, NULL},				// RANDOM_CMD,
	{"repeat %d\n", NULL, mpd_repeat_ok, NULL},				// REPEAT_CMD,
	{"single %d\n", NULL, mpd_single_ok, NULL},				// SINGLE_CMD,
	{"playlistcount\n",	 ans_plcount_line, NULL, NULL},		// PLAYLISTCOUNT_CMD,
	{"playlistname %d\n", ans_plname_line, NULL, NULL},		// PLAYLISTNAME_CMD,
	{"clear\n", NULL, mpd_clear_ok, NULL},					// CLEAR_CMD,
	{"search %s\n", ans_search_line, mpd_search_ok, mpd_search_ack},		// SEARCH_CMD,
	{"result %d\n", ans_result_line, NULL, mpd_result_ack}, 				// RESULT_CMD,
	{"findadd %s\n", ans_status_line, mpd_findadd_ok, NULL},	// FINDADD_CMD,
	{"script %d\n", NULL, mpd_script_ok, NULL}				// SCRIPT_CMD
};	


/* Some action (Communication with MPD) is needed. Start this action.
	Here we construct the string which must be sent to MPD and then
	we spawn a new protothread (handle_cmd()) which sends this string to MPD
	and processes the received answer. 
*/
static
PT_THREAD (exec_action(struct pt *pt, struct MODEL *ans_model) ){
	static struct pt child_pt;
	static char cmd_str[255];			// maximum that rf.c can handle
	static int tries;
	const int max_tries = 2;		// NOTE is this value good, should we retry at all?
	static int response_finished;	// is set by collect_lines if MPD response terminated correctly (OK or ACK)
	
	PT_BEGIN(pt);

	slprintf(cmd_str, cmd_info[ans_model->request.cmd].format_string, &(ans_model->request), sizeof(cmd_str));

	for (tries = 0; tries < max_tries; tries++){	
		
		/* NOTE after acquiring exclusive access to mpd we should initialize line_ready, 
				because we don't want old answers to fool this routine 
			This is not so easy. We need a kind of flush_rx_buffer() routine.
			And even then there could come some belated answers from MPD after the flushing.
			This problem rarely happens, because we always wait some time after sending commands to 
			MPD. So we simply ignore it here.
		*/
		model_reset(ans_model);
		send_cmd(cmd_str);
		dbg(cmd_str);

		/* This thread collects all information from mpd and sets response_finished. 
			It will time out after a short period.
		*/
		PT_SPAWN(pt, &child_pt, collect_lines(&child_pt, 
				cmd_info[ans_model->request.cmd].process_line,
				cmd_info[ans_model->request.cmd].process_ok,
				cmd_info[ans_model->request.cmd].process_ack,
				ans_model,
				&response_finished));
	
		if (response_finished){							// Our cmd got an OK or ACK
			model_set_last_response (system_time());
			break;
	
		} else {
			// MPD did not give a valid response, try sending again
				debug_out("Retry ",tries);
		};
	};
	
	if (tries >= max_tries) {
		debug_out("No answer ", 1);
		model_check_mpd_dead();
	};
	
	PT_END(pt);
};


/* How does the controller work ? 
	We have an internal model of the state of mpd. (see file mpd.c)
	We assume this is ok until we know otherwise.
	The state of mpd is reflected in the view.
	
	Now when the user is not satisfied with the current state, he initiates some action.
	We reflect the changes that the user wants in the user state model. (see mpd.c)
	
	Now whenever the user state and the mpd state differ, 
	the controller issues one ore more commands to MPD to get to the user desired state.
	It also sets the potentially changing items in mpd_state to unknown, because we
	do not know for sure if and how MPD reacts to the command.
	Sometimes we get the new mpd state directly with the answer, so we are done.
	Sometimes mpd does not react or does not tell us enough details in its answer.
	So:
	The controller continuously checks for unknown state items in mpd_model.
	It then sends commands to get the unknown items and updates mpd_model.
	This way we repeatedly try commands until user_state and mpd_state are compatible.
	
	A complication:
	We have a state item that is frequently changing without user intervention, namely the
	remaining playing time of the current song. It would be inefficient and consume wireless bandwidth 
	to check every second for the remaining playing time, because it is predictable.
	We use a seperate task to update the time information regularily. We also assume 
	that the user is expecting the passage of time and set the same time in the user model.
	
	Further complications:
	MPD might change its own status expectedly and unexpectedly.
	MPD changes state expectedly whenever the current song has ended.
	It might also change state unexpectedly due to an error or due to other clients.
	So the mpd model is regularily updated by issuing "status" commands.

	NOTE  We should build in a failure mode, where we check for error messages from mpd and
	whether we can reach mpd at all.
*/

/* ### Controller Task ###
	Started once. Never returns.
	
	This is the brain of the program.
	It has three main functions:
	- Make sure the model is up to date
	- Start actions if they are needed
	- Show current status on screen
	
	Starts a thread to handle sending commands to mpd (handle_cmd).
	Starts a thread to assemble a complete line from mpd rf ring buffer (assemble_line).
	
	Main loop:
	Waits for a necessary action or a change in the model
	and executes the action and informs the view accordingly.
	
*/
PT_THREAD (controller(struct pt *pt)){
	static struct pt child_pt;

	/* We gather the request and some information from the responses in this variable: */
	static struct MODEL ans_model;
	
	PT_BEGIN(pt);

	/* At start of program we should have no response line from mpd */
	PT_SEM_INIT(&line_ready, 0);
	task_add(&assemble_line);
	

	/* The main loop of the controller.
		- Simply wait if nothing to do.
		- If there is some action to do, spawn a new protothread which starts the action (function exec_action() ).
			Then wait until that protothread finished.
		- If there is some change in the model, call the function inform_view().
	*/
	while (1) {
		/* This is a bit tricky. 
			We have two different conditions on which we must react. We must send a command to mpd when action_needed() is TRUE.
			And we must change the view when model_get_changed() is TRUE.
			We want to call action_needed() only once for reasons of efficiency, so we have to know the value of action_needed() 
			after the WAIT is finished.
		 	Therefore action_needed() sets its parameter request to a valid command when it is TRUE or to NO_CMD when it is FALSE.	
			We have to call action_needed first here so that request is set correctly in any case.
			TODO maybe it's more logical to have two threads, one for communication and the other to change the view
				Maybe that could lead to race conditions ?
		*/
		PT_WAIT_UNTIL(pt, ( action_needed(&ans_model.request) || model_get_changed()) );

	
		if ( ans_model.request.cmd != NO_CMD ){
			PT_SPAWN( pt, &child_pt, exec_action(&child_pt, &ans_model) );
			PT_YIELD(pt);
		};
		

		if ( model_get_changed() ) {
			inform_view(model_get_changed());
			PT_YIELD(pt);
		};

	};
	PT_END(pt);
};


/* ------------------------------ End of Controller ----------------------------------------- */
