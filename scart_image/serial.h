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

#ifndef serial_H
#define serial_H

#define CTS			P0_4

//__code unsigned char crlf[] = { 0x0d, 0x0a, 0x00 };

void initSerial(unsigned short baud);
void send_string(const unsigned char* string);
void send_hex(unsigned char c);
void send_bytes(unsigned char* h, unsigned char l);
void send_byte(unsigned char h);

#endif

