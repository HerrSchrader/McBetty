/*
    sokoban.h - game
    Copyright (C) 2007  Colibri <colibri_dvb@lycos.com>

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

#ifndef SOKOBAN_H
#define SOKOBAN_H

#include "../global.h"

#define SOKOBAN_MAX_UNDOS	100
#define SOKOBAN_UNDO_DIR_LEFT	0
#define SOKOBAN_UNDO_DIR_RIGHT	1
#define SOKOBAN_UNDO_DIR_UP		2
#define SOKOBAN_UNDO_DIR_DOWN	3

#define SPACE	0
#define GOAL	1
#define PUSHER	2
#define PACKET	4
#define WALL	8
#define PUSHER_ON_GOAL	(PUSHER | GOAL)
#define PACKET_ON_GOAL	(PACKET | GOAL)

#define SOKOBAN_CELLS_X	21
#define SOKOBAN_CELLS_Y	19
//Size without grid
#define SOKOBAN_CELL_SIZE	5

struct _SokobanGameInfo
{
	BYTE G[SOKOBAN_CELLS_X][SOKOBAN_CELLS_Y];
	short Level;
	int PusherX;
	int PusherY;
	WORD Moves;
	WORD UndoCount;
	BYTE UndoBuffer[SOKOBAN_MAX_UNDOS][3];
};

void GameSokoban(void);
void GameSokoban_InitGameField(struct _SokobanGameInfo *pGI);
void GameSokoban_DrawCell(int x, int y, BYTE Object);
BOOL GameSokoban_IsSolved(BYTE G[SOKOBAN_CELLS_X][SOKOBAN_CELLS_Y]);
void GameSokoban_InitDisplayForNewGame(struct _SokobanGameInfo *pGI);
void GameSokoban_Move(struct _SokobanGameInfo *pGI);
void GameSokoban_SaveUndo(struct _SokobanGameInfo *pGI, BYTE Count, BYTE Dir);
void GameSokoban_UndoAMove(struct _SokobanGameInfo *pGI);


#endif
