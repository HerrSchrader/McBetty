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
#include "testmenu.h"
#include "gamesmenu.h"
#include "settingsmenu.h"
#include "toolsmenu.h"

unsigned int testdata1, testdata2;

const struct MENU_ENTRY mainMenuEntries[] = 
{
	{ 0		, 	(void*)&gamesMenu,	MENUENTRY,	"Games",	"Play some games"	},
	{ 0,	 		(void*)&testMenu,	MENUENTRY,	"TestStuff",	"Various tests"	},
	{ enter_settings_menu,	0,			FUNCENTRY,	"Settings",	"Change settings"	},
	{ 0,	 		(void*)&toolsMenu,	MENUENTRY,	"Tools",	"Tools"	}
};


const struct MENU mainMenu =
{
	4, (MENU_ENTRY*)&mainMenuEntries, "Main Menu"
};
