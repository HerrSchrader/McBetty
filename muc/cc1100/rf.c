/*
    rf.c - 
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

/* 
	- We want to send some bytes out over radio to a specific receiver (or by broadcast).
		This should be non-blocking.
		What if the radio is busy? Return an error status and let application decide if to redo the sending
		
	- We will receive some bytes over radio. This can happen any time.
		We inform the application about it. 
	TODO 	
	- We can put the radio to sleep, if no activity is required to save battery power.
	
*/

#include "lpc2220.h"
#include "global.h"
#include "kernel.h"
#include "timerirq.h"
#include "irq.h"
#include "cc1100.h"
#include "cc1100_defs.h"
#include "rf.h"
#include "fonty.h"

/* Bring the CC1100 to idle mode and wait until it is reached */
// NOTE potential indefinate waiting time 
// belongs in module cc1100.c ?
static void 
switch_to_idle() {
	cc1100_strobe(SIDLE);
	while ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) != MARCSTATE_IDLE); 
}

#define RX_OVERFLOW 17

/* This routine checks if reception is stuck in RX_OVERFLOW state.
	If so, it flushes the buffer and resets radio to RX */
void
rx_reset(){
	if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) == RX_OVERFLOW){
		cc1100_strobe(SFRX);
		cc1100_strobe(SRX);
	};
};

/* ----------------------------------- Sending a single packet over radio--------------------------------------------- */

/* Send the contents of buffer b over radio 
	b must be less than 255 characters
	A EOT (0x04) is appended to the packet.
*/
static void
RF_send(unsigned char* b, int payload_cnt) {
	int n;
	
	// Wait until all previous bytes have been sent out
	while ( (cc1100_read_status_reg_otf(TXBYTES) & 0x7f) != 0);
	
	switch_to_idle();
	cc1100_strobe(SFTX);
	cc1100_strobe(SCAL);	
		
	cc1100_write1(TX_fifo, payload_cnt+1+1);	/* Extra bytes ADDR and EOT */
	cc1100_write1(TX_fifo, DEVICE_ADDRESS);	
	
	/* We send the first 60 bytes to the TXFIFO */
	n = min (MAX_PACKET_SIZE, payload_cnt);
	cc1100_write(TX_fifo | BURST, b, n);
	payload_cnt -= n;
	b += n;

	/* Maybe we have room for the final EOT */
	if (n < MAX_PACKET_SIZE){
		cc1100_write1(TX_fifo, EOT);
		payload_cnt--;
	};

	cc1100_strobe(STX);						// start transmitting
	
	while (payload_cnt >= 0){
		
		/* Wait until bytes are free in TXFIFO */
		while ((cc1100_read_status_reg_otf(TXBYTES) & 0x7F) > 60);
		
		if (payload_cnt > 0)
			cc1100_write1(TX_fifo, *(b++));
		else
			cc1100_write1(TX_fifo, EOT);
		payload_cnt--;
	};
}

/* Under all circumstances do we want to avoid cluttering the air waves with our comunication, because other devices
	might be sending on the same channel (like some radio remote controls).
	So we throttle the possible sending rate to 1 send per SEND_BANDWIDTH seconds average.
	We do this by keeping a variable send_token, which allows sending messages as long as it is not 0.
	Each send consumes 1 token.
	A task regularily produces 1 token per SEND_BANDWIDTH seconds.
	We initialize send_token with around 200 so that we can send more at start up.
	We also limit the token to 1000, so Worst case is betty gone wild sends 1000 messages in rapid 
	succession, but after that is throttled to 1 message per SEND_BANDWIDTH seconds.
*/

#define SEND_BANDWIDTH	(5*TICKS_PER_SEC)
#define MAX_SEND_TOKEN	1000
#define INIT_SEND_TOKEN	200

static int send_token = INIT_SEND_TOKEN;


/* Give a command string to RF module
	The string can be freed after return because it is in TXFIFO.
	NOTE: No error handling, because we can do not much if sending fails.
*/	
void
send_cmd(char *cmd_str){
	if (send_token > 0){
		RF_send((unsigned char *) cmd_str, str_len(cmd_str));
		send_token--;
	} else debug_out("THROTTLED! ",00);
};


static
PT_THREAD (produce_send_token(struct pt *pt)) {
	static struct timer tmr;
	
	PT_BEGIN(pt);
	timer_add(&tmr, SEND_BANDWIDTH, SEND_BANDWIDTH);	
	while (1){	
		PT_WAIT_UNTIL(pt, timer_expired(&tmr));
		tmr.expired = 0;
		if (send_token < MAX_SEND_TOKEN)
			send_token++;
	};	
	PT_END(pt);
};

/* --------------------------------- Ring buffer for radio reception -------------------------------------------------- */

//	Here the rfRcvPacket routine stores its data until the application reads them.
//	First in, first out buffer.
 
/* Number of data bytes in buffer: */
#define RX_BUF_LIM 1024
/* Maximum index into rx buffer */
#define RX_BUF_MAX (RX_BUF_LIM - 1)

static uint8_t rx_buf[RX_BUF_LIM];
static int rx_buf_first;		// first byte in buffer
static int rx_buf_free;			// next free byte in buffer (buffer is empty if first == free)

int
rx_buf_empty(){
	return (rx_buf_first == rx_buf_free);
};

uint8_t
get_from_rx_buf(){
	uint8_t val = rx_buf[rx_buf_first++];
	if (rx_buf_first > RX_BUF_MAX)
		   rx_buf_first = 0;
	return val;
};

// Returns 0 iff buffer is full.
static int 
put_in_buf(uint8_t c){
	int new_free = rx_buf_free+1;
	if (new_free > RX_BUF_MAX)
		new_free=0;
	if (new_free == rx_buf_first)
		return 0;				// Buffer is full !
	rx_buf[rx_buf_free]=c;
	rx_buf_free = new_free;
	return 1;
};

static void
init_rx_buf(){
	rx_buf_first = 0;
	rx_buf_free = 0;
};

/* ------------------------ Interrupt handling for reception over radio ----------------------------- */

/* The length of the last received packet, stored in IRQ routine */
static volatile uint8 cc1100_rx_len;

// Sets signal SIG_RX_PACKET if a complete packet has been received.
// and stores length information in cc1100_rx_len

// TODO maybe change handling elsewhere so that we can trigger this interrupt while still receiving
// TODO reading length byte in an IRQ is time consuming. We can do better than that
void rxIRQ(){
	EXTINT = EINT0; 		// Clear interrupt source
	
	// We have received a packet. Read its length and signal to application to read the rest */

	// We read the length byte. Works while still receiving, but only if more than 1 byte is in FIFO
	// We can guarantee that, because we use Address Checking, so at least the address must be in FIFO also
	// Reading of the fifo automatically deasserts the GDP0 line 
	cc1100_read_fifo( (uint8 *) &cc1100_rx_len, 1);
	
	signal_set(SIG_RX_PACKET);
};


/* Initialize and enable ISR for CC1100 */
void 
startcc1100IRQ(void) {
	VICIntEnClr = INT_EINT0;	/* Disable EINT0 (just to make sure) */
	
	/* Reset P0.16 to default GPIO values (just to make sure) */
	PINSEL1 &= ~((1<<0) | (1<<1));
	PINSEL1 |= 1;			// Select EINT0 function for P0.16
	
	EXTWAKE |= EINT0;		/* EINT0 wakes processor from power down TODO needed ? */
//	EXTMODE &= ~EINT0;		/* EINT0 is level-sensitive */
	EXTMODE |= EINT0;		/* EINT0 is edge-sensitive */
//	EXTPOLAR &= ~EINT0;		/* EINT0 is falling edge /low level sensitive */
	EXTPOLAR |= EINT0;		/* EINT0 is rising edge / high level active */
	
	EXTINT = EINT0;			/* Clear any pending EINT0 */
	
	VICIntSelect &= ~INT_EINT0;		
	VICVectAddr2 = (unsigned long)&(rxIRQ);
	VICVectCntl2 = VIC_SLOT_EN | INT_SRC_EINT0;
	VICIntEnable = INT_EINT0;
}


/* NOTE This signal strength indicator is set by rfRcvPacket, but it is not currently used */
static int rssi_dbm;
#define RSSI_OFFSET 75


/* ---------------------------Bottom half for reception---------------------------------------------- */
//
//  DESCRIPTION:
//	This routine is called when SIG_RX_PACKET is set, i.e. when a complete packet has been received.
//	It has to get the bytes very fast off from the RX_FIFO so that further reception can go on.
//	It puts the bytes into a temporary buffer and resumes reception.
//	Then the bytes are copied into a larger ring buffer, so that the temporary buffer is again free.
//	We assume that the length of the packet has been read into cc1100_rx_length by the ISR.	
//	After doing validity checks reads the packet from cc1100 RX_FIFO and stores it in data array. 
//	Clears SIG_RX_PACKET.
//	Sets SIG_NEW_PACKET if a valid packet has been copied.
//	TODO Flush RX_FIFO or not ?

void 
rfRcvPacket() { 
	uint8_t	tmp_buf[65];
	int length;
	uint8_t addr;
	uint8_t linkstatus[2];
	int i;
	
	signal_clr(SIG_RX_PACKET);

	// We read the address byte. No need to check this, should have been done in hardware 
	cc1100_read_fifo(&addr, 1);
	
	// Check for some obvious erros 
	if ( (cc1100_rx_len == 0) || (cc1100_rx_len > MAX_PKTLEN) || (addr != DEVICE_ADDRESS))	{ 
		// Something went wrong. Discard complete RX FIFO
		debug_out("rcvd invalid packet", 0);
		switch_to_idle();
		cc1100_strobe(SFRX);
		cc1100_strobe(SRX);
		return;
	};

	// Get the remaining packet from the FIFO
	
	// Reading complete RX FIFO is only safe if not in RX
	switch_to_idle();				

	cc1100_read_fifo(tmp_buf, cc1100_rx_len-1);
	
	/* Copy the length information. This will be overwritten first by ISR. */
	length = cc1100_rx_len-1;
	
    // Get the appended status bytes [RSSI, LQI]
	cc1100_read_fifo(linkstatus, 2);

#if 1
    // Check CRC	TODO not necessary, has been done in hardware
	if ((linkstatus[1] & LQI_CRC_OK_BM) != LQI_CRC_OK_BM){
		cc1100_strobe(SFRX);
		cc1100_strobe(SRX);
		return;
	}	
#endif	
	
	/* Now reception can go on. */
	cc1100_strobe(SFRX);
	cc1100_strobe(SRX);
	
	rssi_dbm =  (( (signed char)linkstatus[0]) >> 1) - RSSI_OFFSET;
	
	for (i=0; i<length; i++){
		if (tmp_buf[i] == EOT){
			break;						// EOT signals END OF TRANSMISSION. Rest of packet is padding and can be ignored.
		};
		if (!put_in_buf(tmp_buf[i])){
			debug_out("rx buffer overrun", 0);
			break;						// buffer is full, forget the rest of the data packet
		};
	};	
	return;
}


/* --------------------------- Initialization for both sending and receiving --------------------------------------------- */

//    Set up chip to operate in RX mode
static void 
rxInit(void) {
    // Set GDO0 to assert when packet with correct CRC has been received, deasserts when first byte of FIFO is read 
	cc1100_write_reg(IOCFG0, 0x07);
	
	// Set MCSM1 so that RXOFF mode is RX and TX_OFF mode is RX, CCA mode = 3 (TX only if Channel Clear)
	cc1100_write_reg(MCSM1,0x0F);
	
	// RX timeout is until end of packet
	cc1100_write_reg(MCSM2, 0x07);
	
	// Reset state and set radio in RX mode
	// Safe to set states, as radio is IDLE
	switch_to_idle();
	cc1100_strobe(SFRX);	
	cc1100_strobe(SCAL);		// TODO done automatically - but seems to be absolutely necessary !
	// TODO probably not necessary, the SRX strobe waits until CC1100 ready
	while ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) != 0x01) {}; 
	cc1100_strobe(SRX);
}


// Init all
void 
RF_init (void) {
	cc1100_init();

	task_add(&produce_send_token);
	init_rx_buf();
	rxInit();
	startcc1100IRQ();
}

