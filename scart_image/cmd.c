/*
    cmd.c
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

#include "serial.h"
#include "cc1100.h"
#include <P89LPC932.h>
#include "cmd.h"

extern unsigned char cc1100Addr;
extern char *buf;

/* Transmit l bytes of user data (dat[0]..dat[l-1]) over radio
	Prepend a standard header with length and destination address information
*/
void transmit(unsigned char* dat, unsigned char l) {

        unsigned char header[2];
	
        switch_to_idle();
	
	header[0] = l + 1;
	header[1] = 0;

        cc1100_write(TX_fifo | BURST, header, 2);
        cc1100_write(TX_fifo | BURST, dat, l);
        cc1100_strobe(STX);
        waitTX();

}


void print_cc1100confi(void) {
        
        unsigned char x;
        
        cc1100_read(0x00 | BURST, buf, 0x2f);
        for (x=0 ;x < 0x2F; x++) {  
                send_string("Reg 0x");
                send_hex(x);
                send_string(" = 0x");
                send_hex(buf[x]);
                if ( (x & 3) == 3)
                        send_string(crlf);
                else 
                        send_string("   ");
        }
        send_string(crlf);
        cc1100_read(PATABLE | BURST, buf, 0x08);
                for (x=0 ;x < 0x08; x++) {  
                send_string("PTABLE[");
                send_hex(x);
                send_string("] = 0x");
                send_hex(buf[x]);
                if ( (x & 3) == 3)
                        send_string(crlf);
                else 
                        send_string("   ");
        }
}

void print_cc1100status(void) {
        send_string("CC1100 Read Status: ");
        send_hex(cc1100_strobe(SNOP | READ));
        send_string("  Write Status: ");
        send_hex(cc1100_strobe(SNOP | WRITE));
}

void cmd_strobeB(void) {
        
        unsigned char x;
        
        while (!RI);
        x = SBUF;
        RI = 0;
        
        send_byte(cc1100_strobe(x));
        
}

void send_WORb(void) {
        
        unsigned char x;
        
        while (!RI);
        x = SBUF;
        RI = 0;
        
        sendWOR(x);
        
}

unsigned char check_ping(unsigned char* dat) {
        

        return(0);
}


