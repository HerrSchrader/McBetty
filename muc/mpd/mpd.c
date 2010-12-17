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
#include "mpd.h"

/* Returns TRUE iff the semaphore count has reached zero, i. e. the semaphore is available */
#define PT_SEM_CHECK(pt, s) (0 == (s)->count) 


// Length of response line
#define RESPLEN 1024

static char response[RESPLEN];

static int volume_arg;
static enum PLAYSTATE state_arg;
static int song_arg;

/* Set by function track_list_needed() and read by handle_user_req() */
static int req_track_no;

/* We gather a some information from the responses in this variable: */
static struct STATUS ans_model;

/* ------------------------------ The Communicator ----------------------------------------- */

/* This semaphore protects access to communication with mpd. 
	It is initialized to 1, so that only 1 thread at a time can communicate with mpd.
	Communication means sending a command and receiving the answer.
	We must protect it, so that only 1 thread is potentially interested in the answer from mpd.
	And so that no other thread can send a command before an answer has arrived (or a timeout).
*/
static struct pt_sem mpd_mutex;


/* Variable which is set by the response collecting routines.
	When we got a response with "OK" we set this to 1.
	If no response or "ACK" then it will stay at 0.
	This is used by PT_THREAD collect_lines() to end the thread when a valid answer has been received
	TODO we should also end the thread when an ACK has been received, because then the response is finished.
	TODO where is this needed? Only for error checking ?
*/
static int response_valid;

/* ---------------------------- Response collecting handlers ------------------------------------- */
/* Each request from Betty choses one of the following handlers.
	The handler then reads the response line(s) from mpd and tells the model about the results.
*/
 
/* We got a line from mpd after a "current_song" command.
	Gather the information in the variable mpd_status.
	Sets response_valid to 1 if we received an "OK" string.
*/ 
static void 
process_currentsong_line(char *s){

	/* Compare with "Artist: " */
	if (strstart(response, "Artist: ")){
		mpd_set_artist(response+8);
		return;
	};
	
	/* Compare with "Title: " */
	if (strstart(response, "Title: ")){
		mpd_set_title(response+7);
		return;
	};
	
	/* Compare with "Pos: " */
	if (strstart(response, "Pos: ")){
		mpd_set_pos (atoi (response+5));
		return;
	};
	
	/* Compare with "Id: " */
	if (strstart(response, "Id: ")){
		mpd_set_id (atoi (response+4));
	};
	
	/* Compare with "OK" */
	if (strstart(response, "OK"))
		response_valid = 1;
};


static void 
process_lsinfo_line(char *s){
	/* Compare with "playlist: " */
	if (strstart(response, "playlist: "))
		mpd_set_playlistname(response+10);

	/* Compare with "OK" */
	if (strstart(response, "OK")) {
		response_valid = 1;
	};
};

/* We got a line from mpd after a status command.
	Some information can be handled directly by the model.
	Some information is collected and later transfered to the model.
	We collect that info in the variable ans_status.
	
	Sets response_valid to 1 if we received an "OK" string.
	TODO get more information, namely if some info is not given
*/ 
void 
process_status_line(char *s){
	int tmp;
	char *s2;
	
	if (strstart(response, "playlistlength: ")){ 
		set_playlistlength(atoi(response+16));
		return;
	};
	
	if (strstart(response, "volume: ")){
		mpd_set_volume (atoi(response+8));
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
		mpd_set_state(tmp);
		return;
	};
	
	/* Compare with "song: " */
	if (strstart(response, "song: ")){ 
		mpd_set_pos (atoi(response+6));
		ans_model.pos = atoi(response+6);
		return;
	};
	
	/* Compare with "Id: " */
	if (strstart(response, "songid: ")){
		mpd_set_id (atoi (response+4));
		return;
	};
		
	if (strstart(response, "random: ")){
		mpd_set_random (atoi (response+8));
		return;
	};
	
	if (strstart(response, "repeat: ")){
		mpd_set_repeat (atoi (response+8));
		return;
	};	
	
	if (strstart(response, "single: ")){
		mpd_set_single (atoi (response+8));
		return;
	};
		
	if (strstart(response, "time: ")) {
		s2 = strchr(response+6, ':');
		if (s2)
			mpd_set_time( atoi(response+6), atoi(s2+1) );
		return;
	};
	
	/* Compare with "OK" */
	if (strstart(response, "OK")) {
		/* status command finished successfully.
		 Any variables still unknown were not part of the answer.
		*/
		if (ans_model.pos == SONG_UNKNOWN)
			mpd_set_pos (NO_SONG);
		
		mpd_status_ok();
		response_valid = 1;
		return;
	};
};


/* We got a line from mpd and expect only OK or ACK answer.
	Sets response_valid to 1 if we received an "OK" string.
*/ 
void 
process_ok(char *s){
	/* Compare with "OK" */
	if (strstart(response, "OK")) {
		response_valid = 1;
	};
};


/* We sent a "LOAD" command
	Inform mpd that the new playlist was loaded and set response_valid to 1 if we received an "OK" string.
*/ 
void 
process_load_line(char *s){
	if (strstart(s, "OK")) {
		mpd_load_ok();
		response_valid = 1;
	}
};

void 
process_rnd(char *s){
	if (strstart(s, "OK")) {
		mpd_random_ok();
		response_valid = 1;
	}
};

void 
process_rpt(char *s){
	if (strstart(s, "OK")) {
		mpd_repeat_ok();
		response_valid = 1;
	}
};

void 
process_sgl(char *s){
	if (strstart(s, "OK")) {
		mpd_single_ok();
		response_valid = 1;
	}
};

/*
	Checking answers to a song changing command (next, previous)
*/
void
process_song(char *s){
	if (strstart(s, "OK")) {
		mpd_pos_ok();
		response_valid = 1;
	}
};

/* We sent a "setvol" command */
static void 
process_volume_line(char *s){
	if (strstart(s, "OK")) {
		mpd_set_volume(volume_arg);	
		response_valid = 1;
	};
};

/* We sent a state changing command (PLAY, PAUSE, STOP) */
static void 
process_state_line(char *s){
	if (strstart(s, "OK")) {
		mpd_state_ok();
		response_valid = 1;
	};
	if (strstart(s, "ACK")) {
		mpd_state_ack();	
		response_valid = 1;
	};
};

/*
	The information gathered from playlistinfo response is stored in the variable ans_model.
	It must have been reset to the correct values beforehand.
*/
void 
process_playlistinfo_line(char *s){

	/* Compare with "Artist: " */
	if (strstart(response, "Artist: ")){
		strn_cpy(ans_model.artist_buf, response+8, TITLE_LEN);
		return;
	};
			
	/* Compare with "Title: " */
	if (strstart(response, "Title: ")){
		strn_cpy(ans_model.title_buf, response+7, TITLE_LEN);
		return;
	};

	/* Compare with "Pos: " */
	if (strstart(response, "Pos: ")) {
		ans_model.pos = atoi(response+5);
		return;
	};

	/* Compare with "ACK" */
	/* NOTE It happens not too infrequently, that mpd cannot find a song in the tracklist (database changed).
		We could issue a delete command or inform the user if this is the case.
		For now we just set the info to a value of "- not - found -" so that we do not repeatedly ask mpd about it.
	*/
	if (strstart(response, "ACK")) {
		strn_cpy(ans_model.artist_buf, "- not", TITLE_LEN);
		strn_cpy(ans_model.title_buf, "found -", TITLE_LEN);
		if (ans_model.pos == req_track_no)
			model_store_track(ans_model.title_buf, ans_model.artist_buf, ans_model.pos);
		return;
	};
	
	/* Compare with "OK" */
		//TODO We could here detect tracks without title and name tags and substitute the filename instead.
	if (strstart(response, "OK")) {
		if (ans_model.pos == req_track_no)
			model_store_track(ans_model.title_buf, ans_model.artist_buf, ans_model.pos);
		response_valid = 1;
		return;
	};
};

/* We sent a "PLAY xxx" command. */
static void 
process_play_line(char *s){
	if (strstart(s, "OK")) {
		mpd_set_state(PLAY);
		mpd_set_pos(song_arg);
		response_valid = 1;
	};
	if (strstart(s, "ACK")) {		// TODO mpd does not want to play that song. Inform the user.
		user_wants_state(-1);	
		user_set_pos(-1);	
		response_valid = 1;
	};	
	
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
			
			debug_out(response, 0);
			
			/* Wait until the response buffer has been processed 
				If no one is interested in the line, we forget it.
			*/
			timer_add(&tmr, 5*TICKS_PER_TENTH_SEC, 0);
			PT_WAIT_UNTIL(pt, PT_SEM_CHECK(pt, &line_ready) || timer_expired(&tmr));
			timer_del(&tmr);
	
			/* Either way, reset the semaphore */
			PT_SEM_INIT(&line_ready, 0);

			response_ptr = 0;	
		};
	};
	PT_END(pt);
};


/* We have at least one line of response to our command from mpd.
	Collect this and the following lines until "OK" or "ACK" or time out.
	Each line collected is processed with the function process_line.
	Ends with valid_response set to 1 iff we finally received an OK.
	If ACK or no answer within some time delay exit with response_valid set to 0.
*/
PT_THREAD (collect_lines(struct pt *pt, void (*process_line) (char *s))  ){
	static struct timer tmr;
	
	PT_BEGIN(pt);
	/* We should receive all the answers in a relatively short time frame, else something went wrong anyway */
	timer_add(&tmr, 15*TICKS_PER_TENTH_SEC, 0);
	response_valid = 0;
	
	do { 
		/* gather information from response line by the given function */
		process_line(response);
		PT_SEM_INIT(&line_ready, 0);

		if (response_valid){
			debug_out("valid answer ", 0);
			break;		/* and end the thread */
		};	
						
		// Wait for next line
		PT_WAIT_UNTIL(pt, (!PT_SEM_CHECK(pt, &line_ready) || timer_expired(&tmr)) ); 
	
	} while (!response_valid && !timer_expired(&tmr));
	
	timer_del(&tmr);	
	PT_END(pt);
};



/* Thread to send a command, collect replies from mpd and do error handling 
	The command string sent to mpd is given as second argument.
	Third argument is a function which is called to collect responses.
	Fourth argument is a function which is called after we got a valid response ("OK"). // NOTE not used !
	Fifth argument tells us how often we should try to send the command.
	TODO we could add more parameters concerning time out and error handling.
	Thread waits for semaphore mpd_mutex.
*/
PT_THREAD (handle_cmd(struct pt *pt, char *cmd, void (*process_line) (char *s), void (*process_info) (void), int max_tries)){
	static struct timer tmr;
	static int tries;
	static struct pt child_pt;
	
	PT_BEGIN(pt);
		

	/* We want to acquire access to mpd for this command and its answer */
	PT_SEM_WAIT(pt, &mpd_mutex);

	for (tries = 0; tries < max_tries; tries++){	
		
		/* NOTE after acquiring exclusive access to mpd we should initialize line_ready, 
				because we don't want old answers to fool this routine 
			This is not so easy. We need a kind of flush_rx_buffer() routine.
			And even then there could come some belated answers from MPD after the flushing.
			This problem rarely happens, because we always wait some time after sending commands to 
			MPD. So we simply ignore it here.
		*/
		send_cmd(cmd);
		debug_out(cmd, 0);
		
		// TODO overly complicated
		// move handling of time out etc. to collect_lines
		
		/* Time that we want to wait for first response line */
		timer_add(&tmr, 7*TICKS_PER_TENTH_SEC, 0);
		
		PT_WAIT_UNTIL(pt, !PT_SEM_CHECK(pt, &line_ready) || timer_expired(&tmr));
		
		if (timer_expired(&tmr)){
			timer_del(&tmr);
			debug_out("retry ", tries);
			continue;
		};
	
		timer_del(&tmr);
		if (! PT_SEM_CHECK(pt, &line_ready) ){
			// We got at least one line of response from MPD
			model_set_last_response (system_time());
			 clr_error(MPD_DEAD);
			 
			/* This thread collects all information from mpd and sets response_valid. */
			PT_SPAWN(pt, &child_pt, collect_lines(&child_pt, process_line));

			if (response_valid){
				/* Now our cmd got an OK. Process the info gathered (if necessary) */
				if (process_info) process_info();
				break;
			}
				
		} else {
			// MPD did not respond at all
		};	
	};
	
	/* Now other threads can again communicate with mpd */
	PT_SEM_SIGNAL(pt, &mpd_mutex);
		
	if (tries >= max_tries) {
			debug_out("No answer ", 1);
			model_check_mpd_dead();
	};
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
	
	if (model_changed & TITLE_CHANGED){
		if (mpd_get_curtitle() == NULL)
			view_title_changed("");
		else	
			view_title_changed(mpd_get_curtitle());
	};
	if (model_changed & ARTIST_CHANGED){
		if (mpd_get_curartist() == NULL)
			view_artist_changed("");
		else
			view_artist_changed(mpd_get_curartist());
	};
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

	if (model_changed & TRACKLIST_CHANGED)
		view_tracklist_changed();

	if (model_changed & PL_NAMES_CHANGED){
		view_playlist_changed();
	};

	model_reset_changed();
};
	

/* This thread keeps record of the remaining time of a currently playing song. 
	We could issue a status command every second and get the playing time exactly as seen by mpd.
	But this generates quite some traffic over the air, which we want to avoid.
	So we update the time seen by the user here until we reach the end of the currently playing song.
	After the song has ended, we tell the controller that we need a new status from mpd to get the
	correct time.
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

/* ------------------------------ The Controller ----------------------------------------- */


/* Some action (Communication with MPD) is needed. Start this action.
	Here we construct the string which must be sent to MPD and then
	we spawn a new protothread (handle_cmd()) which sends this string to MPD
	and processes the received answer. 
*/
static
PT_THREAD (exec_action(struct pt *pt, enum USER_CMD cmd, int arg) ){
	static struct pt child_pt;
	static char cmd_str[CMDSTR_LEN+1];
	
	PT_BEGIN(pt);

	// NOTE We avoid using a switch statement because of certain restrictions for protothreads
	/* Volume changing command */
	if ( cmd == VOLUME_NEW ) {
		compose_string (cmd_str, "setvol ", arg, CMDSTR_LEN);
		volume_arg = arg;
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, cmd_str, process_volume_line, NULL, 1));
		PT_EXIT(pt);
	};

	if (cmd == NEXT_CMD){
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "next\n", process_song, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == PREV_CMD){
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "previous\n", process_song, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == SEL_SONG){
		compose_string (cmd_str, "play ", arg, CMDSTR_LEN);
		song_arg = arg;
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, cmd_str, process_play_line, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == PLAY_CMD){
		state_arg = PLAY;
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "play\n", process_state_line, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == STOP_CMD){
		state_arg = STOP;
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "stop\n", process_state_line, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == PAUSE_ON){		
		state_arg = PAUSE;
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "pause 1\n", process_state_line, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == PAUSE_OFF){		
		state_arg = PLAY;
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "pause 0\n", process_state_line, NULL, 1));
		PT_EXIT(pt);
	};
		
	/* The seek command only works if we have a current song to give to mpd (why that?) 
		TODO check not here
		We process the seek command with the status response handler, because all relevant 
		information (time, current song, state) is returned by the status command
	*/		
	if ( ( (cmd == FORWARD_CMD) || (cmd == REWIND_CMD) ) && (mpd_get_song() >= 0) ){
		model_reset(&ans_model);
		compose_string2 (cmd_str, "command_list_begin\nseek ", mpd_get_song(), arg, CMDSTR_LEN);	
		str_cat_max(cmd_str, "\nstatus\ncommand_list_end\n", CMDSTR_LEN);
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, cmd_str, process_status_line, NULL, 1));
		PT_EXIT(pt);
	};

	if (cmd == CUR_SONG_CMD){
		/* We set these values just in case that the current track has no title or artist */
		mpd_set_artist("???");
		mpd_set_title("???");
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "currentsong\n", process_currentsong_line, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == STATUS_CMD){
		model_reset(&ans_model);
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "status\n", process_status_line, NULL, 1));	
		PT_EXIT(pt);
	};	
	
	if (cmd == PLINFO_CMD){
		model_reset(&ans_model);
		compose_string (cmd_str, "playlistinfo ", arg, CMDSTR_LEN);
		req_track_no = arg;
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, cmd_str, process_playlistinfo_line, NULL, 1));	
		PT_EXIT(pt);
	};	
	
	if (cmd == LSINFO_CMD){
		start_reading_playlists();
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "lsinfo\n", process_lsinfo_line, NULL, 1));	
		PT_EXIT(pt);
	};	
	
// CLEAR command currently not used
#if 0
	if (cmd == CLEAR_CMD){
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, "clear\n", process_clear_line, NULL, 1));	
		PT_EXIT(pt);
	};	
#endif
															  
	if (cmd == LOAD_CMD){
		strn_cpy (cmd_str, "command_list_begin\nclear\nload \"", CMDSTR_LEN);
		str_cat_max (cmd_str, mpd_get_playlistname(arg), CMDSTR_LEN);
		str_cat_max (cmd_str, "\"\ncommand_list_end\n", CMDSTR_LEN);
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, cmd_str, process_load_line, NULL, 1));	
		PT_EXIT(pt);
	};	
	
	if (cmd == RANDOM_CMD){
		compose_string (cmd_str, "random ", arg, CMDSTR_LEN);
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, cmd_str, process_rnd, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == REPEAT_CMD){
		compose_string (cmd_str, "repeat ", arg, CMDSTR_LEN);
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, cmd_str, process_rpt, NULL, 1));
		PT_EXIT(pt);
	};
	
	if (cmd == SINGLE_CMD){
		compose_string (cmd_str, "single ", arg, CMDSTR_LEN);
		PT_SPAWN(pt, &child_pt, handle_cmd(&child_pt, cmd_str, process_sgl, NULL, 1));
		PT_EXIT(pt);
	};
		
	/* We do not know this command. */
	debug_out("Unknow Request ",cmd);

	PT_END(pt);
};


/* How does the controller work ? 
	We have a model of the status of mpd.
	We assume this is ok until we know otherwise.
	The status of mpd is reflected in the view.
	
	Now when the user is not satisfied with the current state, he initiates some action.
	We reflect the changes that the user wants in the user status model.
	
	Now whenever the user status and the mpd status differ, 
	the controller issues one ore more commands to mpd to get to the user desired status.
	It also sets the potentially changing items in mpd_status to unknown, because we
	do not know for sure if and how mpd reacts to the command.
	Sometimes we get the new mpd status directly with the answer, so we are done.
	Sometimes mpd does not react or does not tell us enough details in its answer.
	So:
	The controller continuously checks for unknown status items in mpd_status.
	It then sends commands to get the unknown items and updates mpd_status.
	This way we repeatedly try commands until user_status and mpd_status are compatible.
	
	A complication:
	We have a status item that is frequently changing without user intervention, namely the
	remaining playing time of the current song. It would be inefficient and consume wireless bandwidth 
	to check every second for the remaining playing time, because it is predictable.
	We use a seperate task to update the time information regularily. We also assume 
	that the user is expecting the passage of time and set the same time in the user model.
	
	Further complications:
	mpd might change its own status expectedly and unexpectedly.
	mpd changes status expectedly whenever the current song has ended.
	It might also change status unexpectedly due to an error.

	We must also reflect the user expectation when listening to a playlist, that after a song has ended
	the next song will be played. We can do that by setting song and songid in user_status to unknown, so 
	that the user is accepting any change in these items.
	
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
	static struct pt action_pt;
	static UserReq request;
	
	PT_BEGIN(pt);
		
	/* Make sure 1 and only 1 available thread can communicate with mpd */
	PT_SEM_INIT(&mpd_mutex, 1);

	/* At start of program we should have no response line from mpd */
	PT_SEM_INIT(&line_ready, 0);
	task_add(&assemble_line);
	
	
	/* Here we have our notion of time.
		When a song is playing, its playtime is increasing every second.
		This task updates the model accordingly.
	*/
	task_add(&update_playtime);	
	
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
		*/
		PT_WAIT_UNTIL(pt, ( action_needed(&request) || model_get_changed()) );

		if ( request.cmd != NO_CMD ){
			debug_out("action ", request.cmd);
			PT_SPAWN( pt, &action_pt, exec_action(&action_pt, request.cmd, request.arg) );
			PT_YIELD(pt);
		};
		
		if ( model_get_changed() ) {
			inform_view(model_get_changed());
			PT_YIELD(pt);
		};
		
		detect_errors();
		
	};
	PT_END(pt);
};


/* ------------------------------ End of Controller ----------------------------------------- */
