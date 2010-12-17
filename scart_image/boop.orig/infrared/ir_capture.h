/*
    ir_capture.h -
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

#ifndef IR_CAPTURE_H
#define IR_CAPTURE_H

#define max_CMD_data	102

#define MAX_slots		4
#define RAWnamelen		20 
struct RAWcmd_ {
	unsigned char tval;
	unsigned char borders;
	unsigned char flags;
	unsigned char toggle;
	unsigned char CMDstart[3];
	unsigned char CMDlen[3];
	unsigned short pulselen[16];
	unsigned char data[max_CMD_data];
};

struct RAWset_ {
	unsigned char 	id[4];
	unsigned char	name[RAWnamelen];
	struct RAWcmd_	RAWcmd[38];
};

struct widetable_ {	
	unsigned short value[15];
	unsigned char count;
};

#define max_capture_data	200

struct ir_raw_ {
	unsigned long pulsetime;
	unsigned char data[max_capture_data];
	struct widetable_ widetable[2];
	unsigned char count;
	
};

void getIR_test(struct RAWcmd_* cmd);
void RAW_learnmenue(void);

#endif
