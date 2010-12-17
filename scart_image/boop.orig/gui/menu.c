/*
    menu.c - menuing system
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
#include "lcd.h"
#include "fonty.h"
#include "keyboard.h"
#include "lpc2220.h"

extern volatile unsigned int keys[2];
//extern unsigned int keys[2];


int menu_execentry(struct MENU_ENTRY *entry)
{
	unsigned int *val;
	struct MENU *m;
	draw_block(0, 100, 128, 8, 3, DRAW_ERASE);
	switch(entry->type)
	{
		case INTENTRY:
			val = (unsigned int*)entry->data; 
			draw_block(0, 50, 128, 8, 3, DRAW_ERASE);
			draw_hexW(0,50,*val, 3, DRAW_PUT);
			break;
		case FUNCENTRY:
			entry->exec();
			return 1;
			break;
		case MENUENTRY:
			m = (MENU*)entry->data;
			if(m->num_entries != 0)
			{
				menu_exec((MENU*)entry->data);
				return 1;
			}
			break;
		default:
			break;
	}
	return 0;
}

void menu_exec(struct MENU *menu)
{
	unsigned char csel, set, total;
	csel = 0;
again:
	lcd_fill(0);

	set_font(BOLDFONT);
	draw_string(0, 0, (char*)menu->name, 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);
	set_font(SMALLFONT);

	total = menu->num_entries;
	for(set=0;set<total; set++)
	{
		draw_string(1, 15+(set*9), (char*)menu->entry[set].name, 3, DRAW_PUT);
	}

	draw_block(0, 14+(csel*9), 60, 9, 3, DRAW_XOR);
	draw_string(0, 152, (char*)menu->entry[csel].help, 3, DRAW_PUT);

	while(KEY_Exit) {};
	while(!KEY_Exit)
	{
		waitKeyUpDown();
		draw_block(0, 14+(csel*9), 60, 9, 3, DRAW_XOR);
		if(KEY_OK)
		{
			if(menu_execentry((MENU_ENTRY*)&menu->entry[csel]))
				goto again;
		}
		if(KEY_Up)
		{
			csel--;
			if(csel >= total)
				csel = total-1;
		}
		if(KEY_Down)
		{
			csel++;
			if(csel == total)
				csel = 0;
		}
		draw_block(0, 14+(csel*9), 60, 9, 3, DRAW_XOR);
		draw_block(0, 152, 128, 8, 3, DRAW_ERASE);
		draw_string(0, 152, (char*)menu->entry[csel].help, 3, DRAW_PUT);
	}
}

void init_menu(void)
{
}

