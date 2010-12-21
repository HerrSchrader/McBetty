                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.7 #5820 (May  6 2010) (Linux)
                              4 ; This file was generated Mon Dec 20 23:27:41 2010
                              5 ;--------------------------------------------------------
                              6 	.module serial
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _TCR20_0
                             13 	.globl _TCR20_1
                             14 	.globl _TCR20_2
                             15 	.globl _TCR20_3
                             16 	.globl _TCR20_4
                             17 	.globl _TCR20_5
                             18 	.globl _TCR20_6
                             19 	.globl _TCR20_7
                             20 	.globl _P3_0
                             21 	.globl _P3_1
                             22 	.globl _P2_0
                             23 	.globl _P2_1
                             24 	.globl _P2_2
                             25 	.globl _P2_3
                             26 	.globl _P2_4
                             27 	.globl _P2_5
                             28 	.globl _P2_6
                             29 	.globl _P2_7
                             30 	.globl _P1_0
                             31 	.globl _P1_1
                             32 	.globl _P1_2
                             33 	.globl _P1_3
                             34 	.globl _P1_4
                             35 	.globl _P1_5
                             36 	.globl _P1_6
                             37 	.globl _P1_7
                             38 	.globl _P0_0
                             39 	.globl _P0_1
                             40 	.globl _P0_2
                             41 	.globl _P0_3
                             42 	.globl _P0_4
                             43 	.globl _P0_5
                             44 	.globl _P0_6
                             45 	.globl _P0_7
                             46 	.globl _I2CON_0
                             47 	.globl _I2CON_2
                             48 	.globl _I2CON_3
                             49 	.globl _I2CON_4
                             50 	.globl _I2CON_5
                             51 	.globl _I2CON_6
                             52 	.globl _SCON_0
                             53 	.globl _SCON_1
                             54 	.globl _SCON_2
                             55 	.globl _SCON_3
                             56 	.globl _SCON_4
                             57 	.globl _SCON_5
                             58 	.globl _SCON_6
                             59 	.globl _SCON_7
                             60 	.globl _IP0_0
                             61 	.globl _IP0_1
                             62 	.globl _IP0_2
                             63 	.globl _IP0_3
                             64 	.globl _IP0_4
                             65 	.globl _IP0_5
                             66 	.globl _IP0_6
                             67 	.globl _IEN1_0
                             68 	.globl _IEN1_1
                             69 	.globl _IEN1_2
                             70 	.globl _IEN1_3
                             71 	.globl _IEN1_4
                             72 	.globl _IEN1_6
                             73 	.globl _IEN1_7
                             74 	.globl _IEN0_0
                             75 	.globl _IEN0_1
                             76 	.globl _IEN0_2
                             77 	.globl _IEN0_3
                             78 	.globl _IEN0_4
                             79 	.globl _IEN0_5
                             80 	.globl _IEN0_6
                             81 	.globl _IEN0_7
                             82 	.globl _TCON_0
                             83 	.globl _TCON_1
                             84 	.globl _TCON_2
                             85 	.globl _TCON_3
                             86 	.globl _TCON_4
                             87 	.globl _TCON_5
                             88 	.globl _TCON_6
                             89 	.globl _TCON_7
                             90 	.globl _PSW_0
                             91 	.globl _PSW_1
                             92 	.globl _PSW_2
                             93 	.globl _PSW_3
                             94 	.globl _PSW_4
                             95 	.globl _PSW_5
                             96 	.globl _PSW_6
                             97 	.globl _PSW_7
                             98 	.globl _IP0H
                             99 	.globl _WFEED2
                            100 	.globl _WFEED1
                            101 	.globl _WDL
                            102 	.globl _WDCON
                            103 	.globl _TRIM
                            104 	.globl _TPCR2L
                            105 	.globl _TPCR2H
                            106 	.globl _TOR2L
                            107 	.globl _TOR2H
                            108 	.globl _TISE2
                            109 	.globl _TIFR2
                            110 	.globl _TICR2
                            111 	.globl _TCR21
                            112 	.globl _TCR20
                            113 	.globl _TAMOD
                            114 	.globl _SPDAT
                            115 	.globl _SPSTAT
                            116 	.globl _SPCTL
                            117 	.globl _SSTAT
                            118 	.globl _RTCL
                            119 	.globl _RTCH
                            120 	.globl _RTCCON
                            121 	.globl _RSTSRC
                            122 	.globl _PT0AD
                            123 	.globl _PCONA
                            124 	.globl _P3M2
                            125 	.globl _P3M1
                            126 	.globl _P2M2
                            127 	.globl _P2M1
                            128 	.globl _P1M2
                            129 	.globl _P1M1
                            130 	.globl _P0M2
                            131 	.globl _P0M1
                            132 	.globl _OCRDL
                            133 	.globl _OCRDH
                            134 	.globl _OCRCL
                            135 	.globl _OCRCH
                            136 	.globl _OCRBL
                            137 	.globl _OCRBH
                            138 	.globl _OCRAL
                            139 	.globl _OCRAH
                            140 	.globl _KBPATN
                            141 	.globl _KBMASK
                            142 	.globl _KBCON
                            143 	.globl _IP1H
                            144 	.globl _IP1
                            145 	.globl _IEN1
                            146 	.globl _ICRBL
                            147 	.globl _ICRBH
                            148 	.globl _ICRAL
                            149 	.globl _ICRAH
                            150 	.globl _I2STAT
                            151 	.globl _I2SCLL
                            152 	.globl _I2SCLH
                            153 	.globl _I2DAT
                            154 	.globl _I2CON
                            155 	.globl _I2ADR
                            156 	.globl _DIVM
                            157 	.globl _DEEADR
                            158 	.globl _DEEDAT
                            159 	.globl _DEECON
                            160 	.globl _CMP2
                            161 	.globl _CMP1
                            162 	.globl _CCCRD
                            163 	.globl _CCCRC
                            164 	.globl _CCCRB
                            165 	.globl _CCCRA
                            166 	.globl _BRGCON
                            167 	.globl _BRGR1
                            168 	.globl _BRGR0
                            169 	.globl _TH2
                            170 	.globl _TL2
                            171 	.globl _SADEN
                            172 	.globl _SADDR
                            173 	.globl _AUXR1
                            174 	.globl _SBUF
                            175 	.globl _SCON
                            176 	.globl _IP0
                            177 	.globl _IEN0
                            178 	.globl _TH1
                            179 	.globl _TH0
                            180 	.globl _TL1
                            181 	.globl _TL0
                            182 	.globl _TMOD
                            183 	.globl _TCON
                            184 	.globl _PCON
                            185 	.globl _DPH
                            186 	.globl _DPL
                            187 	.globl _SP
                            188 	.globl _B
                            189 	.globl _ACC
                            190 	.globl _PSW
                            191 	.globl _P3
                            192 	.globl _P2
                            193 	.globl _P1
                            194 	.globl _P0
                            195 	.globl _initSerial
                            196 	.globl _send_byte
                            197 ;--------------------------------------------------------
                            198 ; special function registers
                            199 ;--------------------------------------------------------
                            200 	.area RSEG    (ABS,DATA)
   0000                     201 	.org 0x0000
                    0080    202 _P0	=	0x0080
                    0090    203 _P1	=	0x0090
                    00A0    204 _P2	=	0x00a0
                    00B0    205 _P3	=	0x00b0
                    00D0    206 _PSW	=	0x00d0
                    00E0    207 _ACC	=	0x00e0
                    00F0    208 _B	=	0x00f0
                    0081    209 _SP	=	0x0081
                    0082    210 _DPL	=	0x0082
                    0083    211 _DPH	=	0x0083
                    0087    212 _PCON	=	0x0087
                    0088    213 _TCON	=	0x0088
                    0089    214 _TMOD	=	0x0089
                    008A    215 _TL0	=	0x008a
                    008B    216 _TL1	=	0x008b
                    008C    217 _TH0	=	0x008c
                    008D    218 _TH1	=	0x008d
                    00A8    219 _IEN0	=	0x00a8
                    00B8    220 _IP0	=	0x00b8
                    0098    221 _SCON	=	0x0098
                    0099    222 _SBUF	=	0x0099
                    00A2    223 _AUXR1	=	0x00a2
                    00A9    224 _SADDR	=	0x00a9
                    00B9    225 _SADEN	=	0x00b9
                    00CC    226 _TL2	=	0x00cc
                    00CD    227 _TH2	=	0x00cd
                    00BE    228 _BRGR0	=	0x00be
                    00BF    229 _BRGR1	=	0x00bf
                    00BD    230 _BRGCON	=	0x00bd
                    00EA    231 _CCCRA	=	0x00ea
                    00EB    232 _CCCRB	=	0x00eb
                    00EC    233 _CCCRC	=	0x00ec
                    00ED    234 _CCCRD	=	0x00ed
                    00AC    235 _CMP1	=	0x00ac
                    00AD    236 _CMP2	=	0x00ad
                    00F1    237 _DEECON	=	0x00f1
                    00F2    238 _DEEDAT	=	0x00f2
                    00F3    239 _DEEADR	=	0x00f3
                    0095    240 _DIVM	=	0x0095
                    00DB    241 _I2ADR	=	0x00db
                    00D8    242 _I2CON	=	0x00d8
                    00DA    243 _I2DAT	=	0x00da
                    00DD    244 _I2SCLH	=	0x00dd
                    00DC    245 _I2SCLL	=	0x00dc
                    00D9    246 _I2STAT	=	0x00d9
                    00AB    247 _ICRAH	=	0x00ab
                    00AA    248 _ICRAL	=	0x00aa
                    00AF    249 _ICRBH	=	0x00af
                    00AE    250 _ICRBL	=	0x00ae
                    00E8    251 _IEN1	=	0x00e8
                    00F8    252 _IP1	=	0x00f8
                    00F7    253 _IP1H	=	0x00f7
                    0094    254 _KBCON	=	0x0094
                    0086    255 _KBMASK	=	0x0086
                    0093    256 _KBPATN	=	0x0093
                    00EF    257 _OCRAH	=	0x00ef
                    00EE    258 _OCRAL	=	0x00ee
                    00FB    259 _OCRBH	=	0x00fb
                    00FA    260 _OCRBL	=	0x00fa
                    00FD    261 _OCRCH	=	0x00fd
                    00FC    262 _OCRCL	=	0x00fc
                    00FF    263 _OCRDH	=	0x00ff
                    00FE    264 _OCRDL	=	0x00fe
                    0084    265 _P0M1	=	0x0084
                    0085    266 _P0M2	=	0x0085
                    0091    267 _P1M1	=	0x0091
                    0092    268 _P1M2	=	0x0092
                    00A4    269 _P2M1	=	0x00a4
                    00A5    270 _P2M2	=	0x00a5
                    00B1    271 _P3M1	=	0x00b1
                    00B2    272 _P3M2	=	0x00b2
                    00B5    273 _PCONA	=	0x00b5
                    00F6    274 _PT0AD	=	0x00f6
                    00DF    275 _RSTSRC	=	0x00df
                    00D1    276 _RTCCON	=	0x00d1
                    00D2    277 _RTCH	=	0x00d2
                    00D3    278 _RTCL	=	0x00d3
                    00BA    279 _SSTAT	=	0x00ba
                    00E2    280 _SPCTL	=	0x00e2
                    00E1    281 _SPSTAT	=	0x00e1
                    00E3    282 _SPDAT	=	0x00e3
                    008F    283 _TAMOD	=	0x008f
                    00C8    284 _TCR20	=	0x00c8
                    00F9    285 _TCR21	=	0x00f9
                    00C9    286 _TICR2	=	0x00c9
                    00E9    287 _TIFR2	=	0x00e9
                    00DE    288 _TISE2	=	0x00de
                    00CF    289 _TOR2H	=	0x00cf
                    00CE    290 _TOR2L	=	0x00ce
                    00CB    291 _TPCR2H	=	0x00cb
                    00CA    292 _TPCR2L	=	0x00ca
                    0096    293 _TRIM	=	0x0096
                    00A7    294 _WDCON	=	0x00a7
                    00C1    295 _WDL	=	0x00c1
                    00C2    296 _WFEED1	=	0x00c2
                    00C3    297 _WFEED2	=	0x00c3
                    00B7    298 _IP0H	=	0x00b7
                            299 ;--------------------------------------------------------
                            300 ; special function bits
                            301 ;--------------------------------------------------------
                            302 	.area RSEG    (ABS,DATA)
   0000                     303 	.org 0x0000
                    00D7    304 _PSW_7	=	0x00d7
                    00D6    305 _PSW_6	=	0x00d6
                    00D5    306 _PSW_5	=	0x00d5
                    00D4    307 _PSW_4	=	0x00d4
                    00D3    308 _PSW_3	=	0x00d3
                    00D2    309 _PSW_2	=	0x00d2
                    00D1    310 _PSW_1	=	0x00d1
                    00D0    311 _PSW_0	=	0x00d0
                    008F    312 _TCON_7	=	0x008f
                    008E    313 _TCON_6	=	0x008e
                    008D    314 _TCON_5	=	0x008d
                    008C    315 _TCON_4	=	0x008c
                    008B    316 _TCON_3	=	0x008b
                    008A    317 _TCON_2	=	0x008a
                    0089    318 _TCON_1	=	0x0089
                    0088    319 _TCON_0	=	0x0088
                    00AF    320 _IEN0_7	=	0x00af
                    00AE    321 _IEN0_6	=	0x00ae
                    00AD    322 _IEN0_5	=	0x00ad
                    00AC    323 _IEN0_4	=	0x00ac
                    00AB    324 _IEN0_3	=	0x00ab
                    00AA    325 _IEN0_2	=	0x00aa
                    00A9    326 _IEN0_1	=	0x00a9
                    00A8    327 _IEN0_0	=	0x00a8
                    00EF    328 _IEN1_7	=	0x00ef
                    00EE    329 _IEN1_6	=	0x00ee
                    00EC    330 _IEN1_4	=	0x00ec
                    00EB    331 _IEN1_3	=	0x00eb
                    00EA    332 _IEN1_2	=	0x00ea
                    00E9    333 _IEN1_1	=	0x00e9
                    00E8    334 _IEN1_0	=	0x00e8
                    00BE    335 _IP0_6	=	0x00be
                    00BD    336 _IP0_5	=	0x00bd
                    00BC    337 _IP0_4	=	0x00bc
                    00BB    338 _IP0_3	=	0x00bb
                    00BA    339 _IP0_2	=	0x00ba
                    00B9    340 _IP0_1	=	0x00b9
                    00B8    341 _IP0_0	=	0x00b8
                    009F    342 _SCON_7	=	0x009f
                    009E    343 _SCON_6	=	0x009e
                    009D    344 _SCON_5	=	0x009d
                    009C    345 _SCON_4	=	0x009c
                    009B    346 _SCON_3	=	0x009b
                    009A    347 _SCON_2	=	0x009a
                    0099    348 _SCON_1	=	0x0099
                    0098    349 _SCON_0	=	0x0098
                    00DE    350 _I2CON_6	=	0x00de
                    00DD    351 _I2CON_5	=	0x00dd
                    00DC    352 _I2CON_4	=	0x00dc
                    00DB    353 _I2CON_3	=	0x00db
                    00DA    354 _I2CON_2	=	0x00da
                    00D8    355 _I2CON_0	=	0x00d8
                    0087    356 _P0_7	=	0x0087
                    0086    357 _P0_6	=	0x0086
                    0085    358 _P0_5	=	0x0085
                    0084    359 _P0_4	=	0x0084
                    0083    360 _P0_3	=	0x0083
                    0082    361 _P0_2	=	0x0082
                    0081    362 _P0_1	=	0x0081
                    0080    363 _P0_0	=	0x0080
                    0097    364 _P1_7	=	0x0097
                    0096    365 _P1_6	=	0x0096
                    0095    366 _P1_5	=	0x0095
                    0094    367 _P1_4	=	0x0094
                    0093    368 _P1_3	=	0x0093
                    0092    369 _P1_2	=	0x0092
                    0091    370 _P1_1	=	0x0091
                    0090    371 _P1_0	=	0x0090
                    00A7    372 _P2_7	=	0x00a7
                    00A6    373 _P2_6	=	0x00a6
                    00A5    374 _P2_5	=	0x00a5
                    00A4    375 _P2_4	=	0x00a4
                    00A3    376 _P2_3	=	0x00a3
                    00A2    377 _P2_2	=	0x00a2
                    00A1    378 _P2_1	=	0x00a1
                    00A0    379 _P2_0	=	0x00a0
                    00B1    380 _P3_1	=	0x00b1
                    00B0    381 _P3_0	=	0x00b0
                    00CF    382 _TCR20_7	=	0x00cf
                    00CE    383 _TCR20_6	=	0x00ce
                    00CD    384 _TCR20_5	=	0x00cd
                    00CC    385 _TCR20_4	=	0x00cc
                    00CB    386 _TCR20_3	=	0x00cb
                    00CA    387 _TCR20_2	=	0x00ca
                    00C9    388 _TCR20_1	=	0x00c9
                    00C8    389 _TCR20_0	=	0x00c8
                            390 ;--------------------------------------------------------
                            391 ; overlayable register banks
                            392 ;--------------------------------------------------------
                            393 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     394 	.ds 8
                            395 ;--------------------------------------------------------
                            396 ; internal ram data
                            397 ;--------------------------------------------------------
                            398 	.area DSEG    (DATA)
                            399 ;--------------------------------------------------------
                            400 ; overlayable items in internal ram 
                            401 ;--------------------------------------------------------
                            402 	.area	OSEG    (OVR,DATA)
                            403 	.area	OSEG    (OVR,DATA)
                            404 ;--------------------------------------------------------
                            405 ; indirectly addressable internal ram data
                            406 ;--------------------------------------------------------
                            407 	.area ISEG    (DATA)
                            408 ;--------------------------------------------------------
                            409 ; absolute internal ram data
                            410 ;--------------------------------------------------------
                            411 	.area IABS    (ABS,DATA)
                            412 	.area IABS    (ABS,DATA)
                            413 ;--------------------------------------------------------
                            414 ; bit data
                            415 ;--------------------------------------------------------
                            416 	.area BSEG    (BIT)
                            417 ;--------------------------------------------------------
                            418 ; paged external ram data
                            419 ;--------------------------------------------------------
                            420 	.area PSEG    (PAG,XDATA)
                            421 ;--------------------------------------------------------
                            422 ; external ram data
                            423 ;--------------------------------------------------------
                            424 	.area XSEG    (XDATA)
                            425 ;--------------------------------------------------------
                            426 ; absolute external ram data
                            427 ;--------------------------------------------------------
                            428 	.area XABS    (ABS,XDATA)
                            429 ;--------------------------------------------------------
                            430 ; external initialized ram data
                            431 ;--------------------------------------------------------
                            432 	.area XISEG   (XDATA)
                            433 	.area HOME    (CODE)
                            434 	.area GSINIT0 (CODE)
                            435 	.area GSINIT1 (CODE)
                            436 	.area GSINIT2 (CODE)
                            437 	.area GSINIT3 (CODE)
                            438 	.area GSINIT4 (CODE)
                            439 	.area GSINIT5 (CODE)
                            440 	.area GSINIT  (CODE)
                            441 	.area GSFINAL (CODE)
                            442 	.area CSEG    (CODE)
                            443 ;--------------------------------------------------------
                            444 ; global & static initialisations
                            445 ;--------------------------------------------------------
                            446 	.area HOME    (CODE)
                            447 	.area GSINIT  (CODE)
                            448 	.area GSFINAL (CODE)
                            449 	.area GSINIT  (CODE)
                            450 ;--------------------------------------------------------
                            451 ; Home
                            452 ;--------------------------------------------------------
                            453 	.area HOME    (CODE)
                            454 	.area HOME    (CODE)
                            455 ;--------------------------------------------------------
                            456 ; code
                            457 ;--------------------------------------------------------
                            458 	.area CSEG    (CODE)
                            459 ;------------------------------------------------------------
                            460 ;Allocation info for local variables in function 'initSerial'
                            461 ;------------------------------------------------------------
                            462 ;baud                      Allocated to registers r2 r3 
                            463 ;------------------------------------------------------------
                            464 ;	serial.c:22: void initSerial(unsigned short baud) {
                            465 ;	-----------------------------------------
                            466 ;	 function initSerial
                            467 ;	-----------------------------------------
   03B3                     468 _initSerial:
                    0002    469 	ar2 = 0x02
                    0003    470 	ar3 = 0x03
                    0004    471 	ar4 = 0x04
                    0005    472 	ar5 = 0x05
                    0006    473 	ar6 = 0x06
                    0007    474 	ar7 = 0x07
                    0000    475 	ar0 = 0x00
                    0001    476 	ar1 = 0x01
   03B3 AA 82               477 	mov	r2,dpl
   03B5 AB 83               478 	mov	r3,dph
                            479 ;	serial.c:25: SCON   = 0x52;		// 0101 0010 = Mode 8N1, enable reception, TxIntFlag=1 ? 
   03B7 75 98 52            480 	mov	_SCON,#0x52
                            481 ;	serial.c:26: SSTAT |= 0x80;		// 1000 0000 = Double buffering mode
   03BA 43 BA 80            482 	orl	_SSTAT,#0x80
                            483 ;	serial.c:27: switch (baud) {
   03BD BA 60 05            484 	cjne	r2,#0x60,00114$
   03C0 BB 00 02            485 	cjne	r3,#0x00,00114$
   03C3 80 20               486 	sjmp	00101$
   03C5                     487 00114$:
   03C5 BA C0 05            488 	cjne	r2,#0xC0,00115$
   03C8 BB 00 02            489 	cjne	r3,#0x00,00115$
   03CB 80 20               490 	sjmp	00102$
   03CD                     491 00115$:
   03CD BA 80 05            492 	cjne	r2,#0x80,00116$
   03D0 BB 01 02            493 	cjne	r3,#0x01,00116$
   03D3 80 20               494 	sjmp	00103$
   03D5                     495 00116$:
   03D5 BA 40 05            496 	cjne	r2,#0x40,00117$
   03D8 BB 02 02            497 	cjne	r3,#0x02,00117$
   03DB 80 20               498 	sjmp	00104$
   03DD                     499 00117$:
                            500 ;	serial.c:29: case 96:
   03DD BA 80 2B            501 	cjne	r2,#0x80,00106$
   03E0 BB 04 28            502 	cjne	r3,#0x04,00106$
   03E3 80 20               503 	sjmp	00105$
   03E5                     504 00101$:
                            505 ;	serial.c:30: BRGR0  = 0x88; 
   03E5 75 BE 88            506 	mov	_BRGR0,#0x88
                            507 ;	serial.c:31: BRGR1  = 0x02;
   03E8 75 BF 02            508 	mov	_BRGR1,#0x02
                            509 ;	serial.c:32: break;
                            510 ;	serial.c:33: case 192:
   03EB 80 1E               511 	sjmp	00106$
   03ED                     512 00102$:
                            513 ;	serial.c:34: BRGR0  = 0xBC; 
   03ED 75 BE BC            514 	mov	_BRGR0,#0xBC
                            515 ;	serial.c:35: BRGR1  = 0x02;
   03F0 75 BF 02            516 	mov	_BRGR1,#0x02
                            517 ;	serial.c:36: break;
                            518 ;	serial.c:37: case 384:
   03F3 80 16               519 	sjmp	00106$
   03F5                     520 00103$:
                            521 ;	serial.c:38: BRGR0  = 0x56; 
   03F5 75 BE 56            522 	mov	_BRGR0,#0x56
                            523 ;	serial.c:39: BRGR1  = 0x01;
   03F8 75 BF 01            524 	mov	_BRGR1,#0x01
                            525 ;	serial.c:40: break;
                            526 ;	serial.c:41: case 576:
   03FB 80 0E               527 	sjmp	00106$
   03FD                     528 00104$:
                            529 ;	serial.c:42: BRGR0  = 0xDE; 
   03FD 75 BE DE            530 	mov	_BRGR0,#0xDE
                            531 ;	serial.c:43: BRGR1  = 0x00;
   0400 75 BF 00            532 	mov	_BRGR1,#0x00
                            533 ;	serial.c:44: break;
                            534 ;	serial.c:45: case 1152:
   0403 80 06               535 	sjmp	00106$
   0405                     536 00105$:
                            537 ;	serial.c:46: BRGR0  = 0x67; 
   0405 75 BE 67            538 	mov	_BRGR0,#0x67
                            539 ;	serial.c:47: BRGR1  = 0x00;
   0408 75 BF 00            540 	mov	_BRGR1,#0x00
                            541 ;	serial.c:49: }
   040B                     542 00106$:
                            543 ;	serial.c:50: BRGCON = 0x03;		// Select the baud rate generator as timing source and enable it
   040B 75 BD 03            544 	mov	_BRGCON,#0x03
                            545 ;	serial.c:52: TI = 1;
   040E D2 99               546 	setb	_SCON_1
   0410 22                  547 	ret
                            548 ;------------------------------------------------------------
                            549 ;Allocation info for local variables in function 'send_byte'
                            550 ;------------------------------------------------------------
                            551 ;h                         Allocated to registers r2 
                            552 ;------------------------------------------------------------
                            553 ;	serial.c:58: void send_byte(unsigned char h) {
                            554 ;	-----------------------------------------
                            555 ;	 function send_byte
                            556 ;	-----------------------------------------
   0411                     557 _send_byte:
   0411 AA 82               558 	mov	r2,dpl
                            559 ;	serial.c:59: while (!TI);
   0413                     560 00101$:
                            561 ;	serial.c:60: TI = 0; 
   0413 10 99 02            562 	jbc	_SCON_1,00108$
   0416 80 FB               563 	sjmp	00101$
   0418                     564 00108$:
                            565 ;	serial.c:61: SBUF = h;  
   0418 8A 99               566 	mov	_SBUF,r2
   041A 22                  567 	ret
                            568 	.area CSEG    (CODE)
                            569 	.area CONST   (CODE)
                            570 	.area XINIT   (CODE)
                            571 	.area CABS    (ABS,CODE)
