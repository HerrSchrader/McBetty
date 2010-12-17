/*
    testmenu.c - testmenu for menuing system
    Copyright (C) 2007  Ch. Klippel <ck@mamalala.net>

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

#include "menu.h"
#include "tic_tac_toe.h"
#include "sokoban.h"
#include "connect4.h"

const struct MENU_ENTRY gamesMenuEntries[] = 
{
	{ &GameTicTacToe, 	0,	FUNCENTRY,	"TicTacToe",	"Play TicTacToe"	},
	{ &GameSokoban, 	0,	FUNCENTRY,	"Sokoban",	"Play Sokoban"		},
	{ &start_connect4, 	0,	FUNCENTRY,	"Connect4",	"Play Connect4"		}
};


const struct MENU gamesMenu =
{
	3, (MENU_ENTRY*)&gamesMenuEntries, "Games"
};
