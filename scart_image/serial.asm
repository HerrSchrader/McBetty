;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 2.9.7 #5820 (May  6 2010) (Linux)
; This file was generated Mon Dec 20 23:27:41 2010
;--------------------------------------------------------
	.module serial
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _TCR20_0
	.globl _TCR20_1
	.globl _TCR20_2
	.globl _TCR20_3
	.globl _TCR20_4
	.globl _TCR20_5
	.globl _TCR20_6
	.globl _TCR20_7
	.globl _P3_0
	.globl _P3_1
	.globl _P2_0
	.globl _P2_1
	.globl _P2_2
	.globl _P2_3
	.globl _P2_4
	.globl _P2_5
	.globl _P2_6
	.globl _P2_7
	.globl _P1_0
	.globl _P1_1
	.globl _P1_2
	.globl _P1_3
	.globl _P1_4
	.globl _P1_5
	.globl _P1_6
	.globl _P1_7
	.globl _P0_0
	.globl _P0_1
	.globl _P0_2
	.globl _P0_3
	.globl _P0_4
	.globl _P0_5
	.globl _P0_6
	.globl _P0_7
	.globl _I2CON_0
	.globl _I2CON_2
	.globl _I2CON_3
	.globl _I2CON_4
	.globl _I2CON_5
	.globl _I2CON_6
	.globl _SCON_0
	.globl _SCON_1
	.globl _SCON_2
	.globl _SCON_3
	.globl _SCON_4
	.globl _SCON_5
	.globl _SCON_6
	.globl _SCON_7
	.globl _IP0_0
	.globl _IP0_1
	.globl _IP0_2
	.globl _IP0_3
	.globl _IP0_4
	.globl _IP0_5
	.globl _IP0_6
	.globl _IEN1_0
	.globl _IEN1_1
	.globl _IEN1_2
	.globl _IEN1_3
	.globl _IEN1_4
	.globl _IEN1_6
	.globl _IEN1_7
	.globl _IEN0_0
	.globl _IEN0_1
	.globl _IEN0_2
	.globl _IEN0_3
	.globl _IEN0_4
	.globl _IEN0_5
	.globl _IEN0_6
	.globl _IEN0_7
	.globl _TCON_0
	.globl _TCON_1
	.globl _TCON_2
	.globl _TCON_3
	.globl _TCON_4
	.globl _TCON_5
	.globl _TCON_6
	.globl _TCON_7
	.globl _PSW_0
	.globl _PSW_1
	.globl _PSW_2
	.globl _PSW_3
	.globl _PSW_4
	.globl _PSW_5
	.globl _PSW_6
	.globl _PSW_7
	.globl _IP0H
	.globl _WFEED2
	.globl _WFEED1
	.globl _WDL
	.globl _WDCON
	.globl _TRIM
	.globl _TPCR2L
	.globl _TPCR2H
	.globl _TOR2L
	.globl _TOR2H
	.globl _TISE2
	.globl _TIFR2
	.globl _TICR2
	.globl _TCR21
	.globl _TCR20
	.globl _TAMOD
	.globl _SPDAT
	.globl _SPSTAT
	.globl _SPCTL
	.globl _SSTAT
	.globl _RTCL
	.globl _RTCH
	.globl _RTCCON
	.globl _RSTSRC
	.globl _PT0AD
	.globl _PCONA
	.globl _P3M2
	.globl _P3M1
	.globl _P2M2
	.globl _P2M1
	.globl _P1M2
	.globl _P1M1
	.globl _P0M2
	.globl _P0M1
	.globl _OCRDL
	.globl _OCRDH
	.globl _OCRCL
	.globl _OCRCH
	.globl _OCRBL
	.globl _OCRBH
	.globl _OCRAL
	.globl _OCRAH
	.globl _KBPATN
	.globl _KBMASK
	.globl _KBCON
	.globl _IP1H
	.globl _IP1
	.globl _IEN1
	.globl _ICRBL
	.globl _ICRBH
	.globl _ICRAL
	.globl _ICRAH
	.globl _I2STAT
	.globl _I2SCLL
	.globl _I2SCLH
	.globl _I2DAT
	.globl _I2CON
	.globl _I2ADR
	.globl _DIVM
	.globl _DEEADR
	.globl _DEEDAT
	.globl _DEECON
	.globl _CMP2
	.globl _CMP1
	.globl _CCCRD
	.globl _CCCRC
	.globl _CCCRB
	.globl _CCCRA
	.globl _BRGCON
	.globl _BRGR1
	.globl _BRGR0
	.globl _TH2
	.globl _TL2
	.globl _SADEN
	.globl _SADDR
	.globl _AUXR1
	.globl _SBUF
	.globl _SCON
	.globl _IP0
	.globl _IEN0
	.globl _TH1
	.globl _TH0
	.globl _TL1
	.globl _TL0
	.globl _TMOD
	.globl _TCON
	.globl _PCON
	.globl _DPH
	.globl _DPL
	.globl _SP
	.globl _B
	.globl _ACC
	.globl _PSW
	.globl _P3
	.globl _P2
	.globl _P1
	.globl _P0
	.globl _initSerial
	.globl _send_byte
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_P0	=	0x0080
_P1	=	0x0090
_P2	=	0x00a0
_P3	=	0x00b0
_PSW	=	0x00d0
_ACC	=	0x00e0
_B	=	0x00f0
_SP	=	0x0081
_DPL	=	0x0082
_DPH	=	0x0083
_PCON	=	0x0087
_TCON	=	0x0088
_TMOD	=	0x0089
_TL0	=	0x008a
_TL1	=	0x008b
_TH0	=	0x008c
_TH1	=	0x008d
_IEN0	=	0x00a8
_IP0	=	0x00b8
_SCON	=	0x0098
_SBUF	=	0x0099
_AUXR1	=	0x00a2
_SADDR	=	0x00a9
_SADEN	=	0x00b9
_TL2	=	0x00cc
_TH2	=	0x00cd
_BRGR0	=	0x00be
_BRGR1	=	0x00bf
_BRGCON	=	0x00bd
_CCCRA	=	0x00ea
_CCCRB	=	0x00eb
_CCCRC	=	0x00ec
_CCCRD	=	0x00ed
_CMP1	=	0x00ac
_CMP2	=	0x00ad
_DEECON	=	0x00f1
_DEEDAT	=	0x00f2
_DEEADR	=	0x00f3
_DIVM	=	0x0095
_I2ADR	=	0x00db
_I2CON	=	0x00d8
_I2DAT	=	0x00da
_I2SCLH	=	0x00dd
_I2SCLL	=	0x00dc
_I2STAT	=	0x00d9
_ICRAH	=	0x00ab
_ICRAL	=	0x00aa
_ICRBH	=	0x00af
_ICRBL	=	0x00ae
_IEN1	=	0x00e8
_IP1	=	0x00f8
_IP1H	=	0x00f7
_KBCON	=	0x0094
_KBMASK	=	0x0086
_KBPATN	=	0x0093
_OCRAH	=	0x00ef
_OCRAL	=	0x00ee
_OCRBH	=	0x00fb
_OCRBL	=	0x00fa
_OCRCH	=	0x00fd
_OCRCL	=	0x00fc
_OCRDH	=	0x00ff
_OCRDL	=	0x00fe
_P0M1	=	0x0084
_P0M2	=	0x0085
_P1M1	=	0x0091
_P1M2	=	0x0092
_P2M1	=	0x00a4
_P2M2	=	0x00a5
_P3M1	=	0x00b1
_P3M2	=	0x00b2
_PCONA	=	0x00b5
_PT0AD	=	0x00f6
_RSTSRC	=	0x00df
_RTCCON	=	0x00d1
_RTCH	=	0x00d2
_RTCL	=	0x00d3
_SSTAT	=	0x00ba
_SPCTL	=	0x00e2
_SPSTAT	=	0x00e1
_SPDAT	=	0x00e3
_TAMOD	=	0x008f
_TCR20	=	0x00c8
_TCR21	=	0x00f9
_TICR2	=	0x00c9
_TIFR2	=	0x00e9
_TISE2	=	0x00de
_TOR2H	=	0x00cf
_TOR2L	=	0x00ce
_TPCR2H	=	0x00cb
_TPCR2L	=	0x00ca
_TRIM	=	0x0096
_WDCON	=	0x00a7
_WDL	=	0x00c1
_WFEED1	=	0x00c2
_WFEED2	=	0x00c3
_IP0H	=	0x00b7
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_PSW_7	=	0x00d7
_PSW_6	=	0x00d6
_PSW_5	=	0x00d5
_PSW_4	=	0x00d4
_PSW_3	=	0x00d3
_PSW_2	=	0x00d2
_PSW_1	=	0x00d1
_PSW_0	=	0x00d0
_TCON_7	=	0x008f
_TCON_6	=	0x008e
_TCON_5	=	0x008d
_TCON_4	=	0x008c
_TCON_3	=	0x008b
_TCON_2	=	0x008a
_TCON_1	=	0x0089
_TCON_0	=	0x0088
_IEN0_7	=	0x00af
_IEN0_6	=	0x00ae
_IEN0_5	=	0x00ad
_IEN0_4	=	0x00ac
_IEN0_3	=	0x00ab
_IEN0_2	=	0x00aa
_IEN0_1	=	0x00a9
_IEN0_0	=	0x00a8
_IEN1_7	=	0x00ef
_IEN1_6	=	0x00ee
_IEN1_4	=	0x00ec
_IEN1_3	=	0x00eb
_IEN1_2	=	0x00ea
_IEN1_1	=	0x00e9
_IEN1_0	=	0x00e8
_IP0_6	=	0x00be
_IP0_5	=	0x00bd
_IP0_4	=	0x00bc
_IP0_3	=	0x00bb
_IP0_2	=	0x00ba
_IP0_1	=	0x00b9
_IP0_0	=	0x00b8
_SCON_7	=	0x009f
_SCON_6	=	0x009e
_SCON_5	=	0x009d
_SCON_4	=	0x009c
_SCON_3	=	0x009b
_SCON_2	=	0x009a
_SCON_1	=	0x0099
_SCON_0	=	0x0098
_I2CON_6	=	0x00de
_I2CON_5	=	0x00dd
_I2CON_4	=	0x00dc
_I2CON_3	=	0x00db
_I2CON_2	=	0x00da
_I2CON_0	=	0x00d8
_P0_7	=	0x0087
_P0_6	=	0x0086
_P0_5	=	0x0085
_P0_4	=	0x0084
_P0_3	=	0x0083
_P0_2	=	0x0082
_P0_1	=	0x0081
_P0_0	=	0x0080
_P1_7	=	0x0097
_P1_6	=	0x0096
_P1_5	=	0x0095
_P1_4	=	0x0094
_P1_3	=	0x0093
_P1_2	=	0x0092
_P1_1	=	0x0091
_P1_0	=	0x0090
_P2_7	=	0x00a7
_P2_6	=	0x00a6
_P2_5	=	0x00a5
_P2_4	=	0x00a4
_P2_3	=	0x00a3
_P2_2	=	0x00a2
_P2_1	=	0x00a1
_P2_0	=	0x00a0
_P3_1	=	0x00b1
_P3_0	=	0x00b0
_TCR20_7	=	0x00cf
_TCR20_6	=	0x00ce
_TCR20_5	=	0x00cd
_TCR20_4	=	0x00cc
_TCR20_3	=	0x00cb
_TCR20_2	=	0x00ca
_TCR20_1	=	0x00c9
_TCR20_0	=	0x00c8
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area	OSEG    (OVR,DATA)
	.area	OSEG    (OVR,DATA)
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'initSerial'
;------------------------------------------------------------
;baud                      Allocated to registers r2 r3 
;------------------------------------------------------------
;	serial.c:22: void initSerial(unsigned short baud) {
;	-----------------------------------------
;	 function initSerial
;	-----------------------------------------
_initSerial:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
	mov	r2,dpl
	mov	r3,dph
;	serial.c:25: SCON   = 0x52;		// 0101 0010 = Mode 8N1, enable reception, TxIntFlag=1 ? 
	mov	_SCON,#0x52
;	serial.c:26: SSTAT |= 0x80;		// 1000 0000 = Double buffering mode
	orl	_SSTAT,#0x80
;	serial.c:27: switch (baud) {
	cjne	r2,#0x60,00114$
	cjne	r3,#0x00,00114$
	sjmp	00101$
00114$:
	cjne	r2,#0xC0,00115$
	cjne	r3,#0x00,00115$
	sjmp	00102$
00115$:
	cjne	r2,#0x80,00116$
	cjne	r3,#0x01,00116$
	sjmp	00103$
00116$:
	cjne	r2,#0x40,00117$
	cjne	r3,#0x02,00117$
	sjmp	00104$
00117$:
;	serial.c:29: case 96:
	cjne	r2,#0x80,00106$
	cjne	r3,#0x04,00106$
	sjmp	00105$
00101$:
;	serial.c:30: BRGR0  = 0x88; 
	mov	_BRGR0,#0x88
;	serial.c:31: BRGR1  = 0x02;
	mov	_BRGR1,#0x02
;	serial.c:32: break;
;	serial.c:33: case 192:
	sjmp	00106$
00102$:
;	serial.c:34: BRGR0  = 0xBC; 
	mov	_BRGR0,#0xBC
;	serial.c:35: BRGR1  = 0x02;
	mov	_BRGR1,#0x02
;	serial.c:36: break;
;	serial.c:37: case 384:
	sjmp	00106$
00103$:
;	serial.c:38: BRGR0  = 0x56; 
	mov	_BRGR0,#0x56
;	serial.c:39: BRGR1  = 0x01;
	mov	_BRGR1,#0x01
;	serial.c:40: break;
;	serial.c:41: case 576:
	sjmp	00106$
00104$:
;	serial.c:42: BRGR0  = 0xDE; 
	mov	_BRGR0,#0xDE
;	serial.c:43: BRGR1  = 0x00;
	mov	_BRGR1,#0x00
;	serial.c:44: break;
;	serial.c:45: case 1152:
	sjmp	00106$
00105$:
;	serial.c:46: BRGR0  = 0x67; 
	mov	_BRGR0,#0x67
;	serial.c:47: BRGR1  = 0x00;
	mov	_BRGR1,#0x00
;	serial.c:49: }
00106$:
;	serial.c:50: BRGCON = 0x03;		// Select the baud rate generator as timing source and enable it
	mov	_BRGCON,#0x03
;	serial.c:52: TI = 1;
	setb	_SCON_1
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'send_byte'
;------------------------------------------------------------
;h                         Allocated to registers r2 
;------------------------------------------------------------
;	serial.c:58: void send_byte(unsigned char h) {
;	-----------------------------------------
;	 function send_byte
;	-----------------------------------------
_send_byte:
	mov	r2,dpl
;	serial.c:59: while (!TI);
00101$:
;	serial.c:60: TI = 0; 
	jbc	_SCON_1,00108$
	sjmp	00101$
00108$:
;	serial.c:61: SBUF = h;  
	mov	_SBUF,r2
	ret
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
