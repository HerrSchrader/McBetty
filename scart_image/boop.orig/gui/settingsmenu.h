/*
    settingsmenue.h 
    Copyright (C) 2008  <telekatz@gmx.de>

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

#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

extern const struct MENU settingsMenu;

struct settings_ {	
	unsigned char version;
	unsigned int bl_timeout;
	unsigned char bl_max;
};

void RF_setting(void);
void display_setting(void);
void load_RC_setting(void);
void load_RF_setting(void);
void load_setting(void);
void enter_settings_menu(void);

#endif
