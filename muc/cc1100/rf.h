/*
    rf.h - 
    Copyright (C) 2008  

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

#ifndef RF_H
#define RF_H

void rx_reset(void);
void send_cmd(char *cmd_str);
int rx_buf_empty(void);
uint8_t get_from_rx_buf();
void rxIRQ();
void rfRcvPacket(void) ;
void RF_init (void);


#endif
