/*
    ir_capture.c - 
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


#include "lcd.h"
#include "fonty.h"
#include "keyboard.h"
#include "lpc2220.h"
#include "irq.h"
#include "backlight.h"
#include "ir_capture.h"
#include "global.h"
#include "ir_raw.h"
#include "infrared.h"
#include "ir_selector.h"
#include "flash.h"
#include "controls.h"
#include "bfs.h"

const char string_unknown[] = "<Unbekannt>";
#define len_string_unknown	12

extern volatile unsigned int keys[2];
extern volatile unsigned char autorepeat;
extern void setSpeed(unsigned char sp);


unsigned long waitIrHi(unsigned short* onpulse, unsigned long* time, unsigned long start) {
	
	unsigned long lowpulse;
	unsigned long l;
	unsigned long x;	
	
	*onpulse = 0;
	lowpulse = 0;
	while (1) {
		T0CCR = (1<<9) | (1<<11);
		while (!(T0IR));
		T0IR = 0xff;
		T0CCR = (1<<9) | (1<<10) | (1<<11);
		l = T0CR3;
		*time = l-start + lowpulse;
		(*onpulse)++;
		x = start + *time + 30;
		while (!(T0IR)) {
			if (T0TC > x)
				return (l + lowpulse);	
		}
		T0IR = 0xff;
		x = T0CR3;
		lowpulse = x-l;
	}
}

unsigned char decodewide(unsigned long value, struct widetable_* wide) {
	
	unsigned char x;
	unsigned long minvalue;
	unsigned long maxvalue;
	unsigned long dif;
	
	dif = (value / 10);
	
	if (dif < 1)
		dif = 1;
	if (value > 1)
		minvalue = value - dif;
	else
		minvalue = 0;
		
	maxvalue = value + dif;
	
	for (x=0;x < (wide->count);x++) {
		if ((wide->value[x] >= minvalue) && (wide->value[x] <= maxvalue)) {
			return x;
		}
	}
	if (wide->count < 15) {
		wide->value[wide->count] = value;
		wide->count++;
		return (wide->count -1);
	} 
	else {
		return 0xff;
	}
}

//---------------------------------------------------------------------------
// analizeIR
//---------------------------------------------------------------------------
/*	Return Values:
	0 = OK
	1 = too manny pulse lengths
	2 = abort
	3 = too long
*/
unsigned char captureIR(struct ir_raw_* capture) {
	
	unsigned long hightime, lowtime;
	unsigned long T0count;
	unsigned short highpulse, lowpulse;
	unsigned long T1count;
	unsigned long timeout;
	unsigned char status;
	unsigned char wide;
	
	while(ANYKEY);
	
	setSpeed(SPEED_60);
	disableIRQ();
	T0MCR = 0x00;
	
	FIOSET0 |= (1<<4);
	FIOCLR0 |= (1<<12);
	PINSEL1 |= (1<<27);
		
	T0CCR = (1<<9) | (1<<10) | (1<<11);
		
	capture->widetable[0].count = 0;
	capture->widetable[1].count = 0;
	capture->count = 0;
	status = 0;
	
	T0TC = 0;
	while (T0TC < 5000);
	
	T0IR = 0xff;
	while (!T0IR && !ANYKEY);
	if(!ANYKEY) {
		T0TC = 0;
		T0IR = 0xff;
		T0count = waitIrHi(&highpulse,&hightime,0);
		capture->pulsetime = (hightime * 1000) / highpulse;
		capture->data[0] = (decodewide(highpulse, &capture->widetable[1]) & 0x0F)<<4;
		
		while (!(T0IR));
		T0IR = 0xff;
		T1count= T0CR3;
		while ((capture->count < max_capture_data-1) && !status) {
			lowtime = T1count-T0count;
			T0count = waitIrHi(&highpulse,&hightime,T1count);		
			lowpulse = ((lowtime *1000)+(capture->pulsetime / 2)) / capture->pulsetime;
			wide = decodewide(lowpulse, &capture->widetable[1]);
			if(wide & 0xf0)
				status = 2;
			capture->data[capture->count] |= wide;
			wide = decodewide(highpulse, &capture->widetable[1]);
			if(wide & 0xf0)
				status = 2;
			capture->count++;
			capture->data[capture->count] = wide<<4;
			timeout = T0count + 500000;
			while (!(T0IR) && !status) {
				if (T0TC > timeout)
					status =1;
			}
			T0IR = 0xff;
			T1count= T0CR3;
		}
	}
	else
		status = 3;
	if(capture->count == max_capture_data-1)
		status = 4;
	T0TC = 0;
	T0MCR = 0x03;
	FIOSET0 |= (1<<12);
	T0CCR = 0;
	T0IR = 0xff;
	enableIRQ();
	setBacklight(BL_AUTO);
	setSpeed(SPEED_30);
	return status - 1;
} 

# define MAX_messages 25

unsigned char compare_message(unsigned char* message1, unsigned char* message2, unsigned char len) {
	
	unsigned char x;
	
	for(x=0; x < (len-1); x++) {
		if(message1[x] != message2[x])
			return 1;
	}
	if((message1[len-1] & 0xf0) != (message2[len-1] & 0xf0))
			return 1;
	return 0;
	
} 

//---------------------------------------------------------------------------
// analizeIR
//---------------------------------------------------------------------------
/*	Return Values:
	0 = OK
	1 = too manny messages
	2 = too few messages
	3 = repeat messages not equal
	4 = messages too long
	5 = unknown format
*/
unsigned char analyzeIR(struct ir_raw_* capture, struct RAWcmd_* cmd) {
	
	unsigned short space;
	unsigned char message[MAX_messages];
	unsigned char messagecount;
	unsigned char x;
	unsigned short sum;
	unsigned char first;
	unsigned char middle;
	unsigned char last;
	
	
	space=32000;
	for (x=4; (x < capture->count);x++) {
		unsigned short y;
		sum = 0;
		for (y = x-4; y < x; y++) {
			sum += capture->widetable[1].value[(capture->data[y]) & 0x0f];
		}
		y = capture->widetable[1].value[(capture->data[x]) & 0x0f];
		if ((sum < y) && (y < space))
			space=y;
	}
	
	messagecount = 1;
	message[0] = 0;
	for(x = 0; (x < capture->count) && (messagecount < MAX_messages); x++) {
		if(capture->widetable[1].value[capture->data[x] & 0x0f] >= space) {
			if(messagecount < MAX_messages)
				message[messagecount++] = x+1;
			else
				return 1;
		}
	}
	
	if(messagecount < 2)
		return 2;
		
	first = compare_message(&capture->data[message[0]], &capture->data[message[1]],message[1] - message[0]);
	middle = 0;
	for(x=1; x < messagecount-2; x++) {
		middle += compare_message(&capture->data[message[x]], &capture->data[message[x+1]],message[x+1] - message[x]);
	}
	last = compare_message(&capture->data[message[messagecount-2]], &capture->data[message[messagecount-1]],message[messagecount-1] - message[messagecount-2]);
	
	if((first && last && (messagecount < 4)) || ((first + last) && (messagecount < 3)))
		return 2;
	
	if(middle)
		return 3;
	
	if(first + last == 0) {
		for(x=0;x<3;x++) {
			cmd->CMDstart[x] = 0;
			cmd->CMDlen[x] = message[1];
		}
		if(cmd->CMDlen[0] > max_CMD_data) return 4;
		memcpy((unsigned char *)&cmd->data[cmd->CMDstart[0]],(unsigned char *)&capture->data[0],cmd->CMDlen[0]);
		
	} 
	else if(first + last == 2) {
		cmd->CMDstart[0] = 0;
		cmd->CMDlen[0] = message[1];
		
		cmd->CMDstart[1] = cmd->CMDstart[0] + cmd->CMDlen[0];
		cmd->CMDlen[1] = message[2]-message[1];
		
		cmd->CMDstart[2] = cmd->CMDstart[1] + cmd->CMDlen[1];
		cmd->CMDlen[2] = message[messagecount-1]-message[messagecount-2];
		
		if(cmd->CMDlen[0]+cmd->CMDlen[1]+cmd->CMDlen[2] > max_CMD_data) return 4;
		memcpy((unsigned char *)&cmd->data[cmd->CMDstart[0]],(unsigned char *)&capture->data[0],cmd->CMDlen[0]);
		memcpy((unsigned char *)&cmd->data[cmd->CMDstart[1]],(unsigned char *)&capture->data[message[1]],cmd->CMDlen[1]);
		memcpy((unsigned char *)&cmd->data[cmd->CMDstart[2]],(unsigned char *)&capture->data[message[messagecount-1]],cmd->CMDlen[2]);
	} 
	else if((first == 1)&& !last) {
		cmd->CMDstart[0] = 0;
		cmd->CMDlen[0] = message[1];
		
		cmd->CMDstart[1] = cmd->CMDstart[0] + cmd->CMDlen[0];
		cmd->CMDlen[1] = message[2]-message[1];
		
		cmd->CMDstart[2] = cmd->CMDstart[1];
		cmd->CMDlen[2] = cmd->CMDlen[1];
		
		if(cmd->CMDlen[0]+cmd->CMDlen[1] > max_CMD_data) return 4;
		memcpy((unsigned char *)&cmd->data[cmd->CMDstart[1]],(unsigned char *)&capture->data[message[1]],cmd->CMDlen[1]);
		memcpy((unsigned char *)&cmd->data[cmd->CMDstart[0]],(unsigned char *)&capture->data[0],cmd->CMDlen[0]);
		
	} 
	else {
		return 5;
	}
	
	cmd->borders = 0x13;
	cmd->tval = (15 * capture->pulsetime) / ((cmd->borders & 0x0f) * 1000);
	memcpy((unsigned char *)&cmd->pulselen[0],(unsigned char *)&capture->widetable[1].value[0],(capture->widetable[1].count) * sizeof(unsigned short));
	
	return 0;
}

void debug_print_capture(struct ir_raw_* capture) {

	unsigned short space;
	unsigned short sum;
	unsigned char x;
	unsigned char ro;
	unsigned char co;
	
	space=32000;
	for (x=4; (x < capture->count);x++) {
		unsigned short y;
		sum = 0;
		for (y = x-4; y < x; y++) {
			sum += capture->widetable[1].value[(capture->data[y]) & 0x0f];
		}
		y = capture->widetable[1].value[(capture->data[x]) & 0x0f];
		if ((sum < y) && (y < space))
			space=y;
	}
	
	x= 0;
	ro = 0;
	co = 0;
	while((x <= capture->count) && (ro < 20)) {
		draw_hexC(co++ * 14,ro * 8,capture->data[x], 3, DRAW_PUT);
		if((capture->widetable[1].value[capture->data[x++] & 0x0f] >= space) || (co > 5)) {
			ro++;
			co = 0;
		}
	}
	
	draw_numU(90,8,capture->pulsetime,0, 3, DRAW_PUT);
	
	for (x = 0; x < capture->widetable[1].count; x++) {
		draw_numU(90,36+(x*8),capture->widetable[1].value[x],0, 3, DRAW_PUT);
	}
}

void debug_print_cmd(struct RAWcmd_* cmd) {

	unsigned char x;
	unsigned char ro;
	unsigned char co;
	
	x= 0;
	ro = 0;
	co = 0;
	
	while((x < cmd->CMDlen[0]) && (ro < 20)) {
		draw_hexC(co++ * 14,ro * 8,cmd->data[cmd->CMDstart[0]+ x++], 3, DRAW_PUT);
		if(co > 4) {
			ro++;
			co = 0;
		}
	}
	x=0;
	co = 0;
	ro++;
	while((x < cmd->CMDlen[1]) && (ro < 20)) {
		draw_hexC(co++ * 14,ro * 8,cmd->data[cmd->CMDstart[1]+ x++], 3, DRAW_PUT);
		if(co > 4) {
			ro++;
			co = 0;
		}
	}
	x=0;
	co = 0;
	ro++;
	while((x < cmd->CMDlen[2]) && (ro < 20)) {
		draw_hexC(co++ * 14,ro * 8,cmd->data[cmd->CMDstart[2]+ x++], 3, DRAW_PUT);
		if(co > 4) {
			ro++;
			co = 0;
		}
	}

	draw_numU(90,8,cmd->tval,0, 3, DRAW_PUT);
	
	for (x = 0; x < 16; x++) {
		draw_numU(75,20+(x*8),cmd->pulselen[x],0, 3, DRAW_PUT);
	}
}

void clear_cmd(struct RAWcmd_* cmd) {
	
	unsigned char x;
	unsigned char* clear;
	
	clear = (unsigned char*)cmd;
	for(x=0;x<sizeof(struct RAWcmd_);x++) {
		clear[x] = 0xff;
	}
}

void getIR_test(struct RAWcmd_* cmd) {

	unsigned char x;
	
	struct ir_raw_ capture;
	
	lcd_fill(0);
	draw_string(0,0,"Beide Fernbedienungen\neinander gegenüber\nlegen, so dass die Infra-\nrotschnittstellen direkt\nzueinander liegen.\nAbstand 2-10cm\nAnzulernene Taste drücken.", 3, DRAW_PUT);
	x=captureIR(&capture);
	lcd_fill(0);
	draw_numU(90,16,x,0, 3, DRAW_PUT);
	debug_print_capture(&capture);
		
	clear_cmd(cmd);
	x = analyzeIR(&capture, cmd);
	
	//debug_print_cmd(cmd);
	
	draw_numU(90,24,x,0, 3, DRAW_PUT);
	
	if(x)
		draw_string(97,24,"FAIL", 3, DRAW_PUT);
	else
		draw_string(97,24,"OK", 3, DRAW_PUT);
	
}


void getIR(struct RAWcmd_* cmd) {

	unsigned char x;
	struct ir_raw_ capture;
	
	x=1;
	while(x) {
		while(x) {
			draw_string(0,20,"Beide Fernbedienungen\neinander gegenueber\nlegen, so dass die Infra-\nrotschnittstellen direkt\nzueinander liegen.\nAbstand 2-10cm\nAnzulernene Taste druecken", 3, DRAW_PUT);
			x=captureIR(&capture);
			draw_block(0,20,128,9*7,3,DRAW_ERASE);
			if(x) {
				switch(x) {
					case 1:
						draw_string(0,20,"Befehl kann nicht eingelesen\nwerden.", 3, DRAW_PUT);
						break;
					case 2:
						return;
					case 3:
						draw_string(0,20,"Signal zu lange.\nBitte kuerzer druecken.", 3, DRAW_PUT);
						break;
					default:
						draw_string(0,20,"Fehler beim eingelesen.", 3, DRAW_PUT);
				}
				draw_string(0,56,"Exit zum abbrechen, andere\nTaste zum wiederholen", 3, DRAW_PUT);
				waitKeyUpDown();	
				if(KEY_Exit) {
					draw_block(0,20,128,54,3,DRAW_ERASE);
					return;
				}
				draw_block(0,20,128,45,3,DRAW_ERASE);
			}
		}
		clear_cmd(cmd);
		x = analyzeIR(&capture, cmd);
		if(x) {
			switch(x) {
				case 1:
					draw_string(0,20,"Signal zu lange.\nBitte kuerzer druecken.", 3, DRAW_PUT);
					break;
				case 2:
					draw_string(0,20,"Signal zu kurz.\nBitte laenger druecken.", 3, DRAW_PUT);
					break;
				case 3:
					draw_string(0,20,"Fehler beim Signalvergleich", 3, DRAW_PUT);
					break;
				case 4:
					draw_string(0,20,"Datenformat zu lange.", 3, DRAW_PUT);
					break;
				case 5:
					draw_string(0,20,"Unbekanntes Datenformat.", 3, DRAW_PUT);
					break;
				default:
					draw_string(0,20,"Fehler beim decodieren.", 3, DRAW_PUT);
			}
			draw_string(0,56,"Exit zum abbrechen, andere\nTaste zum wiederholen", 3, DRAW_PUT);
			waitKeyUpDown();	
			if(KEY_Exit) {
				draw_block(0,20,128,54,3,DRAW_ERASE);
				return;
			}
			draw_block(0,20,128,54,3,DRAW_ERASE);
			
		}
	}
		
}

void RAW_learn(struct RAWset_* RAWset) {
	
	unsigned char cmd;
	unsigned char mode;
	
	lcd_fill(0);
	
	draw_block(0,10,128,2,3,DRAW_PUT);
		
	mode = 1;
	while(KEY_A) {};
	while(!KEY_A) {
		draw_block(0,0,100,9,3,DRAW_ERASE);
		set_font(BOLDFONT);
		if (mode)
			draw_string(0, 0, "Tasten anlernen", 3, DRAW_PUT);
		else
			draw_string(0, 0, "Tasten testen", 3, DRAW_PUT);
		set_font(SMALLFONT);
		draw_string(0, 20, "Gewuenschte Taste auf der\nBetty druecken.\nDie Tasten A, B, C und D\nkoennen nicht angelernt\nwerden", 3, DRAW_PUT);
		draw_string(0, 90, "A - Exit\nB - Anlernen/Testen\n", 3, DRAW_PUT);
		waitKeyUpDown();
		draw_block(0,20,128,45,3,DRAW_ERASE);
		draw_block(0,90,128,45,3,DRAW_ERASE);
		if(KEY_A || KEY_B || KEY_C || KEY_D) {
			if(KEY_B)
				mode = 1-mode;
		}
		else {
			cmd=getKeynum() -4;
			if(mode) {
				getIR(&(RAWset->RAWcmd[cmd]));
			}
			else {
				RAW_Send((unsigned long)&(RAWset->RAWcmd[cmd]));
				while(ANYKEY)
					RAW_Repeat();
				RAW_Stop();
			}
		}
	}
}

void RAW_learnset(FORM* form, CONTROL* control) {
	RAW_learn((struct RAWset_*)(form->tag));
	form_draw(form);
}

void RAW_keyerase(FORM* form, CONTROL* control) {
	unsigned char x;
	
	x=msgbox(50,BC_nobuttons,"\nZu loeschende Taste\ndruecken.\n");
	waitKeyDown();
	x=getKeynum();
	if(x>3) {
			/*
			unsigned short y;
			unsigned char* z;
			z=&((struct RAWset_*)(form->tag))->RAWcmd[x-4];
			for(y=0;y<sizeof(struct RAWcmd_);y++) {
				z[y]=x;
			}*/
	
	
		clear_cmd(&(((struct RAWset_*)(form->tag))->RAWcmd[x-4]));
	}
	form_draw(form);
}

void RAW_allkeyserase(FORM* form, CONTROL* control) {
	unsigned char x;
	
	x=msgbox(50,BC_YesNo | BC_DefaultButton2,"Moechten Sie wirklich\nalle Tasten loeschen?\n");
	if(x == BC_Yes) {
		for(x=0;x<38;x++) {
			clear_cmd(&(((struct RAWset_*)(form->tag))->RAWcmd[x]));
			
		}
	}
	/*
	unsigned long y;
	unsigned char* z;
	z=(struct RAWset_*)(form->tag);
	for(y=0;y<sizeof(struct RAWset_);y++)
		z[y]=y & 0xff;
	((struct RAWset_*)(form->tag))->name[0] = 0;
	*/
	form_draw(form);
}

void RAW_editset(FORM* form, CONTROL* control) {

	struct RAWset_ RAWset;
	const BUTTON button1	= {BC_Button,0,15,120,15,SMALLFONT,0,"Anlernen/Testen",RAW_learnset};
	const BUTTON button2	= {BC_Button,0,35,120,15,SMALLFONT,0,"Taste loeschen",RAW_keyerase};
	const BUTTON button3	= {BC_Button,0,55,120,15,SMALLFONT,0,"Alle Tasten loeschen",RAW_allkeyserase};
	const LABEL label1		= {BC_Labelnotab,0,80,60,10,BOLDFONT,0,"Bezeichnung",0};
	TXTBOX txtbox1			= {BC_Txtbox,0,90,120,10,SMALLFONT,20,0,(char*)RAWset.name,0};
	
	CONTROL* controls[5] = {	(CONTROL*)&button1,
								(CONTROL*)&button2,
								(CONTROL*)&button3,
								(CONTROL*)&label1,
								(CONTROL*)&txtbox1};
	
	FORM form2  = {"RAW Set bearbeiten",0,0,0,0,controls,0,5};	
	unsigned long x;
	
	x = (unsigned long)BFS_LoadFileAddr(BFS_ID_RAWslot0 + ((LABEL*)control)->tag);
	
	draw_hexW(0,100,x,3,DRAW_PUT);
	
	if(x)
		memcpy(&RAWset,(void *)x,sizeof(struct RAWset_));
	else
		RAWset.id[0] = 0xff;
		
	x=memcmp(&RAWset,"RC01",4);
	if(x) {
		for(x=0;x<RAWnamelen;x++)
			RAWset.name[x]=0xff;
		for(x=0;x<38;x++) {
			clear_cmd(&RAWset.RAWcmd[x]);
		}
		BFS_SaveFile(BFS_ID_RAWslot0 + ((LABEL*)control)->tag, sizeof(struct RAWset_), (unsigned char*) &RAWset);
	}
	
	if(RAWset.name[0]==0xff)
		memcpy(&RAWset.name[0],(char*)string_unknown,len_string_unknown);
	
	form2.tag =(unsigned long)&RAWset;
	
	form_exec(&form2);
	
	if(!memcmp(&RAWset.name[0],string_unknown,len_string_unknown))
		for(x=0;x<len_string_unknown;x++)
			RAWset.name[x] = 0xff;
	
	if(BFS_CmpFile(BFS_ID_RAWslot0 + ((LABEL*)control)->tag, sizeof(struct RAWset_), (unsigned char*) &RAWset) == 0) {
		x=msgbox(50,BC_YesNo | BC_DefaultButton1,"Sollen die Aenderungen\ngespeichert werden?\n");
		if(x == BC_Yes) {
			memcpy(&RAWset.id[0],"RC01",4);
			BFS_SaveFile(BFS_ID_RAWslot0 + ((LABEL*)control)->tag, sizeof(struct RAWset_), (unsigned char*) &RAWset);
			if(BFS_CmpFile(BFS_ID_RAWslot0 + ((LABEL*)control)->tag, sizeof(struct RAWset_), (unsigned char*) &RAWset) == 0) {
				msgbox(50,BC_OKOnly | BC_DefaultButton1,"Fehler\n");
			}
		}
	}
	
	form_draw(form);
}

void RAW_eraseall(FORM* form, CONTROL* control) {
	
	unsigned char x;
	
	x=msgbox(50,BC_YesNo | BC_DefaultButton2,"Moechten Sie wirklich\nalle gespeicherten\nCodes loeschen?\n");
	if(x == BC_Yes) {
	for(x=0;x<4;x++)
		BFS_DeleteFile(BFS_ID_RAWslot0 + x);
	}
	form_draw(form);
}

void RAW_learnmenue(void) {

	const LABEL label1		= {BC_Label,0,25,50,10,SMALLFONT,0,"RAW Set 1",RAW_editset};
	const LABEL label2		= {BC_Label,0,35,50,10,SMALLFONT,1,"RAW Set 2",RAW_editset};
	const LABEL label3		= {BC_Label,0,45,50,10,SMALLFONT,2,"RAW Set 3",RAW_editset};
	const LABEL label4		= {BC_Label,0,55,50,10,SMALLFONT,3,"RAW Set 4",RAW_editset};
	const LABEL label5		= {BC_Labelnotab,0,15,100,10,BOLDFONT,0,"RAW Set bearbeiten:",0};
	const BUTTON button1	= {BC_Button,0,70,80,15,SMALLFONT,0,"Alles loeschen",RAW_eraseall};
	
	CONTROL* controls[6] = {	(CONTROL*)&label1,
								(CONTROL*)&label2,
								(CONTROL*)&label3,
								(CONTROL*)&label4,
								(CONTROL*)&label5,
								(CONTROL*)&button1};
	
	FORM form  = {"IR-Codes anlernen",0,0,0,0,controls,0,6};
	
	form_exec(&form);
	setEncoder(irDevTab.device[irDevTab.active].encoder, irDevTab.device[irDevTab.active].set);

}

