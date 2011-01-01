;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 2.9.7 #5820 (May  6 2010) (Linux)
; This file was generated Sat Jan  1 04:08:16 2011
;--------------------------------------------------------
	.module cc1100
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _conf
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
	.globl _cc1100_read_PARM_3
	.globl _cc1100_read_PARM_2
	.globl _cc1100_write1_PARM_2
	.globl _cc1100_write_PARM_3
	.globl _cc1100_write_PARM_2
	.globl _spi_rw
	.globl _cc1100_init
	.globl _cc1100_write
	.globl _cc1100_write1
	.globl _cc1100_read
	.globl _cc1100_read1
	.globl _cc1100_strobe
	.globl _cc1100_read_status_reg_otf
	.globl _cc1100_read_rxstatus
	.globl _switch_to_idle
	.globl _cc1100_tx_finished
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
_cc1100_write_PARM_2:
	.ds 3
_cc1100_write_PARM_3:
	.ds 1
_cc1100_write1_PARM_2:
	.ds 1
_cc1100_read_PARM_2:
	.ds 3
_cc1100_read_PARM_3:
	.ds 1
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
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
;Allocation info for local variables in function 'spi_rw'
;------------------------------------------------------------
;write                     Allocated to registers r2 
;z                         Allocated to registers r3 
;------------------------------------------------------------
;	cc1100.c:114: unsigned char spi_rw(unsigned char write) {
;	-----------------------------------------
;	 function spi_rw
;	-----------------------------------------
_spi_rw:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
	mov	r2,dpl
;	cc1100.c:118: for (z= 8; z > 0; z--) {
	mov	r3,#0x08
00106$:
	mov	a,r3
	jz	00112$
	mov	r4,#0x01
	sjmp	00113$
00112$:
	mov	r4,#0x00
00113$:
	mov	a,r4
	jz	00109$
;	cc1100.c:119: SCK = 0;
	clr	_P1_7
;	cc1100.c:120: if (write & 0x80)
	mov	a,r2
	jnb	acc.7,00102$
;	cc1100.c:121: MOSI1 = 1;
	setb	_P1_6
	sjmp	00103$
00102$:
;	cc1100.c:123: MOSI1 = 0;
	clr	_P1_6
00103$:
;	cc1100.c:124: SCK = 1;
	setb	_P1_7
;	cc1100.c:125: write <<=1;
;	cc1100.c:126: if (MISO1)
;	cc1100.c:127: write |= 0x01;
	mov	a,r2
	mov	c,_P1_5
	addc	a,r2
	mov	r2,a
;	cc1100.c:118: for (z= 8; z > 0; z--) {
	dec	r3
	sjmp	00106$
00109$:
;	cc1100.c:129: SCK = 0;
	clr	_P1_7
;	cc1100.c:131: return(write);  
	mov	dpl,r2
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_init'
;------------------------------------------------------------
;i                         Allocated to registers r2 
;------------------------------------------------------------
;	cc1100.c:135: void cc1100_init(void) {
;	-----------------------------------------
;	 function cc1100_init
;	-----------------------------------------
_cc1100_init:
;	cc1100.c:139: SCK = 1;
	setb	_P1_7
;	cc1100.c:140: MOSI1 = 0;
	clr	_P1_6
;	cc1100.c:141: CS = 0;
	clr	_P0_1
;	cc1100.c:142: while(i) {
	mov	r2,#0xFF
00101$:
	mov	a,r2
	jz	00103$
;	cc1100.c:143: i--;
	dec	r2
	sjmp	00101$
00103$:
;	cc1100.c:145: CS = 1;
	setb	_P0_1
;	cc1100.c:147: while(i) {
	mov	r2,#0xFF
00104$:
	mov	a,r2
	jz	00106$
;	cc1100.c:148: i--;
	dec	r2
	sjmp	00104$
00106$:
;	cc1100.c:150: CS = 0;
	clr	_P0_1
;	cc1100.c:151: SCK = 0; 
	clr	_P1_7
;	cc1100.c:152: while (MISO1);  
00107$:
	jb	_P1_5,00107$
;	cc1100.c:153: spi_rw(SRES);
	mov	dpl,#0x30
	lcall	_spi_rw
;	cc1100.c:154: while (MISO1);
00110$:
	jb	_P1_5,00110$
;	cc1100.c:156: cc1100_write(0x00, conf, 0x2f);
	mov	_cc1100_write_PARM_2,#_conf
	mov	(_cc1100_write_PARM_2 + 1),#(_conf >> 8)
	mov	(_cc1100_write_PARM_2 + 2),#0x80
	mov	_cc1100_write_PARM_3,#0x2F
	mov	dpl,#0x00
	lcall	_cc1100_write
;	cc1100.c:157: cc1100_write1(PATABLE, PA_VALUE);	
	mov	_cc1100_write1_PARM_2,#0x60
	mov	dpl,#0x3E
	ljmp	_cc1100_write1
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_write'
;------------------------------------------------------------
;dat                       Allocated with name '_cc1100_write_PARM_2'
;length                    Allocated with name '_cc1100_write_PARM_3'
;addr                      Allocated to registers r2 
;i                         Allocated to registers r3 
;status                    Allocated to registers r2 
;------------------------------------------------------------
;	cc1100.c:161: unsigned char cc1100_write(unsigned char addr, unsigned char* dat, unsigned char length) {
;	-----------------------------------------
;	 function cc1100_write
;	-----------------------------------------
_cc1100_write:
	mov	r2,dpl
;	cc1100.c:166: CS = 0;
	clr	_P0_1
;	cc1100.c:167: while (MISO1);
00101$:
	jb	_P1_5,00101$
;	cc1100.c:168: status = spi_rw(addr | WRITE | BURST);
	mov	a,#0x40
	orl	a,r2
	mov	dpl,a
	lcall	_spi_rw
	mov	r2,dpl
;	cc1100.c:169: for (i=0; i < length; i++) 
	mov	r3,#0x00
00104$:
	clr	c
	mov	a,r3
	subb	a,_cc1100_write_PARM_3
	jnc	00107$
;	cc1100.c:170: spi_rw(dat[i]); 
	mov	a,r3
	add	a,_cc1100_write_PARM_2
	mov	r4,a
	clr	a
	addc	a,(_cc1100_write_PARM_2 + 1)
	mov	r5,a
	mov	r6,(_cc1100_write_PARM_2 + 2)
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	dpl,a
	push	ar2
	push	ar3
	lcall	_spi_rw
	pop	ar3
	pop	ar2
;	cc1100.c:169: for (i=0; i < length; i++) 
	inc	r3
	sjmp	00104$
00107$:
;	cc1100.c:171: CS = 1;
	setb	_P0_1
;	cc1100.c:173: return(status);
	mov	dpl,r2
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_write1'
;------------------------------------------------------------
;dat                       Allocated with name '_cc1100_write1_PARM_2'
;addr                      Allocated to registers r2 
;status                    Allocated to registers r2 
;------------------------------------------------------------
;	cc1100.c:176: unsigned char cc1100_write1(unsigned char addr,unsigned char dat) {
;	-----------------------------------------
;	 function cc1100_write1
;	-----------------------------------------
_cc1100_write1:
	mov	r2,dpl
;	cc1100.c:180: CS = 0;
	clr	_P0_1
;	cc1100.c:181: while (MISO1);
00101$:
	jb	_P1_5,00101$
;	cc1100.c:182: status = spi_rw(addr | WRITE); 
	mov	dpl,r2
	lcall	_spi_rw
	mov	r2,dpl
;	cc1100.c:183: spi_rw(dat); 
	mov	dpl,_cc1100_write1_PARM_2
	push	ar2
	lcall	_spi_rw
	pop	ar2
;	cc1100.c:184: CS = 1;
	setb	_P0_1
;	cc1100.c:186: return(status);
	mov	dpl,r2
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_read'
;------------------------------------------------------------
;dat                       Allocated with name '_cc1100_read_PARM_2'
;length                    Allocated with name '_cc1100_read_PARM_3'
;addr                      Allocated to registers r2 
;i                         Allocated to registers r3 
;status                    Allocated to registers r2 
;------------------------------------------------------------
;	cc1100.c:189: unsigned char cc1100_read(unsigned char addr, unsigned char* dat, unsigned char length) {
;	-----------------------------------------
;	 function cc1100_read
;	-----------------------------------------
_cc1100_read:
	mov	r2,dpl
;	cc1100.c:194: CS = 0;
	clr	_P0_1
;	cc1100.c:195: while (MISO1);
00101$:
	jb	_P1_5,00101$
;	cc1100.c:196: status = spi_rw(addr | READ);
	mov	a,#0x80
	orl	a,r2
	mov	dpl,a
	lcall	_spi_rw
	mov	r2,dpl
;	cc1100.c:197: for (i=0; i < length; i++)
	mov	r3,#0x00
00104$:
	clr	c
	mov	a,r3
	subb	a,_cc1100_read_PARM_3
	jnc	00107$
;	cc1100.c:198: dat[i]=spi_rw(0x00);
	mov	a,r3
	add	a,_cc1100_read_PARM_2
	mov	r4,a
	clr	a
	addc	a,(_cc1100_read_PARM_2 + 1)
	mov	r5,a
	mov	r6,(_cc1100_read_PARM_2 + 2)
	mov	dpl,#0x00
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	lcall	_spi_rw
	mov	r7,dpl
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r7
	lcall	__gptrput
;	cc1100.c:197: for (i=0; i < length; i++)
	inc	r3
	sjmp	00104$
00107$:
;	cc1100.c:199: CS = 1;
	setb	_P0_1
;	cc1100.c:201: return(status);
	mov	dpl,r2
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_read1'
;------------------------------------------------------------
;addr                      Allocated to registers r2 
;r                         Allocated to registers 
;------------------------------------------------------------
;	cc1100.c:204: unsigned char cc1100_read1(unsigned char addr) {
;	-----------------------------------------
;	 function cc1100_read1
;	-----------------------------------------
_cc1100_read1:
	mov	r2,dpl
;	cc1100.c:208: CS = 0;
	clr	_P0_1
;	cc1100.c:209: while (MISO1);
00101$:
	jb	_P1_5,00101$
;	cc1100.c:210: r = spi_rw(addr | READ);
	mov	a,#0x80
	orl	a,r2
	mov	dpl,a
	lcall	_spi_rw
;	cc1100.c:211: r=spi_rw(0x00);
	mov	dpl,#0x00
	lcall	_spi_rw
;	cc1100.c:212: CS = 1;
	setb	_P0_1
;	cc1100.c:214: return(r);
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_strobe'
;------------------------------------------------------------
;cmd                       Allocated to registers r2 
;status                    Allocated to registers 
;------------------------------------------------------------
;	cc1100.c:217: unsigned char cc1100_strobe(unsigned char cmd) {
;	-----------------------------------------
;	 function cc1100_strobe
;	-----------------------------------------
_cc1100_strobe:
	mov	r2,dpl
;	cc1100.c:221: CS = 0;
	clr	_P0_1
;	cc1100.c:222: while (MISO1);
00101$:
	jb	_P1_5,00101$
;	cc1100.c:223: status = spi_rw(cmd);
	mov	dpl,r2
	lcall	_spi_rw
;	cc1100.c:224: CS = 1;
	setb	_P0_1
;	cc1100.c:226: return(status);
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_read_status_reg_otf'
;------------------------------------------------------------
;reg                       Allocated to registers r2 
;res1                      Allocated to registers r3 
;res2                      Allocated to registers r5 
;------------------------------------------------------------
;	cc1100.c:233: unsigned char cc1100_read_status_reg_otf(unsigned char reg){
;	-----------------------------------------
;	 function cc1100_read_status_reg_otf
;	-----------------------------------------
_cc1100_read_status_reg_otf:
	mov	r2,dpl
;	cc1100.c:236: res1 = cc1100_read1(reg | BURST);
	mov	a,#0x40
	orl	a,r2
	mov	dpl,a
	push	ar2
	lcall	_cc1100_read1
	mov	r3,dpl
	pop	ar2
;	cc1100.c:237: while ( (res2=cc1100_read1(reg | BURST)) != res1)
	orl	ar2,#0x40
00101$:
	mov	dpl,r2
	push	ar2
	push	ar3
	lcall	_cc1100_read1
	mov	r4,dpl
	pop	ar3
	pop	ar2
	mov	a,r4
	mov	r5,a
	cjne	a,ar3,00108$
	sjmp	00103$
00108$:
;	cc1100.c:238: res1 = res2;
	mov	ar3,r5
	sjmp	00101$
00103$:
;	cc1100.c:239: return res2;
	mov	dpl,r5
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_read_rxstatus'
;------------------------------------------------------------
;res1                      Allocated to registers r2 
;res2                      Allocated to registers r4 
;------------------------------------------------------------
;	cc1100.c:247: cc1100_read_rxstatus(){
;	-----------------------------------------
;	 function cc1100_read_rxstatus
;	-----------------------------------------
_cc1100_read_rxstatus:
;	cc1100.c:250: res1 = cc1100_strobe(SNOP | READ);
	mov	dpl,#0xBD
	lcall	_cc1100_strobe
	mov	r2,dpl
;	cc1100.c:251: while ( (res2=cc1100_strobe(SNOP | READ)) != res1)
00101$:
	mov	dpl,#0xBD
	push	ar2
	lcall	_cc1100_strobe
	mov	r3,dpl
	pop	ar2
	mov	a,r3
	mov	r4,a
	cjne	a,ar2,00108$
	sjmp	00103$
00108$:
;	cc1100.c:252: res1 = res2;
	mov	ar2,r4
	sjmp	00101$
00103$:
;	cc1100.c:253: return res2;
	mov	dpl,r4
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'switch_to_idle'
;------------------------------------------------------------
;------------------------------------------------------------
;	cc1100.c:259: void switch_to_idle() {
;	-----------------------------------------
;	 function switch_to_idle
;	-----------------------------------------
_switch_to_idle:
;	cc1100.c:260: cc1100_strobe(SIDLE);
	mov	dpl,#0x36
	lcall	_cc1100_strobe
;	cc1100.c:261: while (cc1100_marcstate() != MARCSTATE_IDLE);
00101$:
	mov	dpl,#0xF5
	lcall	_cc1100_read_status_reg_otf
	mov	a,dpl
	anl	a,#0x1F
	mov	r2,a
	cjne	r2,#0x01,00101$
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'cc1100_tx_finished'
;------------------------------------------------------------
;s                         Allocated to registers r2 
;------------------------------------------------------------
;	cc1100.c:273: cc1100_tx_finished(){
;	-----------------------------------------
;	 function cc1100_tx_finished
;	-----------------------------------------
_cc1100_tx_finished:
;	cc1100.c:276: s = cc1100_marcstate();
	mov	dpl,#0xF5
	lcall	_cc1100_read_status_reg_otf
	mov	a,dpl
	anl	a,#0x1F
	mov	r2,a
;	cc1100.c:277: return ( (s == MARCSTATE_IDLE ) || (s == MARCSTATE_RX) || (s == MARCSTATE_RXFIFO_OVERFLOW) 
	cjne	r2,#0x01,00118$
	sjmp	00110$
00118$:
	cjne	r2,#0x0D,00119$
	sjmp	00110$
00119$:
	mov	r3,#0x00
	sjmp	00111$
00110$:
	mov	r3,#0x01
00111$:
	mov	a,r3
	jnz	00107$
	cjne	r2,#0x11,00121$
	sjmp	00107$
00121$:
	mov	r3,#0x00
	sjmp	00108$
00107$:
	mov	r3,#0x01
00108$:
	mov	a,r3
	jnz	00104$
;	cc1100.c:278: || (s == MARCSTATE_TXFIFO_UNDERFLOW)
	cjne	r2,#0x16,00123$
	sjmp	00104$
00123$:
	mov	r2,#0x00
	sjmp	00105$
00104$:
	mov	r2,#0x01
00105$:
	mov	dpl,r2
	ret
	.area CSEG    (CODE)
	.area CONST   (CODE)
_conf:
	.db #0x06	; 6
	.db #0x2E	; 46
	.db #0x07	; 7
	.db #0x00	; 0
	.db #0xD3	; 211
	.db #0x91	; 145
	.db #0xFF	; 255
	.db #0x06	; 6
	.db #0x45	; 69	E
	.db #0x01	; 1
	.db #0x06	; 6
	.db #0x08	; 8
	.db #0x00	; 0
	.db #0x10	; 16
	.db #0x09	; 9
	.db #0xF4	; 244
	.db #0xCA	; 202
	.db #0x75	; 117	u
	.db #0x83	; 131
	.db #0x31	; 49
	.db #0xE5	; 229
	.db #0x34	; 52
	.db #0x07	; 7
	.db #0x00	; 0
	.db #0x18	; 24
	.db #0x16	; 22
	.db #0x6C	; 108	l
	.db #0x43	; 67	C
	.db #0x40	; 64
	.db #0x91	; 145
	.db #0x46	; 70	F
	.db #0x50	; 80	P
	.db #0x78	; 120	x
	.db #0x56	; 86	V
	.db #0x10	; 16
	.db #0xE9	; 233
	.db #0x2A	; 42
	.db #0x00	; 0
	.db #0x1F	; 31
	.db #0x41	; 65	A
	.db #0x00	; 0
	.db #0x59	; 89	Y
	.db #0x7F	; 127
	.db #0x3F	; 63
	.db #0x81	; 129
	.db #0x35	; 53
	.db #0x09	; 9
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
