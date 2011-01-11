/* Tool to receive mpd commands over serial line and to send back responses */

/*
	Application Layer:
	We do have a half-duplex communication between mpd and its clients.
	All communications are initiated by the clients.
	They send commands to mpd, to which it responds with answers (and actions).
	
	Commands are either single line commands (which end with '\n' )
	or multiple line messages which start with "command_list_begin" and end with "command_list_end\n".
	
	
	MPD signals the end of its (possibly multiple line) answer with a line beginning with either "OK" or "ACK".
	
	
	Link Layer:
	To use the half duplex communication, we need a way to switch between sending and receiving.
	We will use the EOT (End of Transmission) character (0x04). 
	Each party sends this character as the last character when it has finished sending and is ready to receive.

	This program (mpdtool) knows when mpd has finished sending by interpreting the answer. A line starting with "OK"
	or "ACK" signals the last line from mpd's answer. mpdtool then adds an EOT when sending to serial line. 
	
	The betty firmware knows when it has finished sending a command and then sends the EOT character.
	 
	We further need to signal this program (mpdtool) when the serial link is ready to receive the next bunch of characters.
	This program is supposed to run on fast hardware while the serial link hardware might be slow.
	So after sending <MAX_TX> characters, this program sends an EndOfText <ETX> and only starts sending the next bunch when 
	it receives an ACKNOWLEDGE <ACK> from serial line.
	This might not always work. For some as yet unknown reason the serial line firmware does not get the <ETX> or does not send
	the <ACK>. So if we receive other characters while we are waiting for an <ACK> we are assuming an error and stop waiting.
	
	Transport Layer:
	This program receives commands via serial line. It sends them via a TCP/IP socket to mpd.
	The answers are received via the same socket and transferred back over serial line.
	All commands and answers are non-binary characters (ISO-8859-15 I guess).
	
	We must filter some characters: <ETX> <ACK> and <EOT> are not transmitted in either direction.
	If they occur in the input stream, they are simply dropped.

*/

#define VERSION_MAJOR 1
#define VERSION_MINOR 3

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <sys/time.h>
#include <time.h>

// NOTE we need the GNU version of basename() !
#define __USE_GNU
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


// All our buffers can hold 1024 characters (+ trailing 0)
#define BUFFER_SIZE 1024

#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

#define ETX	0x03
#define ACK	0x06
#define EOT	0x04
#define	CR	0x0d

#define ENQ	0x05
#define SO	0x0e
#define SI	0x0f
#define DLE	0x10

#define NAK	0x15
#define CAN	0x18

#define LF	0x0a
#define ESC	0x1b



int min(int x, int y){
	if (x <= y)
		return x;
	return y;
};

int max(int x, int y){
	if (x >= y)
		return x;
	return y;
};

/* Initialize a timer variable with the current time */
void init_timer(double *tmr){
	struct timeval start_time;
	gettimeofday(&start_time, NULL);
	*tmr = (double) start_time.tv_sec +  ((double) start_time.tv_usec) / 1000000;
};

/* time difference since last call of init_timer(&tmr) in seconds */
double timer_diff(double tmr){
	struct timeval t1;
	
	gettimeofday(&t1, NULL);
	return ((double) t1.tv_sec) + ((double) t1.tv_usec) / 1000000 - tmr;
};

/* Print time difference since init of tmr to stderr */
void prt_timer(double tmr){
	fprintf (stderr,"[%.1lf ] ", timer_diff(tmr) );
};

/* General routine to send a buffer with bytes_to_send bytes to the given file descriptor.
	Handles short writes, so that either all bytes are successfully written
	or an error message is printed.
	Returns TRUE iff successful, else 0.
*/
int 
write_all(int fd, char *buffer, int bytes_to_send){
	int bytes_written;
	int num = 0;
	
	for (bytes_written = 0; bytes_written < bytes_to_send; bytes_written += num)
	{
		/* Write all remaining bytes to fd at once (if possible). */
		num = write(fd, (void *)(buffer + bytes_written), (bytes_to_send - bytes_written) );
		
		/* Did our write fail completely ? */
		if (num == -1) {
			perror("write_all()");
			return 0;
		};
		/* Now num is the number of bytes really written. Can be shorter than expected */
	};
	return 1;
};

/*--------------------------- Communication over serial line ---------------- */
// file descriptor of our serial line
int serial_fd;

/* 
	Global buffers and flags and other variables
*/

char ser_in_buf[BUFFER_SIZE + 1];
int ser_in_len;
int cmd_complete;

char ser_out_buf[BUFFER_SIZE + 1];
int ser_out_wrt_idx;
int ser_out_rd_idx;
int wait_ack;


/* resets the serial input buffer */
void
reset_ser_in(){
	ser_in_len = 0;
	cmd_complete = 0;
};

/* resets the serial output buffer */
void
reset_ser_out(){
	ser_out_rd_idx = 0;
	ser_out_wrt_idx = 0;
};

/* A bit tricky. 
	We will check that there is room for the character.
	If the buffer is full, all characters will be ignored,
	exceot the EOF character, which is important for the protocol.
*/
void
ser_out_char(char c){
	if (ser_out_wrt_idx < (BUFFER_SIZE - 1) )
		ser_out_buf[ser_out_wrt_idx++] = c;
	else if (c == EOT)
		ser_out_buf[ser_out_wrt_idx + 1] = c;
}

void
serial_output (char *buf){	
	while (*buf) 
		ser_out_char( *(buf++) );
}

/* 
	Initialize the serial line 
*/
void
init_serial(int serial_fd, int baudrate, int time_out){
	struct termios newtio;


	bzero(&newtio, sizeof(newtio));
	/* 
		BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
		CRTSCTS : output hardware flow control (only used if the cable has
			all necessary lines. See sect. 7 of Serial-HOWTO)
		CS8     : 8n1 (8bit,no parity,1 stopbit)
		CLOCAL  : local connection, no modem contol
		CREAD   : enable receiving characters
	*/
	newtio.c_cflag = baudrate | CS8 | CLOCAL | CREAD;

	/*
	IGNPAR  : ignore bytes with parity errors
	otherwise make device raw (no other input processing)
	*/
	newtio.c_iflag = IGNPAR;

	newtio.c_oflag = 0;			// Raw output.
			
	/* set input mode (non-canonical, no echo,...) 
	disable all echo functionality, and don't send signals to calling program
	*/
	newtio.c_lflag = 0;
 
	newtio.c_cc[VTIME]    = time_out;	/* time out after time_out tenth seconds */
	newtio.c_cc[VMIN]     = 0;		/* non-blocking read */
	
	/* now clean the modem line and activate the settings for the port */
	tcflush(serial_fd, TCIFLUSH);
	tcsetattr(serial_fd,TCSANOW,&newtio);
};

/* Read a boot loader response from serial line 
	A response is a single line, terminated by 0x0a
	Returns number of bytes read, if successful
	Returns 0, if  line is longer than BUFFER_SIZE-1
*/
int read_boot_response (int fd, char *buffer){
	int res;		 
	int stop = 0;
	int bytes_read = 0;
	
	do {
		if (bytes_read >= (BUFFER_SIZE - 1)) {
			fprintf(stderr, "Error, too many characters from serial line!\n");
			bytes_read = 0;		// Forget all characters received so far
		};
		
		/* Read at most one character and time out if none received */
		res = read(fd, buffer+bytes_read, 1);
		
		if (res < 0) continue;
		if (res == 0) {
//			fprintf(stderr, "Time out on serial line\n");
			stop = 1;			// Device has finished sending
		} else {
			stop = (buffer[bytes_read] == 0x0a);
//			printf("Received: %02x\n", (unsigned char) buffer[bytes_read]);
			bytes_read += res;
		};
	} while (!stop);
	
	buffer[bytes_read]=0;             /* set end of string, so we have a real string */

	return (bytes_read);
};


/* Send a data record in intel hex format to device 
	Check that it was received correctly.
*/
int send_string(int fd, char *s, char *answer){
	char buffer[BUFFER_SIZE];
	char buffer2[BUFFER_SIZE];
	int cmd_len = strlen(s);
	int i, len;
	
	/* Make sure that all characters are upper case ! */
	for (i=0; i<cmd_len; i++)
		buffer2[i] = toupper(s[i]);
	buffer2[cmd_len] = 0;
	
	write_all(fd, buffer2, cmd_len);
	len = read_boot_response(fd, buffer);
	buffer[len] = 0;
	
	/* Check that our command was echoed correctly */
	if (0 != strncmp(buffer, buffer2, cmd_len)) {	
		printf("Error: Invalid echo!\n");
		printf("%s\n", buffer);
//		return(-20);	
	};
	
	/* buffer[len-1] is 0x0a, buffer[len-2] is 0x0d, buffer[len-3] is '.'  */
	/* The last character should be a '.' */
	if (0 != strncmp(buffer+len-3, ".", 1)) {	
		printf("Error: No . response!\n");
		return (-20);	
	};
	
	/* Print the response characters (if any) */
	for (i=cmd_len; i < (len-3); i++)
		*(answer++) = buffer[i];
	*answer = 0;
	
	return 0;

};

/* 
	Reboot the scart adapter. Just in case everything went wrong.
*/
void reboot_scart(int serial_fd){
	char *s;
	char buffer[BUFFER_SIZE];
	int i, len;
	struct termios oldtio;
	
	tcgetattr(serial_fd,&oldtio); 			/* save current port settings */
	init_serial(serial_fd, B19200, 50);
//	init_serial(serial_fd, B9600, 20);
		
	printf("Sending break\n");
	/* Send a break to reset device into ISP mode */
	tcsendbreak(serial_fd,5);
	tcdrain(serial_fd);

	tcflush(serial_fd, TCIOFLUSH);
	
	usleep(1000000);
	tcflush(serial_fd, TCIOFLUSH);
	usleep(1000000);
	
	/* Send an uppercase U to negotiate baud rate */
	buffer[0] = 'U';
	
	buffer[1] = 0;
	
	/* Send U to serial line */
	if (write_all(serial_fd, buffer, 1) == -1) {
		perror("sendall");
	};		
	
	len = read_boot_response(serial_fd, buffer);
	buffer[len] = 0;
	printf("Read %d characters:\n\t", len);
	for (i=0; i<len; i++)
		printf("%02x ", buffer[i]);
	printf("\n");
	
	tcflush(serial_fd, TCIFLUSH);
			
	/* Send U to serial line to check that we are in sync */
	buffer[0] = 'U';
	if (write_all(serial_fd, buffer, 1) == -1) {
		perror("sendall");
	};		
	
	len = read_boot_response(serial_fd, buffer);
	printf("Read %d characters:\n\t", len);
	for (i=0; i<len; i++)
		printf("%02x ", buffer[i]);
	printf("\n");
	
	buffer[len] = 0;

	if (buffer[0] == 'U')
		printf("Baud rate successfully negotiated\n");
	else {
		printf("buffer[0] = %02x\n", buffer[0] );
		printf("Error: Could not negotiate baud rate!\n");
		return;
	};
	
	s=":0100000310EC";
	printf("Manuf. ID \t= ");
	if (0 != send_string(serial_fd,s,buffer) ) {
		return;
	};
	printf("%s\n",buffer);
	if (0 != strncmp(buffer, "15", 2)) {
		fprintf(stderr, "ERROR: Device not recognized\n");
		return;
	};
		
	/* Reset the device */
	printf("\nResetting device\n");
	s=":00000008F8";
	write_all(serial_fd, s, strlen(s));
	read(serial_fd, buffer, 13);		/* Read back the echo and forget it */
	tcflush(serial_fd, TCIOFLUSH);	
	
	/* Restore old serial port settings */
	tcsetattr(serial_fd,TCSANOW,&oldtio);
	
	tcflush(serial_fd, TCIOFLUSH);	
};

/*
	Send at most MAX_TX bytes to serial
	Given is ser_out_buf with ser_out_len bytes in it.

	Waits for ACK if MAX_TX bytes have been written.
	Returns when waiting or buffer empty.
	Waiting means setting the global flag wait_ack to 1.
	The serial input routine will reset that flag when it sees an ACK
*/

#define MAX_TX 16
static int cur_tx_len;

void 
send_to_serial(int serial_fd){
	int bytes_to_send, bytes_written;
	int res;
	int num = 0;
	char ETX_char = ETX;

	static int tx_cnt = 0;
	
	/* Are we still waiting for an ACK? */
	if (wait_ack) return;
	
	bytes_to_send = min (ser_out_wrt_idx - ser_out_rd_idx, MAX_TX - tx_cnt);
	
	for (bytes_written = 0; bytes_written < bytes_to_send; bytes_written += num){

		/* Write a certain number of bytes to serial_fd. We either send the remaining bytes in buffer
			or the remaining bytes to fill a packet, whichever is shorter.
		*/
		num = write(serial_fd, (void *)(ser_out_buf + ser_out_rd_idx), bytes_to_send);
		
		/* Did our write fail completely ? */
		if (num == -1) {
			perror("send_to_serial()");
			return;
		};
		
		/* When num == 0 we might enter an infinite loop */
		if (num == 0)
			return;
		
		/* Now num is the number of bytes really written. Can be shorter than expected */
		ser_out_rd_idx += num;
		tx_cnt += num;
	};

	if (tx_cnt >= MAX_TX){
		res = write(serial_fd, &ETX_char, 1);
		if (res == -1) {
			perror("send_to_serial()");
			printf("We could not write ETX!\n");
		} else {
			wait_ack = 1;
		};
		tx_cnt = 0;
	};
	
	return;
};


/* 
	Read some bytes from serial line 
	Sets global flag cmd_finished if EOT is seen.
	Then returns.

	We utilize the fact that Betty sends an EOT when a command is finished.
	Our buffer must be long enough to read multiple lines.
	We can safely assume that commands are below 256 characters (limitation of scart hardware).
	The EOT character will not be included in the returned buffer.
	The buffer is null terminated so that it is a valid C string.
	Sets flag ack_received if a ACK character was received.
	Does not store ACK character.
	
	If scart sends a CANCEL character, the buffer is cleared, The command was invalid.
*/
void 
read_from_serial (int fd){
	int res;		 
	
	res = read(fd, ser_in_buf+ser_in_len, 1);
	if (res == 0){ 
		fprintf(stderr,"empty ser_in \n");
		return;
	}
	
	if (res < 0){
		printf("Error on read from serial line, errno = %d\n", errno);
		return;
	};	

	switch (ser_in_buf[ser_in_len]) {
		case EOT:
			ser_in_buf[ser_in_len]='\0';			// Null terminate string 
			cmd_complete = 1;					// Set flag
			return;
			
		case CAN:
			fprintf(stderr, "Command cancelled\n");	
			reset_ser_in();
			return;
			
		case ACK:
			wait_ack = 0;
			return;
			 
		default:
			if (ser_in_len < BUFFER_SIZE - 1) 
				ser_in_len++;
			else
				fprintf(stderr, "Error, too many characters from serial line!\n");	
	};	
	return;
};
	
/* Returns TRUE iff the scart adapter is connected and 
	responding correctly to our ETX characters
*/
int
scart_alive(){
	int res;
	char ETX_char = ETX;
	char ENQ_char = ENQ;
	
	fprintf(stderr,"Checking scart adapter\n");
	
	tcflush(serial_fd, TCIOFLUSH);
	reset_ser_in();
	reset_ser_out();		
	res = write(serial_fd, &ETX_char, 1);
	if (res == -1) {
		perror("check_scart_alive()");
		printf("We could not write ETX!\n");
	}
	res = read(serial_fd, ser_in_buf+ser_in_len, 1);
	if (res == 0){ 
		fprintf(stderr,"scart_alive() -> no answer \n");
		return 0;
	}
	
	if (res < 0){
		printf("scart_alive() -> Error on read from serial line, errno = %d\n", errno);
		return 0;
	};	
	if (ser_in_buf[ser_in_len] != ACK){
		fprintf(stderr, "scart_alive() got <%02x> from scart, expected 0x06\n", ser_in_buf[ser_in_len]);
		return 0;
	};
	
	
	res = write(serial_fd, &ENQ_char, 1);
	if (res == -1) {
		perror("check_scart_alive()");
		printf("We could not write ENQ!\n");
	}
	res = read(serial_fd, ser_in_buf+ser_in_len, 4);
	if (res == 0){ 
		fprintf(stderr,"scart_alive() -> no answer \n");
		return 0;
	}
	
	if (res < 0){
		printf("scart_alive() -> Error on read from serial line, errno = %d\n", errno);
		return 0;
	};
	
	if (ser_in_buf[ser_in_len] != 'V'){
		fprintf(stderr, "Scart did not answer with firmware version!n");
		return 0;
	};

	fprintf(stderr, "Scart adapter firmware %c%c%c%c\n", 
			ser_in_buf[ser_in_len],
			ser_in_buf[ser_in_len+1],
			ser_in_buf[ser_in_len+2],
		   	ser_in_buf[ser_in_len+3]
		   );

	return 1;
};
	

/* ------------------- Communication with MPD --------------- */
char mpd_resp_buf[BUFFER_SIZE + 1];
int mpd_resp_len;
int response_line_complete;
int mpd_socket;
struct sockaddr_in serverName = { 0 };
double response_tmr;
int response_finished;			// TODO here ?

// Reset the line buffer for input from mpd
void
reset_mpd_line(){	
	mpd_resp_len = 0;
	response_line_complete = 0;
};

/* 
	Read a byte from mpd socket into mpd_resp_buf 
	Returns 0 if end-of-file was reached
	Returns < 0 if error occured
	Return 1 if byte was read
	Sets response_line_complete flag if a '\n' was detected !
*/
int 
read_from_mpd (int mpd_fd){
	int res;		 
		
	res = read(mpd_fd, mpd_resp_buf+mpd_resp_len, 1);
	if (res == 0)
		return res;
	
	if (res < 0){
		printf("Error on read from mpd, errno = %d\n", errno);
		return res;
	};	

	switch (mpd_resp_buf[mpd_resp_len]) {
		case '\n':
			if (mpd_resp_len < BUFFER_SIZE - 2) 
				mpd_resp_len++;
			mpd_resp_buf[mpd_resp_len]=0;			// Null terminate string 
			response_line_complete = 1;				// Set flag
			break;
			
		default:
			if (mpd_resp_len < BUFFER_SIZE - 2) 
				mpd_resp_len++;
			else {
				fprintf(stderr, "Error, too many characters from mpd!\n");
			}
	};
	return 1;
};

/* Sets up sockaddr_in structure for MPD connection */
void
init_mpd(char *remote_host, int remote_port){
	struct hostent *host_ptr = NULL;
	
	/*
	 * need to resolve the remote server name or
	 * IP address 
	*/
	host_ptr = gethostbyname(remote_host);
	if (NULL == host_ptr) {
		host_ptr = gethostbyaddr(remote_host,
				 strlen(remote_host), AF_INET);
		if (NULL == host_ptr) {
			perror("Error resolving server address");
			exit(1);
	 	}
	}

	serverName.sin_family = AF_INET;
	serverName.sin_port = htons(remote_port);
	(void) memcpy(&serverName.sin_addr, host_ptr->h_addr, host_ptr->h_length);
	mpd_socket = -1;
};

/*
	Close the connection to MPD 
*/
void
close_mpd_socket(){
	if (mpd_socket != -1) 
		close(mpd_socket);
	mpd_socket = -1;
};

/* Opens a new socket to MPD if the old one was closed 
	Returns 1 if already connected to MPD, 2 if a new conncetion was established 
	and 0 if unsuccessful.
*/
int
open_mpd_socket(){
	if (-1 != mpd_socket)
		return 1;
		
	mpd_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (-1 == mpd_socket)	{
		perror("socket()");
		return 0;
	};
	return 2;
}

// the same as write_all(), but computes length of string itself
int
write_mpd(char * s){
	return (write_all(mpd_socket, s, strlen(s) ) );	
};

/* This routine returns TRUE, iff the given line is the last line in
	an mpd response, i.e. it starts with "OK" or "ACK"
*/
int
mpd_eot(char *line){
	return ( (0 == strncmp(line, "OK", 2)) || (0 == strncmp(line, "ACK", 3)) );
};


/* Convert a string to ISO 8859-15 
	Attention: Length of the string may change !
*/
void
utf8_to_iso8859_15(unsigned char *s){
	unsigned char *rd = s;
	unsigned char *wr = s;
	unsigned char b0, b1;
	unsigned int o1;
	
	while ( (b0 = *rd++) != '\0'){
		if (b0 & 0x80) {
			b1 = *rd;			// we may read next character here, because we have not hit '\0'.
			if ( ((b0 & 0xE0) != 0xC0) || ((b1 & 0xC0) != 0x80) ){
				/* error: not in the range of ISO 8859-15 */
				*wr++ = '?';
				continue;
			};
			/* We now know b1 is not 0. */
			rd++;
			
			o1 = ((b0 & 0x1f) << 6 ) | (b1 & 0x3f);
			if (o1 > 256) o1 = '?';
			*wr++ = o1;
		} else 
			*wr++ = b0;
	};
	*wr = '\0';
};

/* Convert a string to UTF8 	Attention: Length of the string may change !
*/
void
iso8859_15_to_utf8(unsigned char *s_new, unsigned char *s_old, int max_len){
	unsigned char c, b0, b1;
	int len = 0;
	
	while ((c=*s_old++)){
		if (c <= 0x7f){
			if (len < max_len){
				*s_new++ = c;
				len++;
			};
		} else {
			b0 = 0xC0;
			b1 = 0x80;
			b0 |= ((c & 0xC0) >> 6); 
			b1 |= (c & 0x3F);
			if (len < (max_len-1) ){
				*s_new++ = b0;
				*s_new++ = b1;
				len+=2;
			};
		};
	};
	*s_new=0;
};
	
/* 
	Wait for input, either from serial line or from MPD socket
	Uses select so does sleep when nothing to do
*/
int
wait_for_input(int serialfd, int socketfd, int milliseconds){
	struct timeval tv;
	fd_set readfds;
	int numfds;
	int res;
	int cnt = 0;
	
	/* We try until we have a byte or a time-out or an error */
	while (cnt == 0) {
		tv.tv_sec = milliseconds / 1000;
		tv.tv_usec = (milliseconds % 1000) * 1000;

		FD_ZERO(&readfds);
		if (serialfd != -1)
			FD_SET(serialfd, &readfds);
		if (socketfd != -1)
			FD_SET(socketfd, &readfds);

		numfds = max (serialfd + 1, socketfd + 1);
	
		// don't care about writefds and exceptfds:
again2:		
		res = select(numfds, &readfds, NULL, NULL, &tv);
	
		if (res == -1){
			if (errno == EINTR)
				goto again2;     /* just an interrupted system call */
			perror("select()");
			return 0;
		};
	
		if (res == 0)
			return 0;				// time out
	
		if ( (serialfd != -1) && FD_ISSET(serialfd, &readfds) ){
//			fprintf(stderr,"Reading from serial\n");
        	read_from_serial(serialfd);
			cnt = 1;
		}
	
		if ( (socketfd != -1) && FD_ISSET(socketfd, &readfds) ){
//			fprintf(stderr,"Reading from socket\n");
again:
			res = read_from_mpd(socketfd);
			if (res < 0) {
				if (errno == EINTR)
					goto again;     /* just an interrupted system call */
				return 0;   		/* handle other errors */
        	};
			if (res == 0)						// EOF but select() will return 1 anyway
				socketfd = -1;					// ignore socket 
			if (res > 0)
				return 1;
		}
	}
    return 1;
} 


/* 
	Creates a new connection to MPD if there was no previous one.
	Connects to MPD.
	If a successful NEW connection was established, mpd_resp_buf contains the initial answer from MPD
	Returns 0 if connection could not be made.
	If serial_fd is <> -1, a complete command from serial line aborts this routine
	and no connection to MPD is made.
*/
int
open_mpd_connection(int serial_fd){
	int res;

	res = open_mpd_socket();
	if (0 == res)
		return 0;
	
 	if (1 == res) 
		return 1;
	
	res = connect(mpd_socket, (struct sockaddr*) &serverName, sizeof(serverName));
	if (-1 == res){
		perror("connect() to mpd failed");
		fprintf(stderr,"Please check that MPD is running and is accepting connections from another computer!\n");
		fprintf(stderr,"Maybe restarting MPD helps.\n");
		close_mpd_socket();
		return 0;
	};	
	
	// The mpd server responds to a new connection with a version line beginning with "OK"
	reset_mpd_line();

	while (! response_line_complete){
		// Wait 1 second for next MPD response character(s) or maybe another command from serial
		res = wait_for_input(serial_fd, mpd_socket, 1000);

		// Maybe we were too slow and Betty sent another command
		if (cmd_complete){
			fprintf(stderr, "  Betty sent new command.\n");
			close_mpd_socket();
			return 0;
		};
		
		if (res == 0){
			fprintf(stderr, "  No answer when connecting to MPD.\n");
			close_mpd_socket();
			return 0;
		};	
	};	
	
	if (strncmp(mpd_resp_buf, "OK", 2) != 0) {
		fprintf(stderr,"  Bad initial response from mpd: %s\n", mpd_resp_buf);	
		close_mpd_socket();
		return 0;
	};	
	fprintf(stderr," MPD: %s\n", mpd_resp_buf);
	return 1;
};


/* 
	These commands are important and can be emulated if not present
	or they are simply ignored (and an appropriate answer returned to Betty)
	Some commands are always present, but we need to change their answers 
	for Betty to be useful.
	We have a bit for those commands too.
*/
#define LISTPLAYLISTS_CMD	(1 << 0)
#define PLAYLISTNAME_CMD (1 << 1)
#define PLAYLISTCOUNT_CMD (1 << 2)
#define SCRIPT_CMD (1<<3)
#define SEARCH_CMD (1<<4)
#define RESULT_CMD (1<<5)
#define FINDADD_CMD (1<<6)
#define PLAYLISTINFO_CMD (1<<7)
#define CURSONG_CMD (1<<8)

/* A bit set to 1 means this command is available.
	No need to emulate it.
*/
int mpd_cmd_avail;

/* 
	Here we set a bit if a response has to be modified before it is sent to Betty.
	The bits are the same as in mpd_cmds.
*/
int mpd_emu;

/* Some emulated cmds need to remember an argument.
	Store it here.
*/
int mpd_emu_arg;

/* Some emulated cmds need to count the results.
	Use this.
*/
int mpd_emu_cnt;

/* Send a command to MPD and prepare for the answers.
	Starts response_tmr and resets (clears) mpd_response_line
*/ 
int
mpd_start_cmd(char *cmd_str){
	if ( 0 == open_mpd_connection(-1) ){
		return 0;
	};
	reset_mpd_line();
	
	init_timer(&response_tmr);	
//	fprintf(stderr,"start_cmd: %s\n", cmd_str);
	return write_mpd(cmd_str);	
};


/* Send a command to MPD
	Ignore Betty while this is going on.
	We use this routine to emulate some commands needed by Betty.
	We use a generous timeout of 5 seconds.
	
	The answer lines (except "OK" and "ACK") are processed 
	by ans_func() if not NULL.
*/
int
mpd_cmd(char *cmd_str, void (*ans_func)(char *) ){
	int response_finished = 0;
		
	if (0 == mpd_start_cmd(cmd_str))
		return 0;

	while (!response_finished) {
		wait_for_input(-1, mpd_socket, 0);
		if (response_line_complete){
//			fprintf(stderr, "(MPD): %s", mpd_resp_buf);
				
			// check for "OK" or "ACK"
			if ( mpd_eot(mpd_resp_buf) ) {
				response_finished = 1;
				fprintf(stderr, "(MPD): %s", mpd_resp_buf);

			} else {
				if (ans_func) 
					ans_func(mpd_resp_buf) ;
			};
			reset_mpd_line();
		};
		
		if (timer_diff(response_tmr) > 5.0){
			fprintf(stderr,"MPD response is too late\n");
			close_mpd_socket();
			return 0;
		};
	}
	return 1;
};

void
prt_ans(char *s){
	fprintf(stderr, "ANS: %s", s);
};

void
prt_file(char *s){
	if (0 != strncmp(s, "file: ", 6))
		return; 
	fprintf(stderr, "ANS: %s", s+6);
};

void
cnt_playlists(char *s){
	if (0 != strncmp(s, "playlist: ", 10))
		return; 	
//	fprintf(stderr, "ANS: %s", s+10);
	mpd_emu_cnt++;
};

/* We save all the filenames that we get from MPD
	(up to 500)
*/
#define FNAME_MAX	499
#define FNAME_LEN	399
char fname[FNAME_MAX+1][FNAME_LEN+1];			// 200 kB
int fname_cnt;

void
line_copy(char *new, char *line){
	while (*line != '\n')
		*(new++) = *(line++);
	*new = 0;
};
	
/* We save the filenames without terminating newline!
*/
void
save_file(char *s){
	if (0 != strncmp(s, "file: ", 6))
		return; 
	if (fname_cnt >= FNAME_MAX)
		return;
	line_copy(fname[fname_cnt++], s+6);

	fprintf(stderr, "saving: %s\n",fname[fname_cnt-1]);
};

/* Not every version of MPD has all the commands that we use
	So we get a list of the commands and remember which are not available
*/
void
check_commands(char *s){
	if (0 != strncmp(s, "command: ", 9))
		return;
//	fprintf(stderr, "ANS: %s", s+9);
	
	if (0 == strncmp(s+9, "listplaylists", strlen("listplaylists")) )	
		mpd_cmd_avail |= LISTPLAYLISTS_CMD;
	
	if (0 == strncmp(s+9, "findadd", strlen("findadd")) )	
		mpd_cmd_avail |= FINDADD_CMD;
};

/* 
Find out version of mpd and which commands it understands
and read all available playlists
*/
void
check_mpd(){
	
	mpd_cmd_avail = 0;

	mpd_cmd("commands\n", check_commands);
	
	if ( 0 == open_mpd_connection(-1) ){
		return;
	};
	
	mpd_emu_cnt = 0;
	if ( mpd_cmd_avail & LISTPLAYLISTS_CMD )
		mpd_cmd("listplaylists\n", cnt_playlists);
	else
		mpd_cmd("lsinfo\n", cnt_playlists);
	
	fprintf(stderr,"MPD: Available Playlists = %d\n\n", mpd_emu_cnt);
	
	mpd_emu = 0;
};

/* Copy the command in ser_in_buf() to local buf() to free ser_in_buf */	
void
copy_serial_in(char *buf){
	strcpy(buf, ser_in_buf);
	reset_ser_in();		// reset the serial input buffer, ready to get next command
};	


#define MAX_NAME_LEN 254
/* Here we keep info about returned results from a search */
typedef struct {
 	char name[MAX_NAME_LEN+1];			// this is the info returned to Betty
} search_result;

// This number has to be the same as in Betty
#define MAX_NUM_RESULTS 50
/* We store up to MAX_NUM_RESULTS answers to our search */
search_result results[MAX_NUM_RESULTS];

/* Number of results in list */
int num_results;

/* We check if s is already in our result list.
	If it is, we return 0.
	If it is not and we still have room to store it, we store it and return 1 
*/
int 
cmp_and_store(char *s){
	int i;
	
	for (i=0; i<num_results; i++){
		if (0 == strcmp(s, results[i].name))	
			return 0;
	};	
	if (num_results >= MAX_NUM_RESULTS)
		return 0;
	strncpy(results[num_results].name, s, MAX_NAME_LEN);
	results[num_results].name[MAX_NAME_LEN] = 0;			// Null terminate
	num_results++;
	return 1;
};



/* Copy the serial input buffer to the given mpd_input_buffer buf.
	Commands which are not available are emulated if possible
	Resets serial input buffer.
*/
void
translate_to_mpd(char *buf){

	/* NOTE the order of the ifs is important */
	
	/* Just to make sure we have a valid C-string */
	buf[BUFFER_SIZE] = 0;
	
// The script commands are not given to mpd, but executed directly	
// MPD sees the "ping" command and returns  "OK"
	
	if (0 == strncmp(buf, "script 1", strlen("script 1")) ){
		strcpy(buf, "ping\n");
		system("./script_1.sh");
	};
	
	if (0 == strncmp(buf, "script 2", strlen("script 2")) ){
		strcpy(buf, "ping\n");
		system("./script_2.sh");
	};		
	
	/* We have the command "result n" which will return the nth result of our search result cache. */
	if (0 == strncmp(buf, "result ", strlen("result ")) ){
		mpd_emu_arg = atoi(buf+7);
		mpd_emu |= RESULT_CMD;
		strcpy(buf, "ping\n");
	} else
		mpd_emu &= ~RESULT_CMD;
		
	/* The command "playlistinfo" returns too much information.
		We filter only the necessary lines.
	*/
	if (0 == strncmp(buf, "playlistinfo ", strlen("playlistinfo ")) ){
		mpd_emu |= PLAYLISTINFO_CMD;
	} else 
		mpd_emu &= ~PLAYLISTINFO_CMD;

	/* The command "playlistname x" is our own invention.
		It returns the name of playlist number x (x starts with 0).
		We emulate it by sending "listplaylists" and counting the responses. 
	*/
	if (0 == strncmp(buf, "playlistname ", strlen("playlistname ")) ){
		mpd_emu |= PLAYLISTNAME_CMD;
		mpd_emu_arg = atoi(buf + strlen("playlistname "));
		mpd_emu_cnt = 0;
		strcpy(buf, "listplaylists\n");
	} else 
		mpd_emu &= ~PLAYLISTNAME_CMD;
		
	/* The command "playlistcount" is our own invention.
		It returns the number of playlists known to MPD.
		We emulate it by sending "listplaylists" and counting the responses. 
	*/
	if (0 == strncmp(buf, "playlistcount\n", strlen("playlistcount\n")) ){
		strcpy(buf, "listplaylists\n");
		mpd_emu |= PLAYLISTCOUNT_CMD;
		mpd_emu_cnt = 0;
	} else 
		mpd_emu &= ~PLAYLISTCOUNT_CMD;

	/* The listplaylists command is not available in older versions of mpd 
		We substitute "lsinfo" for it
	*/
	if ( (0 == (mpd_cmd_avail & LISTPLAYLISTS_CMD)) && (0 == strncmp(buf, "listplaylists\n", 14)) ){
		strcpy(buf, "lsinfo\n");
		mpd_emu |= LISTPLAYLISTS_CMD;
	} else 
		mpd_emu &= ~LISTPLAYLISTS_CMD;
		
	/* We will filter the answers to the search command because we may get too many */
	if ( 0 == strncmp(buf, "search", 6)) {
		mpd_emu |= SEARCH_CMD; //| SEND_SEARCH_OK;
		mpd_emu_cnt = 0;
		num_results = 0;
	} else {
		mpd_emu &= ~SEARCH_CMD;
//		mpd_emu &= ~SEND_SEARCH_OK;
	};
	
	/* We want to substitute basename(filename) for missing title tag */
	if (0 == strncmp(buf, "currentsong\n", 12)) {
		mpd_emu |= CURSONG_CMD;
	} else 
		mpd_emu &= ~CURSONG_CMD;
	
	/* The findadd command is not available in older versions of mpd */
	if ( (!(mpd_cmd_avail & FINDADD_CMD)) && (0 == strncmp(buf, "findadd ", 8)) ){
		
		/* We already do the main work of the emulation here.
			Later on we only send ping to mpd.
			Betty may have to wait quite long for this answer !
		*/
		char newbuf[400];
		int i;
		
		newbuf[399] = 0;
		iso8859_15_to_utf8(newbuf, buf+8, 399);
		mpd_emu |= FINDADD_CMD;
		sprintf(buf, "find %s\n", newbuf);

		fname_cnt = 0;
		mpd_cmd(buf, save_file);	
	
		for (i=0; i<fname_cnt; i++){
			sprintf(newbuf,"add \"%s\"\n",fname[i]);
			if (!mpd_cmd(newbuf, NULL))
				break;;
		};
		mpd_emu &= ~FINDADD_CMD;
		strcpy(buf, "ping\n");
	} else
		mpd_emu &= ~FINDADD_CMD;
};

/* We have an answer from mpd in mpd_resp_buf.
	Convert it to the character set understood by Betty
	and do any necessary emulation and/or filtering
	normally just sends the translated string to serial_output()
	
	NOTE the main loop depends on an "OK" or "ACK" answer to detect the end of the MPD response.
		So we have to return that when we leave this routine
		When we send a fake "OK"/"ACK" to Betty we must make sure it is not at the start
		of mpd_resp_buf when we leave the routine
*/
void
translate_to_serial(){
	
	// Convert line to iso8859-15			
	utf8_to_iso8859_15( (unsigned char *) mpd_resp_buf);
	
	// If the LISTPLAYLISTS emulation is on, we let only 3 types of output lines go through
	if (mpd_emu & LISTPLAYLISTS_CMD){
		if (! (
			(0 == strncmp(mpd_resp_buf, "playlist:", 9)) ||
			(0 == strncmp(mpd_resp_buf, "OK", 2)) || 
			(0 == strncmp(mpd_resp_buf, "ACK", 3)) ) )
		return;
	}
	
	// If the PLAYLISTINFO emulation is on, we let only 5 types of output lines go through
	if (mpd_emu & PLAYLISTINFO_CMD){
		if (! (
			(0 == strncmp(mpd_resp_buf, "Title: ", 7)) ||
			(0 == strncmp(mpd_resp_buf, "Artist: ", 8)) ||
			(0 == strncmp(mpd_resp_buf, "Pos: ", 5)) ||
			(0 == strncmp(mpd_resp_buf, "OK", 2)) || 
			(0 == strncmp(mpd_resp_buf, "ACK", 3)) ) )
		return;
	}

	// If the PLAYLISTNAME emulation is on, we want one specific playlist name to go through
	// as well as "OK" or "ACK"	
	if (mpd_emu & PLAYLISTNAME_CMD){
		if ( (0 == strncmp(mpd_resp_buf, "playlist:", 9)) ){
			if (mpd_emu_cnt++ != mpd_emu_arg)
				return;
		};
	}

	// If the PLAYLISTCOUNT emulation is on, we count playlist: entries and return the total number
	if (mpd_emu & PLAYLISTCOUNT_CMD){
		if  (0 == strncmp(mpd_resp_buf, "playlist:", 9)) {
			mpd_emu_cnt++;
			return;
		};

		if (0 == strncmp(mpd_resp_buf, "OK", 2)) {
			sprintf(mpd_resp_buf, "playlistcount: %d\n",mpd_emu_cnt);
			serial_output(mpd_resp_buf);
			
			strcpy(mpd_resp_buf, "OK\n");
			serial_output(mpd_resp_buf);
			return;
		};
	};

	if (mpd_emu & SEARCH_CMD){
		if (0 == strncmp(mpd_resp_buf, "OK", 2)) {
			sprintf(mpd_resp_buf, "results: %d\n", num_results);
			serial_output(mpd_resp_buf);

			strcpy(mpd_resp_buf, "OK\n");
			serial_output(mpd_resp_buf);
			return;
		};
				
		if (0 == strncmp(mpd_resp_buf, "ACK", 3)) {
			serial_output(mpd_resp_buf);
			return;
		}; 
		
		mpd_emu_cnt++;
	
		if (0 == strncmp(mpd_resp_buf, "Artist: ", 8)){
			if (num_results < MAX_NUM_RESULTS)	
				cmp_and_store(mpd_resp_buf + 8);
				
			/* MPD sends every single matching file, which can take very long.
				So after MAX_NUM_RESULTS or 1000 lines we cancel the connection to stop mpd.
				We must fake the OK answer.
			*/				
			if ( (num_results == MAX_NUM_RESULTS) || (mpd_emu_cnt > 1000) ){
				if (num_results == MAX_NUM_RESULTS)
					sprintf(mpd_resp_buf, "results: 99\n");
				else 
					sprintf(mpd_resp_buf, "results: %d\n", num_results);

				serial_output(mpd_resp_buf);
				close_mpd_socket();	
				sprintf(mpd_resp_buf, "OK\n");
				serial_output(mpd_resp_buf);
			};	
		};
		return;
	};

	if (mpd_emu & RESULT_CMD) {
		// check if argument is within bounds
		if (mpd_emu_arg < num_results){
			sprintf(mpd_resp_buf, "name: %s\n", results[mpd_emu_arg].name);
			serial_output(mpd_resp_buf);
			strcpy(mpd_resp_buf, "OK\n");
			serial_output(mpd_resp_buf);
		} else {
			sprintf(mpd_resp_buf, "ACK: wrong result index %d\n", mpd_emu_arg);
			serial_output(mpd_resp_buf);
		};
		return;
	};
	
	
	if (mpd_emu & CURSONG_CMD) {
		// We make 2 assumptions for this to work:
		//	1 - The filename is given before the real title (hope MPD does never change that)
		//	2 - Betty can handle 2 lines with "title: " correctly (she can) 
		if (0 == strncmp(mpd_resp_buf,"file: ", 6)){ 
			sprintf(mpd_resp_buf, "Title: %s\n", basename(mpd_resp_buf+6));
			serial_output(mpd_resp_buf);
			return;
		};
	};
		
	// put the line in serial output buffer	
	serial_output(mpd_resp_buf);	
};


/* 
	The MPD protocol is line oriented (terminated by '\n')!
	Normally a single line is a complete command.
	Only when the first line is "command_line_begin\n" does the command end
	when the line "command_line_end\n" is seen.
	
	Our half duplex radio connection adds an extra EOT character at the end of a command.
	We use that here to detect when a command is finished.
	
*/

int main(int argc, char *argv[])
{
	int res;
	char *serial_device;
	struct termios oldtio;
	int time_out_lim = 1, time_out_cnt = 0;
	double total_tmr;
	char mpd_input_buf[BUFFER_SIZE+1];
	
	
	if (4 != argc)
 	{
		fprintf(stderr, "Usage: %s <serial_device> <serverHost> <serverPort>\n", argv[0]);
		exit(1);
	};
	
	/* Initialize total program run time */
	init_timer(&total_tmr);
	
	serial_device = argv[1];
	
/*
	Open serial device for reading and writing and not as controlling tty
	because we don't want to get killed if linenoise sends CTRL-C.
*/
	serial_fd = open(serial_device, O_RDWR | O_NOCTTY ); 
	if (serial_fd <0) {perror(serial_device); exit(-1); }
	
	tcgetattr(serial_fd,&oldtio); 	/* save current port settings */
	init_serial(serial_fd, B38400, 50);
	
	/* Check that the scart adapter is connected and responding */
	if (!scart_alive()){
		fprintf(stderr,"Error. Scart adapter not responding.\n");
		fprintf(stderr,"Is scart adapter connected to %s ?\n", serial_device );
		fprintf(stderr,"Maybe powercycling the scart adapter could help.\n");
		exit(20);
	};	
	
	init_mpd(argv[2], atoi(argv[3]));

	check_mpd();
	
	/*
		This main loop has to be very error tolerant.	
		The idea is to get a command from serial line (terminated by EOT),
		send this command to MPD over TCP socket,
		get the response from MPD (multiple lines), terminated with a line beginning with "OK" or "ACK",
		and send the response to serial line.
		All of these steps may fail:
		No or no complete response from MPD within a short time: 
			Forget it. Send "ACK'EOT'" to serial line to indicate error.
		Could not send command to MPD: 
			Forget it. Send "ACK'EOT'" to serial line to indicate error.
		Serial line could not accept characters fast enough:
			Maybe reset scart adapter ?
		No input from serial line for some time?
			Check if the scart adapter is still alive.
			
		We might not always get synchronous responses, i.e. MPD might send us a belated response,
		while we were waiting for a command from Betty.
			Simply discard those responses.
		Or Betty could send the next command even if we are still waiting for responses from MPD.
			Betty has given up waiting for the answer. So do we.
				
	*/
	
	response_line_complete = 0;

	wait_ack = 0;
	cur_tx_len = 0;
	
	reset_ser_in();
	reset_ser_out();
		
	while (1){	
		
		// if nothing to do, wait for some time (61 secs) for input	
		if (! cmd_complete){
			res = wait_for_input(serial_fd, mpd_socket, 61000);

			// if still no input, check if scart adapter (and Betty) is alive.
			if (res == 0) {
				/* No (more) input for some time. Forget all previous bytes */
				fprintf(stderr,"No command from Betty for some time.\n");

				if (!scart_alive()){				
					if (++time_out_cnt >= time_out_lim){
						reboot_scart(serial_fd);
						time_out_cnt = 0;
						time_out_lim *= 2;
					};
					continue;
				};
			};
		};
		// if we got input from MPD here, something must be wrong.
		if (response_line_complete)
			reset_mpd_line();
		
		// read more bytes until command is complete
		if (!cmd_complete) continue;
		
		prt_timer(total_tmr); fprintf(stderr, "BETTY: %s", ser_in_buf);
		
		/* Free serial input buffer */
		copy_serial_in(mpd_input_buf);
		
		translate_to_mpd (mpd_input_buf);
		
		prt_timer(total_tmr); fprintf(stderr, "Betty: %s", mpd_input_buf);
		
		// got a complete input via serial line
		// send it to MPD, start response_tmr
		// resets mpd_resp_buf to allow fresh input
		res = mpd_start_cmd (mpd_input_buf);

		// reset the serial output buffer
		// All previous bytes are not a response to this command
		reset_ser_out();

		// The response is not finished yet. 
		response_finished = 0;
		
		/* We will break out of this loop if another command from serial is detected */
		while (! response_finished){
			// Poll for MPD response or maybe another command from serial
			res = wait_for_input(serial_fd, mpd_socket, 0);

			// Maybe we were too slow and Betty sent another command
			if (cmd_complete){
				prt_timer(total_tmr);
				fprintf(stderr, "  Time out. MPD response cancelled.\n");
				close_mpd_socket();
				reset_ser_out();
				response_line_complete = 0;
				break;
			};
				
			// if there are bytes in the output buffer, send them to serial if it is ready
			send_to_serial(serial_fd);
			
			if (!res){
				if (timer_diff(response_tmr) > 10.0){
					prt_timer(total_tmr);
					fprintf(stderr,"MPD response is too late\n");
					close_mpd_socket();
					break;
				}
			};
			
			if (response_line_complete){
				fprintf(stderr, "  MPD: %s", mpd_resp_buf);
				translate_to_serial();
//				fprintf(stderr, "  Mpd: %s", mpd_resp_buf);

				// check for "OK" or "ACK"
				if ( mpd_eot(mpd_resp_buf) ) {
					response_finished = 1;
					// Send EOT to serial out !
					ser_out_char(EOT);
					fprintf(stderr,"\n");
				};
				
				reset_mpd_line();
			};
		};
	
		/* Send out all unsent bytes to serial buffer (as long as there is not another cmd) */
		while ( (!cmd_complete)  && ( (ser_out_wrt_idx - ser_out_rd_idx) > 0 ) ){
			// Poll for a new command from serial
			res = wait_for_input(serial_fd, mpd_socket, 0);
			
			// if there are bytes in the output buffer, send them to serial if it is ready
			send_to_serial(serial_fd);
			
			if (timer_diff(response_tmr) > 10.0){
					prt_timer(total_tmr);
					fprintf(stderr,"Sending response to SCART hangs\n");
					break;
				}
		};	
		reset_ser_out();
	};
	
	/* Restore old serial port settings */
	tcsetattr(serial_fd,TCSANOW,&oldtio);
	tcflush(serial_fd, TCIOFLUSH);	
	return 0;
}
