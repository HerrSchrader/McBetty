/*
    settingsmenue.c
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

#include "menu.h"
#include "settingsmenu.h"
#include "lcd.h"
#include "fonty.h"
#include "keyboard.h"
#include "rtc.h"
#include "ir_selector.h"
#include "backlight.h"
#include "bfs.h"
#include "rf.h"
#include "cc1100.h"
#include "ir_capture.h"

extern volatile unsigned int keys[2];
extern volatile unsigned char bl_val;
extern unsigned int bl_timeout;
extern unsigned char bl_max;

const struct MENU_ENTRY settingsMenuEntries[] =
{
	{ change_clock,		0,			FUNCENTRY,	"Clock",	"set time/date"	},
	{ RC_setting,		0,			FUNCENTRY,	"RC keys",	"RC keys assigment"	},
	{ display_setting,	0,			FUNCENTRY,	"Display",	"Display settings"	},
	{ RF_setting,		0,			FUNCENTRY,	"RF",		"RF settings"	},
	{ RAW_learnmenue,	0,			FUNCENTRY,	"IR learn",	"Learn IR commands"	},
};

const struct MENU settingsMenu =
{
	5, (MENU_ENTRY*)&settingsMenuEntries, "Settings" 
};

const struct settings_ default_settings =
{
	0x01,	//version
	1000,	//bl_timeout
	63		//bl_max
};

void display_setting(void) {
	unsigned char total,set, csel;
	
	struct settingItems_ {
		char* name;	
		unsigned char value;
		unsigned char min;
		unsigned char max;
		unsigned char clen;
	};
	
	struct settingItems_ settingItems[] = {
		{"Brightness", bl_val, 0, 63,13},
		{"Backlight timeout", bl_timeout/200, 1, 100,19}
	};
	
	csel = 0;

	lcd_fill(0);

	set_font(BOLDFONT);
	draw_string(0, 0, "Display settings", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);

	total = 2;
	for(set=0;set<total; set++) {
		set_font(BOLDFONT);
		draw_string(1, 15+(set*22), (char*)settingItems[set].name, 3, DRAW_PUT);
		set_font(SMALLFONT);
		draw_numU(1, 25+(set*22), settingItems[set].value, 0, 3, DRAW_PUT);
	}

	draw_block(0, 24+(csel*22), settingItems[csel].clen, 9, 3, DRAW_XOR);

	while(KEY_Exit) {};
	while(!KEY_Exit)
	{
		waitKeyUpDown();
		draw_block(0, 24+(csel*22), settingItems[csel].clen, 9, 3, DRAW_XOR);
		
		draw_numU(1, 25+(csel*22), settingItems[csel].value, 0, 3, DRAW_XOR);
		if(KEY_Left) {
			if(settingItems[csel].value > settingItems[csel].min)
				settingItems[csel].value--;
		}
		if(KEY_Right) {
			if(settingItems[csel].value < settingItems[csel].max)
				settingItems[csel].value++;
		}
		draw_numU(1, 25+(csel*22), settingItems[csel].value, 0, 3, DRAW_XOR);
		
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
		
		bl_timeout=settingItems[1].value * 200;
		bl_max = settingItems[0].value;
		setBacklight(settingItems[0].value);
		
		draw_block(0, 24+(csel*22), settingItems[csel].clen, 9, 3, DRAW_XOR);
	}

}

void RF_setting(void) {
	unsigned char total,set, csel;
	struct RFsettings_ RFsettings;
	
	BFS_LoadFile(BFS_ID_RFsettings, sizeof(struct RFsettings_), (unsigned char*) &RFsettings);
		
	struct settingItems_ {
		char* name;	
		unsigned char value;
		unsigned char min;
		unsigned char max;
		unsigned char clen;
	};
	
	struct settingItems_ settingItems[] = {
		{"Device address", RFsettings.address, 1, 127,19},
		{"Channel", RFsettings.channel, 0, 255,19}
	};
	
	csel = 0;

	lcd_fill(0);

	set_font(BOLDFONT);
	draw_string(0, 0, "RF settings", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);

	total = 2;
	for(set=0;set<total; set++) {
		set_font(BOLDFONT);
		draw_string(1, 15+(set*22), (char*)settingItems[set].name, 3, DRAW_PUT);
		set_font(SMALLFONT);
		draw_numU(1, 25+(set*22), settingItems[set].value, 0, 3, DRAW_PUT);
	}

	draw_block(0, 24+(csel*22), settingItems[csel].clen, 9, 3, DRAW_XOR);

	while(KEY_Exit) {};
	while(!KEY_Exit)
	{
		waitKeyUpDown();
		draw_block(0, 24+(csel*22), settingItems[csel].clen, 9, 3, DRAW_XOR);
		
		draw_numU(1, 25+(csel*22), settingItems[csel].value, 0, 3, DRAW_XOR);
		if(KEY_Left) {
			if(settingItems[csel].value > settingItems[csel].min)
				settingItems[csel].value--;
		}
		if(KEY_Right) {
			if(settingItems[csel].value < settingItems[csel].max)
				settingItems[csel].value++;
		}
		draw_numU(1, 25+(csel*22), settingItems[csel].value, 0, 3, DRAW_XOR);
		
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
		
		RFsettings.address = settingItems[0].value;
		RFsettings.channel = settingItems[1].value;
		
		draw_block(0, 24+(csel*22), settingItems[csel].clen, 9, 3, DRAW_XOR);
	}

	if(BFS_CmpFile(BFS_ID_RFsettings, sizeof(struct RFsettings_), (unsigned char*) &RFsettings) == 0) {
		BFS_SaveFile(BFS_ID_RFsettings, sizeof(struct RFsettings_), (unsigned char*) &RFsettings);
		load_RF_setting();
	}
}

void load_RC_setting(void) {
	
	if(BFS_LoadFile(BFS_ID_remotes, sizeof(struct irDeviceTable_), (unsigned char*) &irDevTab) != sizeof(struct irDeviceTable_)) {
		irDevTab = default_irDevTab;
		BFS_SaveFile(BFS_ID_remotes, sizeof(struct irDeviceTable_), (unsigned char*) &irDevTab);
		draw_string(0, 30, "default RC load", 3, DRAW_PUT);
	}

}

void load_RF_setting(void) {
	struct RFsettings_ RFsettings;
	
	if(BFS_LoadFile(BFS_ID_RFsettings, sizeof(struct RFsettings_), (unsigned char*) &RFsettings) != sizeof(struct RFsettings_)) {
		RFsettings = default_RFsettings;
		BFS_SaveFile(BFS_ID_RFsettings, sizeof(struct RFsettings_), (unsigned char*) &RFsettings);
		draw_string(0, 50, "default RF load", 3, DRAW_PUT);
	}
	
	cc1100_write1((0x09),RFsettings.address);
	cc1100_write1((0x0a),RFsettings.channel);
	cc1100Addr = RFsettings.address;
	waitTX();
		
}

void load_setting(void) {
	struct settings_ settings;
	
	if(BFS_LoadFile(BFS_ID_settings, sizeof(struct settings_), (unsigned char*) &settings) != sizeof(struct settings_)) {
		settings = default_settings;
		BFS_SaveFile(BFS_ID_settings, sizeof(struct settings_), (unsigned char*) &settings);
		draw_string(0, 40, "default settings load", 3, DRAW_PUT);
	}
	bl_timeout=settings.bl_timeout;
	bl_max = settings.bl_max;
	setBacklight(bl_max);
		
}

void enter_settings_menu(void) {
	struct settings_ settings;
	
	menu_exec((struct MENU *)&settingsMenu);
	
	settings = default_settings;
	settings.bl_timeout=bl_timeout;
	settings.bl_max = bl_max;
		
	if(BFS_CmpFile(BFS_ID_settings, sizeof(struct settings_), (unsigned char*) &settings) == 0) {
		BFS_SaveFile(BFS_ID_settings, sizeof(struct settings_), (unsigned char*) &settings);
	}
}
