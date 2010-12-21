                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.7 #5820 (May  6 2010) (Linux)
                              4 ; This file was generated Tue Dec 21 00:37:19 2010
                              5 ;--------------------------------------------------------
                              6 	.module main
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _main
                             13 	.globl _check_radio_input
                             14 	.globl _check_radio_packet
                             15 	.globl _check_etx
                             16 	.globl _handle_tx
                             17 	.globl _re_enter_rx
                             18 	.globl _tx_finished
                             19 	.globl _start_tx
                             20 	.globl _start_rx
                             21 	.globl _rx_overflow_reset
                             22 	.globl _has_room
                             23 	.globl _buffer_out
                             24 	.globl _serial_isr
                             25 	.globl _buffer_init
                             26 	.globl _feed_wd
                             27 	.globl _TCR20_0
                             28 	.globl _TCR20_1
                             29 	.globl _TCR20_2
                             30 	.globl _TCR20_3
                             31 	.globl _TCR20_4
                             32 	.globl _TCR20_5
                             33 	.globl _TCR20_6
                             34 	.globl _TCR20_7
                             35 	.globl _P3_0
                             36 	.globl _P3_1
                             37 	.globl _P2_0
                             38 	.globl _P2_1
                             39 	.globl _P2_2
                             40 	.globl _P2_3
                             41 	.globl _P2_4
                             42 	.globl _P2_5
                             43 	.globl _P2_6
                             44 	.globl _P2_7
                             45 	.globl _P1_0
                             46 	.globl _P1_1
                             47 	.globl _P1_2
                             48 	.globl _P1_3
                             49 	.globl _P1_4
                             50 	.globl _P1_5
                             51 	.globl _P1_6
                             52 	.globl _P1_7
                             53 	.globl _P0_0
                             54 	.globl _P0_1
                             55 	.globl _P0_2
                             56 	.globl _P0_3
                             57 	.globl _P0_4
                             58 	.globl _P0_5
                             59 	.globl _P0_6
                             60 	.globl _P0_7
                             61 	.globl _I2CON_0
                             62 	.globl _I2CON_2
                             63 	.globl _I2CON_3
                             64 	.globl _I2CON_4
                             65 	.globl _I2CON_5
                             66 	.globl _I2CON_6
                             67 	.globl _SCON_0
                             68 	.globl _SCON_1
                             69 	.globl _SCON_2
                             70 	.globl _SCON_3
                             71 	.globl _SCON_4
                             72 	.globl _SCON_5
                             73 	.globl _SCON_6
                             74 	.globl _SCON_7
                             75 	.globl _IP0_0
                             76 	.globl _IP0_1
                             77 	.globl _IP0_2
                             78 	.globl _IP0_3
                             79 	.globl _IP0_4
                             80 	.globl _IP0_5
                             81 	.globl _IP0_6
                             82 	.globl _IEN1_0
                             83 	.globl _IEN1_1
                             84 	.globl _IEN1_2
                             85 	.globl _IEN1_3
                             86 	.globl _IEN1_4
                             87 	.globl _IEN1_6
                             88 	.globl _IEN1_7
                             89 	.globl _IEN0_0
                             90 	.globl _IEN0_1
                             91 	.globl _IEN0_2
                             92 	.globl _IEN0_3
                             93 	.globl _IEN0_4
                             94 	.globl _IEN0_5
                             95 	.globl _IEN0_6
                             96 	.globl _IEN0_7
                             97 	.globl _TCON_0
                             98 	.globl _TCON_1
                             99 	.globl _TCON_2
                            100 	.globl _TCON_3
                            101 	.globl _TCON_4
                            102 	.globl _TCON_5
                            103 	.globl _TCON_6
                            104 	.globl _TCON_7
                            105 	.globl _PSW_0
                            106 	.globl _PSW_1
                            107 	.globl _PSW_2
                            108 	.globl _PSW_3
                            109 	.globl _PSW_4
                            110 	.globl _PSW_5
                            111 	.globl _PSW_6
                            112 	.globl _PSW_7
                            113 	.globl _CMP2_
                            114 	.globl _IP0H
                            115 	.globl _WFEED2
                            116 	.globl _WFEED1
                            117 	.globl _WDL
                            118 	.globl _WDCON
                            119 	.globl _TRIM
                            120 	.globl _TPCR2L
                            121 	.globl _TPCR2H
                            122 	.globl _TOR2L
                            123 	.globl _TOR2H
                            124 	.globl _TISE2
                            125 	.globl _TIFR2
                            126 	.globl _TICR2
                            127 	.globl _TCR21
                            128 	.globl _TCR20
                            129 	.globl _TAMOD
                            130 	.globl _SPDAT
                            131 	.globl _SPSTAT
                            132 	.globl _SPCTL
                            133 	.globl _SSTAT
                            134 	.globl _RTCL
                            135 	.globl _RTCH
                            136 	.globl _RTCCON
                            137 	.globl _RSTSRC
                            138 	.globl _PT0AD
                            139 	.globl _PCONA
                            140 	.globl _P3M2
                            141 	.globl _P3M1
                            142 	.globl _P2M2
                            143 	.globl _P2M1
                            144 	.globl _P1M2
                            145 	.globl _P1M1
                            146 	.globl _P0M2
                            147 	.globl _P0M1
                            148 	.globl _OCRDL
                            149 	.globl _OCRDH
                            150 	.globl _OCRCL
                            151 	.globl _OCRCH
                            152 	.globl _OCRBL
                            153 	.globl _OCRBH
                            154 	.globl _OCRAL
                            155 	.globl _OCRAH
                            156 	.globl _KBPATN
                            157 	.globl _KBMASK
                            158 	.globl _KBCON
                            159 	.globl _IP1H
                            160 	.globl _IP1
                            161 	.globl _IEN1
                            162 	.globl _ICRBL
                            163 	.globl _ICRBH
                            164 	.globl _ICRAL
                            165 	.globl _ICRAH
                            166 	.globl _I2STAT
                            167 	.globl _I2SCLL
                            168 	.globl _I2SCLH
                            169 	.globl _I2DAT
                            170 	.globl _I2CON
                            171 	.globl _I2ADR
                            172 	.globl _DIVM
                            173 	.globl _DEEADR
                            174 	.globl _DEEDAT
                            175 	.globl _DEECON
                            176 	.globl _CMP2
                            177 	.globl _CMP1
                            178 	.globl _CCCRD
                            179 	.globl _CCCRC
                            180 	.globl _CCCRB
                            181 	.globl _CCCRA
                            182 	.globl _BRGCON
                            183 	.globl _BRGR1
                            184 	.globl _BRGR0
                            185 	.globl _TH2
                            186 	.globl _TL2
                            187 	.globl _SADEN
                            188 	.globl _SADDR
                            189 	.globl _AUXR1
                            190 	.globl _SBUF
                            191 	.globl _SCON
                            192 	.globl _IP0
                            193 	.globl _IEN0
                            194 	.globl _TH1
                            195 	.globl _TH0
                            196 	.globl _TL1
                            197 	.globl _TL0
                            198 	.globl _TMOD
                            199 	.globl _TCON
                            200 	.globl _PCON
                            201 	.globl _DPH
                            202 	.globl _DPL
                            203 	.globl _SP
                            204 	.globl _B
                            205 	.globl _ACC
                            206 	.globl _PSW
                            207 	.globl _P3
                            208 	.globl _P2
                            209 	.globl _P1
                            210 	.globl _P0
                            211 	.globl _got_eot
                            212 	.globl _got_etx
                            213 	.globl _buf
                            214 	.globl _bufnxt
                            215 	.globl _bufcnt
                            216 	.globl _bufstart
                            217 	.globl _radio_mode
                            218 ;--------------------------------------------------------
                            219 ; special function registers
                            220 ;--------------------------------------------------------
                            221 	.area RSEG    (ABS,DATA)
   0000                     222 	.org 0x0000
                    0080    223 _P0	=	0x0080
                    0090    224 _P1	=	0x0090
                    00A0    225 _P2	=	0x00a0
                    00B0    226 _P3	=	0x00b0
                    00D0    227 _PSW	=	0x00d0
                    00E0    228 _ACC	=	0x00e0
                    00F0    229 _B	=	0x00f0
                    0081    230 _SP	=	0x0081
                    0082    231 _DPL	=	0x0082
                    0083    232 _DPH	=	0x0083
                    0087    233 _PCON	=	0x0087
                    0088    234 _TCON	=	0x0088
                    0089    235 _TMOD	=	0x0089
                    008A    236 _TL0	=	0x008a
                    008B    237 _TL1	=	0x008b
                    008C    238 _TH0	=	0x008c
                    008D    239 _TH1	=	0x008d
                    00A8    240 _IEN0	=	0x00a8
                    00B8    241 _IP0	=	0x00b8
                    0098    242 _SCON	=	0x0098
                    0099    243 _SBUF	=	0x0099
                    00A2    244 _AUXR1	=	0x00a2
                    00A9    245 _SADDR	=	0x00a9
                    00B9    246 _SADEN	=	0x00b9
                    00CC    247 _TL2	=	0x00cc
                    00CD    248 _TH2	=	0x00cd
                    00BE    249 _BRGR0	=	0x00be
                    00BF    250 _BRGR1	=	0x00bf
                    00BD    251 _BRGCON	=	0x00bd
                    00EA    252 _CCCRA	=	0x00ea
                    00EB    253 _CCCRB	=	0x00eb
                    00EC    254 _CCCRC	=	0x00ec
                    00ED    255 _CCCRD	=	0x00ed
                    00AC    256 _CMP1	=	0x00ac
                    00AD    257 _CMP2	=	0x00ad
                    00F1    258 _DEECON	=	0x00f1
                    00F2    259 _DEEDAT	=	0x00f2
                    00F3    260 _DEEADR	=	0x00f3
                    0095    261 _DIVM	=	0x0095
                    00DB    262 _I2ADR	=	0x00db
                    00D8    263 _I2CON	=	0x00d8
                    00DA    264 _I2DAT	=	0x00da
                    00DD    265 _I2SCLH	=	0x00dd
                    00DC    266 _I2SCLL	=	0x00dc
                    00D9    267 _I2STAT	=	0x00d9
                    00AB    268 _ICRAH	=	0x00ab
                    00AA    269 _ICRAL	=	0x00aa
                    00AF    270 _ICRBH	=	0x00af
                    00AE    271 _ICRBL	=	0x00ae
                    00E8    272 _IEN1	=	0x00e8
                    00F8    273 _IP1	=	0x00f8
                    00F7    274 _IP1H	=	0x00f7
                    0094    275 _KBCON	=	0x0094
                    0086    276 _KBMASK	=	0x0086
                    0093    277 _KBPATN	=	0x0093
                    00EF    278 _OCRAH	=	0x00ef
                    00EE    279 _OCRAL	=	0x00ee
                    00FB    280 _OCRBH	=	0x00fb
                    00FA    281 _OCRBL	=	0x00fa
                    00FD    282 _OCRCH	=	0x00fd
                    00FC    283 _OCRCL	=	0x00fc
                    00FF    284 _OCRDH	=	0x00ff
                    00FE    285 _OCRDL	=	0x00fe
                    0084    286 _P0M1	=	0x0084
                    0085    287 _P0M2	=	0x0085
                    0091    288 _P1M1	=	0x0091
                    0092    289 _P1M2	=	0x0092
                    00A4    290 _P2M1	=	0x00a4
                    00A5    291 _P2M2	=	0x00a5
                    00B1    292 _P3M1	=	0x00b1
                    00B2    293 _P3M2	=	0x00b2
                    00B5    294 _PCONA	=	0x00b5
                    00F6    295 _PT0AD	=	0x00f6
                    00DF    296 _RSTSRC	=	0x00df
                    00D1    297 _RTCCON	=	0x00d1
                    00D2    298 _RTCH	=	0x00d2
                    00D3    299 _RTCL	=	0x00d3
                    00BA    300 _SSTAT	=	0x00ba
                    00E2    301 _SPCTL	=	0x00e2
                    00E1    302 _SPSTAT	=	0x00e1
                    00E3    303 _SPDAT	=	0x00e3
                    008F    304 _TAMOD	=	0x008f
                    00C8    305 _TCR20	=	0x00c8
                    00F9    306 _TCR21	=	0x00f9
                    00C9    307 _TICR2	=	0x00c9
                    00E9    308 _TIFR2	=	0x00e9
                    00DE    309 _TISE2	=	0x00de
                    00CF    310 _TOR2H	=	0x00cf
                    00CE    311 _TOR2L	=	0x00ce
                    00CB    312 _TPCR2H	=	0x00cb
                    00CA    313 _TPCR2L	=	0x00ca
                    0096    314 _TRIM	=	0x0096
                    00A7    315 _WDCON	=	0x00a7
                    00C1    316 _WDL	=	0x00c1
                    00C2    317 _WFEED1	=	0x00c2
                    00C3    318 _WFEED2	=	0x00c3
                    00B7    319 _IP0H	=	0x00b7
                    00AD    320 _CMP2_	=	0x00ad
                            321 ;--------------------------------------------------------
                            322 ; special function bits
                            323 ;--------------------------------------------------------
                            324 	.area RSEG    (ABS,DATA)
   0000                     325 	.org 0x0000
                    00D7    326 _PSW_7	=	0x00d7
                    00D6    327 _PSW_6	=	0x00d6
                    00D5    328 _PSW_5	=	0x00d5
                    00D4    329 _PSW_4	=	0x00d4
                    00D3    330 _PSW_3	=	0x00d3
                    00D2    331 _PSW_2	=	0x00d2
                    00D1    332 _PSW_1	=	0x00d1
                    00D0    333 _PSW_0	=	0x00d0
                    008F    334 _TCON_7	=	0x008f
                    008E    335 _TCON_6	=	0x008e
                    008D    336 _TCON_5	=	0x008d
                    008C    337 _TCON_4	=	0x008c
                    008B    338 _TCON_3	=	0x008b
                    008A    339 _TCON_2	=	0x008a
                    0089    340 _TCON_1	=	0x0089
                    0088    341 _TCON_0	=	0x0088
                    00AF    342 _IEN0_7	=	0x00af
                    00AE    343 _IEN0_6	=	0x00ae
                    00AD    344 _IEN0_5	=	0x00ad
                    00AC    345 _IEN0_4	=	0x00ac
                    00AB    346 _IEN0_3	=	0x00ab
                    00AA    347 _IEN0_2	=	0x00aa
                    00A9    348 _IEN0_1	=	0x00a9
                    00A8    349 _IEN0_0	=	0x00a8
                    00EF    350 _IEN1_7	=	0x00ef
                    00EE    351 _IEN1_6	=	0x00ee
                    00EC    352 _IEN1_4	=	0x00ec
                    00EB    353 _IEN1_3	=	0x00eb
                    00EA    354 _IEN1_2	=	0x00ea
                    00E9    355 _IEN1_1	=	0x00e9
                    00E8    356 _IEN1_0	=	0x00e8
                    00BE    357 _IP0_6	=	0x00be
                    00BD    358 _IP0_5	=	0x00bd
                    00BC    359 _IP0_4	=	0x00bc
                    00BB    360 _IP0_3	=	0x00bb
                    00BA    361 _IP0_2	=	0x00ba
                    00B9    362 _IP0_1	=	0x00b9
                    00B8    363 _IP0_0	=	0x00b8
                    009F    364 _SCON_7	=	0x009f
                    009E    365 _SCON_6	=	0x009e
                    009D    366 _SCON_5	=	0x009d
                    009C    367 _SCON_4	=	0x009c
                    009B    368 _SCON_3	=	0x009b
                    009A    369 _SCON_2	=	0x009a
                    0099    370 _SCON_1	=	0x0099
                    0098    371 _SCON_0	=	0x0098
                    00DE    372 _I2CON_6	=	0x00de
                    00DD    373 _I2CON_5	=	0x00dd
                    00DC    374 _I2CON_4	=	0x00dc
                    00DB    375 _I2CON_3	=	0x00db
                    00DA    376 _I2CON_2	=	0x00da
                    00D8    377 _I2CON_0	=	0x00d8
                    0087    378 _P0_7	=	0x0087
                    0086    379 _P0_6	=	0x0086
                    0085    380 _P0_5	=	0x0085
                    0084    381 _P0_4	=	0x0084
                    0083    382 _P0_3	=	0x0083
                    0082    383 _P0_2	=	0x0082
                    0081    384 _P0_1	=	0x0081
                    0080    385 _P0_0	=	0x0080
                    0097    386 _P1_7	=	0x0097
                    0096    387 _P1_6	=	0x0096
                    0095    388 _P1_5	=	0x0095
                    0094    389 _P1_4	=	0x0094
                    0093    390 _P1_3	=	0x0093
                    0092    391 _P1_2	=	0x0092
                    0091    392 _P1_1	=	0x0091
                    0090    393 _P1_0	=	0x0090
                    00A7    394 _P2_7	=	0x00a7
                    00A6    395 _P2_6	=	0x00a6
                    00A5    396 _P2_5	=	0x00a5
                    00A4    397 _P2_4	=	0x00a4
                    00A3    398 _P2_3	=	0x00a3
                    00A2    399 _P2_2	=	0x00a2
                    00A1    400 _P2_1	=	0x00a1
                    00A0    401 _P2_0	=	0x00a0
                    00B1    402 _P3_1	=	0x00b1
                    00B0    403 _P3_0	=	0x00b0
                    00CF    404 _TCR20_7	=	0x00cf
                    00CE    405 _TCR20_6	=	0x00ce
                    00CD    406 _TCR20_5	=	0x00cd
                    00CC    407 _TCR20_4	=	0x00cc
                    00CB    408 _TCR20_3	=	0x00cb
                    00CA    409 _TCR20_2	=	0x00ca
                    00C9    410 _TCR20_1	=	0x00c9
                    00C8    411 _TCR20_0	=	0x00c8
                            412 ;--------------------------------------------------------
                            413 ; overlayable register banks
                            414 ;--------------------------------------------------------
                            415 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     416 	.ds 8
                            417 ;--------------------------------------------------------
                            418 ; internal ram data
                            419 ;--------------------------------------------------------
                            420 	.area DSEG    (DATA)
   0008                     421 _radio_mode::
   0008                     422 	.ds 1
   0009                     423 _bufstart::
   0009                     424 	.ds 1
   000A                     425 _bufcnt::
   000A                     426 	.ds 1
   000B                     427 _bufnxt::
   000B                     428 	.ds 1
   000C                     429 _handle_tx_tx_state_1_1:
   000C                     430 	.ds 1
   000D                     431 _handle_tx_tx_cnt_1_1:
   000D                     432 	.ds 1
   000E                     433 _check_radio_packet_x_1_1:
   000E                     434 	.ds 1
   000F                     435 _check_radio_packet_length_1_1:
   000F                     436 	.ds 1
   0010                     437 _check_radio_packet_address_1_1:
   0010                     438 	.ds 1
   0011                     439 _check_radio_input_length_1_1:
   0011                     440 	.ds 1
   0012                     441 _check_radio_input_address_1_1:
   0012                     442 	.ds 1
   0013                     443 _check_radio_input_x_1_1:
   0013                     444 	.ds 1
                            445 ;--------------------------------------------------------
                            446 ; overlayable items in internal ram 
                            447 ;--------------------------------------------------------
                            448 	.area	OSEG    (OVR,DATA)
                            449 ;--------------------------------------------------------
                            450 ; Stack segment in internal ram 
                            451 ;--------------------------------------------------------
                            452 	.area	SSEG	(DATA)
   009B                     453 __start__stack:
   009B                     454 	.ds	1
                            455 
                            456 ;--------------------------------------------------------
                            457 ; indirectly addressable internal ram data
                            458 ;--------------------------------------------------------
                            459 	.area ISEG    (DATA)
   004D                     460 _buf::
   004D                     461 	.ds 78
                            462 ;--------------------------------------------------------
                            463 ; absolute internal ram data
                            464 ;--------------------------------------------------------
                            465 	.area IABS    (ABS,DATA)
                            466 	.area IABS    (ABS,DATA)
                            467 ;--------------------------------------------------------
                            468 ; bit data
                            469 ;--------------------------------------------------------
                            470 	.area BSEG    (BIT)
   0000                     471 _got_etx::
   0000                     472 	.ds 1
   0001                     473 _got_eot::
   0001                     474 	.ds 1
                            475 ;--------------------------------------------------------
                            476 ; paged external ram data
                            477 ;--------------------------------------------------------
                            478 	.area PSEG    (PAG,XDATA)
                            479 ;--------------------------------------------------------
                            480 ; external ram data
                            481 ;--------------------------------------------------------
                            482 	.area XSEG    (XDATA)
                            483 ;--------------------------------------------------------
                            484 ; absolute external ram data
                            485 ;--------------------------------------------------------
                            486 	.area XABS    (ABS,XDATA)
                            487 ;--------------------------------------------------------
                            488 ; external initialized ram data
                            489 ;--------------------------------------------------------
                            490 	.area XISEG   (XDATA)
                            491 	.area HOME    (CODE)
                            492 	.area GSINIT0 (CODE)
                            493 	.area GSINIT1 (CODE)
                            494 	.area GSINIT2 (CODE)
                            495 	.area GSINIT3 (CODE)
                            496 	.area GSINIT4 (CODE)
                            497 	.area GSINIT5 (CODE)
                            498 	.area GSINIT  (CODE)
                            499 	.area GSFINAL (CODE)
                            500 	.area CSEG    (CODE)
                            501 ;--------------------------------------------------------
                            502 ; interrupt vector 
                            503 ;--------------------------------------------------------
                            504 	.area HOME    (CODE)
   0000                     505 __interrupt_vect:
   0000 02 00 2B            506 	ljmp	__sdcc_gsinit_startup
   0003 32                  507 	reti
   0004                     508 	.ds	7
   000B 32                  509 	reti
   000C                     510 	.ds	7
   0013 32                  511 	reti
   0014                     512 	.ds	7
   001B 32                  513 	reti
   001C                     514 	.ds	7
   0023 02 00 A2            515 	ljmp	_serial_isr
                            516 ;--------------------------------------------------------
                            517 ; global & static initialisations
                            518 ;--------------------------------------------------------
                            519 	.area HOME    (CODE)
                            520 	.area GSINIT  (CODE)
                            521 	.area GSFINAL (CODE)
                            522 	.area GSINIT  (CODE)
                            523 	.globl __sdcc_gsinit_startup
                            524 	.globl __sdcc_program_startup
                            525 	.globl __start__stack
                            526 	.globl __mcs51_genXINIT
                            527 	.globl __mcs51_genXRAMCLEAR
                            528 	.globl __mcs51_genRAMCLEAR
                            529 ;------------------------------------------------------------
                            530 ;Allocation info for local variables in function 'handle_tx'
                            531 ;------------------------------------------------------------
                            532 ;tx_state                  Allocated with name '_handle_tx_tx_state_1_1'
                            533 ;tx_cnt                    Allocated with name '_handle_tx_tx_cnt_1_1'
                            534 ;x                         Allocated to registers 
                            535 ;------------------------------------------------------------
                            536 ;	main.c:333: static unsigned char tx_state = TX_IDLE;
   0084 75 0C 00            537 	mov	_handle_tx_tx_state_1_1,#0x00
                            538 ;------------------------------------------------------------
                            539 ;Allocation info for local variables in function 'check_radio_input'
                            540 ;------------------------------------------------------------
                            541 ;length                    Allocated with name '_check_radio_input_length_1_1'
                            542 ;address                   Allocated with name '_check_radio_input_address_1_1'
                            543 ;n                         Allocated to registers r2 
                            544 ;x                         Allocated with name '_check_radio_input_x_1_1'
                            545 ;------------------------------------------------------------
                            546 ;	main.c:446: static unsigned char length = 0;
   0087 75 11 00            547 	mov	_check_radio_input_length_1_1,#0x00
                            548 	.area GSFINAL (CODE)
   008A 02 00 26            549 	ljmp	__sdcc_program_startup
                            550 ;--------------------------------------------------------
                            551 ; Home
                            552 ;--------------------------------------------------------
                            553 	.area HOME    (CODE)
                            554 	.area HOME    (CODE)
   0026                     555 __sdcc_program_startup:
   0026 12 03 40            556 	lcall	_main
                            557 ;	return from main will lock up
   0029 80 FE               558 	sjmp .
                            559 ;--------------------------------------------------------
                            560 ; code
                            561 ;--------------------------------------------------------
                            562 	.area CSEG    (CODE)
                            563 ;------------------------------------------------------------
                            564 ;Allocation info for local variables in function 'feed_wd'
                            565 ;------------------------------------------------------------
                            566 ;------------------------------------------------------------
                            567 ;	main.c:156: feed_wd(){
                            568 ;	-----------------------------------------
                            569 ;	 function feed_wd
                            570 ;	-----------------------------------------
   008D                     571 _feed_wd:
                    0002    572 	ar2 = 0x02
                    0003    573 	ar3 = 0x03
                    0004    574 	ar4 = 0x04
                    0005    575 	ar5 = 0x05
                    0006    576 	ar6 = 0x06
                    0007    577 	ar7 = 0x07
                    0000    578 	ar0 = 0x00
                    0001    579 	ar1 = 0x01
                            580 ;	main.c:157: EA = 0;
   008D C2 AF               581 	clr	_IEN0_7
                            582 ;	main.c:158: WFEED1 = 0xA5;
   008F 75 C2 A5            583 	mov	_WFEED1,#0xA5
                            584 ;	main.c:159: WFEED2 = 0x5A;
   0092 75 C3 5A            585 	mov	_WFEED2,#0x5A
                            586 ;	main.c:160: EA = 1;
   0095 D2 AF               587 	setb	_IEN0_7
   0097 22                  588 	ret
                            589 ;------------------------------------------------------------
                            590 ;Allocation info for local variables in function 'buffer_init'
                            591 ;------------------------------------------------------------
                            592 ;------------------------------------------------------------
                            593 ;	main.c:163: void buffer_init(){
                            594 ;	-----------------------------------------
                            595 ;	 function buffer_init
                            596 ;	-----------------------------------------
   0098                     597 _buffer_init:
                            598 ;	main.c:164: bufcnt = 0;			// Number of bytes in the buffer
   0098 75 0A 00            599 	mov	_bufcnt,#0x00
                            600 ;	main.c:165: bufnxt = 0;			// Index of next free place in buffer
   009B 75 0B 00            601 	mov	_bufnxt,#0x00
                            602 ;	main.c:166: bufstart = 0;		// Index of first data byte in buffer
   009E 75 09 00            603 	mov	_bufstart,#0x00
   00A1 22                  604 	ret
                            605 ;------------------------------------------------------------
                            606 ;Allocation info for local variables in function 'serial_isr'
                            607 ;------------------------------------------------------------
                            608 ;x                         Allocated to registers r2 
                            609 ;------------------------------------------------------------
                            610 ;	main.c:182: void serial_isr (void) __interrupt (4) {
                            611 ;	-----------------------------------------
                            612 ;	 function serial_isr
                            613 ;	-----------------------------------------
   00A2                     614 _serial_isr:
   00A2 C0 E0               615 	push	acc
   00A4 C0 02               616 	push	ar2
   00A6 C0 03               617 	push	ar3
   00A8 C0 00               618 	push	ar0
   00AA C0 D0               619 	push	psw
   00AC 75 D0 00            620 	mov	psw,#0x00
                            621 ;	main.c:185: x=SBUF;
   00AF AA 99               622 	mov	r2,_SBUF
                            623 ;	main.c:186: RI = 0;
   00B1 C2 98               624 	clr	_SCON_0
                            625 ;	main.c:191: if (x == ETX){
   00B3 BA 03 04            626 	cjne	r2,#0x03,00102$
                            627 ;	main.c:192: got_etx = 1;
   00B6 D2 00               628 	setb	_got_etx
                            629 ;	main.c:193: return;
   00B8 80 32               630 	sjmp	00114$
   00BA                     631 00102$:
                            632 ;	main.c:197: if (got_eot){
   00BA 30 01 02            633 	jnb	_got_eot,00104$
                            634 ;	main.c:198: return;
   00BD 80 2D               635 	sjmp	00114$
   00BF                     636 00104$:
                            637 ;	main.c:202: if (bufcnt >= BUFSIZE){
   00BF 74 B2               638 	mov	a,#0x100 - 0x4E
   00C1 25 0A               639 	add	a,_bufcnt
   00C3 50 0D               640 	jnc	00109$
                            641 ;	main.c:203: bufcnt--;
   00C5 15 0A               642 	dec	_bufcnt
                            643 ;	main.c:205: if (bufnxt == 0)
   00C7 E5 0B               644 	mov	a,_bufnxt
   00C9 70 05               645 	jnz	00106$
                            646 ;	main.c:206: bufnxt = BUFSIZE-1;
   00CB 75 0B 4D            647 	mov	_bufnxt,#0x4D
   00CE 80 02               648 	sjmp	00109$
   00D0                     649 00106$:
                            650 ;	main.c:208: bufnxt--;
   00D0 15 0B               651 	dec	_bufnxt
   00D2                     652 00109$:
                            653 ;	main.c:212: buf[bufnxt++] = x;
   00D2 AB 0B               654 	mov	r3,_bufnxt
   00D4 05 0B               655 	inc	_bufnxt
   00D6 EB                  656 	mov	a,r3
   00D7 24 4D               657 	add	a,#_buf
   00D9 F8                  658 	mov	r0,a
   00DA A6 02               659 	mov	@r0,ar2
                            660 ;	main.c:213: if (bufnxt >= BUFSIZE)
   00DC 74 B2               661 	mov	a,#0x100 - 0x4E
   00DE 25 0B               662 	add	a,_bufnxt
   00E0 50 03               663 	jnc	00111$
                            664 ;	main.c:214: bufnxt = 0;
   00E2 75 0B 00            665 	mov	_bufnxt,#0x00
   00E5                     666 00111$:
                            667 ;	main.c:215: bufcnt++;
   00E5 05 0A               668 	inc	_bufcnt
                            669 ;	main.c:217: if (x == EOT)
   00E7 BA 04 02            670 	cjne	r2,#0x04,00113$
                            671 ;	main.c:218: got_eot = 1;
   00EA D2 01               672 	setb	_got_eot
   00EC                     673 00113$:
                            674 ;	main.c:220: return;
   00EC                     675 00114$:
   00EC D0 D0               676 	pop	psw
   00EE D0 00               677 	pop	ar0
   00F0 D0 03               678 	pop	ar3
   00F2 D0 02               679 	pop	ar2
   00F4 D0 E0               680 	pop	acc
   00F6 32                  681 	reti
                            682 ;	eliminated unneeded push/pop ar1
                            683 ;	eliminated unneeded push/pop dpl
                            684 ;	eliminated unneeded push/pop dph
                            685 ;	eliminated unneeded push/pop b
                            686 ;------------------------------------------------------------
                            687 ;Allocation info for local variables in function 'buffer_out'
                            688 ;------------------------------------------------------------
                            689 ;x                         Allocated to registers r2 
                            690 ;------------------------------------------------------------
                            691 ;	main.c:227: char buffer_out(){
                            692 ;	-----------------------------------------
                            693 ;	 function buffer_out
                            694 ;	-----------------------------------------
   00F7                     695 _buffer_out:
                            696 ;	main.c:230: if (bufcnt == 0) return (0);
   00F7 E5 0A               697 	mov	a,_bufcnt
   00F9 70 03               698 	jnz	00102$
   00FB F5 82               699 	mov	dpl,a
   00FD 22                  700 	ret
   00FE                     701 00102$:
                            702 ;	main.c:231: x = buf[bufstart++];
   00FE AA 09               703 	mov	r2,_bufstart
   0100 05 09               704 	inc	_bufstart
   0102 EA                  705 	mov	a,r2
   0103 24 4D               706 	add	a,#_buf
   0105 F8                  707 	mov	r0,a
   0106 86 02               708 	mov	ar2,@r0
                            709 ;	main.c:232: if (bufstart >= BUFSIZE)
   0108 74 B2               710 	mov	a,#0x100 - 0x4E
   010A 25 09               711 	add	a,_bufstart
   010C 50 03               712 	jnc	00104$
                            713 ;	main.c:233: bufstart = 0;
   010E 75 09 00            714 	mov	_bufstart,#0x00
   0111                     715 00104$:
                            716 ;	main.c:236: bufcnt--;
   0111 15 0A               717 	dec	_bufcnt
                            718 ;	main.c:238: return x;
   0113 8A 82               719 	mov	dpl,r2
   0115 22                  720 	ret
                            721 ;------------------------------------------------------------
                            722 ;Allocation info for local variables in function 'has_room'
                            723 ;------------------------------------------------------------
                            724 ;------------------------------------------------------------
                            725 ;	main.c:255: unsigned char has_room(){
                            726 ;	-----------------------------------------
                            727 ;	 function has_room
                            728 ;	-----------------------------------------
   0116                     729 _has_room:
                            730 ;	main.c:256: return ( (BUFSIZE - bufcnt) > (MPDTOOL_PKTSIZE + 2) );
   0116 AA 0A               731 	mov	r2,_bufcnt
   0118 7B 00               732 	mov	r3,#0x00
   011A 74 4E               733 	mov	a,#0x4E
   011C C3                  734 	clr	c
   011D 9A                  735 	subb	a,r2
   011E FA                  736 	mov	r2,a
   011F E4                  737 	clr	a
   0120 9B                  738 	subb	a,r3
   0121 FB                  739 	mov	r3,a
   0122 C3                  740 	clr	c
   0123 74 12               741 	mov	a,#0x12
   0125 9A                  742 	subb	a,r2
   0126 74 80               743 	mov	a,#(0x00 ^ 0x80)
   0128 8B F0               744 	mov	b,r3
   012A 63 F0 80            745 	xrl	b,#0x80
   012D 95 F0               746 	subb	a,b
   012F E4                  747 	clr	a
   0130 33                  748 	rlc	a
   0131 F5 82               749 	mov	dpl,a
   0133 22                  750 	ret
                            751 ;------------------------------------------------------------
                            752 ;Allocation info for local variables in function 'rx_overflow_reset'
                            753 ;------------------------------------------------------------
                            754 ;------------------------------------------------------------
                            755 ;	main.c:266: rx_overflow_reset(){
                            756 ;	-----------------------------------------
                            757 ;	 function rx_overflow_reset
                            758 ;	-----------------------------------------
   0134                     759 _rx_overflow_reset:
                            760 ;	main.c:267: if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) == RX_OVERFLOW){
   0134 75 82 F5            761 	mov	dpl,#0xF5
   0137 12 05 5F            762 	lcall	_cc1100_read_status_reg_otf
   013A E5 82               763 	mov	a,dpl
   013C 54 1F               764 	anl	a,#0x1F
   013E FA                  765 	mov	r2,a
   013F BA 11 0C            766 	cjne	r2,#0x11,00103$
                            767 ;	main.c:268: cc1100_strobe(SFRX);
   0142 75 82 3A            768 	mov	dpl,#0x3A
   0145 12 05 50            769 	lcall	_cc1100_strobe
                            770 ;	main.c:269: cc1100_strobe(SRX);
   0148 75 82 34            771 	mov	dpl,#0x34
   014B 02 05 50            772 	ljmp	_cc1100_strobe
   014E                     773 00103$:
   014E 22                  774 	ret
                            775 ;------------------------------------------------------------
                            776 ;Allocation info for local variables in function 'start_rx'
                            777 ;------------------------------------------------------------
                            778 ;------------------------------------------------------------
                            779 ;	main.c:275: void start_rx() {
                            780 ;	-----------------------------------------
                            781 ;	 function start_rx
                            782 ;	-----------------------------------------
   014F                     783 _start_rx:
                            784 ;	main.c:277: rx_overflow_reset();
   014F 12 01 34            785 	lcall	_rx_overflow_reset
                            786 ;	main.c:278: switch_to_idle();
   0152 12 05 8F            787 	lcall	_switch_to_idle
                            788 ;	main.c:280: cc1100_strobe(SFRX);
   0155 75 82 3A            789 	mov	dpl,#0x3A
   0158 12 05 50            790 	lcall	_cc1100_strobe
                            791 ;	main.c:281: cc1100_strobe(SCAL);
   015B 75 82 33            792 	mov	dpl,#0x33
   015E 12 05 50            793 	lcall	_cc1100_strobe
                            794 ;	main.c:282: cc1100_strobe(SRX);
   0161 75 82 34            795 	mov	dpl,#0x34
   0164 12 05 50            796 	lcall	_cc1100_strobe
                            797 ;	main.c:283: radio_mode = RADIO_RX;
   0167 75 08 01            798 	mov	_radio_mode,#0x01
   016A 22                  799 	ret
                            800 ;------------------------------------------------------------
                            801 ;Allocation info for local variables in function 'start_tx'
                            802 ;------------------------------------------------------------
                            803 ;------------------------------------------------------------
                            804 ;	main.c:288: void start_tx() {	
                            805 ;	-----------------------------------------
                            806 ;	 function start_tx
                            807 ;	-----------------------------------------
   016B                     808 _start_tx:
                            809 ;	main.c:289: rx_overflow_reset();
   016B 12 01 34            810 	lcall	_rx_overflow_reset
                            811 ;	main.c:290: switch_to_idle();
   016E 12 05 8F            812 	lcall	_switch_to_idle
                            813 ;	main.c:292: cc1100_strobe(SCAL);
   0171 75 82 33            814 	mov	dpl,#0x33
   0174 12 05 50            815 	lcall	_cc1100_strobe
                            816 ;	main.c:293: cc1100_strobe(STX);
   0177 75 82 35            817 	mov	dpl,#0x35
   017A 12 05 50            818 	lcall	_cc1100_strobe
                            819 ;	main.c:294: radio_mode = RADIO_TX;
   017D 75 08 02            820 	mov	_radio_mode,#0x02
   0180 22                  821 	ret
                            822 ;------------------------------------------------------------
                            823 ;Allocation info for local variables in function 'tx_finished'
                            824 ;------------------------------------------------------------
                            825 ;------------------------------------------------------------
                            826 ;	main.c:298: unsigned char tx_finished(){
                            827 ;	-----------------------------------------
                            828 ;	 function tx_finished
                            829 ;	-----------------------------------------
   0181                     830 _tx_finished:
                            831 ;	main.c:299: return ( (cc1100_read_status_reg_otf(TXBYTES) & 0x7f) == 0  &&
   0181 75 82 FA            832 	mov	dpl,#0xFA
   0184 12 05 5F            833 	lcall	_cc1100_read_status_reg_otf
   0187 E5 82               834 	mov	a,dpl
   0189 54 7F               835 	anl	a,#0x7F
   018B FA                  836 	mov	r2,a
   018C 60 02               837 	jz	00108$
   018E 80 0D               838 	sjmp	00103$
   0190                     839 00108$:
                            840 ;	main.c:300: ( (cc1100_read1(PKTSTATUS | BURST) & (1<<2)) == 0 ) );		// GDO2 == 0
   0190 75 82 F8            841 	mov	dpl,#0xF8
   0193 12 05 38            842 	lcall	_cc1100_read1
   0196 E5 82               843 	mov	a,dpl
   0198 54 04               844 	anl	a,#0x04
   019A FA                  845 	mov	r2,a
   019B 60 04               846 	jz	00104$
   019D                     847 00103$:
   019D 7A 00               848 	mov	r2,#0x00
   019F 80 02               849 	sjmp	00105$
   01A1                     850 00104$:
   01A1 7A 01               851 	mov	r2,#0x01
   01A3                     852 00105$:
   01A3 8A 82               853 	mov	dpl,r2
   01A5 22                  854 	ret
                            855 ;------------------------------------------------------------
                            856 ;Allocation info for local variables in function 're_enter_rx'
                            857 ;------------------------------------------------------------
                            858 ;------------------------------------------------------------
                            859 ;	main.c:304: void re_enter_rx(){
                            860 ;	-----------------------------------------
                            861 ;	 function re_enter_rx
                            862 ;	-----------------------------------------
   01A6                     863 _re_enter_rx:
                            864 ;	main.c:305: if ((radio_mode == RADIO_TX) && tx_finished())
   01A6 74 02               865 	mov	a,#0x02
   01A8 B5 08 0A            866 	cjne	a,_radio_mode,00104$
   01AB 12 01 81            867 	lcall	_tx_finished
   01AE E5 82               868 	mov	a,dpl
   01B0 60 03               869 	jz	00104$
                            870 ;	main.c:306: start_rx();
   01B2 02 01 4F            871 	ljmp	_start_rx
   01B5                     872 00104$:
   01B5 22                  873 	ret
                            874 ;------------------------------------------------------------
                            875 ;Allocation info for local variables in function 'handle_tx'
                            876 ;------------------------------------------------------------
                            877 ;tx_state                  Allocated with name '_handle_tx_tx_state_1_1'
                            878 ;tx_cnt                    Allocated with name '_handle_tx_tx_cnt_1_1'
                            879 ;x                         Allocated to registers 
                            880 ;------------------------------------------------------------
                            881 ;	main.c:330: void handle_tx(){
                            882 ;	-----------------------------------------
                            883 ;	 function handle_tx
                            884 ;	-----------------------------------------
   01B6                     885 _handle_tx:
                            886 ;	main.c:337: switch (tx_state){
   01B6 E4                  887 	clr	a
   01B7 B5 0C 02            888 	cjne	a,_handle_tx_tx_state_1_1,00121$
   01BA 80 0E               889 	sjmp	00101$
   01BC                     890 00121$:
   01BC 74 01               891 	mov	a,#0x01
   01BE B5 0C 02            892 	cjne	a,_handle_tx_tx_state_1_1,00122$
   01C1 80 45               893 	sjmp	00107$
   01C3                     894 00122$:
   01C3 74 02               895 	mov	a,#0x02
                            896 ;	main.c:339: case TX_IDLE:
   01C5 B5 0C 5D            897 	cjne	a,_handle_tx_tx_state_1_1,00113$
   01C8 80 55               898 	sjmp	00111$
   01CA                     899 00101$:
                            900 ;	main.c:341: if (got_eot) {
   01CA 30 01 1C            901 	jnb	_got_eot,00105$
                            902 ;	main.c:342: tx_cnt = bufcnt;	/* = number of payload bytes to be transferred to TXFIFO (not counting address and length byte) */
                            903 ;	main.c:345: cc1100_write1(TX_fifo, tx_cnt + 1);
   01CD E5 0A               904 	mov	a,_bufcnt
   01CF F5 0D               905 	mov	_handle_tx_tx_cnt_1_1,a
   01D1 04                  906 	inc	a
   01D2 F5 18               907 	mov	_cc1100_write1_PARM_2,a
   01D4 75 82 3F            908 	mov	dpl,#0x3F
   01D7 12 04 CA            909 	lcall	_cc1100_write1
                            910 ;	main.c:348: cc1100_write1(TX_fifo, DEV_ADDR);	
   01DA 75 18 01            911 	mov	_cc1100_write1_PARM_2,#0x01
   01DD 75 82 3F            912 	mov	dpl,#0x3F
   01E0 12 04 CA            913 	lcall	_cc1100_write1
                            914 ;	main.c:350: tx_state = TX_COPY;
   01E3 75 0C 01            915 	mov	_handle_tx_tx_state_1_1,#0x01
                            916 ;	main.c:351: got_eot=0;
   01E6 C2 01               917 	clr	_got_eot
                            918 ;	main.c:353: break;
   01E8 22                  919 	ret
   01E9                     920 00105$:
                            921 ;	main.c:355: } else if ( bufcnt >= MAX_TX_PAYLOAD ) {
   01E9 74 C4               922 	mov	a,#0x100 - 0x3C
   01EB 25 0A               923 	add	a,_bufcnt
   01ED 50 36               924 	jnc	00113$
                            925 ;	main.c:358: tx_cnt = MAX_TX_PAYLOAD;	
   01EF 75 0D 3C            926 	mov	_handle_tx_tx_cnt_1_1,#0x3C
                            927 ;	main.c:361: cc1100_write1(TX_fifo, MAX_TX_PAYLOAD + 1);
   01F2 75 18 3D            928 	mov	_cc1100_write1_PARM_2,#0x3D
   01F5 75 82 3F            929 	mov	dpl,#0x3F
   01F8 12 04 CA            930 	lcall	_cc1100_write1
                            931 ;	main.c:364: cc1100_write1(TX_fifo, DEV_ADDR);	
   01FB 75 18 01            932 	mov	_cc1100_write1_PARM_2,#0x01
   01FE 75 82 3F            933 	mov	dpl,#0x3F
   0201 12 04 CA            934 	lcall	_cc1100_write1
                            935 ;	main.c:366: tx_state = TX_COPY;
   0204 75 0C 01            936 	mov	_handle_tx_tx_state_1_1,#0x01
                            937 ;	main.c:368: break;
                            938 ;	main.c:370: case TX_COPY:
   0207 22                  939 	ret
   0208                     940 00107$:
                            941 ;	main.c:372: if (tx_cnt > 0){
   0208 E5 0D               942 	mov	a,_handle_tx_tx_cnt_1_1
   020A 60 0F               943 	jz	00109$
                            944 ;	main.c:373: x = buffer_out();
   020C 12 00 F7            945 	lcall	_buffer_out
   020F 85 82 18            946 	mov	_cc1100_write1_PARM_2,dpl
                            947 ;	main.c:374: cc1100_write1(TX_fifo, x);	
   0212 75 82 3F            948 	mov	dpl,#0x3F
   0215 12 04 CA            949 	lcall	_cc1100_write1
                            950 ;	main.c:375: tx_cnt--;
   0218 15 0D               951 	dec	_handle_tx_tx_cnt_1_1
   021A 22                  952 	ret
   021B                     953 00109$:
                            954 ;	main.c:378: tx_state = TX_SEND;
   021B 75 0C 02            955 	mov	_handle_tx_tx_state_1_1,#0x02
                            956 ;	main.c:380: break;
                            957 ;	main.c:382: case TX_SEND:
   021E 22                  958 	ret
   021F                     959 00111$:
                            960 ;	main.c:383: start_tx();
   021F 12 01 6B            961 	lcall	_start_tx
                            962 ;	main.c:384: tx_state = TX_IDLE;
   0222 75 0C 00            963 	mov	_handle_tx_tx_state_1_1,#0x00
                            964 ;	main.c:386: }
   0225                     965 00113$:
   0225 22                  966 	ret
                            967 ;------------------------------------------------------------
                            968 ;Allocation info for local variables in function 'check_etx'
                            969 ;------------------------------------------------------------
                            970 ;------------------------------------------------------------
                            971 ;	main.c:390: void check_etx(){
                            972 ;	-----------------------------------------
                            973 ;	 function check_etx
                            974 ;	-----------------------------------------
   0226                     975 _check_etx:
                            976 ;	main.c:391: if (got_etx) {	/* Is the sender waiting for an ACK ? */		
   0226 30 00 0F            977 	jnb	_got_etx,00105$
                            978 ;	main.c:392: if (has_room()){	/* Still enough space in buffer ? */
   0229 12 01 16            979 	lcall	_has_room
   022C E5 82               980 	mov	a,dpl
   022E 60 08               981 	jz	00105$
                            982 ;	main.c:393: got_etx = 0;		// Atomic Operation ! (see sdcc manual)
   0230 C2 00               983 	clr	_got_etx
                            984 ;	main.c:394: send_byte(ACK);
   0232 75 82 06            985 	mov	dpl,#0x06
   0235 02 04 11            986 	ljmp	_send_byte
   0238                     987 00105$:
   0238 22                  988 	ret
                            989 ;------------------------------------------------------------
                            990 ;Allocation info for local variables in function 'check_radio_packet'
                            991 ;------------------------------------------------------------
                            992 ;x                         Allocated with name '_check_radio_packet_x_1_1'
                            993 ;i                         Allocated to registers r2 
                            994 ;length                    Allocated with name '_check_radio_packet_length_1_1'
                            995 ;address                   Allocated with name '_check_radio_packet_address_1_1'
                            996 ;------------------------------------------------------------
                            997 ;	main.c:405: void check_radio_packet (){
                            998 ;	-----------------------------------------
                            999 ;	 function check_radio_packet
                           1000 ;	-----------------------------------------
   0239                    1001 _check_radio_packet:
                           1002 ;	main.c:409: if ( GDO0 ){
   0239 30 86 6B           1003 	jnb	_P0_6,00109$
                           1004 ;	main.c:412: cc1100_read(RX_fifo|BURST, &length, 1);		// Length byte = payload length + 1
   023C 75 19 0F           1005 	mov	_cc1100_read_PARM_2,#_check_radio_packet_length_1_1
   023F 75 1A 00           1006 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0242 75 1B 40           1007 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   0245 75 1C 01           1008 	mov	_cc1100_read_PARM_3,#0x01
   0248 75 82 7F           1009 	mov	dpl,#0x7F
   024B 12 04 E7           1010 	lcall	_cc1100_read
                           1011 ;	main.c:414: cc1100_read(RX_fifo|BURST, &address, 1); 	// Address byte (unused!)
   024E 75 19 10           1012 	mov	_cc1100_read_PARM_2,#_check_radio_packet_address_1_1
   0251 75 1A 00           1013 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0254 75 1B 40           1014 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   0257 75 1C 01           1015 	mov	_cc1100_read_PARM_3,#0x01
   025A 75 82 7F           1016 	mov	dpl,#0x7F
   025D 12 04 E7           1017 	lcall	_cc1100_read
                           1018 ;	main.c:417: for (i=0; i < (length - 1); i++){
   0260 7A 00              1019 	mov	r2,#0x00
   0262                    1020 00103$:
   0262 AB 0F              1021 	mov	r3,_check_radio_packet_length_1_1
   0264 7C 00              1022 	mov	r4,#0x00
   0266 1B                 1023 	dec	r3
   0267 BB FF 01           1024 	cjne	r3,#0xff,00117$
   026A 1C                 1025 	dec	r4
   026B                    1026 00117$:
   026B 8A 05              1027 	mov	ar5,r2
   026D 7E 00              1028 	mov	r6,#0x00
   026F C3                 1029 	clr	c
   0270 ED                 1030 	mov	a,r5
   0271 9B                 1031 	subb	a,r3
   0272 EE                 1032 	mov	a,r6
   0273 64 80              1033 	xrl	a,#0x80
   0275 8C F0              1034 	mov	b,r4
   0277 63 F0 80           1035 	xrl	b,#0x80
   027A 95 F0              1036 	subb	a,b
   027C 50 26              1037 	jnc	00106$
                           1038 ;	main.c:418: cc1100_read(RX_fifo|BURST, &x, 1);
   027E 75 19 0E           1039 	mov	_cc1100_read_PARM_2,#_check_radio_packet_x_1_1
   0281 75 1A 00           1040 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0284 75 1B 40           1041 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   0287 75 1C 01           1042 	mov	_cc1100_read_PARM_3,#0x01
   028A 75 82 7F           1043 	mov	dpl,#0x7F
   028D C0 02              1044 	push	ar2
   028F 12 04 E7           1045 	lcall	_cc1100_read
                           1046 ;	main.c:419: send_byte(x);
   0292 85 0E 82           1047 	mov	dpl,_check_radio_packet_x_1_1
   0295 12 04 11           1048 	lcall	_send_byte
   0298 D0 02              1049 	pop	ar2
                           1050 ;	main.c:420: if (x == EOT)
   029A 74 04              1051 	mov	a,#0x04
   029C B5 0E 02           1052 	cjne	a,_check_radio_packet_x_1_1,00119$
   029F 80 03              1053 	sjmp	00106$
   02A1                    1054 00119$:
                           1055 ;	main.c:417: for (i=0; i < (length - 1); i++){
   02A1 0A                 1056 	inc	r2
   02A2 80 BE              1057 	sjmp	00103$
   02A4                    1058 00106$:
                           1059 ;	main.c:423: start_rx();	
   02A4 02 01 4F           1060 	ljmp	_start_rx
   02A7                    1061 00109$:
   02A7 22                 1062 	ret
                           1063 ;------------------------------------------------------------
                           1064 ;Allocation info for local variables in function 'check_radio_input'
                           1065 ;------------------------------------------------------------
                           1066 ;length                    Allocated with name '_check_radio_input_length_1_1'
                           1067 ;address                   Allocated with name '_check_radio_input_address_1_1'
                           1068 ;n                         Allocated to registers r2 
                           1069 ;x                         Allocated with name '_check_radio_input_x_1_1'
                           1070 ;------------------------------------------------------------
                           1071 ;	main.c:445: void check_radio_input (){
                           1072 ;	-----------------------------------------
                           1073 ;	 function check_radio_input
                           1074 ;	-----------------------------------------
   02A8                    1075 _check_radio_input:
                           1076 ;	main.c:452: if (length == 0){
   02A8 E5 11              1077 	mov	a,_check_radio_input_length_1_1
   02AA 70 34              1078 	jnz	00112$
                           1079 ;	main.c:453: n = cc1100_read_status_reg_otf(RXBYTES) & 0x7f;
   02AC 75 82 FB           1080 	mov	dpl,#0xFB
   02AF 12 05 5F           1081 	lcall	_cc1100_read_status_reg_otf
   02B2 E5 82              1082 	mov	a,dpl
   02B4 54 7F              1083 	anl	a,#0x7F
                           1084 ;	main.c:454: if (n > 2) {
   02B6 FA                 1085 	mov  r2,a
   02B7 24 FD              1086 	add	a,#0xff - 0x02
   02B9 40 01              1087 	jc	00122$
   02BB 22                 1088 	ret
   02BC                    1089 00122$:
                           1090 ;	main.c:455: cc1100_read(RX_fifo|BURST, &length, 1);		// Length byte = payload length + 1 address byte
   02BC 75 19 11           1091 	mov	_cc1100_read_PARM_2,#_check_radio_input_length_1_1
   02BF 75 1A 00           1092 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02C2 75 1B 40           1093 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02C5 75 1C 01           1094 	mov	_cc1100_read_PARM_3,#0x01
   02C8 75 82 7F           1095 	mov	dpl,#0x7F
   02CB 12 04 E7           1096 	lcall	_cc1100_read
                           1097 ;	main.c:456: cc1100_read(RX_fifo|BURST, &address, 1); 	// Address byte (unused!)
   02CE 75 19 12           1098 	mov	_cc1100_read_PARM_2,#_check_radio_input_address_1_1
   02D1 75 1A 00           1099 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02D4 75 1B 40           1100 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02D7 75 1C 01           1101 	mov	_cc1100_read_PARM_3,#0x01
   02DA 75 82 7F           1102 	mov	dpl,#0x7F
   02DD 02 04 E7           1103 	ljmp	_cc1100_read
   02E0                    1104 00112$:
                           1105 ;	main.c:463: if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) == MARCSTATE_IDLE) {
   02E0 75 82 F5           1106 	mov	dpl,#0xF5
   02E3 12 05 5F           1107 	lcall	_cc1100_read_status_reg_otf
   02E6 E5 82              1108 	mov	a,dpl
   02E8 54 1F              1109 	anl	a,#0x1F
   02EA FB                 1110 	mov	r3,a
   02EB BB 01 28           1111 	cjne	r3,#0x01,00109$
                           1112 ;	main.c:464: while (length > 1) {
   02EE                    1113 00103$:
   02EE E5 11              1114 	mov	a,_check_radio_input_length_1_1
   02F0 24 FE              1115 	add	a,#0xff - 0x01
   02F2 50 1C              1116 	jnc	00105$
                           1117 ;	main.c:465: cc1100_read(RX_fifo|BURST, &x, 1);
   02F4 75 19 13           1118 	mov	_cc1100_read_PARM_2,#_check_radio_input_x_1_1
   02F7 75 1A 00           1119 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02FA 75 1B 40           1120 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02FD 75 1C 01           1121 	mov	_cc1100_read_PARM_3,#0x01
   0300 75 82 7F           1122 	mov	dpl,#0x7F
   0303 12 04 E7           1123 	lcall	_cc1100_read
                           1124 ;	main.c:466: send_byte(x);	
   0306 85 13 82           1125 	mov	dpl,_check_radio_input_x_1_1
   0309 12 04 11           1126 	lcall	_send_byte
                           1127 ;	main.c:467: length--;
   030C 15 11              1128 	dec	_check_radio_input_length_1_1
   030E 80 DE              1129 	sjmp	00103$
   0310                    1130 00105$:
                           1131 ;	main.c:469: length = 0;
   0310 75 11 00           1132 	mov	_check_radio_input_length_1_1,#0x00
                           1133 ;	main.c:470: start_rx();	
   0313 02 01 4F           1134 	ljmp	_start_rx
   0316                    1135 00109$:
                           1136 ;	main.c:473: n = cc1100_read_status_reg_otf(RXBYTES) & 0x7f;	
   0316 75 82 FB           1137 	mov	dpl,#0xFB
   0319 12 05 5F           1138 	lcall	_cc1100_read_status_reg_otf
   031C E5 82              1139 	mov	a,dpl
   031E 54 7F              1140 	anl	a,#0x7F
                           1141 ;	main.c:476: if (n > 1){
   0320 FA                 1142 	mov  r2,a
   0321 24 FE              1143 	add	a,#0xff - 0x01
   0323 50 1A              1144 	jnc	00114$
                           1145 ;	main.c:477: cc1100_read(RX_fifo|BURST, &x, 1);
   0325 75 19 13           1146 	mov	_cc1100_read_PARM_2,#_check_radio_input_x_1_1
   0328 75 1A 00           1147 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   032B 75 1B 40           1148 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   032E 75 1C 01           1149 	mov	_cc1100_read_PARM_3,#0x01
   0331 75 82 7F           1150 	mov	dpl,#0x7F
   0334 12 04 E7           1151 	lcall	_cc1100_read
                           1152 ;	main.c:478: send_byte(x);	
   0337 85 13 82           1153 	mov	dpl,_check_radio_input_x_1_1
   033A 12 04 11           1154 	lcall	_send_byte
                           1155 ;	main.c:479: length--;	
   033D 15 11              1156 	dec	_check_radio_input_length_1_1
   033F                    1157 00114$:
   033F 22                 1158 	ret
                           1159 ;------------------------------------------------------------
                           1160 ;Allocation info for local variables in function 'main'
                           1161 ;------------------------------------------------------------
                           1162 ;------------------------------------------------------------
                           1163 ;	main.c:506: void main(void) {
                           1164 ;	-----------------------------------------
                           1165 ;	 function main
                           1166 ;	-----------------------------------------
   0340                    1167 _main:
                           1168 ;	main.c:516: P0M1 &= 0x6c;		// 0110 1100 -> P0M1
   0340 53 84 6C           1169 	anl	_P0M1,#0x6C
                           1170 ;	main.c:517: P0M2 |= 0x92;		// 1001 0010 -> P0M2
   0343 43 85 92           1171 	orl	_P0M2,#0x92
                           1172 ;	main.c:519: P0 = 0x83;			// 1000 0011 -> Port0
   0346 75 80 83           1173 	mov	_P0,#0x83
                           1174 ;	main.c:526: P1M1 &= 0x3e;		// 0011 1110
   0349 53 91 3E           1175 	anl	_P1M1,#0x3E
                           1176 ;	main.c:527: P1M2 |= 0xcd;		// 1100 1101
   034C 43 92 CD           1177 	orl	_P1M2,#0xCD
                           1178 ;	main.c:529: P1 = 0xcd;			// 1100 1101 -> Port1
   034F 75 90 CD           1179 	mov	_P1,#0xCD
                           1180 ;	main.c:533: P3M1 &= 0xfe;
   0352 53 B1 FE           1181 	anl	_P3M1,#0xFE
                           1182 ;	main.c:534: P3M2 |= 0x01;
   0355 43 B2 01           1183 	orl	_P3M2,#0x01
                           1184 ;	main.c:536: P3 = 0;
   0358 75 B0 00           1185 	mov	_P3,#0x00
                           1186 ;	main.c:557: AUXR1 |= (1<<6);
   035B 43 A2 40           1187 	orl	_AUXR1,#0x40
                           1188 ;	main.c:559: RTCCON |= 1<<RTCS1;
   035E 43 D1 40           1189 	orl	_RTCCON,#0x40
                           1190 ;	main.c:560: RTCCON |= 1<<RTCS0;
   0361 43 D1 20           1191 	orl	_RTCCON,#0x20
                           1192 ;	main.c:561: RTCH = 0xff;
   0364 75 D2 FF           1193 	mov	_RTCH,#0xFF
                           1194 ;	main.c:562: RTCL = 0xff;
   0367 75 D3 FF           1195 	mov	_RTCL,#0xFF
                           1196 ;	main.c:564: initSerial(384);		// Serial baudrate 38400
   036A 90 01 80           1197 	mov	dptr,#0x0180
   036D 12 03 B3           1198 	lcall	_initSerial
                           1199 ;	main.c:565: SSTAT |= (1<<CIDIS);	// Combined interrupt disabled, RX and TX generate different interrupts
   0370 43 BA 20           1200 	orl	_SSTAT,#0x20
                           1201 ;	main.c:567: cc1100_init();
   0373 12 04 46           1202 	lcall	_cc1100_init
                           1203 ;	main.c:569: ESR = 1;
   0376 D2 AC              1204 	setb	_IEN0_4
                           1205 ;	main.c:570: EA = 1;
   0378 D2 AF              1206 	setb	_IEN0_7
                           1207 ;	main.c:630: got_etx = 0;
   037A C2 00              1208 	clr	_got_etx
                           1209 ;	main.c:631: got_eot = 0;
   037C C2 01              1210 	clr	_got_eot
                           1211 ;	main.c:633: buffer_init();
   037E 12 00 98           1212 	lcall	_buffer_init
                           1213 ;	main.c:635: start_rx();								// Start receiving via radio
   0381 12 01 4F           1214 	lcall	_start_rx
                           1215 ;	main.c:638: WDL = 0xFF;			// WDT counter
   0384 75 C1 FF           1216 	mov	_WDL,#0xFF
                           1217 ;	main.c:639: EA = 0;
   0387 C2 AF              1218 	clr	_IEN0_7
                           1219 ;	main.c:640: WDCON = 0xE5;		// Start WDT
   0389 75 A7 E5           1220 	mov	_WDCON,#0xE5
                           1221 ;	main.c:641: WFEED1 = 0xA5;
   038C 75 C2 A5           1222 	mov	_WFEED1,#0xA5
                           1223 ;	main.c:642: WFEED2 = 0x5A;
   038F 75 C3 5A           1224 	mov	_WFEED2,#0x5A
                           1225 ;	main.c:643: EA = 1;
   0392 D2 AF              1226 	setb	_IEN0_7
                           1227 ;	main.c:645: while (1) {						/* Forever: */
   0394                    1228 00107$:
                           1229 ;	main.c:647: feed_wd();
   0394 12 00 8D           1230 	lcall	_feed_wd
                           1231 ;	main.c:650: check_etx();
   0397 12 02 26           1232 	lcall	_check_etx
                           1233 ;	main.c:658: if (radio_mode == RADIO_RX)
   039A 74 01              1234 	mov	a,#0x01
   039C B5 08 05           1235 	cjne	a,_radio_mode,00102$
                           1236 ;	main.c:660: check_radio_input();
   039F 12 02 A8           1237 	lcall	_check_radio_input
   03A2 80 03              1238 	sjmp	00103$
   03A4                    1239 00102$:
                           1240 ;	main.c:664: re_enter_rx();
   03A4 12 01 A6           1241 	lcall	_re_enter_rx
   03A7                    1242 00103$:
                           1243 ;	main.c:669: if (radio_mode != RADIO_TX)
   03A7 74 02              1244 	mov	a,#0x02
   03A9 B5 08 02           1245 	cjne	a,_radio_mode,00116$
   03AC 80 E6              1246 	sjmp	00107$
   03AE                    1247 00116$:
                           1248 ;	main.c:670: handle_tx();
   03AE 12 01 B6           1249 	lcall	_handle_tx
   03B1 80 E1              1250 	sjmp	00107$
                           1251 	.area CSEG    (CODE)
                           1252 	.area CONST   (CODE)
                           1253 	.area XINIT   (CODE)
                           1254 	.area CABS    (ABS,CODE)
