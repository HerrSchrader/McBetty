/*
    menu.h - menuing system
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

#ifndef MENU_H
#define MENU_H

#define	NONE		0
#define	INTENTRY	1
#define	FUNCENTRY	2
#define	MENUENTRY	3

typedef void(*menu_fn)(void);

typedef struct MENU_ENTRY
{
	menu_fn		exec;
	void		*data;
	unsigned char	type;
	char 		*name;
	char		*help;
} MENU_ENTRY;

typedef struct MENU
{
	unsigned int		num_entries;
	struct MENU_ENTRY	*entry;
	char			*name;
} MENU;

void init_menu(void);

void menu_exec(struct MENU *menu);
int menu_execentry(struct MENU_ENTRY *entry);

#endif
