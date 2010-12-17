/*
    connect4.c - game

    the bulk of this file is code reused from:
        * Connect-4 Algorithm
        * Version 3.10
        * By Keith Pomakis
        * (pomakis@pobox.com)
        *April, 2005

    modifications & adoption to boop:
    Copyright (C) 2007  Ch. Klippel <ck@mamalala.org>

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

#include "lcd.h"
#include "fonty.h"
#include "keyboard.h"
#include "timerfuncs.h"
#include "backlight.h"
#include "connect4.h"

//extern volatile unsigned int keys[2];
extern unsigned int keys[2];
extern unsigned char autorepeat;

unsigned int seed;

unsigned char c4_abort;

#define other(x)        ((x) ^ 1)
#define real_player(x)  ((x) & 1)

#define pop_state() \
        (current_state = &state_stack[--depth])

/* The "goodness" of the current state with respect to a player is the */
/* score of that player minus the score of the player's opponent.  A   */
/* positive value will result if the specified player is in a better   */
/* situation than his/her opponent.                                    */

#define goodness_of(player) \
        (current_state->score[player] - current_state->score[other(player)])

/* A local struct which defines the state of a game. */

#define	C4_SIZEX 	7
#define	C4_SIZEY 	6
#define	C4_TOTALSIZE	(C4_SIZEX*C4_SIZEY)

#define	C4_MAGICWIN	(1 << 4)

#define	C4_WINPLACES	69

typedef int(*ev_fn)(int player, int level, int alpha, int beta);
typedef void(*ps_fn)(void);
typedef int(*dp_fn)(int player, int column);
typedef void(*mc_fn)(char *d, char *s, unsigned int cnt);

static ev_fn evaluate;
static ps_fn push_state;
static dp_fn drop_piece;
static mc_fn mcpy;

typedef struct {

    char board[C4_SIZEX][C4_SIZEY];           /* The board configuration of the game state.  */
                            /* board[x][y] specifies the position of the   */
                            /* xth column and the yth row of the board,    */
                            /* where column and row numbering starts at 0. */
                            /* (The 0th row is the bottom row.)            */
                            /* A value of 0 specifies that the position is */
                            /* occupied by a piece owned by player 0, a    */
                            /* value of 1 specifies that the position is   */
                            /* occupied by a piece owned by player 1, and  */
                            /* a value of C4_NONE specifies that the       */
                            /* position is unoccupied.                     */

    int score_array[2][C4_WINPLACES];  /* An array specifying statistics on both      */
                            /* players.  score_array[0] specifies the      */
                            /* statistics for player 0, while              */
                            /* score_array[1] specifies the statistics for */
                            /* player 1.                                   */

    int score[2];           /* The actual scores of each player, deducible */
                            /* from score_array, but kept separately for   */
                            /* efficiency.  The score of player x is the   */
                            /* sum of score_array[x].  A score is          */
                            /* basically a function of how many winning    */
                            /* positions are still available to the        */
                            /* and how close he/she is to achieving each   */
                            /* of these positions.                         */

    short int winner;       /* The winner of the game - either 0, 1 or     */
                            /* C4_NONE.  Deducible from score_array, but   */
                            /* kept separately for efficiency.             */

    int num_of_pieces;      /* The number of pieces currently occupying    */
                            /* board spaces.  Deducible from board, but    */
                            /* kept separately for efficiency.             */

} Game_state;

/* Static global variables. */

#include "c4_map.h"

static Game_state *state_stack; //[C4_MAX_LEVEL+1];
static Game_state *current_state;
static int depth;
static int states_allocated = 0;
//static int drop_order[7];


unsigned long int rand31(void)
{
	unsigned long int hi, lo;
	lo = 16807 * (seed & 0xFFFF);
	hi = 16807 * (seed >> 16);
	lo += (hi & 0x7FFF) << 16;
	lo += hi >> 15;
	if(lo > 0x7FFFFFFF) lo -= 0x7FFFFFFF;
	return(seed = (long)lo);
}

/****************************************************************************/
/**                                                                        **/
/**  This function sets up a new game.  This must be called exactly once   **/
/**  before each game is started.  Before it can be called a second time,  **/
/**  end_game() must be called to destroy the previous game.               **/
/**                                                                        **/
/**  width and height are the desired dimensions of the game board, while  **/
/**  num is the number of pieces required to connect in a row in order to  **/
/**  win the game.                                                         **/
/**                                                                        **/
/****************************************************************************/

void
c4_new_game(int width, int height, int num)
{
    register int i, j;

    depth = 0;
    current_state = &state_stack[0];

    for (i=0; i<C4_SIZEX; i++) 
    {
        for (j=0; j<C4_SIZEY; j++)
            current_state->board[i][j] = C4_NONE;
    }

    for (i=0; i<C4_WINPLACES; i++) {
        current_state->score_array[0][i] = 1;
        current_state->score_array[1][i] = 1;
    }

    current_state->score[0] = current_state->score[1] = C4_WINPLACES;
    current_state->winner = C4_NONE;
    current_state->num_of_pieces = 0;

    states_allocated = 1;
}

/****************************************************************************/
/**                                                                        **/
/**  This function updates the score of the specified player in the        **/
/**  context of the current state,  given that the player has just placed  **/
/**  a game piece in column x, row y.                                      **/
/**                                                                        **/
/****************************************************************************/

static void
update_score(int player, int x, int y)
{
    register int i;
    int win_index;
    int this_difference = 0, other_difference = 0;
    int other_player = other(player);

    for (i=0; map[x][y][i] != -1; i++) {
        win_index = map[x][y][i];
        this_difference += current_state->score_array[player][win_index];
        other_difference += current_state->score_array[other_player][win_index];

        current_state->score_array[player][win_index] <<= 1;
        current_state->score_array[other_player][win_index] = 0;

        if (current_state->score_array[player][win_index] == C4_MAGICWIN)
            if (current_state->winner == C4_NONE)
                current_state->winner = player;
    }

    current_state->score[player] += this_difference;
    current_state->score[other_player] -= other_difference;
}

/****************************************************************************/
/**                                                                        **/
/**  This function drops a piece of the specified player into the          **/
/**  specified column.  The row where the piece ended up is returned, or   **/
/**  -1 if the drop was unsuccessful (i.e., the specified column is full). **/
/**                                                                        **/
/****************************************************************************/

static int
_drop_piece(int player, int column)
{
    int y = 0;

    while (current_state->board[column][y] != C4_NONE && ++y < C4_SIZEY)
        ;

    if (y == C4_SIZEY)
        return -1;

    current_state->board[column][y] = player;
    current_state->num_of_pieces++;
    update_score(player, column, y);

    return y;
}

/****************************************************************************/
/**                                                                        **/
/**  This function drops a piece of the specified player into the          **/
/**  specified column.  A value of TRUE is returned if the drop is         **/
/**  successful, or FALSE otherwise.  A drop is unsuccessful if the        **/
/**  specified column number is invalid or full.  If the drop is           **/
/**  successful and row is a non-NULL pointer, the row where the piece     **/
/**  ended up is returned through the row pointer.  Note that column and   **/
/**  row numbering start at 0.                                             **/
/**                                                                        **/
/****************************************************************************/

Boolean
c4_make_move(int player, int column, int *row)
{
    int result; 

    if (column >= C4_SIZEX || column < 0)
        return FALSE;

    result = drop_piece(real_player(player), column);
    if (row && result >= 0)
        *row = result;
    return (result >= 0);
}

void __attribute__ ((section(".text.fastcode"))) 
_mcpy(char *d, char *s, unsigned int cnt)
{
	unsigned int c = cnt;
	do
	{
		d[c] = s[c];
	} while(c--);
}

/****************************************************************************/
/**                                                                        **/
/**  This function pushes the current state onto a stack.  pop_state()     **/
/**  is used to pop from this stack.                                       **/
/**                                                                        **/
/**  Technically what it does, since the current state is considered to    **/
/**  be the top of the stack, is push a copy of the current state onto     **/
/**  the stack right above it.  The stack pointer (depth) is then          **/
/**  incremented so that the new copy is considered to be the current      **/
/**  state.  That way, all pop_state() has to do is decrement the stack    **/
/**  pointer.                                                              **/
/**                                                                        **/
/**  For efficiency, memory for each stack state used is only allocated    **/
/**  once per game, and reused for the remainder of the game.              **/
/**                                                                        **/
/****************************************************************************/

static void __attribute__ ((section(".text.fastcode")))
_push_state(void)
{
    register int i;
    Game_state *old_state, *new_state;
    old_state = &state_stack[depth++];
    new_state = &state_stack[depth];

    /* Copy the board */

    for (i=0; i<C4_SIZEX; i++)
        mcpy((char*)new_state->board[i], (char*)old_state->board[i], C4_SIZEY);

    /* Copy the score array */


    mcpy((char*)new_state->score_array[0], (char*)old_state->score_array[0],
           C4_WINPLACES*4);
    mcpy((char*)new_state->score_array[1], (char*)old_state->score_array[1],
           C4_WINPLACES*4);

    new_state->score[0] = old_state->score[0];
    new_state->score[1] = old_state->score[1];
    new_state->winner = old_state->winner;
    new_state->num_of_pieces = old_state->num_of_pieces;

    current_state = new_state;
}

/****************************************************************************/
/**                                                                        **/
/**  This recursive function determines how good the current state may     **/
/**  turn out to be for the specified player.  It does this by looking     **/
/**  ahead level moves.  It is assumed that both the specified player and  **/
/**  the opponent may make the best move possible.  alpha and beta are     **/
/**  used for alpha-beta cutoff so that the game tree can be pruned to     **/
/**  avoid searching unneccessary paths.                                   **/
/**                                                                        **/
/**  The specified poll function (if any) is called at the appropriate     **/
/**  intervals.                                                            **/
/**                                                                        **/
/**  The worst goodness that the current state can produce in the number   **/
/**  of moves (levels) searched is returned.  This is the best the         **/
/**  specified player can hope to achieve with this state (since it is     **/
/**  assumed that the opponent will make the best moves possible).         **/
/**                                                                        **/
/****************************************************************************/

static int __attribute__ ((section(".text.fastcode")))
_evaluate(int player, int level, int alpha, int beta)
{
    int i, goodness, best, maxab;

    if (current_state->winner == player)
        return 0x7FFFFFFF - depth;
    else if (current_state->winner == other(player))
        return -(0x7FFFFFFF - depth);
    else if (current_state->num_of_pieces == C4_TOTALSIZE)
        return 0; /* a tie */
    else if (level == depth)
        return goodness_of(player);
    else {
        /* Assume it is the other player's turn. */
        best = -(0x7FFFFFFF);
        maxab = alpha;
        for(i=0; i<C4_SIZEX; i++) {
	    if(c4_abort)
		return 0;
            if (current_state->board[drop_order[i]][C4_SIZEY-1] != C4_NONE)
                continue; /* The column is full. */
            push_state();
            drop_piece(other(player), drop_order[i]);
            goodness = evaluate(other(player), level, -beta, -maxab);
            if (goodness > best) {
                best = goodness;
                if (best > maxab)
                    maxab = best;
            }
            pop_state();
            if (best > beta)
                break;
        }

        /* What's good for the other player is bad for this one. */
        return -best;
    }
}

/****************************************************************************/
/**                                                                        **/
/**  This function instructs the computer to make a move for the specified **/
/**  player.  level specifies the number of levels deep the computer       **/
/**  should search the game tree in order to make its decision.  This      **/
/**  corresponds to the number of "moves" in the game, where each player's **/
/**  turn is considered a move.  A value of TRUE is returned if a move was **/
/**  made, or FALSE otherwise (i.e. if the board is full).  If a move was  **/
/**  made, the column and row where the piece ended up is returned through **/
/**  the column and row pointers (unless a pointer is NULL, in which case  **/
/**  it won't be used to return any information).  Note that column and    **/
/**  row numbering start at 0.  Also note that for a standard 7x6 game of  **/
/**  Connect-4, the computer is brain-dead at levels of three or less,     **/
/**  while at levels of four or more the computer provides a challenge.    **/
/**                                                                        **/
/****************************************************************************/

Boolean
c4_auto_move(int player, int level, int *column, int *row)
{
    int i, best_column = -1, goodness = 0, best_worst = -(0x7FFFFFFF);
    int num_of_equal = 0, real_player, current_column, result;

    real_player = real_player(player);

    /* It has been proven that the best first move for a standard 7x6 game  */
    /* of connect-4 is the center column.  See Victor Allis' masters thesis */
    /* ("ftp://ftp.cs.vu.nl/pub/victor/connect4.ps") for this proof.        */

    if (current_state->num_of_pieces < 2 && (current_state->num_of_pieces == 0 ||
                         current_state->board[3][0] != C4_NONE)) {
        if (column)
            *column = 3;
        if (row)
            *row = current_state->num_of_pieces;
        drop_piece(real_player, 3);
        return TRUE;
    }

    /* Simulate a drop in each of the columns and see what the results are. */

    for (i=0; i<C4_SIZEX; i++) {
        push_state();
        current_column = drop_order[i];

        result = drop_piece(real_player, current_column);

        /* If this column is full, ignore it as a possibility. */
        if (result < 0) {
            pop_state();
            continue;
        }

        /* If this drop wins the game, take it! */
        else if (current_state->winner == real_player) {
            best_column = current_column;
            pop_state();
            break;
        }

        /* Otherwise, look ahead to see how good this move may turn out */
        /* to be (assuming the opponent makes the best moves possible). */
        else {
//            next_poll = clock() + poll_interval;
            goodness = evaluate(real_player, level, -(0x7FFFFFFF), -best_worst);
        }

        /* If this move looks better than the ones previously considered, */
        /* remember it.                                                   */
        if (goodness > best_worst) {
            best_worst = goodness;
            best_column = current_column;
            num_of_equal = 1;
        }

        /* If two moves are equally as good, make a random decision. */
        else if (goodness == best_worst) {
            num_of_equal++;
            if ((rand31()>>4) % num_of_equal == 0)
                best_column = current_column;
        }

        pop_state();
	if(c4_abort)
		return 0;
    }

    /* Drop the piece in the column decided upon. */

    if (best_column >= 0) {
        result = drop_piece(real_player, best_column);
        if (column)
            *column = best_column;
        if (row)
            *row = result;
        return TRUE;
    }
    else
        return FALSE;
}

/****************************************************************************/
/**                                                                        **/
/**  This function returns the "score" of the specified player.  This      **/
/**  score is a function of how many winning positions are still available **/
/**  to the player and how close he/she is to achieving each of these      **/
/**  positions.  The scores of both players can be compared to observe how **/
/**  well they are doing relative to each other.                           **/
/**                                                                        **/
/****************************************************************************/

int
c4_score_of_player(int player)
{
    return current_state->score[real_player(player)];
}


/****************************************************************************/
/**                                                                        **/
/**  This function returns TRUE if the specified player has won the game,  **/
/**  and FALSE otherwise.                                                  **/
/**                                                                        **/
/****************************************************************************/

Boolean
c4_is_winner(int player)
{
    return (current_state->winner == real_player(player));
}


/****************************************************************************/
/**                                                                        **/
/**  This function returns TRUE if the board is completely full without a  **/
/**  winner, and FALSE otherwise.                                          **/
/**                                                                        **/
/****************************************************************************/

Boolean
c4_is_tie(void)
{
    return (current_state->num_of_pieces == C4_TOTALSIZE &&
            current_state->winner == C4_NONE);
}


/****************************************************************************/
/**                                                                        **/
/**  This function returns the coordinates of the winning connections of   **/
/**  the winning player.  It is assumed that a player has indeed won the   **/
/**  game.  The coordinates are returned in x1, y1, x2, y2, where (x1, y1) **/
/**  specifies the lower-left piece of the winning connection, and         **/
/**  (x2, y2) specifies the upper-right piece of the winning connection.   **/
/**  If more than one winning connection exists, only one will be          **/
/**  returned.                                                             **/
/**                                                                        **/
/****************************************************************************/


void
c4_win_coords(int *x1, int *y1, int *x2, int *y2)
{
    register int i, j, k;
    int winner, win_pos = 0;
    Boolean found;

    winner = current_state->winner;

    while (current_state->score_array[winner][win_pos] != C4_MAGICWIN)
        win_pos++;

    found = FALSE;
    for (j=0; j<C4_SIZEY && !found; j++)
        for (i=0; i<C4_SIZEX && !found; i++)
            for (k=0; map[i][j][k] != -1; k++)
                if (map[i][j][k] == win_pos) {
                    *x1 = i;
                    *y1 = j;
                    found = TRUE;
                    break;
                }

    found = FALSE;
    for (j=C4_SIZEY-1; j>=0 && !found; j--)
        for (i=C4_SIZEX-1; i>=0 && !found; i--)
            for (k=0; map[i][j][k] != -1; k++)
                if (map[i][j][k] == win_pos) {
                    *x2 = i;
                    *y2 = j;
                    found = TRUE;
                    break;
                }
}


/****************************************************************************/
/**                                                                        **/
/**  This function ends the current game.  It is assumed that a game is    **/
/**  in progress.  It is illegal to call any other game function           **/
/**  immediately after this one except for c4_new_game(), c4_poll() and    **/
/**  c4_reset().                                                           **/
/**                                                                        **/
/****************************************************************************/

void
c4_end_game(void)
{
    states_allocated = 0;
}


/****************************************************************************/
/**                                                                        **/
/**  This function resets the state of the algorithm to the starting state **/
/**  (i.e., no game in progress and a NULL poll function).  There should   **/
/**  no reason to call this function unless for some reason the calling    **/
/**  algorithm loses track of the game state.  It is illegal to call any   **/
/**  other game function immediately after this one except for             **/
/**  c4_new_game(), c4_poll() and c4_reset().                              **/
/**                                                                        **/
/****************************************************************************/

void
c4_reset(void)
{
        c4_end_game();
}

int row, col, ccol;
unsigned int c4_cb, c4_wcb;
unsigned char c4_player, c4_moves, c4_level;

void draw_stone(unsigned char x, unsigned char y, unsigned char p)
{

	unsigned char xx, yy;
	xx = 10+(x*18);
	yy = 46+(y*18);

	switch(p)
	{
		case 0:
			draw_disc(xx, yy, 7, 2, DRAW_PUT);
			draw_disc(xx, yy, 5, 3, DRAW_XOR);
			draw_disc(xx, yy, 2, 3, DRAW_XOR);
		break;

		case 1:
			draw_disc(xx, yy, 7, 3, DRAW_PUT);
			draw_disc(xx, yy, 5, 1, DRAW_ERASE);
			draw_disc(xx, yy, 2, 1, DRAW_PUT);
		break;

		case 2:
			draw_disc(xx, yy, 8, 3, DRAW_ERASE);
		break;

		case 3:
			draw_disc(x, y, 7, 2, DRAW_PUT);
			draw_disc(x, y, 5, 3, DRAW_XOR);
			draw_disc(x, y, 2, 3, DRAW_XOR);
		break;

		case 4:
			draw_disc(x, y, 7, 3, DRAW_PUT);
			draw_disc(x, y, 5, 1, DRAW_ERASE);
			draw_disc(x, y, 2, 1, DRAW_PUT);
		break;

		case 5:
			draw_disc(xx, yy, 7, 3, DRAW_XOR);
		break;
	}
}

void progress_callback(void)
{
	static unsigned char step = 0;
	unsigned char x;
	if(keys[0] || keys[1])
	{
		setBacklight(BL_AUTO);
		if(KEY_Exit)
			c4_abort = 1;
	}

	for(x=0;x<6;x++)
	{
		draw_char(40+(x<<3),24,224+step,3,DRAW_NORCU);
	}
	draw_hline(40,24,48,3,DRAW_PUT);
	step--;
	step &= 0x07;
}

void win_callback(void)
{
	int x1, y1, x2, y2;

	c4_win_coords(&x1, &y1, &x2, &y2);

	if(x1 > x2)
		x1 = 1;
	else if(x1 < x2)
		x1 = -1;
	else
		x1 = 0;

	if(y1 > y2)
		y1 = 1;
	else if(y1 < y2)
		y1 = -1;
	else
		y1 = 0;

	draw_stone(x2,5-y2,5);
	x2+=x1;
	y2+=y1;
	draw_stone(x2,5-y2,5);
	x2+=x1;
	y2+=y1;
	draw_stone(x2,5-y2,5);
	x2+=x1;
	y2+=y1;
	draw_stone(x2,5-y2,5);
	x2+=x1;
	y2+=y1;
}

void draw_marker(void)
{
	draw_stone(10+(col*18), 22,c4_player+3);
	draw_arrow(7+(col*18), 31, ARROW_DOWN, ARROW_LEN, 3, DRAW_PUT);
	draw_arrow(    0, 152, ARROW_LEFT, ARROW_LEN, 3, DRAW_PUT);
	draw_arrow(   6, 152, ARROW_RIGHT, ARROW_LEN, 3, DRAW_PUT);
	draw_string(12,152,"Select",3,DRAW_PUT);
	draw_arrow(   75, 153, ARROW_DOWN, ARROW_LEN, 3, DRAW_PUT);
	draw_string(84,152,"Play move",3,DRAW_PUT);
}

void delete_marker(void)
{
	draw_disc(10+(col*18), 22, 7, 3, DRAW_ERASE);
	draw_arrow(7+(col*18), 31, ARROW_DOWN, ARROW_LEN, 3, DRAW_ERASE);
	draw_block(0, 152, 128, 8, 3, DRAW_ERASE);
}

int checkwin(void)
{
	if(c4_is_winner(0) || c4_is_winner(1) || c4_is_tie())
	{
		if(!c4_is_tie())
		{
			startCB(c4_wcb);
		}
		draw_block(0, 152, 128, 8, 3, DRAW_ERASE);
		delete_marker();
		set_font(BOLDFONT);
		if(c4_is_winner(c4_player))
			draw_string(19,14,"Spieler gewinnt!",3,DRAW_PUT);
		else if(c4_is_tie())
			draw_string(24,14,"Unentschieden!",3,DRAW_PUT);
		else
			draw_string(13,14,"Computer gewinnt!",3,DRAW_PUT);

		draw_string(2,25,"OK: Nochmal  Exit: Ende",3,DRAW_PUT);
		return 1;
	}
	return 0;
}

void move(unsigned char p)
{
	unsigned char pp;
	if(p)
	{
		p = 1;
		pp = 0;
	}
	else
		pp = 1;

	if(c4_make_move(p, col, &row))
	{
		draw_stone(col, 5-row, p);
		draw_disc(10+(col*18), 23, 7, 3, DRAW_ERASE);
		delete_marker();
		if(!checkwin())
		{
			set_font(BOLDFONT);
			draw_frame(35,13,57,22,5,5,3,DRAW_PUT);
			draw_block(40,18,47,12,3,DRAW_PUT);
			draw_string(40, 15, "Berechne", 3, DRAW_ERASE);
			draw_string(0, 152, "Exit: Spiel abbrechen", 3, DRAW_PUT);
			set_font(SMALLFONT);
			progress_callback();
			startCB(c4_cb);
			c4_abort = 0;
			c4_auto_move(~p, c4_level, &ccol, &row);
			setBacklight(BL_AUTO);
			stopCB(c4_cb);
			draw_block(0,13,128,22,3,DRAW_ERASE);
			draw_block(0, 152, 128, 8, 3, DRAW_ERASE);
			draw_stone(ccol, 5-row, pp);
		}
		draw_marker();
		c4_moves++;
		checkwin();
	}
}

void c4_selplayer(unsigned char p)
{
	draw_string(30, 15, "Chooose Your", 3, DRAW_PUT);
	draw_string(25, 26, "Colour & Level", 3, DRAW_PUT);
	draw_arrow(    0, 154, ARROW_UP, ARROW_LEN, 3, DRAW_PUT);
	draw_arrow(1 * 6, 154, ARROW_DOWN, ARROW_LEN, 3, DRAW_PUT);
	draw_string(15,152,"Level",3,DRAW_PUT);

	draw_arrow(   82, 153, ARROW_LEFT, ARROW_LEN, 3, DRAW_PUT);
	draw_arrow(   88, 153, ARROW_RIGHT, ARROW_LEN, 3, DRAW_PUT);
	draw_string(94,152,"Colour",3,DRAW_PUT);

	draw_frame(1,  15, 19, 19, 1, 3, 3, DRAW_PUT);
	draw_frame(108,15, 19, 19, 1, 3, 3, DRAW_PUT);

	draw_disc(10, 24, 7, 3, DRAW_ERASE);
	draw_stone(10, 24, p+3);
	draw_disc(117, 24, 7, 3, DRAW_ERASE);
	draw_stone(117,24, p+3);
}

void start_connect4(void)
{
	unsigned char x,y;

	Game_state s_stk[C4_MAX_LEVEL+1];
	state_stack = s_stk;
	
	evaluate = _evaluate;
	push_state = _push_state;
	drop_piece = _drop_piece;
	mcpy = _mcpy;

	seed = 0x86f2a981;

	c4_cb = addTimerCB(progress_callback, 35);
	c4_wcb = addTimerCB(win_callback, 100);

	row = 0; ccol = 0;
	c4_player = 0;
	c4_level = 3;
	lcd_fill(0);//Clr screen

	set_font(BOLDFONT);
	draw_string(0, 0, "Connect4", 3, DRAW_PUT);

	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);

	draw_frame(1,37,127,109,9,9,3,DRAW_PUT);
	draw_block(10,46,109,91,3,DRAW_PUT);

restart:
	stopCB(c4_wcb);
	col = 3;
	draw_block(0,14,128,20,3,DRAW_ERASE);
	draw_block(0, 152, 128, 8, 3, DRAW_ERASE);
	c4_selplayer(c4_player);
	draw_numU(48,152,c4_level,0,3,DRAW_PUT);

	c4_moves = 0;

	for(x=0;x<7;x++)
	{
		for(y=0;y<6;y++)
		{
			draw_disc(10+(x*18), 46+(y*18),8,3,DRAW_ERASE);
		}
	}

	c4_reset();
	c4_new_game(C4_SIZEX, C4_SIZEY, 4);

	while(1)
	{
wait4c:
		waitKeyUpDown();

		if((KEY_OK || KEY_0) && (c4_is_winner(0) || c4_is_winner(1) || c4_is_tie()) && (c4_moves != 0)) goto restart;
		else if(KEY_0)
			goto restart;
		if(KEY_Exit)
		{
			stopCB(c4_cb);
			stopCB(c4_wcb);
			removeTimerCB(c4_cb);
			removeTimerCB(c4_wcb);
			return;
		}
		else if(c4_moves == 0)
		{
			if(KEY_Left || KEY_Right)
			{
				if(c4_player) c4_player=0; else c4_player = 1;
				c4_selplayer(c4_player);
			}
			else if(KEY_OK && c4_player == 1)
			{
				delete_marker();
				c4_auto_move(0, c4_level, &ccol, &row);
				draw_stone(ccol, 5-row, 0);
				draw_marker();
			}
			else if(KEY_Up)
			{
				draw_numU(48,152,c4_level,0,3,DRAW_ERASE);
				c4_level++;
				if(c4_level > C4_MAX_LEVEL)
					c4_level = 1;
				draw_numU(48,152,c4_level,0,3,DRAW_PUT);
			}
			else if(KEY_Down)
			{
				draw_numU(48,152,c4_level,0,3,DRAW_ERASE);
				c4_level--;
				if(c4_level < 1)
					c4_level = C4_MAX_LEVEL;
				draw_numU(48,152,c4_level,0,3,DRAW_PUT);
			}
			if(KEY_OK)
			{
				c4_moves++;
				draw_block(0, 152, 128, 8, 3, DRAW_ERASE);
				delete_marker();
				draw_block(0,13,128,22,3,DRAW_ERASE);
				set_font(SMALLFONT);
				draw_marker();
			}
			goto wait4c;
		}
		else if(!c4_is_winner(0) && !c4_is_winner(1) && !c4_is_tie())
		{
			if(KEY_1 || KEY_2 || KEY_3 || KEY_4 || KEY_5 || KEY_6 || KEY_7)
			{
				delete_marker();
				if(KEY_1)
					col = 0;
				else if(KEY_2)
					col = 1;
				else if(KEY_3)
					col = 2;
				else if(KEY_4)
					col = 3;
				else if(KEY_5)
					col = 4;
				else if(KEY_6)
					col = 5;
				else if(KEY_7)
					col = 6;
				draw_marker();

				move(c4_player);
			}
			else if(KEY_Left || KEY_Right)
			{
				delete_marker();
				if(KEY_Left)
					col--;
				else if(KEY_Right)
					col++;	

				if(col > 6)
					col = 0;
				else if(col < 0)
					col = 6;
				draw_marker();
			}
			else if(KEY_Down || KEY_OK)
			{
				move(c4_player);
				if(c4_abort)
				{
					c4_abort = 0;
					goto restart;
				}
			}
		}
	}
}

