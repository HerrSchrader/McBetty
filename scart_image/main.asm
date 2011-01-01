;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 2.9.7 #5820 (May  6 2010) (Linux)
; This file was generated Sat Jan  1 04:08:16 2011
;--------------------------------------------------------
	.module main
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _check_radio_input
	.globl _check_enq
	.globl _check_etx
	.globl _handle_tx
	.globl _re_enter_rx
	.globl _start_tx
	.globl _start_rx
	.globl _rx_overflow_reset
	.globl _has_room
	.globl _buffer_out
	.globl _serial_isr
	.globl _buffer_init
	.globl _feed_wd
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
	.globl _CMP2_
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
	.globl _got_enq
	.globl _got_eot
	.globl _got_etx
	.globl _buf
	.globl _bufnxt
	.globl _bufcnt
	.globl _bufstart
	.globl _radio_mode
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
_CMP2_	=	0x00ad
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
_radio_mode::
	.ds 1
_bufstart::
	.ds 1
_bufcnt::
	.ds 1
_bufnxt::
	.ds 1
_handle_tx_tx_state_1_1:
	.ds 1
_handle_tx_tx_cnt_1_1:
	.ds 1
_check_radio_input_length_1_1:
	.ds 1
_check_radio_input_address_1_1:
	.ds 1
_check_radio_input_x_1_1:
	.ds 1
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area	OSEG    (OVR,DATA)
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG	(DATA)
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
_buf::
	.ds 78
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
_got_etx::
	.ds 1
_got_eot::
	.ds 1
_got_enq::
	.ds 1
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
; interrupt vector 
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
	reti
	.ds	7
	reti
	.ds	7
	reti
	.ds	7
	reti
	.ds	7
	ljmp	_serial_isr
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
;------------------------------------------------------------
;Allocation info for local variables in function 'handle_tx'
;------------------------------------------------------------
;tx_state                  Allocated with name '_handle_tx_tx_state_1_1'
;tx_cnt                    Allocated with name '_handle_tx_tx_cnt_1_1'
;x                         Allocated to registers 
;------------------------------------------------------------
;	main.c:381: static unsigned char tx_state = TX_IDLE;
	mov	_handle_tx_tx_state_1_1,#0x00
;------------------------------------------------------------
;Allocation info for local variables in function 'check_radio_input'
;------------------------------------------------------------
;length                    Allocated with name '_check_radio_input_length_1_1'
;address                   Allocated with name '_check_radio_input_address_1_1'
;n                         Allocated to registers r3 
;x                         Allocated with name '_check_radio_input_x_1_1'
;status                    Allocated to registers r2 
;------------------------------------------------------------
;	main.c:520: static unsigned char length = 0;
	mov	_check_radio_input_length_1_1,#0x00
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	lcall	_main
;	return from main will lock up
	sjmp .
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'feed_wd'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:191: feed_wd(){
;	-----------------------------------------
;	 function feed_wd
;	-----------------------------------------
_feed_wd:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	main.c:192: EA = 0;
	clr	_IEN0_7
;	main.c:193: WFEED1 = 0xA5;
	mov	_WFEED1,#0xA5
;	main.c:194: WFEED2 = 0x5A;
	mov	_WFEED2,#0x5A
;	main.c:195: EA = 1;
	setb	_IEN0_7
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'buffer_init'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:198: void buffer_init(){
;	-----------------------------------------
;	 function buffer_init
;	-----------------------------------------
_buffer_init:
;	main.c:199: bufcnt = 0;			// Number of bytes in the buffer
	mov	_bufcnt,#0x00
;	main.c:200: bufnxt = 0;			// Index of next free place in buffer
	mov	_bufnxt,#0x00
;	main.c:201: bufstart = 0;		// Index of first data byte in buffer
	mov	_bufstart,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'serial_isr'
;------------------------------------------------------------
;x                         Allocated to registers r2 
;------------------------------------------------------------
;	main.c:231: void serial_isr (void) __interrupt (4) {
;	-----------------------------------------
;	 function serial_isr
;	-----------------------------------------
_serial_isr:
	push	acc
	push	ar2
	push	ar3
	push	ar0
	push	psw
	mov	psw,#0x00
;	main.c:234: x=SBUF;
	mov	r2,_SBUF
;	main.c:235: RI = 0;
	clr	_SCON_0
;	main.c:240: if (x == ETX){
	cjne	r2,#0x03,00102$
;	main.c:241: got_etx = 1;
	setb	_got_etx
;	main.c:242: return;
	sjmp	00116$
00102$:
;	main.c:245: if (x == ENQ){
	cjne	r2,#0x05,00104$
;	main.c:246: got_enq = 1;
	setb	_got_enq
;	main.c:247: return;
	sjmp	00116$
00104$:
;	main.c:251: if (got_eot){
	jnb	_got_eot,00106$
;	main.c:252: return;
	sjmp	00116$
00106$:
;	main.c:256: if (bufcnt >= BUFSIZE){
	mov	a,#0x100 - 0x4E
	add	a,_bufcnt
	jnc	00111$
;	main.c:258: if (bufnxt == 0)
	mov	a,_bufnxt
	jnz	00108$
;	main.c:259: bufnxt = BUFMAX;
	mov	_bufnxt,#0x4D
	sjmp	00109$
00108$:
;	main.c:261: bufnxt--;
	dec	_bufnxt
00109$:
;	main.c:262: bufcnt--;
	dec	_bufcnt
00111$:
;	main.c:266: buf[bufnxt++] = x;
	mov	r3,_bufnxt
	inc	_bufnxt
	mov	a,r3
	add	a,#_buf
	mov	r0,a
	mov	@r0,ar2
;	main.c:267: if (bufnxt > BUFMAX)
	mov	a,_bufnxt
	add	a,#0xff - 0x4D
	jnc	00113$
;	main.c:268: bufnxt = 0;
	mov	_bufnxt,#0x00
00113$:
;	main.c:269: bufcnt++;
	inc	_bufcnt
;	main.c:271: if (x == EOT)
	cjne	r2,#0x04,00115$
;	main.c:272: got_eot = 1;
	setb	_got_eot
00115$:
;	main.c:274: return;
00116$:
	pop	psw
	pop	ar0
	pop	ar3
	pop	ar2
	pop	acc
	reti
;	eliminated unneeded push/pop ar1
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;------------------------------------------------------------
;Allocation info for local variables in function 'buffer_out'
;------------------------------------------------------------
;x                         Allocated to registers r2 
;------------------------------------------------------------
;	main.c:281: char buffer_out(){
;	-----------------------------------------
;	 function buffer_out
;	-----------------------------------------
_buffer_out:
;	main.c:284: if (bufcnt == 0) return (0);
	mov	a,_bufcnt
	jnz	00102$
	mov	dpl,a
	ret
00102$:
;	main.c:285: x = buf[bufstart++];
	mov	r2,_bufstart
	inc	_bufstart
	mov	a,r2
	add	a,#_buf
	mov	r0,a
	mov	ar2,@r0
;	main.c:286: if (bufstart >= BUFSIZE)
	mov	a,#0x100 - 0x4E
	add	a,_bufstart
	jnc	00104$
;	main.c:287: bufstart = 0;
	mov	_bufstart,#0x00
00104$:
;	main.c:290: bufcnt--;
	dec	_bufcnt
;	main.c:292: return x;
	mov	dpl,r2
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'has_room'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:305: unsigned char has_room(){
;	-----------------------------------------
;	 function has_room
;	-----------------------------------------
_has_room:
;	main.c:306: return ( (BUFSIZE - bufcnt) > (MPDTOOL_PKTSIZE + 2) );
	mov	r2,_bufcnt
	mov	r3,#0x00
	mov	a,#0x4E
	clr	c
	subb	a,r2
	mov	r2,a
	clr	a
	subb	a,r3
	mov	r3,a
	clr	c
	mov	a,#0x12
	subb	a,r2
	mov	a,#(0x00 ^ 0x80)
	mov	b,r3
	xrl	b,#0x80
	subb	a,b
	clr	a
	rlc	a
	mov	dpl,a
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'rx_overflow_reset'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:318: rx_overflow_reset(){
;	-----------------------------------------
;	 function rx_overflow_reset
;	-----------------------------------------
_rx_overflow_reset:
;	main.c:319: if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) == RX_OVERFLOW){
	mov	dpl,#0xF5
	lcall	_cc1100_read_status_reg_otf
	mov	a,dpl
	anl	a,#0x1F
	mov	r2,a
	cjne	r2,#0x11,00103$
;	main.c:320: cc1100_strobe(SFRX);
	mov	dpl,#0x3A
	lcall	_cc1100_strobe
;	main.c:321: cc1100_strobe(SRX);
	mov	dpl,#0x34
	ljmp	_cc1100_strobe
00103$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'start_rx'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:327: void start_rx() {
;	-----------------------------------------
;	 function start_rx
;	-----------------------------------------
_start_rx:
;	main.c:329: rx_overflow_reset();
	lcall	_rx_overflow_reset
;	main.c:330: switch_to_idle();
	lcall	_switch_to_idle
;	main.c:332: cc1100_strobe(SFRX);
	mov	dpl,#0x3A
	lcall	_cc1100_strobe
;	main.c:333: cc1100_strobe(SCAL);
	mov	dpl,#0x33
	lcall	_cc1100_strobe
;	main.c:334: cc1100_strobe(SRX);
	mov	dpl,#0x34
	lcall	_cc1100_strobe
;	main.c:335: radio_mode = RADIO_RX;
	mov	_radio_mode,#0x01
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'start_tx'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:340: void start_tx() {	
;	-----------------------------------------
;	 function start_tx
;	-----------------------------------------
_start_tx:
;	main.c:341: rx_overflow_reset();
	lcall	_rx_overflow_reset
;	main.c:342: switch_to_idle();
	lcall	_switch_to_idle
;	main.c:344: cc1100_strobe(SCAL);
	mov	dpl,#0x33
	lcall	_cc1100_strobe
;	main.c:345: cc1100_strobe(STX);
	mov	dpl,#0x35
	lcall	_cc1100_strobe
;	main.c:346: radio_mode = RADIO_TX;
	mov	_radio_mode,#0x02
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 're_enter_rx'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:352: void re_enter_rx(){
;	-----------------------------------------
;	 function re_enter_rx
;	-----------------------------------------
_re_enter_rx:
;	main.c:353: if ((radio_mode == RADIO_TX) && tx_finished())
	mov	a,#0x02
	cjne	a,_radio_mode,00104$
	lcall	_cc1100_tx_finished
	mov	a,dpl
	jz	00104$
;	main.c:354: start_rx();
	ljmp	_start_rx
00104$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'handle_tx'
;------------------------------------------------------------
;tx_state                  Allocated with name '_handle_tx_tx_state_1_1'
;tx_cnt                    Allocated with name '_handle_tx_tx_cnt_1_1'
;x                         Allocated to registers 
;------------------------------------------------------------
;	main.c:378: void handle_tx(){
;	-----------------------------------------
;	 function handle_tx
;	-----------------------------------------
_handle_tx:
;	main.c:385: switch (tx_state){
	clr	a
	cjne	a,_handle_tx_tx_state_1_1,00121$
	sjmp	00101$
00121$:
	mov	a,#0x01
	cjne	a,_handle_tx_tx_state_1_1,00122$
	sjmp	00107$
00122$:
	mov	a,#0x02
;	main.c:387: case TX_IDLE:
	cjne	a,_handle_tx_tx_state_1_1,00113$
	sjmp	00111$
00101$:
;	main.c:389: if (got_eot) {
	jnb	_got_eot,00105$
;	main.c:390: tx_cnt = bufcnt;	/* = number of payload bytes to be transferred to TXFIFO (not counting address and length byte) */
;	main.c:393: cc1100_write1(TX_fifo, tx_cnt + 1);
	mov	a,_bufcnt
	mov	_handle_tx_tx_cnt_1_1,a
	inc	a
	mov	_cc1100_write1_PARM_2,a
	mov	dpl,#0x3F
	lcall	_cc1100_write1
;	main.c:396: cc1100_write1(TX_fifo, DEV_ADDR);	
	mov	_cc1100_write1_PARM_2,#0x01
	mov	dpl,#0x3F
	lcall	_cc1100_write1
;	main.c:398: tx_state = TX_COPY;
	mov	_handle_tx_tx_state_1_1,#0x01
;	main.c:399: got_eot=0;
	clr	_got_eot
;	main.c:401: break;
	ret
00105$:
;	main.c:403: } else if ( bufcnt >= MAX_TX_PAYLOAD ) {
	mov	a,#0x100 - 0x3C
	add	a,_bufcnt
	jnc	00113$
;	main.c:406: tx_cnt = MAX_TX_PAYLOAD;	
	mov	_handle_tx_tx_cnt_1_1,#0x3C
;	main.c:409: cc1100_write1(TX_fifo, MAX_TX_PAYLOAD + 1);
	mov	_cc1100_write1_PARM_2,#0x3D
	mov	dpl,#0x3F
	lcall	_cc1100_write1
;	main.c:412: cc1100_write1(TX_fifo, DEV_ADDR);	
	mov	_cc1100_write1_PARM_2,#0x01
	mov	dpl,#0x3F
	lcall	_cc1100_write1
;	main.c:414: tx_state = TX_COPY;
	mov	_handle_tx_tx_state_1_1,#0x01
;	main.c:416: break;
;	main.c:418: case TX_COPY:
	ret
00107$:
;	main.c:420: if (tx_cnt > 0){
	mov	a,_handle_tx_tx_cnt_1_1
	jz	00109$
;	main.c:421: x = buffer_out();
	lcall	_buffer_out
	mov	_cc1100_write1_PARM_2,dpl
;	main.c:422: cc1100_write1(TX_fifo, x);	
	mov	dpl,#0x3F
	lcall	_cc1100_write1
;	main.c:423: tx_cnt--;
	dec	_handle_tx_tx_cnt_1_1
	ret
00109$:
;	main.c:426: tx_state = TX_SEND;
	mov	_handle_tx_tx_state_1_1,#0x02
;	main.c:428: break;
;	main.c:430: case TX_SEND:
	ret
00111$:
;	main.c:431: start_tx();
	lcall	_start_tx
;	main.c:432: tx_state = TX_IDLE;
	mov	_handle_tx_tx_state_1_1,#0x00
;	main.c:434: }
00113$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'check_etx'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:438: void check_etx(){
;	-----------------------------------------
;	 function check_etx
;	-----------------------------------------
_check_etx:
;	main.c:439: if (got_etx) {				/* Is mpdtool waiting for an ACK ? */		
	jnb	_got_etx,00105$
;	main.c:440: if (has_room()){		/* Still enough space in buffer ? */
	lcall	_has_room
	mov	a,dpl
	jz	00105$
;	main.c:441: got_etx = 0;		// Atomic Operation ! (see sdcc manual)
	clr	_got_etx
;	main.c:442: send_byte(ACK);
	mov	dpl,#0x06
	ljmp	_send_byte
00105$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'check_enq'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:450: void check_enq(){
;	-----------------------------------------
;	 function check_enq
;	-----------------------------------------
_check_enq:
;	main.c:451: if (got_enq) {
;	main.c:452: got_enq = 0;		// Atomic Operation ! (see sdcc manual)
	jbc	_got_enq,00117$
	ret
00117$:
;	main.c:454: if (radio_mode == RADIO_RX){
	mov	a,#0x01
	cjne	a,_radio_mode,00107$
;	main.c:457: if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) != MARCSTATE_RX) {
	mov	dpl,#0xF5
	lcall	_cc1100_read_status_reg_otf
	mov	a,dpl
	anl	a,#0x1F
	mov	r2,a
	cjne	r2,#0x0D,00120$
	sjmp	00102$
00120$:
;	main.c:458: send_byte(NAK);
	mov	dpl,#0x15
;	main.c:459: return;
	ljmp	_send_byte
00102$:
;	main.c:461: send_byte(ASCII_SI);
	mov	dpl,#0x0F
	ljmp	_send_byte
00107$:
;	main.c:462: } else if (radio_mode == RADIO_TX)
	mov	a,#0x02
	cjne	a,_radio_mode,00104$
;	main.c:463: send_byte(SO);
	mov	dpl,#0x0E
	ljmp	_send_byte
00104$:
;	main.c:465: send_byte(DLE);
	mov	dpl,#0x10
	ljmp	_send_byte
;------------------------------------------------------------
;Allocation info for local variables in function 'check_radio_input'
;------------------------------------------------------------
;length                    Allocated with name '_check_radio_input_length_1_1'
;address                   Allocated with name '_check_radio_input_address_1_1'
;n                         Allocated to registers r3 
;x                         Allocated with name '_check_radio_input_x_1_1'
;status                    Allocated to registers r2 
;------------------------------------------------------------
;	main.c:519: check_radio_input (){
;	-----------------------------------------
;	 function check_radio_input
;	-----------------------------------------
_check_radio_input:
;	main.c:527: status = cc1100_read_rxstatus();
	lcall	_cc1100_read_rxstatus
	mov	r2,dpl
;	main.c:528: n = status & 0x0f;
	mov	a,#0x0F
	anl	a,r2
	mov	r3,a
;	main.c:531: if ( (status & STATE_MASK) == CHIP_RX_OVFL){
	mov	a,#0x70
	anl	a,r2
	mov	r4,a
	cjne	r4,#0x60,00102$
;	main.c:532: rx_overflow_reset();
	lcall	_rx_overflow_reset
;	main.c:533: return 0;
	mov	dptr,#0x0000
	ret
00102$:
;	main.c:537: if ( ( (status & STATE_MASK) != CHIP_RX) && ((status & STATE_MASK) != CHIP_IDLE) )
	mov	a,#0x70
	anl	a,r2
	mov	r4,a
	cjne	r4,#0x10,00142$
	sjmp	00104$
00142$:
	mov	a,r2
	anl	a,#0x70
	jz	00104$
;	main.c:538: return 0;
	mov	dptr,#0x0000
	ret
00104$:
;	main.c:540: if (length == 0){										/* no length byte received so far */
	mov	a,_check_radio_input_length_1_1
	jnz	00124$
;	main.c:541: if (n > 2) {
	mov	a,r3
	add	a,#0xff - 0x02
	jnc	00113$
;	main.c:542: cc1100_read(RX_fifo|BURST, &length, 1);		// Length byte = payload length + 1 address byte
	mov	_cc1100_read_PARM_2,#_check_radio_input_length_1_1
	mov	(_cc1100_read_PARM_2 + 1),#0x00
	mov	(_cc1100_read_PARM_2 + 2),#0x40
	mov	_cc1100_read_PARM_3,#0x01
	mov	dpl,#0x7F
	lcall	_cc1100_read
;	main.c:543: if (length < 1) return LEN_INVALID;
	mov	a,#0x100 - 0x01
	add	a,_check_radio_input_length_1_1
	jc	00107$
	mov	dptr,#0x0012
	ret
00107$:
;	main.c:544: if (length > MAX_LEN) return LEN_INVALID;
	mov	a,_check_radio_input_length_1_1
	add	a,#0xff - 0xFC
	jnc	00109$
	mov	dptr,#0x0012
	ret
00109$:
;	main.c:546: cc1100_read(RX_fifo|BURST, &address, 1); 	// Address byte
	mov	_cc1100_read_PARM_2,#_check_radio_input_address_1_1
	mov	(_cc1100_read_PARM_2 + 1),#0x00
	mov	(_cc1100_read_PARM_2 + 2),#0x40
	mov	_cc1100_read_PARM_3,#0x01
	mov	dpl,#0x7F
	lcall	_cc1100_read
;	main.c:547: if (address != DEV_ADDR) return ADR_INVALID;
	mov	a,#0x01
	cjne	a,_check_radio_input_address_1_1,00148$
	sjmp	00125$
00148$:
	mov	dptr,#0x0014
	ret
00113$:
;	main.c:553: } else return 0;									// not safe to read length and address
	mov	dptr,#0x0000
	ret
00124$:
;	main.c:557: if ( (status && STATE_MASK) == CHIP_IDLE) {
	mov	a,r2
	jnz	00121$
;	main.c:558: while (length > 1) {
00115$:
	mov	a,_check_radio_input_length_1_1
	add	a,#0xff - 0x01
	jnc	00117$
;	main.c:559: cc1100_read(RX_fifo|BURST, &x, 1);
	mov	_cc1100_read_PARM_2,#_check_radio_input_x_1_1
	mov	(_cc1100_read_PARM_2 + 1),#0x00
	mov	(_cc1100_read_PARM_2 + 2),#0x40
	mov	_cc1100_read_PARM_3,#0x01
	mov	dpl,#0x7F
	lcall	_cc1100_read
;	main.c:560: send_byte(x);	
	mov	dpl,_check_radio_input_x_1_1
	lcall	_send_byte
;	main.c:561: length--;
	dec	_check_radio_input_length_1_1
	sjmp	00115$
00117$:
;	main.c:563: length = 0;
	mov	_check_radio_input_length_1_1,#0x00
;	main.c:564: start_rx();	
	lcall	_start_rx
	sjmp	00125$
00121$:
;	main.c:570: if (n > 1){
	mov	a,r3
	add	a,#0xff - 0x01
	jnc	00125$
;	main.c:571: cc1100_read(RX_fifo|BURST, &x, 1);
	mov	_cc1100_read_PARM_2,#_check_radio_input_x_1_1
	mov	(_cc1100_read_PARM_2 + 1),#0x00
	mov	(_cc1100_read_PARM_2 + 2),#0x40
	mov	_cc1100_read_PARM_3,#0x01
	mov	dpl,#0x7F
	lcall	_cc1100_read
;	main.c:572: send_byte(x);	
	mov	dpl,_check_radio_input_x_1_1
	lcall	_send_byte
;	main.c:573: length--;	
	dec	_check_radio_input_length_1_1
00125$:
;	main.c:577: return 0;
	mov	dptr,#0x0000
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;------------------------------------------------------------
;	main.c:601: void main(void) {
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	main.c:611: P0M1 &= 0x6c;		// 0110 1100 -> P0M1
	anl	_P0M1,#0x6C
;	main.c:612: P0M2 |= 0x92;		// 1001 0010 -> P0M2
	orl	_P0M2,#0x92
;	main.c:614: P0 = 0x83;			// 1000 0011 -> Port0
	mov	_P0,#0x83
;	main.c:621: P1M1 &= 0x3e;		// 0011 1110
	anl	_P1M1,#0x3E
;	main.c:622: P1M2 |= 0xcd;		// 1100 1101
	orl	_P1M2,#0xCD
;	main.c:624: P1 = 0xcd;			// 1100 1101 -> Port1
	mov	_P1,#0xCD
;	main.c:628: P3M1 &= 0xfe;
	anl	_P3M1,#0xFE
;	main.c:629: P3M2 |= 0x01;
	orl	_P3M2,#0x01
;	main.c:631: P3 = 0;
	mov	_P3,#0x00
;	main.c:652: AUXR1 |= (1<<6);
	orl	_AUXR1,#0x40
;	main.c:654: RTCCON |= 1<<RTCS1;
	orl	_RTCCON,#0x40
;	main.c:655: RTCCON |= 1<<RTCS0;
	orl	_RTCCON,#0x20
;	main.c:656: RTCH = 0xff;
	mov	_RTCH,#0xFF
;	main.c:657: RTCL = 0xff;
	mov	_RTCL,#0xFF
;	main.c:659: initSerial(384);		// Serial baudrate 38400
	mov	dptr,#0x0180
	lcall	_initSerial
;	main.c:660: SSTAT |= (1<<CIDIS);	// Combined interrupt disabled, RX and TX generate different interrupts
	orl	_SSTAT,#0x20
;	main.c:662: cc1100_init();
	lcall	_cc1100_init
;	main.c:664: ESR = 1;
	setb	_IEN0_4
;	main.c:665: EA = 1;
	setb	_IEN0_7
;	main.c:725: got_etx = 0;
	clr	_got_etx
;	main.c:726: got_eot = 0;
	clr	_got_eot
;	main.c:727: got_enq = 0;
	clr	_got_enq
;	main.c:729: buffer_init();
	lcall	_buffer_init
;	main.c:731: start_rx();								// Start receiving via radio
	lcall	_start_rx
;	main.c:734: WDL = 0xFF;			// WDT counter
	mov	_WDL,#0xFF
;	main.c:735: EA = 0;
	clr	_IEN0_7
;	main.c:736: WDCON = 0xE5;		// Start WDT
	mov	_WDCON,#0xE5
;	main.c:737: WFEED1 = 0xA5;
	mov	_WFEED1,#0xA5
;	main.c:738: WFEED2 = 0x5A;
	mov	_WFEED2,#0x5A
;	main.c:739: EA = 1;
	setb	_IEN0_7
;	main.c:741: while (1) {						/* Forever: */
00107$:
;	main.c:743: feed_wd();
	lcall	_feed_wd
;	main.c:746: check_etx();
	lcall	_check_etx
;	main.c:748: check_enq();
	lcall	_check_enq
;	main.c:756: if (radio_mode == RADIO_RX)
	mov	a,#0x01
	cjne	a,_radio_mode,00102$
;	main.c:758: check_radio_input();
	lcall	_check_radio_input
	sjmp	00103$
00102$:
;	main.c:762: re_enter_rx();
	lcall	_re_enter_rx
00103$:
;	main.c:767: if (radio_mode != RADIO_TX)
	mov	a,#0x02
	cjne	a,_radio_mode,00116$
	sjmp	00107$
00116$:
;	main.c:768: handle_tx();
	lcall	_handle_tx
	sjmp	00107$
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
