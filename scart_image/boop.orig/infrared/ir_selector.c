/*
    ir_selector.c - select ir encoder & codetable
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

#include "infrared.h"
#include "codes.h"
#include "encoders.h"
#include "lcd.h"
#include "fonty.h"
#include "keyboard.h"
#include "ir_selector.h"
#include "bfs.h"
#include "ir_capture.h"

extern volatile unsigned int keys[2];
extern volatile unsigned char autorepeat;
extern unsigned char act_encoder, act_set;

const struct irDeviceTable_ default_irDevTab =
{
	0x04,
	0x00,
	{
		{ 0x01, 0x00 },
		{ 0x02, 0x00 },
		{ 0xff, 0xff },
		{ 0xff, 0xff }
	}
};

struct irDeviceTable_ irDevTab;

char* getEncName(unsigned char enc)
{
	return (char*) encoders.encoder[enc].name;
}

char* getSetName(unsigned char enc, unsigned char set)
{
	struct TABLES_C *ctab;
	struct TABLES_S *stab;
	struct TABLES_I *itab;
	struct TABLES_N *ntab;
	struct RAWset_ *RAWset;
	
	if (enc < encoders.num_encoders) {
		switch(encoders.encoder[enc].type)
		{
			case 0:
				ctab = (TABLES_C*)encoders.encoder[enc].data;
				if (set < ctab->num_tables)
					return (char*) ctab->table[set].name;
				break;
			case 1:
				stab = (TABLES_S*)encoders.encoder[enc].data;
				if (set < stab->num_tables)
					return (char*) stab->table[set].name;
				break;
			case 2:
				itab = (TABLES_I*)encoders.encoder[enc].data;
				if (set < itab->num_tables)
					return (char*) itab->table[set].name;
				break;
			case 3:
				ntab = (TABLES_N*)encoders.encoder[enc].data;
				if (set < ntab->num_tables) {
					RAWset = (struct RAWset_*)BFS_LoadFileAddr(BFS_ID_RAWslot0 + set);
					if(RAWset && (RAWset->name[0] != 0xff)) {
						return (char*) RAWset->name;
					}
					else
						return (char*) ntab->table[set].name;
				}
				break;
		}
	}
	
	return "unknown\0";
}

unsigned char EncIsValid(unsigned char enc, unsigned char set)
{
	struct TABLES_C *ctab;

	if (!(enc < encoders.num_encoders))
		return 0;
	ctab = (TABLES_C*)encoders.encoder[enc].data;
	if (!(set < ctab->num_tables))
		return 0;
	return 1;
}

unsigned char listSet(unsigned int enc, unsigned char sset)
{
	unsigned char set;

	if (!(sset < (((TABLES_C*)(encoders.encoder[enc].data))->num_tables)))
		sset = 0;

	lcd_fill(0);
	set_font(BOLDFONT);
	draw_hexC(0,0,((TABLES_C*)(encoders.encoder[enc].data))->num_tables, 3, DRAW_PUT);
	draw_string(16, 0, "tables for", 3, DRAW_PUT);
	draw_string(77, 0, (char*) encoders.encoder[enc].name, 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);

	set_font(SMALLFONT);

	for(set=0;set<((TABLES_C*)(encoders.encoder[enc].data))->num_tables; set++)
	{
		draw_string(1, 15+(set*9), getSetName(enc, set), 3, DRAW_PUT);
	}

	draw_string(0, 120, "\"Left\" to return", 3, DRAW_PUT);
	draw_string(0, 130, "\"OK\" to select remote", 3, DRAW_PUT);

	draw_block(0, 14+(sset*9), 128, 9, 3, DRAW_XOR);

	while(!KEY_Left)
	{
		waitKeyUpDown();
		draw_block(0, 14+(sset*9), 128, 9, 3, DRAW_XOR);
		if(KEY_OK)
		{
			return sset;
		}
		if(KEY_Up)
		{
			sset--;
			if(sset >= ((TABLES_C*)(encoders.encoder[enc].data))->num_tables)
				sset = ((TABLES_C*)(encoders.encoder[enc].data))->num_tables-1;
		}
		if(KEY_Down)
		{
			sset++;
			if(sset == ((TABLES_C*)(encoders.encoder[enc].data))->num_tables)
				sset = 0;
		}
		draw_block(0, 14+(sset*9), 128, 9, 3, DRAW_XOR);
	}
	return 0xFF;
}

#define maxenclist	8
void select_ir(unsigned char *encoder,unsigned char *set)
{
	static unsigned char enc, senc, cenc, sset;
	unsigned char moff;
	unsigned char moffold;
	
	if (*encoder < encoders.num_encoders)
		senc = *encoder;
	else
		senc = 0;
	cenc = senc;
	sset = *set;
	moff = 0;
	
	if(senc >= maxenclist)
		moff = senc - maxenclist +1;
	
again:
	moffold = encoders.num_encoders;
	
	set_font(BOLDFONT);

	lcd_fill(0);
	draw_string(0, 0, "Encoders: ", 3, DRAW_PUT);
	draw_hexC(54,0,encoders.num_encoders,3,DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);

	set_font(SMALLFONT);
	
	draw_string(0, 106, "\"OK\" to accept select remote", 3, DRAW_PUT);
	draw_string(0, 115, "\"Exit\" to return", 3, DRAW_PUT);
	draw_string(0, 124, "\"Right\" to show names for", 3, DRAW_PUT);
	draw_string(35, 133, "selected encoder", 3, DRAW_PUT);
	set_font(BOLDFONT);
	draw_string(0, 142, "Currently Selected:", 3, DRAW_PUT);
	draw_string(0, 152, (char*) encoders.encoder[cenc].name, 3, DRAW_PUT);
	draw_string(48, 152, getSetName(cenc,sset), 3, DRAW_PUT);
	set_font(SMALLFONT);
	
	while(!KEY_Exit)
	{
		if(moff!=moffold) {
			draw_block(0, 14, 128, 9*maxenclist, 3, DRAW_ERASE);
			for(enc=moff;(enc<encoders.num_encoders) && (enc-moff<maxenclist);enc++) {
				draw_hexC(1,15+((enc-moff)*9), ((TABLES_C*)(encoders.encoder[enc].data))->num_tables, 3, DRAW_PUT);
				draw_string(15, 15+((enc-moff)*9), "remotes in", 3, DRAW_PUT);
				draw_string(65, 15+((enc-moff)*9), (char*) encoders.encoder[enc].name, 3, DRAW_PUT);
			}
			
			if(encoders.num_encoders > moff+maxenclist) {
				draw_hline(120,8+9*maxenclist,5,3,DRAW_PUT);
				draw_hline(121,9+9*maxenclist,3,3,DRAW_PUT);
				draw_hline(122,10+9*maxenclist,1,3,DRAW_PUT);
			}
			if(moff) {
				draw_hline(120,19,5,3,DRAW_PUT);
				draw_hline(121,18,3,3,DRAW_PUT);
				draw_hline(122,17,1,3,DRAW_PUT);
			}
			draw_block(0, 14+((senc-moff)*9), 128, 9, 3, DRAW_XOR);
		}
		moffold = moff;
		waitKeyUpDown();
		draw_block(0, 14+((senc-moff)*9), 128, 9, 3, DRAW_XOR);
		if(KEY_Right)
		{
			enc = sset;
			sset = listSet(senc,sset);
			if(sset != 0xFF)
				cenc = senc;
			else
				sset = enc;
			goto again;
		}
		if(KEY_Up)
		{
			senc--;
			if(senc<moff)
				moff--;
			if(senc >= encoders.num_encoders) {
				if(encoders.num_encoders > maxenclist)
					moff = encoders.num_encoders - maxenclist;
				senc = encoders.num_encoders-1;
			}
		}
		if(KEY_Down)
		{
			senc++;
			if(senc == maxenclist+moff)
				moff++;
			if(senc == encoders.num_encoders) {
				moff = 0;
				senc = 0;
			}
		}
		if(KEY_OK)
		{
			//setEncoder(cenc, sset);
			*encoder = cenc;
			*set = sset;
			return;
		}
		draw_block(0, 14+((senc-moff)*9), 128, 9, 3, DRAW_XOR);
	}
}

void RC_setting(void) {
	unsigned char set, total, csel;
	csel = 0;
again:
	lcd_fill(0);

	set_font(BOLDFONT);
	draw_string(0, 0, "RC keys assigment", 3, DRAW_PUT);
	draw_block(0,10,128,2,3,DRAW_PUT);
	draw_block(0,149,128,2,3,DRAW_PUT);
	set_font(SMALLFONT);

	total = irDevTab.count;
	for(set=0;set<total; set++)
	{
		set_font(BOLDFONT);
		draw_string(1, 15+(set*22), "Key", 3, DRAW_PUT);
		draw_char(20, 15+(set*22),'A'+set, 3, DRAW_PUT);
		set_font(SMALLFONT);
		draw_string(1, 25+(set*22),getSetName(irDevTab.device[set].encoder, irDevTab.device[set].set), 3, DRAW_PUT);
	}

	draw_block(0, 24+(csel*22), 128, 9, 3, DRAW_XOR);
	//draw_string(0, 152, (char*)menu->entry[csel].help, 3, DRAW_PUT);

	while(KEY_Exit) {};
	while(!KEY_Exit)
	{
		waitKeyUpDown();
		draw_block(0, 24+(csel*22), 128, 9, 3, DRAW_XOR);
		if(KEY_OK)
		{
				select_ir(&irDevTab.device[csel].encoder, &irDevTab.device[csel].set);
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
		draw_block(0, 24+(csel*22), 128, 9, 3, DRAW_XOR);
		//draw_block(0, 152, 128, 8, 3, DRAW_ERASE);
		//draw_string(0, 152, (char*)menu->entry[csel].help, 3, DRAW_PUT);
	}
	
	set = irDevTab.active;
	irDevTab.active = 0;
	if(BFS_CmpFile(BFS_ID_remotes, sizeof(struct irDeviceTable_), (unsigned char*) &irDevTab) == 0) {
		BFS_SaveFile(BFS_ID_remotes, sizeof(struct irDeviceTable_), (unsigned char*) &irDevTab);
	}
	irDevTab.active = set;
}

