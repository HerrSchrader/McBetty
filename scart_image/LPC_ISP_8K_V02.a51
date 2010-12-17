
;89LPC935 boot flash program for IN-SYSTEM PROGRAMMING
;copyright Philips Semiconductors 2003

;FILENAME: 		LPC2_ISP_8K_V02.TXT
;CODE TYPE: 		ISP
;VERSION: 		02
;RELEASE DATE: 		14 JUN 03
;CODE MEMORY SIZE: 	8KB
;CODE ADDRESS RANGE: 	1E00h - 1FFFh
;BOOT VECTOR: 		1F00h
;USES IAP VERSION: 	02
;USES IAP ENTRY: 	FF03h
;Author:		Bill Houghton
;
;Features:
;
;Version 2:
;- Modifieds WDL and WDCON to select longest WDT timeout & provides periodic feeding.
;- Program user code page has option to use either IDATA or XDATA (if available on chip).
;- Software code corruption protection - valid key needed for write operations 
;- Code traps (software reset) added to start of ISP & IAP sections
;
;Version 1:
;- Includes standard features originally released with LPC932.

;code memory space for LPC2
;
;block 0, 1KB, 0000h - 03FFh
;block 1, 1KB, 0400h - 07FFh
;block 2, 1KB, 0800h - 0BFFh
;block 3, 1KB, 0C00h - 0FFFh
;block 4, 1KB, 1000h - 13FFh
;block 5, 1KB, 1400h - 17FFh
;block 6, 1KB, 1800h - 1BFFh
;block 7, 1KB, 1C00h - 1FFFh

;Bootrom, 240 bytes , FF00h - FFEF


PGMU	EQU	00
VRD	EQU	01               
MWR	EQU	02               
MRD	EQU	03               
ERS	EQU	04               
SCRC	EQU	05               
GCRC	EQU	06               
RUSR	EQU	07
F1	EQU	0D1H
WDL	EQU	0C1H
WDCON	EQU	0A7H
WFEED1	EQU	0C2H
WFEED2	EQU	0C3H
               
PGM_MTP	EQU	0FF03H		


;byte variables definition

DSEG	AT	30H

ADR0:		DS	1		;low byte of address
ADR1:		DS	1		;high byte of address
CHKSUM:	DS	1		;record checksum
NBYTES:	DS	1		;number of bytes in record
RTYPE:	DS	1		;record type
TMP3:		DS	1		;temporary storage

UCFG1:	DS	1	;User configuration register 1
UCFG2:	DS	1	;User configuration register 2
BOOTV:	DS	1	;Boot Vector
STATBY:	DS	1	;Status Byte
FCFG1:	DS	1	;Factory config 1, read only
FCFG2:	DS	1	;Factory config 2, read only 
DERIV:	DS	1	;Derivative
TMEB_v:	DS	1	;TMEB
SEC0:		DS	1	;Security byte 0
SEC1:		DS	1	;Security byte 1
SEC2:		DS	1	;Security byte 2
SEC3:		DS	1	;Security byte 3
SEC4:		DS	1	;Security byte 4
SEC5:		DS	1	;Security byte 5
SEC6:		DS	1	;Security byte 6
SEC7:		DS	1	;Security byte 7
MF_ID:		DS	1	;Signature byte 0 (mfg id)
ID_1:		DS	1	;Signature byte 1 (device id)
ID_2:		DS	1	;Signature byte 2 (derivative id)

CRC0:		DS	1	;CRC data
CRC1:		DS	1	;CRC data
CRC2:		DS	1	;CRC data
CRC3:		DS	1	;CRC data

;ISEG	AT	0FFH

;KEY:		DS	1	;IAP request key
KEY data 0xFF

;*************** equates list       ************************

CONFB		EQU	UCFG1		;start of CONF register space
RXDn		EQU	P1.1		;RxD pin
ISP_VER		EQU	02H		;ISP version id = 2
AUXR		EQU	08EH		;auxr register
AUXR1		EQU	0A2H		;auxr 1 register
SRST		EQU	8H		;OR mask for software reset bit
TAMOD		EQU	8FH		;timer aux mode register
P1M1		EQU	91H
P1M2		EQU	92H

OI		EQU	ACC.0		;operation aborted by interrupt 
SV		EQU	ACC.1		;security violation
HVE		EQU	ACC.2		;high voltage error 

SBVAL		EQU	0FFH		;status byte default value
BVVAL		EQU	0FCH		;boot vector default value
DBYTES		EQU	80H		;start of RAM buffer for hex string
KEYVAL		EQU	96H		;IAP request key value

CSEG	AT	2000H-512		;Should be 1E00h

RESET:
	ORL	AUXR1,#SRST	;set the software reset bit
;*********************************************************
;
;	START OF PROGRAM
;
;*********************************************************

;	First, we need to measure the baud rate of
;the host in terms of our own clock speed. This
;measurement can be made on a start bit provided
;the first data bit is a logical one. A capital "U"
;is a good choice since it has alternating 1s and 0s . 
;	Our measurement uses T1 which is clocked at
;fosc/2, which is the same as when T1 is used as 
;a baud rate generator. The UART uses 16x sampling
;so we need to divide the T1 count by 16. Even
;though the timer will be used in the 8-bit
;auto-reload mode for baud rate generation, non-reload
;16-bit mode is used for the measurement to give
;more clock counts for slower baud rates. This number
;will be divided by 16. This method allows the timer
;to count up to 4096 counts (16 x 256). The timer
;counts up towards zero thus counts loaded into the
;timer counter need to be negative numbers. A two's
;complement of the adjusted count produces this result.


INIT:

	ACALL	I_WDT		;
	MOV	P1M1,#00H
	MOV	P1M2,#00H
	MOV	P1,#0FFH
	MOV	TMOD,#10H	;16-bit non-reload 
	ANL	TAMOD,#0EFH	;not pwm mode 
	MOV	PCON,#80H	;SMOD = 1 = baud rate = T1/16
	CLR	A		;
	MOV	TH1,A		;set T1 to zero since we will
	MOV	TL1,A		;use this to count the start bit
MEAS:	
M1:	ACALL	FD_WDT		;feed the WDT
	JNB	RXDn,M1		;wait for RXD to be high
M2:	ACALL	FD_WDT		;feed the WDT
	JB	RXDn,M2		;wait until RXD goes low
	SETB	TR1		;start measuring the bit time
M3:	JNB	RXDn,M3		;wait until RXD goes high
	CLR	TR1		;stop measuring
	ACALL	FD_WDT		;feed the WDT
	MOV	RTYPE,TH1	;copy timer to RAM
	MOV	R1,#RTYPE	;for indirect addressing
	MOV	A,TL1		;get timer low byte
	XCHD	A,@R1		;acc= TL upper nibble & TH lower nibble
	SWAP	A		;acc= TH lower nibble & TL upper nibble
	CPL	A		;complement lower byte of count
	INC	A		;two's complement  = - count/16
	MOV	TL1,A		;
	MOV	TH1,A		;load counts & switch to
	MOV	TMOD,#20H	;8-bit auto-reload mode
	SETB	TR1		;start T1
	MOV	SCON,#52H	;init UART 8-bit variable, TI=1 RI=0
QRZ:	ACALL	ECHO		;wait until character is rcv'd & get it
	CJNE	A,#'U',QRZ	;check to see if uppercase "U"

;***** Intel Hex File Load routine *****
;
;This routine loads an Intel Hex formatted file into 
;the buffer memory. The hex file is received as a series
;of ASCII characters on the serial input line of the
;serial port. A record type of 00H is considered to be
;a data field. Any other type of record is considered
;to be an End-of-File marker. This routine also calculates
;the checksum on the field as it is received and compares
;this calculated checksum with the checksum field received
;in the record.

LCMD:
	MOV	R5,#0	;begin record... zero checksum
	ACALL	ECHO		;get first char and echo
	CJNE	A,#':',LCMD	;record starts with ':' char
	ACALL	GET2		;get the number of bytes in record
	MOV	NBYTES,TMP3	;and save
	ACALL	GET2		;get MSB of load address
	MOV	ADR1,TMP3	;and save
	ACALL	GET2		;get LSB of load address
	MOV	ADR0,TMP3	;and save it
	ACALL	GET2		;get record type
	MOV	RTYPE,TMP3	;and save it
	MOV	A,NBYTES	;else, more than
	MOV	R2,A	
	JZ	EOR		;zero data bytes ?
	MOV	R1,#DBYTES	;pointer for data bytes
LDATA:	ACALL	GET2		;get data byte
	MOV	@R1,TMP3	;store it
	INC	R1		;and bump up the pointer	
	DJNZ	R2,LDATA	;repeat if more bytes in record
EOR:
	MOV	A,R5		;
	MOV	R4,A		;save calculated checksum
	ACALL	GET2		;get the checksum byte
	MOV	A,R4		;and compare with calculated checksum byte
	CJNE	A,TMP3,CHKERR	;recv'd & calc'd chksums match ?
	AJMP	PROCESS		;YES, process command

CHKERR:	MOV	A,#'X'
	AJMP	RSPND1

GET2:	ACALL	ECHO		;get first char of length
	ACALL	A2HEX		;convert to hex
	SWAP	A		;set in high nibble
	MOV	TMP3,A		;store in NBYTES
	ACALL	ECHO		;get second char of length
	ACALL	A2HEX		;convert to hex
	ORL	TMP3,A		;add into NBYTES
	MOV	A,R5		;get checksum
	CLR	C		;subtract NBYTES
	SUBB	A,TMP3		;from checksum and
	MOV	R5,A		;store as new checksum
	RET

;***** console output routine *****
;
;Outputs character in the ACC to 
;the serial output line.

CO:	ACALL	FD_WDT		;feed the WDT
	JNB	TI,CO		;wait till xmtr ready
	CLR	TI		;reset xmtr flag
	MOV	SBUF,A	;output char to SIO
	RET			;and done

;***** console input routine *****
;
;Waits until character has been received
;and then returns char in ACC.

CI:	ACALL	FD_WDT		;feed the WDT
	JNB	RI,CI		;wait till char
	CLR	RI		;reset rcvr flag
	MOV	A,SBUF	;read the char
	RET			;and done

;***** character echo routine *****
;
;waits until a character is received from
;the console input and echos this character
;to the console output. The received char
;is also passed to the caller in the ACC.

ECHO:	ACALL	CI			;get char from console 
	ACALL	CO			;print the character
	JNB	ACC.6,EXECHO	;exit if not 4x,5x, or 6x Hex
	CLR	ACC.5			;convert to upper case
EXECHO:
RET				;and done

;***** ASCII to HEX routine *****
;
;This routine accepts an ASCII char in the ACC
;and converts it into the corresponding hex digit.
;The routine checks to see if the char is in the
;range of '0' through '9' or in the range of 'A'
;through 'F'. If not in either range then the ASCII
;char is not a valid hex entry from the operator
;and an error flag is returned true along with the
;original ASCII char returned in the ACC.

A2HEX:
	JNB	ACC.6,HEX1
	ADD	A,#09H
HEX1:	ANL	A,#0FH
	RET

;***** HEX to ASCII routine *****
;
;This routine receives a single hex digit
;(a four bit nibble) in the ACC and returns
;the equivilent ASCII char in the ACC.

HEX2A:
	ANL	A,#0FH
	CLR	C		;carry affects the testing
	SUBB	A,#0AH	;test for range of 0-9, A-F
	JNC	HAHIGH	;no carry then A-F range
	ADD	A,#3AH	;add offset for 0-9 range
	RET
HAHIGH:
	ADD	A,#41H	;add in offset for A-F range
	RET

PROCESS:
	MOV	A,RTYPE		;get record type
	RL	A			;double ACC for two byte jumps
	MOV	DPTR,#RECTBL	;pointer = start of table
	JMP	@A+DPTR		;branch on record type

RECTBL:
	AJMP	PROGRAM		;0 = program data bytes
	AJMP	RDVER		;1 = read code versions
	AJMP	AUXWR		;2 = misc 'write' functions
	AJMP	AUXRD		;3 = misc 'read' functions
	AJMP	ERASE		;4 = erase block or page
	AJMP	CRCS		;5 = sector CRC
	AJMP	CRCG		;6 = global CRC
	AJMP	SETBR		;7 = set baud rate
	AJMP	RESET		;8 = reset MCU
	AJMP	DCMD		;9 = display device data
	

	
I_WDT:
	MOV	WDL,#0FFH	;set to max count
	ORL	WDCON,#0E0H	;set pre= max
FD_WDT:	MOV	WFEED1,#0A5H	;
	MOV	WFEED2,#5AH		;	
	RET
	

RDVER:
	MOV	A,#ISP_VER	;get ISP version id
	ACALL	OUTBYT		;and print it
	MOV	A,#VRD		;function code 
	CALL	PGM_MTP		;and perform the function
	MOV	A,R7		;get the response 
	ACALL	OUTBYT		;and print it
	AJMP	EOF		;and we're done	
	

	
CSEG	AT	2000H-256		;Should be 1F00h

	AJMP	INIT			;Boot vector entry point

ERROR:
	MOV	A,#'R'		;print a verify error
	ACALL	CO		;send an okay message
	MOV	A,R7		;get status
	ACALL	OUTBYT		;and print
	AJMP	DEXIT		;and done
	
	
PROGRAM:
	MOV	R3,NBYTES	;get the number of bytes in record
	MOV	A,R3		;get the number of bytes in record
	JZ	EOF		;exit if no bytes in record
	MOV	R5,ADR0		;get the load address
	MOV	R4,ADR1		;of the first byte in record
	MOV	R7,#DBYTES	;pointer to data
	MOV	A,#PGMU		;program user code
EXEC:	MOV	R0,#KEY		;address for the key
	MOV	@R0,#KEYVAL	;setup a valid key
	CLR	F1		;specify IRAM
	CALL	PGM_MTP		;write the entire record & verify
	JB	F0,ERROR	;check if an error occured

EOF:	MOV	A,#'.'		;no error
RSPND1:
	ACALL	CO		;send an okay message
	AJMP	DEXIT		;and done


;***** display buffer contents routine *****
;
;This routine displays the contents of the buffer memory
;over a user specified range. The displayed output is formatted
;into a series of lines on the console. A line begins with the
;address of the first byte in the line. Line length is limited
;to a maximum of 16 bytes per line. Once this limit is reached,
;new formatted lines are used.

DCMD:
	MOV	R0,#DBYTES	;
	MOV	DPH,@R0		;get high byte of starting address
	INC	R0		;point to low byte of starting address
	MOV	DPL,@R0		;get low byte of starting address
	INC	R0		;point to high byte of ending address
	MOV	ADR1,@R0	;get high byte of ending address
	INC	R0		;point to low byte of ending address
	MOV	ADR0,@R0	;get low byte of ending address
	INC	R0		;point to function either display or blankcheck
	JNB	RI,$		;wait till host ready to receive
	CLR	RI

DLINE:
	CJNE	@R0,#00H,DAGN	;ignore this if its not a display command
	ACALL	CRLF
	MOV	R2,#10H		;R2 = 16 bytes per line
	MOV	A,DPH
	ACALL	OUTBYT
	MOV	A,DPL
	ACALL	OUTBYT		;print the address
	MOV	A,#'='		;of first byte of
	ACALL	CO		;the line along with

DAGN:

	MOV	R4,DPH
	MOV	R5,DPL
	MOV	A,#RUSR		;READ_USER
	CALL	PGM_MTP		;read the byte
	MOV	A,R7		;get result
DPRN:
	CJNE	@R0,#00H,BLKCHK ;ignore this if its not a display command
	ACALL	OUTBYT		;and print it
	SJMP	CKDEND		;and then check if we've reached the end
BLKCHK:
	CJNE	A,#00H,BLANKERR
CKDEND:
	MOV	A,ADR1		;
	CJNE	A,DPH,DNEXT	;check if DPH = stop high
	MOV	A,ADR0		;
	CJNE	A,DPL,DNEXT	;check if DPL = stop low
	CJNE	@R0,#01H,DEXIT 	;if display command use display exit 
	AJMP	EOF		;blankcheck exit (print a period)

DEXIT:
	ACALL	CRLF		;print a CRLF
	JNB	TI,$
	SETB	REN		;TURN ON UART RECEIVER
	AJMP	LCMD		;branch to main loop
DNEXT:
	INC	DPTR		;more bytes so point to next byte
DNXT1:
	DJNZ	R2,DAGN
	AJMP	DLINE		;we start a new line or not

BLANKERR:
	MOV	A,DPH
	ACALL	OUTBYT		;print DPH
	MOV	A,DPL
	ACALL	OUTBYT		;print DPL
	AJMP	DEXIT		;and exit


AUXWR:
	MOV	R1,#DBYTES	;pointer for data
	MOV	A,@R1		;
	MOV	R7,A		;get the subfunction code 
	INC	R1		;
	MOV	A,@R1		;
	MOV	R5,A		;get the data to write 
	MOV	A,#MWR		;function code 
	AJMP	EXEC		;perform the function & check for errors

ERR:	AJMP	ERROR		;error vector

AUXRD:
	MOV	R1,#DBYTES	;pointer for data
	MOV	A,@R1		;
	MOV	R7,A		;get the subfunction code 
	MOV	A,#MRD		;function code 
	CALL	PGM_MTP		;and perform the function
	JB	F0,ERR		;exit if an error occured
	MOV	A,R7		;get the response 
	ACALL	OUTBYT		;and print it
	AJMP	EOF		;and we're done
	
ERASE:
	MOV	R1,#DBYTES	;pointer for data
	MOV	A,@R1		;
	MOV	R7,A		;get the block or page erase command 
	INC	R1		;
	MOV	A,@R1		;
	MOV	R4,A		;get the high address 
	INC	R1		;
	MOV	A,@R1		;
	MOV	R5,A		;get the low address 
	MOV	A,#ERS		;function code 
	AJMP	EXEC		;perform the function & check for errors

CRCG:
	MOV	A,#GCRC		;function code 
	AJMP	DO_CRC		;and do the CRC
CRCS:
	MOV	R1,#DBYTES	;pointer for data
	MOV	A,@R1		;
	MOV	R7,A		;get the sector number
	MOV	A,#SCRC		;function code 
DO_CRC:
	CALL	PGM_MTP		;and perform the function
	JB	F0,ERR		;exit if an error occured
	MOV	A,R4		;get CRC bits 31:24 
	ACALL	OUTBYT		;and print
	MOV	A,R5		;get CRC bits 23:16 
	ACALL	OUTBYT		;and print
	MOV	A,R6		;get CRC bits 15:8 
	ACALL	OUTBYT		;and print
	MOV	A,R7		;get CRC bits 7:0 
	ACALL	OUTBYT		;and print
	AJMP	EOF		;and we're done

CRLF:	MOV	A,#0DH
	ACALL	CO
	MOV	A,#0AH
	ACALL	CO
	RET



SETBR:
	MOV	A,#'.'		;respond with okay status before
	ACALL	CO		;changing the baud rate (i.e.- at the old baud rate)
	MOV	R1,#0FFH	;
	DJNZ	R1,$		;wait before proceeding	
	DJNZ	R1,$		;wait before proceeding	
	CLR	TR1		;stop the timer
	MOV	R1,#DBYTES	;pointer for data
	MOV	TH1,@R1		;get baud rate
	MOV	TL1,@R1		;get baud rate
	SETB	TR1		;start the timer
	AJMP	EOF		;and we're done

OUTBYT:	MOV	R4,A		;"push acc"
	SWAP	A
	ACALL	HEX2A
	ACALL	CO
	MOV	A,R4		;"pop acc"
	ACALL	HEX2A
	ACALL	CO
	RET



END

