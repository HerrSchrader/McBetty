/* Tool to flash 89lpc931 over serial line */


#define VERSION_MAJOR 0
#define VERSION_MINOR 9

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <fcntl.h>


#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define BUFFER_SIZE 1024


//#define BAUDRATE B38400
// Flashing works more reliably with a lower baud rate 
#define BAUDRATE B19200

#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

#include "scart_image.h"

		
/* Read a command from serial line 
	A command is a single line, terminated by 0x0a
	Returns number of bytes read, if successful
	Returns 0, if 
		- line is longer than BUFFER_SIZE-1
*/
int read_cmd (int fd, char *buffer){
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


/*
	Write bytesToSend bytes out on fileDesc
	Handles short writes.
	returns -1 on failure, 0 on success
*/
int write_all(int fileDesc, char * buffer, int bytesToSend){
	int bytesWritten;
	int num = 0;

	for (bytesWritten = 0; bytesWritten < bytesToSend; bytesWritten += num)
	{
		num = write(	fileDesc,	
			    	(void *)(buffer + bytesWritten),
				bytesToSend - bytesWritten);

		if (num == -1) {
			perror("write()");
			break;
		};
		
	};
	return num == -1?-1:0;
};




/* Send a data record in intel hex format to device 
	Ceck that it was received correctly.
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
	len = read_cmd(fd, buffer);
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
	
	
int hex2dez(unsigned char h1){
	int res;
	
	h1 = toupper(h1);
	
	if (h1 >='A')
		h1 = '9' + 1 + (h1 - 'A');
	return (h1 - '0'); 
};


/* Calculate the CRC from the binary image */
unsigned int calc_crc(unsigned int adr){
	unsigned int poly, crc_im, crcf, tap;
	unsigned char data;
	int i;
	int blklen = 1024;
	
	poly = 0x00400007;
	crc_im = tap = 0;
	
	for (i=0; i<blklen; i++){
		crcf = crc_im & 0x80000000;
		crc_im <<= 1;
		data = scart_image[adr+i];
		tap  = ((data & 0x80) << 11) | ((data & 0x40) << 10) | ((data & 0x20) << 8) | ((data & 0x10) << 6);
		tap |= ((data & 0x08) << 5)  | ((data & 0x04) << 3)  | ((data & 0x02) << 2) | ((data & 0x01) << 0);
		crc_im ^= tap;
		if (crcf) crc_im ^= poly;
	}
	return (crc_im);
};

/* Append intel hex string checksum */
void checksum(char *s){
	unsigned int chksum = 0;
	
	s++;				// ignore ':'
	while (*s) {
		chksum += 16 * hex2dez(*s) + hex2dez(*(s+1));
		s += 2;
	};
	chksum = (((chksum & 0xff) ^ 0xff) + 1) & 0xff;
	sprintf(s, "%02X", chksum); 
};
	
void make_ihex(unsigned int adr, char *buf){
	int i;
	char *s = buf;
		
	buf += sprintf(buf, ":40%04X00",adr);
	for (i=0; i<64; i++){
		buf += sprintf(buf, "%02X",scart_image[adr+i]);
	};
	checksum(s); 
};


int write_sector (int fd, unsigned int sector){
	char buffer[BUFFER_SIZE], ans[BUFFER_SIZE];
	unsigned int adr;
	
	if (sector >= 7) {
		fprintf(stderr,"ERROR: Writing sector %d is forbidden\n", sector);
		return;
	};
	
	printf("Erasing secor %d \n", sector);
	sprintf(buffer, ":0300000401%04x", sector * 1024);
	checksum(buffer);
	if (0 != send_string(fd, buffer, ans)) {
		return(-20);
	};
	
	for (adr = sector * 1024; adr < (sector * 1024 + 1023); adr += 64){
		make_ihex(adr, buffer);
		if (0 != send_string(fd, buffer, ans))
			return (20);
	};
			
	sprintf(buffer, ":01000005%02X", sector * 4);
	checksum(buffer);	

	if (0 != send_string(fd, buffer, ans)) {
		return(20);	;
	};
		
	sprintf(buffer,"%08X",calc_crc(sector * 1024));
		
	if (0 != strncmp(ans, buffer, 8)){
		printf("Calculated CRC Sector %d = %s\n", sector, buffer);
		printf("Read CRC = %s\n", ans);
		return(20);
	} else
		printf("Sector %d CRC OK.\n", sector);
	
	return(0);	
};

void really_write_sector (int fd, unsigned int sector){
	int i;
	
	if (sector >= 7) {
		fprintf(stderr,"ERROR: Write sector %d is forbidden\n", sector);
		return;
	};
	
	for (i=0; i<10; i++){
		if (0 == write_sector(fd, sector))
			return;
	};
	fprintf(stderr,"ERROR: Could not write sector %d\n", sector);
	exit(20);
};


int main(int argc, char *argv[])
{
	int len, cmd_finished, stop, i,try;
	int clientSocket, serial_fd, 
	remotePort,
	status = 0;
	struct hostent *hostPtr = NULL;
	struct sockaddr_in serverName = { 0 };
	unsigned char buffer[BUFFER_SIZE];
	unsigned char buffer2[BUFFER_SIZE];
	char *remoteHost = NULL;
	char *serial_device;
	struct termios oldtio,newtio;
	char *s;
	char *hex_filename;
	FILE *hexfile;
	
	
	if (2 != argc)
 	{
		fprintf(stderr, "Usage: %s <serial_device>\n", argv[0]);
		exit(1);
	};
	
	serial_device = argv[1];
	

	/* 
	Open modem device for reading and writing and not as controlling tty
	because we don't want to get killed if linenoise sends CTRL-C.
       */	
	serial_fd = open(serial_device, O_RDWR | O_NOCTTY ); 
	if (serial_fd <0) {perror(serial_device); exit(-1); }
	
	tcgetattr(serial_fd,&oldtio); /* save current port settings */

	bzero(&newtio, sizeof(newtio));
       /* 
	BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
	CRTSCTS : output hardware flow control (only used if the cable has
	all necessary lines. See sect. 7 of Serial-HOWTO)
	CS8     : 8n1 (8bit,no parity,1 stopbit)
	CLOCAL  : local connection, no modem contol
	CREAD   : enable receiving characters
	*/
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

	/*
	IGNPAR  : ignore bytes with parity errors
	ICRNL   : map CR to NL (otherwise a CR input on the other computer
	will not terminate input)
	otherwise make device raw (no other input processing)
	*/
	newtio.c_iflag = IGNPAR;

	/*
	Raw output.
	*/	
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) 
	disable all echo functionality, and don't send signals to calling program
	*/
	newtio.c_lflag = 0;
 
	newtio.c_cc[VTIME]    = 50;   /* time out after 5 seconds */
	newtio.c_cc[VMIN]     = 0;   /* non-blocking read */
	
	/* now clean the modem line and activate the settings for the port */
	tcflush(serial_fd, TCIFLUSH);
	tcsetattr(serial_fd,TCSANOW,&newtio);
	
	printf("Sending break\n");
	/* Send a break to reset device into ISP mode */
	tcsendbreak(serial_fd,3);

	tcflush(serial_fd, TCIFLUSH);
	
	usleep(1000000);
	tcflush(serial_fd, TCIFLUSH);
	usleep(1000000);
	
	/* Send an uppercase U to negotiate baud rate */
	buffer[0] = 'U';
	buffer[1] = 0;
	
	
	/* Send U to serial line */
	if (write_all(serial_fd, buffer, 1) == -1) {
		perror("sendall");
		printf("We only sent %d bytes because of the error!\n", len);
	};		
	
	len = read_cmd(serial_fd, buffer);
	buffer[len] = 0;
	printf("Read %d characters:\n\t", len, buffer[0]);
	for (i=0; i<len; i++)
		printf("%02x ", buffer[i]);
	printf("\n");
	
	tcflush(serial_fd, TCIFLUSH);
			
	/* Send U to serial line to check that we are in sync */
	buffer[0] = 'U';
	if (write_all(serial_fd, buffer, 1) == -1) {
		perror("sendall");
		printf("We only sent %d bytes because of the error!\n", len);
	};		
	
	len = read_cmd(serial_fd, buffer);
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
		exit(20);
	};

	s=":0100000310EC";
	printf("Manuf. ID \t= ");
	if (0 != send_string(serial_fd,s,buffer2) ) {
		exit(20);	;
	};
	printf("%s\n",buffer2);
	if (0 != strncmp(buffer2, "15", 2)) {
		fprintf(stderr, "ERROR: Device not recognized\n");
		exit(20);
	};
	 
	s=":0100000311EB";
	printf("Device ID \t= ");
	if (0 != send_string(serial_fd,s,buffer2)) {
		exit(20);	;
	};
	printf("%s\n",buffer2);
	if (0 != strncmp(buffer2, "DD", 2)) {
		fprintf(stderr, "ERROR: Device not recognized\n");
		exit(20);
	};
	
	s=":0100000312EA";
	printf("Derivative ID \t= ");
	if (0 != send_string(serial_fd,s,buffer2)) {
		exit(20);	;
	};
	printf("%s\n",buffer2);
	if (0 != strncmp(buffer2, "09", 2)) {
		fprintf(stderr, "ERROR: Device not recognized\n");
		exit(20);
	};
		
	/* Read configuration data */
	s=":0100000300FC";
	printf("UCFG1    \t= ");
	if (0 != send_string(serial_fd,s, buffer2)) {
		exit(-20);	;
	};
	printf("%s\n",buffer2);	
	
	s=":0100000302FA";
	printf("Boot Vector \t= ");
	if (0 != send_string(serial_fd,s,buffer2)) {
		exit(-20);	;
	};
	printf("%s\n",buffer2);
		
	s=":0100000303F9";
	printf("Status Byte \t= ");
	if (0 != send_string(serial_fd,s,buffer2)) {
		exit(-20);	;
	};
	printf("%s\n",buffer2);					

	s=":00000001FF";
	printf("Version ID \t= ");
	if (0 != send_string(serial_fd,s, buffer2)) {
		exit(-20);	;
	};
	printf("%s\n",buffer2);
	
	really_write_sector(serial_fd, 0);
	really_write_sector(serial_fd, 1);
	really_write_sector(serial_fd, 2);
	really_write_sector(serial_fd, 3);
	really_write_sector(serial_fd, 4);
	really_write_sector(serial_fd, 5);
	really_write_sector(serial_fd, 6);

	// The configuration bytes are defined and set in bin2c.c !!! 
	
	
	/* Write Configuration Byte UCFG1 */
	sprintf(buffer, ":0200000200%02X", scart_UCFG1);
	checksum(buffer);	
	if (0 != send_string(serial_fd, buffer, buffer2)) {
		return(20);	;
	};
		
	/* Read UCFG1 */
	s=":0100000300FC";
	printf("UCFG1    \t= ");
	if (0 != send_string(serial_fd,s, buffer2)) {
		exit(-20);	;
	};
	printf("%s\n",buffer2);	
	

	/* Write Boot Vector Byte */
	sprintf(buffer, ":0200000202%02X", scart_bootvec);
	checksum(buffer);	
	if (0 != send_string(serial_fd, buffer, buffer2)) {
		return(20);	;
	};
	s=":0100000302FA";
	printf("Boot Vector \t= ");
	if (0 != send_string(serial_fd,s,buffer2)) {
		exit(-20);	;
	};
	printf("%s\n",buffer2);
				
	
	/* Write Boot Status Byte */	
	sprintf(buffer, ":0200000203%02X", scart_bootstat);
	checksum(buffer);	
	if (0 != send_string(serial_fd, buffer, buffer2)) {
		return(20);	;
	};
	s=":0100000303F9";
	printf("Status Byte \t= ");
	if (0 != send_string(serial_fd,s,buffer2)) {
		exit(-20);	;
	};
	printf("%s\n",buffer2);					
	
	/* Reset the device */
	printf("\nResetting device\n");
	s=":00000008F8";
	write_all(serial_fd, s, strlen(s));

	/* This should be the echo of our RESET command */
	read_cmd(serial_fd, buffer);
	printf("%s\n", buffer);

	/* Restore old serial port settings */
	tcsetattr(serial_fd,TCSANOW,&oldtio);

	
	return 0;
}
