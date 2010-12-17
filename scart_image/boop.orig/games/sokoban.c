/*
    sokoban.c - game
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

#include "lcd.h"
#include "fonty.h"
#include "keyboard.h"

#include "sokoban.h"
#include "sokoban_levels.h"

//#include "stdio.h"

//extern volatile unsigned int keys[2];
extern unsigned int keys[2];

void GameSokoban_InitGameField(struct _SokobanGameInfo *pGI)
{
	int SizeX;
	int SizeY;
	int x;
	int y;
	int Pos;
	int i;

	if((pGI->Level < 1) || (pGI->Level > SOKOBAN_LEVELS))
	{
		//Invalid level
		pGI->Level = 1;
	}

	//Empty the game field
	for(x=0; x<SOKOBAN_CELLS_X; x++)
	{
		for(y=0; y<SOKOBAN_CELLS_Y; y++)
		{
			pGI->G[x][y] = SPACE;
		}
	}

	//Find the position to searched level
	Pos = 0;
	i = 1;
	while(i<pGI->Level)
	{
		SizeX = Sokoban_Levels[Pos++];
		SizeY = Sokoban_Levels[Pos++];
		Pos += SizeY * ((SizeX+1)/2);
		i++;
	}

	//Copy the (may be smaller) level to the center of the game field
	SizeX = Sokoban_Levels[Pos++];
	SizeY = Sokoban_Levels[Pos++];
	for(y=0; y<SizeY; y++)
	{
		for(x=0; x<SizeX; x++)
		{
			if(x & 1)
			{
				//Odd x -> lo nibble
				pGI->G[x+((SOKOBAN_CELLS_X-SizeX)/2)][y+((SOKOBAN_CELLS_Y-SizeY)/2)] = Sokoban_Levels[Pos + (y * ((SizeX+1)/2)) + (x/2)] & 0x0F;
			}
			else
			{
				//Even x -> hi nibble
				pGI->G[x+((SOKOBAN_CELLS_X-SizeX)/2)][y+((SOKOBAN_CELLS_Y-SizeY)/2)] = (Sokoban_Levels[Pos + (y * ((SizeX+1)/2)) + (x/2)] >> 4) & 0x0F;
			}
		}
	}
}


BOOL GameSokoban_IsSolved(BYTE G[SOKOBAN_CELLS_X][SOKOBAN_CELLS_Y])
{
	//Return true=Solved
	int x;
	int y;

	for(x=0; x<SOKOBAN_CELLS_X; x++)
	{
		for(y=0; y<SOKOBAN_CELLS_Y; y++)
		{
			if(G[x][y] & GOAL)
			{
				//Goal found
				if(!(G[x][y] & PACKET))
				{
					//But no packet
					return false;//Not solved
				}
			}
		}
	}

	return true;//Solved
}

void GameSokoban_DrawCell(int x, int y, BYTE Object)
{
	BYTE Color;
	BYTE Mode;
	int dx;
	int dy;

	Color = LCD_COLOR_B;
	Mode = DRAW_PUT;

/*
	ASSERT(x < SOKOBAN_CELLS_X);
	ASSERT(y < SOKOBAN_CELLS_Y);
*/
	dx = 1 + x * (SOKOBAN_CELL_SIZE+1);
	dy = 16 + y * (SOKOBAN_CELL_SIZE+1) + 1;

	//Clear first
	draw_block(dx, dy, SOKOBAN_CELL_SIZE, SOKOBAN_CELL_SIZE, LCD_COLOR_B, DRAW_ERASE);

	if(Object & GOAL)
	{
		draw_pixel(dx+1, dy+1, Color, Mode);
		draw_pixel(dx+2, dy+2, Color, Mode);
		draw_pixel(dx+3, dy+3, Color, Mode);
		draw_pixel(dx+3, dy+1, Color, Mode);
		draw_pixel(dx+1, dy+3, Color, Mode);
	}

	if(Object & PUSHER)
	{
		draw_hline(dx, dy+1, SOKOBAN_CELL_SIZE, Color, Mode);
		draw_hline(dx, dy+3, SOKOBAN_CELL_SIZE, Color, Mode);
		draw_vline(dx+1, dy, SOKOBAN_CELL_SIZE, Color, Mode);
		draw_vline(dx+3, dy, SOKOBAN_CELL_SIZE, Color, Mode);
	}

	if(Object & PACKET)
	{
		draw_hline(dx+1, dy, 3, Color, Mode);
		draw_hline(dx+1, dy+4, 3, Color, Mode);
		draw_vline(dx, dy+1, 3, Color, Mode);
		draw_vline(dx+4, dy+1, 3, Color, Mode);
	}

	if(Object & WALL)
	{
		draw_block(dx, dy, SOKOBAN_CELL_SIZE, SOKOBAN_CELL_SIZE, Color, Mode);
	}
}

void GameSokoban_InitDisplayForNewGame(struct _SokobanGameInfo *pGI)
{
	int x;
	int y;

	pGI->Moves = 0;
	pGI->UndoCount = 0;

	GameSokoban_InitGameField(pGI);

	//Game field to display
	for(x=0; x<SOKOBAN_CELLS_X; x++)
	{
		for(y=0; y<SOKOBAN_CELLS_Y; y++)
		{
			GameSokoban_DrawCell(x, y, pGI->G[x][y]);
			if(pGI->G[x][y] & PUSHER)
			{
				pGI->PusherX = x;
				pGI->PusherY = y;
			}
		}
	}
}

void GameSokoban_Move(struct _SokobanGameInfo *pGI)
{
	if(KEY_Up)
	{
		if(!(pGI->G[pGI->PusherX][pGI->PusherY - 1] & WALL))
		{
			//No wall
			if(!(pGI->G[pGI->PusherX][pGI->PusherY - 1] & PACKET))
			{
				//No wall and no packet
				GameSokoban_SaveUndo(pGI, 2, SOKOBAN_UNDO_DIR_UP);

				//Move pusher
				pGI->G[pGI->PusherX][pGI->PusherY] ^= PUSHER;
				pGI->G[pGI->PusherX][pGI->PusherY-1] ^= PUSHER;

				//Draw changed cells
				GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY, pGI->G[pGI->PusherX][pGI->PusherY]);
				GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY-1, pGI->G[pGI->PusherX][pGI->PusherY-1]);

				pGI->PusherY--;
				pGI->Moves++;
			}
			else
			{
				//Packet -> check if it is space behind the packet
				if(!(pGI->G[pGI->PusherX][pGI->PusherY - 2] & (PACKET | WALL)))
				{
					//No packet or wall behind the packet
					GameSokoban_SaveUndo(pGI, 3, SOKOBAN_UNDO_DIR_UP);
					//Move pusher
					pGI->G[pGI->PusherX][pGI->PusherY] ^= PUSHER;
					pGI->G[pGI->PusherX][pGI->PusherY - 1] ^= PUSHER;

					//Move packet
					pGI->G[pGI->PusherX][pGI->PusherY-1] ^= PACKET;
					pGI->G[pGI->PusherX][pGI->PusherY-2] ^= PACKET;

					//Draw changed cells
					GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY, pGI->G[pGI->PusherX][pGI->PusherY]);
					GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY-1, pGI->G[pGI->PusherX][pGI->PusherY-1]);
					GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY-2, pGI->G[pGI->PusherX][pGI->PusherY-2]);

					pGI->PusherY--;
					pGI->Moves++;
				}
			}
		}
	}
	if(KEY_Down)
	{
		if(!(pGI->G[pGI->PusherX][pGI->PusherY + 1] & WALL))
		{
			//No wall
			if(!(pGI->G[pGI->PusherX][pGI->PusherY + 1] & PACKET))
			{
				//No wall and no packet
				GameSokoban_SaveUndo(pGI, 2, SOKOBAN_UNDO_DIR_DOWN);
				//Move pusher
				pGI->G[pGI->PusherX][pGI->PusherY] ^= PUSHER;
				pGI->G[pGI->PusherX][pGI->PusherY+1] ^= PUSHER;

				//Draw changed cells
				GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY, pGI->G[pGI->PusherX][pGI->PusherY]);
				GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY+1, pGI->G[pGI->PusherX][pGI->PusherY+1]);

				pGI->PusherY++;
				pGI->Moves++;
			}
			else
			{
				//Packet -> check if it is space behind the packet
				if(!(pGI->G[pGI->PusherX][pGI->PusherY + 2] & (PACKET | WALL)))
				{
					//No packet or wall behind the packet
					GameSokoban_SaveUndo(pGI, 3, SOKOBAN_UNDO_DIR_DOWN);
					//Move pusher
					pGI->G[pGI->PusherX][pGI->PusherY] ^= PUSHER;
					pGI->G[pGI->PusherX][pGI->PusherY + 1] ^= PUSHER;

					//Move packet
					pGI->G[pGI->PusherX][pGI->PusherY+1] ^= PACKET;
					pGI->G[pGI->PusherX][pGI->PusherY+2] ^= PACKET;

					//Draw changed cells
					GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY, pGI->G[pGI->PusherX][pGI->PusherY]);
					GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY+1, pGI->G[pGI->PusherX][pGI->PusherY+1]);
					GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY+2, pGI->G[pGI->PusherX][pGI->PusherY+2]);

					pGI->PusherY++;
					pGI->Moves++;
				}
			}
		}
	}
	if(KEY_Left)
	{
		if(!(pGI->G[pGI->PusherX - 1][pGI->PusherY] & WALL))
		{
			//No wall
			if(!(pGI->G[pGI->PusherX - 1][pGI->PusherY] & PACKET))
			{
				//No wall and no packet
				GameSokoban_SaveUndo(pGI, 2, SOKOBAN_UNDO_DIR_LEFT);
				//Move pusher
				pGI->G[pGI->PusherX][pGI->PusherY] ^= PUSHER;
				pGI->G[pGI->PusherX - 1][pGI->PusherY] ^= PUSHER;

				//Draw changed cells
				GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY, pGI->G[pGI->PusherX][pGI->PusherY]);
				GameSokoban_DrawCell(pGI->PusherX - 1, pGI->PusherY, pGI->G[pGI->PusherX - 1][pGI->PusherY]);

				pGI->PusherX--;
				pGI->Moves++;
			}
			else
			{
				//Packet -> check if it is space behind the packet
				if(!(pGI->G[pGI->PusherX - 2][pGI->PusherY] & (PACKET | WALL)))
				{
					//No packet or wall behind the packet
					GameSokoban_SaveUndo(pGI, 3, SOKOBAN_UNDO_DIR_LEFT);
					//Move pusher
					pGI->G[pGI->PusherX][pGI->PusherY] ^= PUSHER;
					pGI->G[pGI->PusherX - 1][pGI->PusherY] ^= PUSHER;

					//Move packet
					pGI->G[pGI->PusherX - 1][pGI->PusherY] ^= PACKET;
					pGI->G[pGI->PusherX - 2][pGI->PusherY] ^= PACKET;

					//Draw changed cells
					GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY, pGI->G[pGI->PusherX][pGI->PusherY]);
					GameSokoban_DrawCell(pGI->PusherX - 1, pGI->PusherY, pGI->G[pGI->PusherX - 1][pGI->PusherY]);
					GameSokoban_DrawCell(pGI->PusherX - 2, pGI->PusherY, pGI->G[pGI->PusherX - 2][pGI->PusherY]);

					pGI->PusherX--;
					pGI->Moves++;
				}
			}
		}
	}
	if(KEY_Right)
	{
		if(!(pGI->G[pGI->PusherX + 1][pGI->PusherY] & WALL))
		{
			//No wall
			if(!(pGI->G[pGI->PusherX + 1][pGI->PusherY] & PACKET))
			{
				//No wall and no packet
				GameSokoban_SaveUndo(pGI, 2, SOKOBAN_UNDO_DIR_RIGHT);
				//Move pusher
				pGI->G[pGI->PusherX][pGI->PusherY] ^= PUSHER;
				pGI->G[pGI->PusherX + 1][pGI->PusherY] ^= PUSHER;

				//Draw changed cells
				GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY, pGI->G[pGI->PusherX][pGI->PusherY]);
				GameSokoban_DrawCell(pGI->PusherX + 1, pGI->PusherY, pGI->G[pGI->PusherX + 1][pGI->PusherY]);

				pGI->PusherX++;
				pGI->Moves++;
			}
			else
			{
				//Packet -> check if it is space behind the packet
				if(!(pGI->G[pGI->PusherX + 2][pGI->PusherY] & (PACKET | WALL)))
				{
					//No packet or wall behind the packet
					GameSokoban_SaveUndo(pGI, 3, SOKOBAN_UNDO_DIR_RIGHT);
					//Move pusher
					pGI->G[pGI->PusherX][pGI->PusherY] ^= PUSHER;
					pGI->G[pGI->PusherX + 1][pGI->PusherY] ^= PUSHER;

					//Move packet
					pGI->G[pGI->PusherX + 1][pGI->PusherY] ^= PACKET;
					pGI->G[pGI->PusherX + 2][pGI->PusherY] ^= PACKET;

					//Draw changed cells
					GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY, pGI->G[pGI->PusherX][pGI->PusherY]);
					GameSokoban_DrawCell(pGI->PusherX + 1, pGI->PusherY, pGI->G[pGI->PusherX + 1][pGI->PusherY]);
					GameSokoban_DrawCell(pGI->PusherX + 2, pGI->PusherY, pGI->G[pGI->PusherX + 2][pGI->PusherY]);

					pGI->PusherX++;
					pGI->Moves++;
				}
			}
		}
	}

}

void GameSokoban_UndoAMove(struct _SokobanGameInfo *pGI)
{
	//3 Byte UndoBuffer:
	//[  0   ][  1   ][  2   ]
	//765432107654321076543210
	//\-x-/\-y-/\/\o0/\o1/\o2/

	BYTE Object[3];
	BYTE Dir;
	int i;
	int Count;

	if(pGI->UndoCount <= 0)
	{
		return;//No more undos available
	}

	pGI->UndoCount--;

	pGI->PusherX = (pGI->UndoBuffer[pGI->UndoCount][0] >> 3) & 0x1F;
	pGI->PusherY = (pGI->UndoBuffer[pGI->UndoCount][0] << 2) & 0x1F;
	pGI->PusherY |= (pGI->UndoBuffer[pGI->UndoCount][1] >> 6) & 0x03;
	Dir = (pGI->UndoBuffer[pGI->UndoCount][1] >> 4) & 0x03;
	Object[0] = pGI->UndoBuffer[pGI->UndoCount][1] & 0x0F;
	Object[1] = (pGI->UndoBuffer[pGI->UndoCount][2] >> 4) & 0x0F;
	Object[2] = pGI->UndoBuffer[pGI->UndoCount][2] & 0x0F;

	Count = (Object[2] == 0x0F) ? 2 : 3;

	for(i=0; i<Count; i++)
	{
		switch(Dir)
		{
		case SOKOBAN_UNDO_DIR_LEFT:
			pGI->G[pGI->PusherX - i][pGI->PusherY] = Object[i];
			GameSokoban_DrawCell(pGI->PusherX - i, pGI->PusherY, pGI->G[pGI->PusherX - i][pGI->PusherY]);
			break;
		case SOKOBAN_UNDO_DIR_RIGHT:
			pGI->G[pGI->PusherX + i][pGI->PusherY] = Object[i];
			GameSokoban_DrawCell(pGI->PusherX + i, pGI->PusherY, pGI->G[pGI->PusherX + i][pGI->PusherY]);
			break;
		case SOKOBAN_UNDO_DIR_UP:
			pGI->G[pGI->PusherX][pGI->PusherY - i] = Object[i];
			GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY - i, pGI->G[pGI->PusherX][pGI->PusherY - i]);
			break;
		case SOKOBAN_UNDO_DIR_DOWN:
			pGI->G[pGI->PusherX][pGI->PusherY + i] = Object[i];
			GameSokoban_DrawCell(pGI->PusherX, pGI->PusherY + i, pGI->G[pGI->PusherX][pGI->PusherY + i]);
			break;
		}
	}

	pGI->Moves--;
}

void GameSokoban_SaveUndo(struct _SokobanGameInfo *pGI, BYTE Count, BYTE Dir)
{
	//3 Byte UndoBuffer:
	//[  0   ][  1   ][  2   ]
	//765432107654321076543210
	//\-x-/\-y-/\/\o0/\o1/\o2/

	BYTE Object[3];
	int i;

//	ASSERT((Count == 2) || (Count == 3));

	Object[2] = 0x0F;//Default value for not used

	for(i=0; i<Count; i++)
	{
		switch(Dir)
		{
		case SOKOBAN_UNDO_DIR_LEFT:
			Object[i] = pGI->G[pGI->PusherX - i][pGI->PusherY];
			break;
		case SOKOBAN_UNDO_DIR_RIGHT:
			Object[i] = pGI->G[pGI->PusherX + i][pGI->PusherY];
			break;
		case SOKOBAN_UNDO_DIR_UP:
			Object[i] = pGI->G[pGI->PusherX][pGI->PusherY - i];
			break;
		case SOKOBAN_UNDO_DIR_DOWN:
			Object[i] = pGI->G[pGI->PusherX][pGI->PusherY + i];
			break;
		}
	}

	if(pGI->UndoCount >= SOKOBAN_MAX_UNDOS)
	{
		//Undo buffer is full -> free oldest
		for(i=0; i<(SOKOBAN_MAX_UNDOS-1); i++)
		{
			pGI->UndoBuffer[i][0] = pGI->UndoBuffer[i+1][0];
			pGI->UndoBuffer[i][1] = pGI->UndoBuffer[i+1][1];
			pGI->UndoBuffer[i][2] = pGI->UndoBuffer[i+1][2];
		}
		pGI->UndoCount--;
	}

	pGI->UndoBuffer[pGI->UndoCount][0]  = (pGI->PusherX & 0x1F) << 3;
	pGI->UndoBuffer[pGI->UndoCount][0] |= ((pGI->PusherY & 0x1F) >> 2) & 0x07;

	pGI->UndoBuffer[pGI->UndoCount][1]  = (pGI->PusherY & 0x1F) << 6;
	pGI->UndoBuffer[pGI->UndoCount][1] |= (Dir & 3) << 4;
	pGI->UndoBuffer[pGI->UndoCount][1] |= Object[0] & 0x0F;

	pGI->UndoBuffer[pGI->UndoCount][2]  = (Object[1] & 0x0F) << 4;
	pGI->UndoBuffer[pGI->UndoCount][2] |= Object[2] & 0x0F;

	pGI->UndoCount++;
}

void GameSokoban()
{
	struct _SokobanGameInfo GI;
	int i;

	GI.Level = 1;
restart:
	//Init the display
	lcd_fill(0);//Clr screen

	//Title
	draw_string(0, 0, "Sokoban", LCD_COLOR_B, DRAW_PUT);

	//Grid
	for(i=0; i<=SOKOBAN_CELLS_X; i++)
	{
		draw_vline(i*(SOKOBAN_CELL_SIZE+1),16,SOKOBAN_CELLS_Y*(SOKOBAN_CELL_SIZE+1)+1, LCD_COLOR_LG, DRAW_PUT);
	}
	for(i=0; i<=SOKOBAN_CELLS_Y; i++)
	{
		draw_hline(0, 16 + i*(SOKOBAN_CELL_SIZE+1),SOKOBAN_CELLS_X*(SOKOBAN_CELL_SIZE+1)+1, LCD_COLOR_LG, DRAW_PUT);
	}

	draw_string(0, 8, "Level:            Moves:", LCD_COLOR_B, DRAW_PUT);
	draw_char(98,8,'0',LCD_COLOR_B,DRAW_PUT);
	draw_string(0, LCD_SIZE_Y - (8*3), "Program +/-:", LCD_COLOR_B, DRAW_PUT);
	draw_string(65, LCD_SIZE_Y - (8*3), "Level +/- 1", LCD_COLOR_B, DRAW_PUT);
	draw_string(0, LCD_SIZE_Y - (8*2), "Volume +/-:", LCD_COLOR_B, DRAW_PUT);
	draw_string(65, LCD_SIZE_Y - (8*2), "Level +/- 20", LCD_COLOR_B, DRAW_PUT);
	draw_arrow(    0, LCD_SIZE_Y - 8, ARROW_UP, ARROW_LEN, LCD_COLOR_B, DRAW_PUT);
	draw_arrow(1 * 6, LCD_SIZE_Y - 8, ARROW_DOWN, ARROW_LEN, LCD_COLOR_B, DRAW_PUT);
	draw_arrow(2 * 6, LCD_SIZE_Y - 8, ARROW_LEFT, ARROW_LEN, LCD_COLOR_B, DRAW_PUT);
	draw_arrow(3 * 6, LCD_SIZE_Y - 8, ARROW_RIGHT, ARROW_LEN, LCD_COLOR_B, DRAW_PUT);
	draw_string(32, LCD_SIZE_Y - 8, "Bewegen", LCD_COLOR_B, DRAW_PUT);

again:
	GameSokoban_InitDisplayForNewGame(&GI);
	draw_block(32, 8, 30, 8, LCD_COLOR_B, DRAW_ERASE);
	draw_numU(32,8,GI.Level,0,LCD_COLOR_B,DRAW_PUT);

	while(1)
	{
		if(KEY_Up || KEY_Down || KEY_Left || KEY_Right || KEY_8)
		{
			draw_block(98, 8, 30, 8, LCD_COLOR_B, DRAW_ERASE);
			if(KEY_8)
				GameSokoban_UndoAMove(&GI);
			else
				GameSokoban_Move(&GI);
	
			if(GameSokoban_IsSolved(GI.G) && !KEY_8)
			{
				//Solved
				GI.Level++;//Next level
				if(GI.Level > SOKOBAN_LEVELS)
				{
					GI.Level = 1;
				}
				waitKeyUp();
				goto restart;
			}
			draw_numU(98,8,GI.Moves,0,LCD_COLOR_B,DRAW_PUT);
		}

	
		//Footer
		if(GI.Moves == 1)
		{
			draw_block(0, LCD_SIZE_Y - (8*3), LCD_SIZE_X, 16, LCD_COLOR_B, DRAW_ERASE);
			draw_string(0, LCD_SIZE_Y - (8*3), "0", LCD_COLOR_B, DRAW_PUT);
			draw_string(32, LCD_SIZE_Y - (8*3), "Neues Spiel", LCD_COLOR_B, DRAW_PUT);
			draw_string(0, LCD_SIZE_Y - (8*2), "Exit", LCD_COLOR_B, DRAW_PUT);
			draw_string(32, LCD_SIZE_Y - (8*2), "Spiel beenden", LCD_COLOR_B, DRAW_PUT);
		}
	
		waitKeyUpDown();
	
		if(GI.Moves == 0)
		{
			if(KEY_Pplus || KEY_Pminus || KEY_Vplus || KEY_Vminus)
			{
				//Change level
				//Program keys +/- 1
				if(KEY_Pplus)
				{
					GI.Level++;
				}
				if(KEY_Pminus)
				{
					GI.Level--;
				}
				//Volume keys +/- 20
				if(KEY_Vplus)
				{
					GI.Level += 20;
				}
				if(KEY_Vminus)
				{
					GI.Level -= 20;
				}
				//Wrap around if necessary
				if(GI.Level < 1)
				{
					GI.Level += SOKOBAN_LEVELS;
				}
				if(GI.Level > SOKOBAN_LEVELS)
				{
					GI.Level -= SOKOBAN_LEVELS;
				}
				//New game
				goto again;
			}
		}
		if(KEY_0) goto restart;
		if(KEY_Exit) return;
	}
}

