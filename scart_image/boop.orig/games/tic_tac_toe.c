/*
    tic_tac_toe.c - game
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
#include "tic_tac_toe.h"

//extern volatile unsigned int keys[2];
extern unsigned int keys[2];

void GameTicTacToe(void)
{
	BYTE Color;
	BYTE Mode;
	int i;
	int k;
	BOOL ComputerWon;
	BOOL PlayerWon;
	BYTE G[2][9];//GameArray [0=Computer / 1=Player][Game fields]
	BOOL GameIsRunning = false;
	BYTE Moves = 0;
	int Pos;

	Color = LCD_COLOR_B;
	Mode = DRAW_PUT;

again:
	//New game
	//Init the display
	lcd_fill(0);//Clr screen

	//Title
	set_font(SMALLFONT);
	draw_string(0, 0, "TicTacToe", Color, Mode);

	//Grid
	draw_block(16,		16+30,		96, 3, Color, Mode);
	draw_block(16,		16+30+3+30,	96, 3, Color, Mode);
	draw_block(16+30,	16,		3, 96, Color, Mode);
	draw_block(16+30+3+30,	16,		3, 96, Color, Mode);

	//Reset game field
	for(k=0; k<2; k++)
	{
		for(i=0; i<9; i++)
		{
			G[k][i] = 0;
		}
	}

	Moves = 0;

	GameIsRunning = true;

	while(1)
	{
		Pos = -1;
		if(KEY_1){Pos = 0;}
		if(KEY_2){Pos = 1;}
		if(KEY_3){Pos = 2;}
		if(KEY_4){Pos = 3;}
		if(KEY_5){Pos = 4;}
		if(KEY_6){Pos = 5;}
		if(KEY_7){Pos = 6;}
		if(KEY_8){Pos = 7;}
		if(KEY_9){Pos = 8;}

		if((Pos >= 0) && (Pos <= 8))
		{
			if(GameIsRunning)
			{
				if((G[0][Pos] + G[1][Pos]) == 0)
				{
					//field is empty
					G[1][Pos] = 1;
					Moves++;

					if(Moves < 9)
					{
						//Moves left
						if(!GameTicTacToe_IsSolved(G, &ComputerWon, &PlayerWon))
						{
							//Not solved -> computer move
							GameTicTacToe_ComputerMove(G);
							Moves++;
						}
					}					
					GameTicTacToe_DrawGameField(G);
				}
			}		
		}
	
		//Is solved?
		GameTicTacToe_IsSolved(G, &ComputerWon, &PlayerWon);
		if(ComputerWon)
		{
			draw_string(0, LCD_SIZE_Y - (8*4), "Computer hat gewonnen", Color, Mode);
			GameIsRunning = false;
		}
		else
		{
			if(PlayerWon)
			{
				draw_string(0, LCD_SIZE_Y - (8*4), "Spieler hat gewonnen", Color, Mode);
				GameIsRunning = false;
			}
			else
			{
				if(Moves == 9)
				{
					draw_string(0, LCD_SIZE_Y - (8*4), "Unentschieden", Color, Mode);
					GameIsRunning = false;
				}
			}
		}
	
		//Footer
		if(Moves > 0)
		{
			draw_string(0, LCD_SIZE_Y - (8*3), "0", Color, Mode);
			draw_string(32, LCD_SIZE_Y - (8*3), "Neues Spiel", Color, Mode);
			draw_string(0, LCD_SIZE_Y - (8*2), "Exit", Color, Mode);
			draw_string(32, LCD_SIZE_Y - (8*2), "Spiel beenden", Color, Mode);
		}
		else
		{
			draw_block(0, LCD_SIZE_Y - (8*2), LCD_SIZE_X, 8, Color, DRAW_ERASE);
		}
	
		if(GameIsRunning)
		{
			draw_string(0, LCD_SIZE_Y - 8, "1-9", Color, Mode);
			draw_string(32, LCD_SIZE_Y - 8, "Feld waehlen", Color, Mode);
		}
		else
		{
			draw_block(0, LCD_SIZE_Y - 8, LCD_SIZE_X, 8, Color, DRAW_ERASE);
		}
		waitKeyUpDown();
		if(KEY_0) goto again;
		if(KEY_Exit) return;
	}
}

void GameTicTacToe_ComputerMove(BYTE G[2][9])
{
	int Pos;
	BYTE TempG[2][9];
	BOOL ComputerWon;
	BOOL PlayerWon;

	for(Pos=0; Pos<9; Pos++)
	{
		TempG[0][Pos] = G[0][Pos];
		TempG[1][Pos] = G[1][Pos];
	}

	//1: Check if the computer can win the game with the next move
	for(Pos=0; Pos<9; Pos++)
	{
		if((G[0][Pos] + G[1][Pos]) == 0)
		{
			//Free pos
			TempG[0][Pos] = 1;//Try this move
			GameTicTacToe_IsSolved(TempG, &ComputerWon, &PlayerWon);
			if(ComputerWon)
			{
				G[0][Pos] = 1;//Do the move
				return;
			}
			TempG[0][Pos] = 0;//Undo the try
		}
	}

	//2: Check if the computer can prohibit the player to win the game with the next move
	for(Pos=0; Pos<9; Pos++)
	{
		if((G[0][Pos] + G[1][Pos]) == 0)
		{
			//Free pos
			TempG[1][Pos] = 1;//Try this move
			GameTicTacToe_IsSolved(TempG, &ComputerWon, &PlayerWon);
			if(PlayerWon)
			{
				G[0][Pos] = 1;//Do the move
				return;
			}
			TempG[1][Pos] = 0;//Undo the try
		}
	}

	//The following strategy is not implemented to allow the player to win:
	//3: Fork: threaten a win with two possible completions in two ways
	//4: Block Fork 1: if there is a configuration where the opponent can fork, create two in a row to force a block
	//5: Block Fork 2: if there is a configuration where the opponent can fork, block that fork

	//6: Try to play the centre
	if((G[0][4] + G[1][4]) == 0)
	{
		G[0][4] = 1;
		return;
	}

	//7: If the Player is in a corner then try to play the opposite corner
	if(((G[0][0] + G[1][0]) == 0) && (G[1][8]))
	{
		//Corner 0 is free and player is in corner 8
		G[0][0] = 1;
		return;
	}
	if(((G[0][2] + G[1][2]) == 0) && (G[1][6]))
	{
		//Corner 2 is free and player is in corner 6
		G[0][2] = 1;
		return;
	}
	if(((G[0][6] + G[1][6]) == 0) && (G[1][2]))
	{
		//Corner 6 is free and player is in corner 2
		G[0][6] = 1;
		return;
	}
	if(((G[0][8] + G[1][8]) == 0) && (G[1][0]))
	{
		//Corner 8 is free and player is in corner 0
		G[0][8] = 1;
		return;
	}

	//8: Try to play an free corner
	Pos = 0;
	if((G[0][Pos] + G[1][Pos]) == 0)
	{
		G[0][Pos] = 1;
		return;
	}
	Pos = 2;
	if((G[0][Pos] + G[1][Pos]) == 0)
	{
		G[0][Pos] = 1;
		return;
	}
	Pos = 6;
	if((G[0][Pos] + G[1][Pos]) == 0)
	{
		G[0][Pos] = 1;
		return;
	}
	Pos = 8;
	if((G[0][Pos] + G[1][Pos]) == 0)
	{
		G[0][Pos] = 1;
		return;
	}

	//9: Play on a free pos
	for(Pos=0; Pos<9; Pos++)
	{
		if((G[0][Pos] + G[1][Pos]) == 0)
		{
			//Free pos
			G[0][Pos] = 1;//Do the move
			return;
		}
	}
}

BOOL GameTicTacToe_IsSolved(BYTE G[2][9], BOOL *pComputerWon, BOOL *pPlayerWon)
{
	//Return true=GameIsSolved
	int k;
	BYTE Result[2];

	*pComputerWon = false;
	*pPlayerWon = false;

	//Is solved?
	Result[0] = 0;
	Result[1] = 0;
	for(k=0; k<2; k++)
	{
		Result[k] |= 1 << (G[k][0] + G[k][1] + G[k][2]);//Row 1
		Result[k] |= 1 << (G[k][3] + G[k][4] + G[k][5]);//Row 2
		Result[k] |= 1 << (G[k][6] + G[k][7] + G[k][8]);//Row 3

		Result[k] |= 1 << (G[k][0] + G[k][3] + G[k][6]);//Column 1
		Result[k] |= 1 << (G[k][1] + G[k][4] + G[k][7]);//Column 2
		Result[k] |= 1 << (G[k][2] + G[k][5] + G[k][8]);//Column 3

		Result[k] |= 1 << (G[k][0] + G[k][4] + G[k][8]);//Diagonal
		Result[k] |= 1 << (G[k][2] + G[k][4] + G[k][6]);//Diagonal
	}

	if(Result[0] & (1 << 3))
	{
		*pComputerWon = true;
		return true;//Solved
	}
	if(Result[1] & (1 << 3))
	{
		*pPlayerWon = true;
		return true;//Solved
	}

	return false;//Not solved
}

void GameTicTacToe_DrawGameField(BYTE G[2][9])
{
	int Pos;
	int x;
	int y;
	BYTE Color;
	BYTE Mode;

	Color = LCD_COLOR_B;
	Mode = DRAW_PUT;

	for(Pos=0; Pos<9; Pos++)
	{

//		x = Pos % 3;
//		y = Pos / 3;
		y = 0;
		x = Pos;
		while(x >= 3)
		{
			x -= 3;
			y++;
		}

		if(G[0][Pos])
		{
			//Computer (O)
			draw_disc(16+14 + (x*33), 16+14 + (y*33), 11, Color, Mode);
			draw_disc(16+14 + (x*33), 16+14 + (y*33), 8, Color, DRAW_XOR);
		
		}
		else
		{
			if(G[1][Pos])
			{
				//Player (X)
draw_line(16+3 + (x*33), 	16+3 + (y*33)+2,	16+3 + (x*33) + 23-2, 	16+3 + (y*33) + 23, Color, Mode);
draw_line(16+3 + (x*33), 	16+3 + (y*33)+1,	16+3 + (x*33) + 23-1, 	16+3 + (y*33) + 23, Color, Mode);
draw_line(16+3 + (x*33), 	16+3 + (y*33),		16+3 + (x*33) + 23, 	16+3 + (y*33) + 23, Color, Mode);
draw_line(16+3 + (x*33)+1, 	16+3 + (y*33),		16+3 + (x*33) + 23, 	16+3 + (y*33) + 23-1, Color, Mode);
draw_line(16+3 + (x*33)+2, 	16+3 + (y*33),		16+3 + (x*33) + 23, 	16+3 + (y*33) + 23-2, Color, Mode);


draw_line(16+3-1+(x*33)+2, 16+3+(y*33)+23+1,   16+3+(x*33)+23,   16+3 + (y*33)+2, Color, Mode);
draw_line(16+3-1+(x*33)+1, 16+3+(y*33)+23+1,   16+3+(x*33)+23,   16+3 + (y*33)+1, Color, Mode);
draw_line(16+3-1+(x*33),   16+3+(y*33)+23+1,   16+3+(x*33)+23,   16+3 + (y*33),   Color, Mode);
draw_line(16+3-1+(x*33),   16+3+(y*33)+23+1-1, 16+3+(x*33)+23-1, 16+3 + (y*33),   Color, Mode);
draw_line(16+3-1+(x*33),   16+3+(y*33)+23+1-2, 16+3+(x*33)+23-2, 16+3 + (y*33),   Color, Mode);

			}
		}	
	}
}


