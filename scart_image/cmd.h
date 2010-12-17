/*
    serial.h
    Copyright (C) 2007  

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

#ifndef cmd_H
#define cmd_H

void transmit(unsigned char* dat, unsigned char l);
void transmitB(void);
void print_cc1100confi(void);
void print_cc1100status(void);
void print_rx(void);
void print_rxB(void);
void read_byteB(void);
void write_byteB(void);
void read_burst_byteB(void);
void write_burst_byteB(void);
void cmd_strobeB(void);
void send_WORb(void);
unsigned char check_ping(unsigned char* dat);

#endif

