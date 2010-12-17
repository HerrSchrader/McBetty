                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.7 #5820 (May  6 2010) (Linux)
                              4 ; This file was generated Fri Dec 17 01:43:32 2010
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
                             26 	.globl _TCR20_0
                             27 	.globl _TCR20_1
                             28 	.globl _TCR20_2
                             29 	.globl _TCR20_3
                             30 	.globl _TCR20_4
                             31 	.globl _TCR20_5
                             32 	.globl _TCR20_6
                             33 	.globl _TCR20_7
                             34 	.globl _P3_0
                             35 	.globl _P3_1
                             36 	.globl _P2_0
                             37 	.globl _P2_1
                             38 	.globl _P2_2
                             39 	.globl _P2_3
                             40 	.globl _P2_4
                             41 	.globl _P2_5
                             42 	.globl _P2_6
                             43 	.globl _P2_7
                             44 	.globl _P1_0
                             45 	.globl _P1_1
                             46 	.globl _P1_2
                             47 	.globl _P1_3
                             48 	.globl _P1_4
                             49 	.globl _P1_5
                             50 	.globl _P1_6
                             51 	.globl _P1_7
                             52 	.globl _P0_0
                             53 	.globl _P0_1
                             54 	.globl _P0_2
                             55 	.globl _P0_3
                             56 	.globl _P0_4
                             57 	.globl _P0_5
                             58 	.globl _P0_6
                             59 	.globl _P0_7
                             60 	.globl _I2CON_0
                             61 	.globl _I2CON_2
                             62 	.globl _I2CON_3
                             63 	.globl _I2CON_4
                             64 	.globl _I2CON_5
                             65 	.globl _I2CON_6
                             66 	.globl _SCON_0
                             67 	.globl _SCON_1
                             68 	.globl _SCON_2
                             69 	.globl _SCON_3
                             70 	.globl _SCON_4
                             71 	.globl _SCON_5
                             72 	.globl _SCON_6
                             73 	.globl _SCON_7
                             74 	.globl _IP0_0
                             75 	.globl _IP0_1
                             76 	.globl _IP0_2
                             77 	.globl _IP0_3
                             78 	.globl _IP0_4
                             79 	.globl _IP0_5
                             80 	.globl _IP0_6
                             81 	.globl _IEN1_0
                             82 	.globl _IEN1_1
                             83 	.globl _IEN1_2
                             84 	.globl _IEN1_3
                             85 	.globl _IEN1_4
                             86 	.globl _IEN1_6
                             87 	.globl _IEN1_7
                             88 	.globl _IEN0_0
                             89 	.globl _IEN0_1
                             90 	.globl _IEN0_2
                             91 	.globl _IEN0_3
                             92 	.globl _IEN0_4
                             93 	.globl _IEN0_5
                             94 	.globl _IEN0_6
                             95 	.globl _IEN0_7
                             96 	.globl _TCON_0
                             97 	.globl _TCON_1
                             98 	.globl _TCON_2
                             99 	.globl _TCON_3
                            100 	.globl _TCON_4
                            101 	.globl _TCON_5
                            102 	.globl _TCON_6
                            103 	.globl _TCON_7
                            104 	.globl _PSW_0
                            105 	.globl _PSW_1
                            106 	.globl _PSW_2
                            107 	.globl _PSW_3
                            108 	.globl _PSW_4
                            109 	.globl _PSW_5
                            110 	.globl _PSW_6
                            111 	.globl _PSW_7
                            112 	.globl _CMP2_
                            113 	.globl _IP0H
                            114 	.globl _WFEED2
                            115 	.globl _WFEED1
                            116 	.globl _WDL
                            117 	.globl _WDCON
                            118 	.globl _TRIM
                            119 	.globl _TPCR2L
                            120 	.globl _TPCR2H
                            121 	.globl _TOR2L
                            122 	.globl _TOR2H
                            123 	.globl _TISE2
                            124 	.globl _TIFR2
                            125 	.globl _TICR2
                            126 	.globl _TCR21
                            127 	.globl _TCR20
                            128 	.globl _TAMOD
                            129 	.globl _SPDAT
                            130 	.globl _SPSTAT
                            131 	.globl _SPCTL
                            132 	.globl _SSTAT
                            133 	.globl _RTCL
                            134 	.globl _RTCH
                            135 	.globl _RTCCON
                            136 	.globl _RSTSRC
                            137 	.globl _PT0AD
                            138 	.globl _PCONA
                            139 	.globl _P3M2
                            140 	.globl _P3M1
                            141 	.globl _P2M2
                            142 	.globl _P2M1
                            143 	.globl _P1M2
                            144 	.globl _P1M1
                            145 	.globl _P0M2
                            146 	.globl _P0M1
                            147 	.globl _OCRDL
                            148 	.globl _OCRDH
                            149 	.globl _OCRCL
                            150 	.globl _OCRCH
                            151 	.globl _OCRBL
                            152 	.globl _OCRBH
                            153 	.globl _OCRAL
                            154 	.globl _OCRAH
                            155 	.globl _KBPATN
                            156 	.globl _KBMASK
                            157 	.globl _KBCON
                            158 	.globl _IP1H
                            159 	.globl _IP1
                            160 	.globl _IEN1
                            161 	.globl _ICRBL
                            162 	.globl _ICRBH
                            163 	.globl _ICRAL
                            164 	.globl _ICRAH
                            165 	.globl _I2STAT
                            166 	.globl _I2SCLL
                            167 	.globl _I2SCLH
                            168 	.globl _I2DAT
                            169 	.globl _I2CON
                            170 	.globl _I2ADR
                            171 	.globl _DIVM
                            172 	.globl _DEEADR
                            173 	.globl _DEEDAT
                            174 	.globl _DEECON
                            175 	.globl _CMP2
                            176 	.globl _CMP1
                            177 	.globl _CCCRD
                            178 	.globl _CCCRC
                            179 	.globl _CCCRB
                            180 	.globl _CCCRA
                            181 	.globl _BRGCON
                            182 	.globl _BRGR1
                            183 	.globl _BRGR0
                            184 	.globl _TH2
                            185 	.globl _TL2
                            186 	.globl _SADEN
                            187 	.globl _SADDR
                            188 	.globl _AUXR1
                            189 	.globl _SBUF
                            190 	.globl _SCON
                            191 	.globl _IP0
                            192 	.globl _IEN0
                            193 	.globl _TH1
                            194 	.globl _TH0
                            195 	.globl _TL1
                            196 	.globl _TL0
                            197 	.globl _TMOD
                            198 	.globl _TCON
                            199 	.globl _PCON
                            200 	.globl _DPH
                            201 	.globl _DPL
                            202 	.globl _SP
                            203 	.globl _B
                            204 	.globl _ACC
                            205 	.globl _PSW
                            206 	.globl _P3
                            207 	.globl _P2
                            208 	.globl _P1
                            209 	.globl _P0
                            210 	.globl _got_eot
                            211 	.globl _got_etx
                            212 	.globl _buf
                            213 	.globl _bufnxt
                            214 	.globl _bufcnt
                            215 	.globl _bufstart
                            216 	.globl _radio_mode
                            217 ;--------------------------------------------------------
                            218 ; special function registers
                            219 ;--------------------------------------------------------
                            220 	.area RSEG    (ABS,DATA)
   0000                     221 	.org 0x0000
                    0080    222 _P0	=	0x0080
                    0090    223 _P1	=	0x0090
                    00A0    224 _P2	=	0x00a0
                    00B0    225 _P3	=	0x00b0
                    00D0    226 _PSW	=	0x00d0
                    00E0    227 _ACC	=	0x00e0
                    00F0    228 _B	=	0x00f0
                    0081    229 _SP	=	0x0081
                    0082    230 _DPL	=	0x0082
                    0083    231 _DPH	=	0x0083
                    0087    232 _PCON	=	0x0087
                    0088    233 _TCON	=	0x0088
                    0089    234 _TMOD	=	0x0089
                    008A    235 _TL0	=	0x008a
                    008B    236 _TL1	=	0x008b
                    008C    237 _TH0	=	0x008c
                    008D    238 _TH1	=	0x008d
                    00A8    239 _IEN0	=	0x00a8
                    00B8    240 _IP0	=	0x00b8
                    0098    241 _SCON	=	0x0098
                    0099    242 _SBUF	=	0x0099
                    00A2    243 _AUXR1	=	0x00a2
                    00A9    244 _SADDR	=	0x00a9
                    00B9    245 _SADEN	=	0x00b9
                    00CC    246 _TL2	=	0x00cc
                    00CD    247 _TH2	=	0x00cd
                    00BE    248 _BRGR0	=	0x00be
                    00BF    249 _BRGR1	=	0x00bf
                    00BD    250 _BRGCON	=	0x00bd
                    00EA    251 _CCCRA	=	0x00ea
                    00EB    252 _CCCRB	=	0x00eb
                    00EC    253 _CCCRC	=	0x00ec
                    00ED    254 _CCCRD	=	0x00ed
                    00AC    255 _CMP1	=	0x00ac
                    00AD    256 _CMP2	=	0x00ad
                    00F1    257 _DEECON	=	0x00f1
                    00F2    258 _DEEDAT	=	0x00f2
                    00F3    259 _DEEADR	=	0x00f3
                    0095    260 _DIVM	=	0x0095
                    00DB    261 _I2ADR	=	0x00db
                    00D8    262 _I2CON	=	0x00d8
                    00DA    263 _I2DAT	=	0x00da
                    00DD    264 _I2SCLH	=	0x00dd
                    00DC    265 _I2SCLL	=	0x00dc
                    00D9    266 _I2STAT	=	0x00d9
                    00AB    267 _ICRAH	=	0x00ab
                    00AA    268 _ICRAL	=	0x00aa
                    00AF    269 _ICRBH	=	0x00af
                    00AE    270 _ICRBL	=	0x00ae
                    00E8    271 _IEN1	=	0x00e8
                    00F8    272 _IP1	=	0x00f8
                    00F7    273 _IP1H	=	0x00f7
                    0094    274 _KBCON	=	0x0094
                    0086    275 _KBMASK	=	0x0086
                    0093    276 _KBPATN	=	0x0093
                    00EF    277 _OCRAH	=	0x00ef
                    00EE    278 _OCRAL	=	0x00ee
                    00FB    279 _OCRBH	=	0x00fb
                    00FA    280 _OCRBL	=	0x00fa
                    00FD    281 _OCRCH	=	0x00fd
                    00FC    282 _OCRCL	=	0x00fc
                    00FF    283 _OCRDH	=	0x00ff
                    00FE    284 _OCRDL	=	0x00fe
                    0084    285 _P0M1	=	0x0084
                    0085    286 _P0M2	=	0x0085
                    0091    287 _P1M1	=	0x0091
                    0092    288 _P1M2	=	0x0092
                    00A4    289 _P2M1	=	0x00a4
                    00A5    290 _P2M2	=	0x00a5
                    00B1    291 _P3M1	=	0x00b1
                    00B2    292 _P3M2	=	0x00b2
                    00B5    293 _PCONA	=	0x00b5
                    00F6    294 _PT0AD	=	0x00f6
                    00DF    295 _RSTSRC	=	0x00df
                    00D1    296 _RTCCON	=	0x00d1
                    00D2    297 _RTCH	=	0x00d2
                    00D3    298 _RTCL	=	0x00d3
                    00BA    299 _SSTAT	=	0x00ba
                    00E2    300 _SPCTL	=	0x00e2
                    00E1    301 _SPSTAT	=	0x00e1
                    00E3    302 _SPDAT	=	0x00e3
                    008F    303 _TAMOD	=	0x008f
                    00C8    304 _TCR20	=	0x00c8
                    00F9    305 _TCR21	=	0x00f9
                    00C9    306 _TICR2	=	0x00c9
                    00E9    307 _TIFR2	=	0x00e9
                    00DE    308 _TISE2	=	0x00de
                    00CF    309 _TOR2H	=	0x00cf
                    00CE    310 _TOR2L	=	0x00ce
                    00CB    311 _TPCR2H	=	0x00cb
                    00CA    312 _TPCR2L	=	0x00ca
                    0096    313 _TRIM	=	0x0096
                    00A7    314 _WDCON	=	0x00a7
                    00C1    315 _WDL	=	0x00c1
                    00C2    316 _WFEED1	=	0x00c2
                    00C3    317 _WFEED2	=	0x00c3
                    00B7    318 _IP0H	=	0x00b7
                    00AD    319 _CMP2_	=	0x00ad
                            320 ;--------------------------------------------------------
                            321 ; special function bits
                            322 ;--------------------------------------------------------
                            323 	.area RSEG    (ABS,DATA)
   0000                     324 	.org 0x0000
                    00D7    325 _PSW_7	=	0x00d7
                    00D6    326 _PSW_6	=	0x00d6
                    00D5    327 _PSW_5	=	0x00d5
                    00D4    328 _PSW_4	=	0x00d4
                    00D3    329 _PSW_3	=	0x00d3
                    00D2    330 _PSW_2	=	0x00d2
                    00D1    331 _PSW_1	=	0x00d1
                    00D0    332 _PSW_0	=	0x00d0
                    008F    333 _TCON_7	=	0x008f
                    008E    334 _TCON_6	=	0x008e
                    008D    335 _TCON_5	=	0x008d
                    008C    336 _TCON_4	=	0x008c
                    008B    337 _TCON_3	=	0x008b
                    008A    338 _TCON_2	=	0x008a
                    0089    339 _TCON_1	=	0x0089
                    0088    340 _TCON_0	=	0x0088
                    00AF    341 _IEN0_7	=	0x00af
                    00AE    342 _IEN0_6	=	0x00ae
                    00AD    343 _IEN0_5	=	0x00ad
                    00AC    344 _IEN0_4	=	0x00ac
                    00AB    345 _IEN0_3	=	0x00ab
                    00AA    346 _IEN0_2	=	0x00aa
                    00A9    347 _IEN0_1	=	0x00a9
                    00A8    348 _IEN0_0	=	0x00a8
                    00EF    349 _IEN1_7	=	0x00ef
                    00EE    350 _IEN1_6	=	0x00ee
                    00EC    351 _IEN1_4	=	0x00ec
                    00EB    352 _IEN1_3	=	0x00eb
                    00EA    353 _IEN1_2	=	0x00ea
                    00E9    354 _IEN1_1	=	0x00e9
                    00E8    355 _IEN1_0	=	0x00e8
                    00BE    356 _IP0_6	=	0x00be
                    00BD    357 _IP0_5	=	0x00bd
                    00BC    358 _IP0_4	=	0x00bc
                    00BB    359 _IP0_3	=	0x00bb
                    00BA    360 _IP0_2	=	0x00ba
                    00B9    361 _IP0_1	=	0x00b9
                    00B8    362 _IP0_0	=	0x00b8
                    009F    363 _SCON_7	=	0x009f
                    009E    364 _SCON_6	=	0x009e
                    009D    365 _SCON_5	=	0x009d
                    009C    366 _SCON_4	=	0x009c
                    009B    367 _SCON_3	=	0x009b
                    009A    368 _SCON_2	=	0x009a
                    0099    369 _SCON_1	=	0x0099
                    0098    370 _SCON_0	=	0x0098
                    00DE    371 _I2CON_6	=	0x00de
                    00DD    372 _I2CON_5	=	0x00dd
                    00DC    373 _I2CON_4	=	0x00dc
                    00DB    374 _I2CON_3	=	0x00db
                    00DA    375 _I2CON_2	=	0x00da
                    00D8    376 _I2CON_0	=	0x00d8
                    0087    377 _P0_7	=	0x0087
                    0086    378 _P0_6	=	0x0086
                    0085    379 _P0_5	=	0x0085
                    0084    380 _P0_4	=	0x0084
                    0083    381 _P0_3	=	0x0083
                    0082    382 _P0_2	=	0x0082
                    0081    383 _P0_1	=	0x0081
                    0080    384 _P0_0	=	0x0080
                    0097    385 _P1_7	=	0x0097
                    0096    386 _P1_6	=	0x0096
                    0095    387 _P1_5	=	0x0095
                    0094    388 _P1_4	=	0x0094
                    0093    389 _P1_3	=	0x0093
                    0092    390 _P1_2	=	0x0092
                    0091    391 _P1_1	=	0x0091
                    0090    392 _P1_0	=	0x0090
                    00A7    393 _P2_7	=	0x00a7
                    00A6    394 _P2_6	=	0x00a6
                    00A5    395 _P2_5	=	0x00a5
                    00A4    396 _P2_4	=	0x00a4
                    00A3    397 _P2_3	=	0x00a3
                    00A2    398 _P2_2	=	0x00a2
                    00A1    399 _P2_1	=	0x00a1
                    00A0    400 _P2_0	=	0x00a0
                    00B1    401 _P3_1	=	0x00b1
                    00B0    402 _P3_0	=	0x00b0
                    00CF    403 _TCR20_7	=	0x00cf
                    00CE    404 _TCR20_6	=	0x00ce
                    00CD    405 _TCR20_5	=	0x00cd
                    00CC    406 _TCR20_4	=	0x00cc
                    00CB    407 _TCR20_3	=	0x00cb
                    00CA    408 _TCR20_2	=	0x00ca
                    00C9    409 _TCR20_1	=	0x00c9
                    00C8    410 _TCR20_0	=	0x00c8
                            411 ;--------------------------------------------------------
                            412 ; overlayable register banks
                            413 ;--------------------------------------------------------
                            414 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     415 	.ds 8
                            416 ;--------------------------------------------------------
                            417 ; internal ram data
                            418 ;--------------------------------------------------------
                            419 	.area DSEG    (DATA)
   0008                     420 _radio_mode::
   0008                     421 	.ds 1
   0009                     422 _bufstart::
   0009                     423 	.ds 1
   000A                     424 _bufcnt::
   000A                     425 	.ds 1
   000B                     426 _bufnxt::
   000B                     427 	.ds 1
   000C                     428 _handle_tx_tx_state_1_1:
   000C                     429 	.ds 1
   000D                     430 _handle_tx_tx_cnt_1_1:
   000D                     431 	.ds 1
   000E                     432 _check_radio_packet_x_1_1:
   000E                     433 	.ds 1
   000F                     434 _check_radio_packet_length_1_1:
   000F                     435 	.ds 1
   0010                     436 _check_radio_packet_address_1_1:
   0010                     437 	.ds 1
   0011                     438 _check_radio_input_length_1_1:
   0011                     439 	.ds 1
   0012                     440 _check_radio_input_address_1_1:
   0012                     441 	.ds 1
   0013                     442 _check_radio_input_x_1_1:
   0013                     443 	.ds 1
                            444 ;--------------------------------------------------------
                            445 ; overlayable items in internal ram 
                            446 ;--------------------------------------------------------
                            447 	.area	OSEG    (OVR,DATA)
                            448 ;--------------------------------------------------------
                            449 ; Stack segment in internal ram 
                            450 ;--------------------------------------------------------
                            451 	.area	SSEG	(DATA)
   009B                     452 __start__stack:
   009B                     453 	.ds	1
                            454 
                            455 ;--------------------------------------------------------
                            456 ; indirectly addressable internal ram data
                            457 ;--------------------------------------------------------
                            458 	.area ISEG    (DATA)
   004D                     459 _buf::
   004D                     460 	.ds 78
                            461 ;--------------------------------------------------------
                            462 ; absolute internal ram data
                            463 ;--------------------------------------------------------
                            464 	.area IABS    (ABS,DATA)
                            465 	.area IABS    (ABS,DATA)
                            466 ;--------------------------------------------------------
                            467 ; bit data
                            468 ;--------------------------------------------------------
                            469 	.area BSEG    (BIT)
   0000                     470 _got_etx::
   0000                     471 	.ds 1
   0001                     472 _got_eot::
   0001                     473 	.ds 1
                            474 ;--------------------------------------------------------
                            475 ; paged external ram data
                            476 ;--------------------------------------------------------
                            477 	.area PSEG    (PAG,XDATA)
                            478 ;--------------------------------------------------------
                            479 ; external ram data
                            480 ;--------------------------------------------------------
                            481 	.area XSEG    (XDATA)
                            482 ;--------------------------------------------------------
                            483 ; absolute external ram data
                            484 ;--------------------------------------------------------
                            485 	.area XABS    (ABS,XDATA)
                            486 ;--------------------------------------------------------
                            487 ; external initialized ram data
                            488 ;--------------------------------------------------------
                            489 	.area XISEG   (XDATA)
                            490 	.area HOME    (CODE)
                            491 	.area GSINIT0 (CODE)
                            492 	.area GSINIT1 (CODE)
                            493 	.area GSINIT2 (CODE)
                            494 	.area GSINIT3 (CODE)
                            495 	.area GSINIT4 (CODE)
                            496 	.area GSINIT5 (CODE)
                            497 	.area GSINIT  (CODE)
                            498 	.area GSFINAL (CODE)
                            499 	.area CSEG    (CODE)
                            500 ;--------------------------------------------------------
                            501 ; interrupt vector 
                            502 ;--------------------------------------------------------
                            503 	.area HOME    (CODE)
   0000                     504 __interrupt_vect:
   0000 02 00 2B            505 	ljmp	__sdcc_gsinit_startup
   0003 32                  506 	reti
   0004                     507 	.ds	7
   000B 32                  508 	reti
   000C                     509 	.ds	7
   0013 32                  510 	reti
   0014                     511 	.ds	7
   001B 32                  512 	reti
   001C                     513 	.ds	7
   0023 02 00 97            514 	ljmp	_serial_isr
                            515 ;--------------------------------------------------------
                            516 ; global & static initialisations
                            517 ;--------------------------------------------------------
                            518 	.area HOME    (CODE)
                            519 	.area GSINIT  (CODE)
                            520 	.area GSFINAL (CODE)
                            521 	.area GSINIT  (CODE)
                            522 	.globl __sdcc_gsinit_startup
                            523 	.globl __sdcc_program_startup
                            524 	.globl __start__stack
                            525 	.globl __mcs51_genXINIT
                            526 	.globl __mcs51_genXRAMCLEAR
                            527 	.globl __mcs51_genRAMCLEAR
                            528 ;------------------------------------------------------------
                            529 ;Allocation info for local variables in function 'handle_tx'
                            530 ;------------------------------------------------------------
                            531 ;tx_state                  Allocated with name '_handle_tx_tx_state_1_1'
                            532 ;tx_cnt                    Allocated with name '_handle_tx_tx_cnt_1_1'
                            533 ;x                         Allocated to registers 
                            534 ;------------------------------------------------------------
                            535 ;	main.c:325: static unsigned char tx_state = TX_IDLE;
   0084 75 0C 00            536 	mov	_handle_tx_tx_state_1_1,#0x00
                            537 ;------------------------------------------------------------
                            538 ;Allocation info for local variables in function 'check_radio_input'
                            539 ;------------------------------------------------------------
                            540 ;length                    Allocated with name '_check_radio_input_length_1_1'
                            541 ;address                   Allocated with name '_check_radio_input_address_1_1'
                            542 ;n                         Allocated to registers r2 
                            543 ;x                         Allocated with name '_check_radio_input_x_1_1'
                            544 ;------------------------------------------------------------
                            545 ;	main.c:438: static unsigned char length = 0;
   0087 75 11 00            546 	mov	_check_radio_input_length_1_1,#0x00
                            547 	.area GSFINAL (CODE)
   008A 02 00 26            548 	ljmp	__sdcc_program_startup
                            549 ;--------------------------------------------------------
                            550 ; Home
                            551 ;--------------------------------------------------------
                            552 	.area HOME    (CODE)
                            553 	.area HOME    (CODE)
   0026                     554 __sdcc_program_startup:
   0026 12 03 35            555 	lcall	_main
                            556 ;	return from main will lock up
   0029 80 FE               557 	sjmp .
                            558 ;--------------------------------------------------------
                            559 ; code
                            560 ;--------------------------------------------------------
                            561 	.area CSEG    (CODE)
                            562 ;------------------------------------------------------------
                            563 ;Allocation info for local variables in function 'buffer_init'
                            564 ;------------------------------------------------------------
                            565 ;------------------------------------------------------------
                            566 ;	main.c:155: void buffer_init(){
                            567 ;	-----------------------------------------
                            568 ;	 function buffer_init
                            569 ;	-----------------------------------------
   008D                     570 _buffer_init:
                    0002    571 	ar2 = 0x02
                    0003    572 	ar3 = 0x03
                    0004    573 	ar4 = 0x04
                    0005    574 	ar5 = 0x05
                    0006    575 	ar6 = 0x06
                    0007    576 	ar7 = 0x07
                    0000    577 	ar0 = 0x00
                    0001    578 	ar1 = 0x01
                            579 ;	main.c:156: bufcnt = 0;			// Number of bytes in the buffer
   008D 75 0A 00            580 	mov	_bufcnt,#0x00
                            581 ;	main.c:157: bufnxt = 0;			// Index of next free place in buffer
   0090 75 0B 00            582 	mov	_bufnxt,#0x00
                            583 ;	main.c:158: bufstart = 0;		// Index of first data byte in buffer
   0093 75 09 00            584 	mov	_bufstart,#0x00
   0096 22                  585 	ret
                            586 ;------------------------------------------------------------
                            587 ;Allocation info for local variables in function 'serial_isr'
                            588 ;------------------------------------------------------------
                            589 ;x                         Allocated to registers r2 
                            590 ;------------------------------------------------------------
                            591 ;	main.c:174: void serial_isr (void) __interrupt (4) {
                            592 ;	-----------------------------------------
                            593 ;	 function serial_isr
                            594 ;	-----------------------------------------
   0097                     595 _serial_isr:
   0097 C0 E0               596 	push	acc
   0099 C0 02               597 	push	ar2
   009B C0 03               598 	push	ar3
   009D C0 00               599 	push	ar0
   009F C0 D0               600 	push	psw
   00A1 75 D0 00            601 	mov	psw,#0x00
                            602 ;	main.c:177: x=SBUF;
   00A4 AA 99               603 	mov	r2,_SBUF
                            604 ;	main.c:178: RI = 0;
   00A6 C2 98               605 	clr	_SCON_0
                            606 ;	main.c:183: if (x == ETX){
   00A8 BA 03 04            607 	cjne	r2,#0x03,00102$
                            608 ;	main.c:184: got_etx = 1;
   00AB D2 00               609 	setb	_got_etx
                            610 ;	main.c:185: return;
   00AD 80 32               611 	sjmp	00114$
   00AF                     612 00102$:
                            613 ;	main.c:189: if (got_eot){
   00AF 30 01 02            614 	jnb	_got_eot,00104$
                            615 ;	main.c:190: return;
   00B2 80 2D               616 	sjmp	00114$
   00B4                     617 00104$:
                            618 ;	main.c:194: if (bufcnt >= BUFSIZE){
   00B4 74 B2               619 	mov	a,#0x100 - 0x4E
   00B6 25 0A               620 	add	a,_bufcnt
   00B8 50 0D               621 	jnc	00109$
                            622 ;	main.c:195: bufcnt--;
   00BA 15 0A               623 	dec	_bufcnt
                            624 ;	main.c:197: if (bufnxt == 0)
   00BC E5 0B               625 	mov	a,_bufnxt
   00BE 70 05               626 	jnz	00106$
                            627 ;	main.c:198: bufnxt = BUFSIZE-1;
   00C0 75 0B 4D            628 	mov	_bufnxt,#0x4D
   00C3 80 02               629 	sjmp	00109$
   00C5                     630 00106$:
                            631 ;	main.c:200: bufnxt--;
   00C5 15 0B               632 	dec	_bufnxt
   00C7                     633 00109$:
                            634 ;	main.c:204: buf[bufnxt++] = x;
   00C7 AB 0B               635 	mov	r3,_bufnxt
   00C9 05 0B               636 	inc	_bufnxt
   00CB EB                  637 	mov	a,r3
   00CC 24 4D               638 	add	a,#_buf
   00CE F8                  639 	mov	r0,a
   00CF A6 02               640 	mov	@r0,ar2
                            641 ;	main.c:205: if (bufnxt >= BUFSIZE)
   00D1 74 B2               642 	mov	a,#0x100 - 0x4E
   00D3 25 0B               643 	add	a,_bufnxt
   00D5 50 03               644 	jnc	00111$
                            645 ;	main.c:206: bufnxt = 0;
   00D7 75 0B 00            646 	mov	_bufnxt,#0x00
   00DA                     647 00111$:
                            648 ;	main.c:207: bufcnt++;
   00DA 05 0A               649 	inc	_bufcnt
                            650 ;	main.c:209: if (x == EOT)
   00DC BA 04 02            651 	cjne	r2,#0x04,00113$
                            652 ;	main.c:210: got_eot = 1;
   00DF D2 01               653 	setb	_got_eot
   00E1                     654 00113$:
                            655 ;	main.c:212: return;
   00E1                     656 00114$:
   00E1 D0 D0               657 	pop	psw
   00E3 D0 00               658 	pop	ar0
   00E5 D0 03               659 	pop	ar3
   00E7 D0 02               660 	pop	ar2
   00E9 D0 E0               661 	pop	acc
   00EB 32                  662 	reti
                            663 ;	eliminated unneeded push/pop ar1
                            664 ;	eliminated unneeded push/pop dpl
                            665 ;	eliminated unneeded push/pop dph
                            666 ;	eliminated unneeded push/pop b
                            667 ;------------------------------------------------------------
                            668 ;Allocation info for local variables in function 'buffer_out'
                            669 ;------------------------------------------------------------
                            670 ;x                         Allocated to registers r2 
                            671 ;------------------------------------------------------------
                            672 ;	main.c:219: char buffer_out(){
                            673 ;	-----------------------------------------
                            674 ;	 function buffer_out
                            675 ;	-----------------------------------------
   00EC                     676 _buffer_out:
                            677 ;	main.c:222: if (bufcnt == 0) return (0);
   00EC E5 0A               678 	mov	a,_bufcnt
   00EE 70 03               679 	jnz	00102$
   00F0 F5 82               680 	mov	dpl,a
   00F2 22                  681 	ret
   00F3                     682 00102$:
                            683 ;	main.c:223: x = buf[bufstart++];
   00F3 AA 09               684 	mov	r2,_bufstart
   00F5 05 09               685 	inc	_bufstart
   00F7 EA                  686 	mov	a,r2
   00F8 24 4D               687 	add	a,#_buf
   00FA F8                  688 	mov	r0,a
   00FB 86 02               689 	mov	ar2,@r0
                            690 ;	main.c:224: if (bufstart >= BUFSIZE)
   00FD 74 B2               691 	mov	a,#0x100 - 0x4E
   00FF 25 09               692 	add	a,_bufstart
   0101 50 03               693 	jnc	00104$
                            694 ;	main.c:225: bufstart = 0;
   0103 75 09 00            695 	mov	_bufstart,#0x00
   0106                     696 00104$:
                            697 ;	main.c:228: bufcnt--;
   0106 15 0A               698 	dec	_bufcnt
                            699 ;	main.c:230: return x;
   0108 8A 82               700 	mov	dpl,r2
   010A 22                  701 	ret
                            702 ;------------------------------------------------------------
                            703 ;Allocation info for local variables in function 'has_room'
                            704 ;------------------------------------------------------------
                            705 ;------------------------------------------------------------
                            706 ;	main.c:247: unsigned char has_room(){
                            707 ;	-----------------------------------------
                            708 ;	 function has_room
                            709 ;	-----------------------------------------
   010B                     710 _has_room:
                            711 ;	main.c:248: return ( (BUFSIZE - bufcnt) > (MPDTOOL_PKTSIZE + 2) );
   010B AA 0A               712 	mov	r2,_bufcnt
   010D 7B 00               713 	mov	r3,#0x00
   010F 74 4E               714 	mov	a,#0x4E
   0111 C3                  715 	clr	c
   0112 9A                  716 	subb	a,r2
   0113 FA                  717 	mov	r2,a
   0114 E4                  718 	clr	a
   0115 9B                  719 	subb	a,r3
   0116 FB                  720 	mov	r3,a
   0117 C3                  721 	clr	c
   0118 74 12               722 	mov	a,#0x12
   011A 9A                  723 	subb	a,r2
   011B 74 80               724 	mov	a,#(0x00 ^ 0x80)
   011D 8B F0               725 	mov	b,r3
   011F 63 F0 80            726 	xrl	b,#0x80
   0122 95 F0               727 	subb	a,b
   0124 E4                  728 	clr	a
   0125 33                  729 	rlc	a
   0126 F5 82               730 	mov	dpl,a
   0128 22                  731 	ret
                            732 ;------------------------------------------------------------
                            733 ;Allocation info for local variables in function 'rx_overflow_reset'
                            734 ;------------------------------------------------------------
                            735 ;------------------------------------------------------------
                            736 ;	main.c:258: rx_overflow_reset(){
                            737 ;	-----------------------------------------
                            738 ;	 function rx_overflow_reset
                            739 ;	-----------------------------------------
   0129                     740 _rx_overflow_reset:
                            741 ;	main.c:259: if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) == RX_OVERFLOW){
   0129 75 82 F5            742 	mov	dpl,#0xF5
   012C 12 05 DB            743 	lcall	_cc1100_read_status_reg_otf
   012F E5 82               744 	mov	a,dpl
   0131 54 1F               745 	anl	a,#0x1F
   0133 FA                  746 	mov	r2,a
   0134 BA 11 0C            747 	cjne	r2,#0x11,00103$
                            748 ;	main.c:260: cc1100_strobe(SFRX);
   0137 75 82 3A            749 	mov	dpl,#0x3A
   013A 12 05 CC            750 	lcall	_cc1100_strobe
                            751 ;	main.c:261: cc1100_strobe(SRX);
   013D 75 82 34            752 	mov	dpl,#0x34
   0140 02 05 CC            753 	ljmp	_cc1100_strobe
   0143                     754 00103$:
   0143 22                  755 	ret
                            756 ;------------------------------------------------------------
                            757 ;Allocation info for local variables in function 'start_rx'
                            758 ;------------------------------------------------------------
                            759 ;------------------------------------------------------------
                            760 ;	main.c:267: void start_rx() {
                            761 ;	-----------------------------------------
                            762 ;	 function start_rx
                            763 ;	-----------------------------------------
   0144                     764 _start_rx:
                            765 ;	main.c:269: rx_overflow_reset();
   0144 12 01 29            766 	lcall	_rx_overflow_reset
                            767 ;	main.c:270: switch_to_idle();
   0147 12 06 0B            768 	lcall	_switch_to_idle
                            769 ;	main.c:272: cc1100_strobe(SFRX);
   014A 75 82 3A            770 	mov	dpl,#0x3A
   014D 12 05 CC            771 	lcall	_cc1100_strobe
                            772 ;	main.c:273: cc1100_strobe(SCAL);
   0150 75 82 33            773 	mov	dpl,#0x33
   0153 12 05 CC            774 	lcall	_cc1100_strobe
                            775 ;	main.c:274: cc1100_strobe(SRX);
   0156 75 82 34            776 	mov	dpl,#0x34
   0159 12 05 CC            777 	lcall	_cc1100_strobe
                            778 ;	main.c:275: radio_mode = RADIO_RX;
   015C 75 08 01            779 	mov	_radio_mode,#0x01
   015F 22                  780 	ret
                            781 ;------------------------------------------------------------
                            782 ;Allocation info for local variables in function 'start_tx'
                            783 ;------------------------------------------------------------
                            784 ;------------------------------------------------------------
                            785 ;	main.c:280: void start_tx() {	
                            786 ;	-----------------------------------------
                            787 ;	 function start_tx
                            788 ;	-----------------------------------------
   0160                     789 _start_tx:
                            790 ;	main.c:281: rx_overflow_reset();
   0160 12 01 29            791 	lcall	_rx_overflow_reset
                            792 ;	main.c:282: switch_to_idle();
   0163 12 06 0B            793 	lcall	_switch_to_idle
                            794 ;	main.c:284: cc1100_strobe(SCAL);
   0166 75 82 33            795 	mov	dpl,#0x33
   0169 12 05 CC            796 	lcall	_cc1100_strobe
                            797 ;	main.c:285: cc1100_strobe(STX);
   016C 75 82 35            798 	mov	dpl,#0x35
   016F 12 05 CC            799 	lcall	_cc1100_strobe
                            800 ;	main.c:286: radio_mode = RADIO_TX;
   0172 75 08 02            801 	mov	_radio_mode,#0x02
   0175 22                  802 	ret
                            803 ;------------------------------------------------------------
                            804 ;Allocation info for local variables in function 'tx_finished'
                            805 ;------------------------------------------------------------
                            806 ;------------------------------------------------------------
                            807 ;	main.c:290: unsigned char tx_finished(){
                            808 ;	-----------------------------------------
                            809 ;	 function tx_finished
                            810 ;	-----------------------------------------
   0176                     811 _tx_finished:
                            812 ;	main.c:291: return ( (cc1100_read_status_reg_otf(TXBYTES) & 0x7f) == 0  &&
   0176 75 82 FA            813 	mov	dpl,#0xFA
   0179 12 05 DB            814 	lcall	_cc1100_read_status_reg_otf
   017C E5 82               815 	mov	a,dpl
   017E 54 7F               816 	anl	a,#0x7F
   0180 FA                  817 	mov	r2,a
   0181 60 02               818 	jz	00108$
   0183 80 0D               819 	sjmp	00103$
   0185                     820 00108$:
                            821 ;	main.c:292: ( (cc1100_read1(PKTSTATUS | BURST) & (1<<2)) == 0 ) );		// GDO2 == 0
   0185 75 82 F8            822 	mov	dpl,#0xF8
   0188 12 05 B4            823 	lcall	_cc1100_read1
   018B E5 82               824 	mov	a,dpl
   018D 54 04               825 	anl	a,#0x04
   018F FA                  826 	mov	r2,a
   0190 60 04               827 	jz	00104$
   0192                     828 00103$:
   0192 7A 00               829 	mov	r2,#0x00
   0194 80 02               830 	sjmp	00105$
   0196                     831 00104$:
   0196 7A 01               832 	mov	r2,#0x01
   0198                     833 00105$:
   0198 8A 82               834 	mov	dpl,r2
   019A 22                  835 	ret
                            836 ;------------------------------------------------------------
                            837 ;Allocation info for local variables in function 're_enter_rx'
                            838 ;------------------------------------------------------------
                            839 ;------------------------------------------------------------
                            840 ;	main.c:296: void re_enter_rx(){
                            841 ;	-----------------------------------------
                            842 ;	 function re_enter_rx
                            843 ;	-----------------------------------------
   019B                     844 _re_enter_rx:
                            845 ;	main.c:297: if ((radio_mode == RADIO_TX) && tx_finished())
   019B 74 02               846 	mov	a,#0x02
   019D B5 08 0A            847 	cjne	a,_radio_mode,00104$
   01A0 12 01 76            848 	lcall	_tx_finished
   01A3 E5 82               849 	mov	a,dpl
   01A5 60 03               850 	jz	00104$
                            851 ;	main.c:298: start_rx();
   01A7 02 01 44            852 	ljmp	_start_rx
   01AA                     853 00104$:
   01AA 22                  854 	ret
                            855 ;------------------------------------------------------------
                            856 ;Allocation info for local variables in function 'handle_tx'
                            857 ;------------------------------------------------------------
                            858 ;tx_state                  Allocated with name '_handle_tx_tx_state_1_1'
                            859 ;tx_cnt                    Allocated with name '_handle_tx_tx_cnt_1_1'
                            860 ;x                         Allocated to registers 
                            861 ;------------------------------------------------------------
                            862 ;	main.c:322: void handle_tx(){
                            863 ;	-----------------------------------------
                            864 ;	 function handle_tx
                            865 ;	-----------------------------------------
   01AB                     866 _handle_tx:
                            867 ;	main.c:329: switch (tx_state){
   01AB E4                  868 	clr	a
   01AC B5 0C 02            869 	cjne	a,_handle_tx_tx_state_1_1,00121$
   01AF 80 0E               870 	sjmp	00101$
   01B1                     871 00121$:
   01B1 74 01               872 	mov	a,#0x01
   01B3 B5 0C 02            873 	cjne	a,_handle_tx_tx_state_1_1,00122$
   01B6 80 45               874 	sjmp	00107$
   01B8                     875 00122$:
   01B8 74 02               876 	mov	a,#0x02
                            877 ;	main.c:331: case TX_IDLE:
   01BA B5 0C 5D            878 	cjne	a,_handle_tx_tx_state_1_1,00113$
   01BD 80 55               879 	sjmp	00111$
   01BF                     880 00101$:
                            881 ;	main.c:333: if (got_eot) {
   01BF 30 01 1C            882 	jnb	_got_eot,00105$
                            883 ;	main.c:334: tx_cnt = bufcnt;	/* = number of payload bytes to be transferred to TXFIFO (not counting address and length byte) */
                            884 ;	main.c:337: cc1100_write1(TX_fifo, tx_cnt + 1);
   01C2 E5 0A               885 	mov	a,_bufcnt
   01C4 F5 0D               886 	mov	_handle_tx_tx_cnt_1_1,a
   01C6 04                  887 	inc	a
   01C7 F5 18               888 	mov	_cc1100_write1_PARM_2,a
   01C9 75 82 3F            889 	mov	dpl,#0x3F
   01CC 12 05 46            890 	lcall	_cc1100_write1
                            891 ;	main.c:340: cc1100_write1(TX_fifo, DEV_ADDR);	
   01CF 75 18 01            892 	mov	_cc1100_write1_PARM_2,#0x01
   01D2 75 82 3F            893 	mov	dpl,#0x3F
   01D5 12 05 46            894 	lcall	_cc1100_write1
                            895 ;	main.c:342: tx_state = TX_COPY;
   01D8 75 0C 01            896 	mov	_handle_tx_tx_state_1_1,#0x01
                            897 ;	main.c:343: got_eot=0;
   01DB C2 01               898 	clr	_got_eot
                            899 ;	main.c:345: break;
   01DD 22                  900 	ret
   01DE                     901 00105$:
                            902 ;	main.c:347: } else if ( bufcnt >= MAX_TX_PAYLOAD ) {
   01DE 74 C4               903 	mov	a,#0x100 - 0x3C
   01E0 25 0A               904 	add	a,_bufcnt
   01E2 50 36               905 	jnc	00113$
                            906 ;	main.c:350: tx_cnt = MAX_TX_PAYLOAD;	
   01E4 75 0D 3C            907 	mov	_handle_tx_tx_cnt_1_1,#0x3C
                            908 ;	main.c:353: cc1100_write1(TX_fifo, MAX_TX_PAYLOAD + 1);
   01E7 75 18 3D            909 	mov	_cc1100_write1_PARM_2,#0x3D
   01EA 75 82 3F            910 	mov	dpl,#0x3F
   01ED 12 05 46            911 	lcall	_cc1100_write1
                            912 ;	main.c:356: cc1100_write1(TX_fifo, DEV_ADDR);	
   01F0 75 18 01            913 	mov	_cc1100_write1_PARM_2,#0x01
   01F3 75 82 3F            914 	mov	dpl,#0x3F
   01F6 12 05 46            915 	lcall	_cc1100_write1
                            916 ;	main.c:358: tx_state = TX_COPY;
   01F9 75 0C 01            917 	mov	_handle_tx_tx_state_1_1,#0x01
                            918 ;	main.c:360: break;
                            919 ;	main.c:362: case TX_COPY:
   01FC 22                  920 	ret
   01FD                     921 00107$:
                            922 ;	main.c:364: if (tx_cnt > 0){
   01FD E5 0D               923 	mov	a,_handle_tx_tx_cnt_1_1
   01FF 60 0F               924 	jz	00109$
                            925 ;	main.c:365: x = buffer_out();
   0201 12 00 EC            926 	lcall	_buffer_out
   0204 85 82 18            927 	mov	_cc1100_write1_PARM_2,dpl
                            928 ;	main.c:366: cc1100_write1(TX_fifo, x);	
   0207 75 82 3F            929 	mov	dpl,#0x3F
   020A 12 05 46            930 	lcall	_cc1100_write1
                            931 ;	main.c:367: tx_cnt--;
   020D 15 0D               932 	dec	_handle_tx_tx_cnt_1_1
   020F 22                  933 	ret
   0210                     934 00109$:
                            935 ;	main.c:370: tx_state = TX_SEND;
   0210 75 0C 02            936 	mov	_handle_tx_tx_state_1_1,#0x02
                            937 ;	main.c:372: break;
                            938 ;	main.c:374: case TX_SEND:
   0213 22                  939 	ret
   0214                     940 00111$:
                            941 ;	main.c:375: start_tx();
   0214 12 01 60            942 	lcall	_start_tx
                            943 ;	main.c:376: tx_state = TX_IDLE;
   0217 75 0C 00            944 	mov	_handle_tx_tx_state_1_1,#0x00
                            945 ;	main.c:378: }
   021A                     946 00113$:
   021A 22                  947 	ret
                            948 ;------------------------------------------------------------
                            949 ;Allocation info for local variables in function 'check_etx'
                            950 ;------------------------------------------------------------
                            951 ;------------------------------------------------------------
                            952 ;	main.c:382: void check_etx(){
                            953 ;	-----------------------------------------
                            954 ;	 function check_etx
                            955 ;	-----------------------------------------
   021B                     956 _check_etx:
                            957 ;	main.c:383: if (got_etx) {	/* Is the sender waiting for an ACK ? */		
   021B 30 00 0F            958 	jnb	_got_etx,00105$
                            959 ;	main.c:384: if (has_room()){	/* Still enough space in buffer ? */
   021E 12 01 0B            960 	lcall	_has_room
   0221 E5 82               961 	mov	a,dpl
   0223 60 08               962 	jz	00105$
                            963 ;	main.c:385: got_etx = 0;		// Atomic Operation ! (see sdcc manual)
   0225 C2 00               964 	clr	_got_etx
                            965 ;	main.c:386: send_byte(ACK);
   0227 75 82 06            966 	mov	dpl,#0x06
   022A 02 04 58            967 	ljmp	_send_byte
   022D                     968 00105$:
   022D 22                  969 	ret
                            970 ;------------------------------------------------------------
                            971 ;Allocation info for local variables in function 'check_radio_packet'
                            972 ;------------------------------------------------------------
                            973 ;x                         Allocated with name '_check_radio_packet_x_1_1'
                            974 ;i                         Allocated to registers r2 
                            975 ;length                    Allocated with name '_check_radio_packet_length_1_1'
                            976 ;address                   Allocated with name '_check_radio_packet_address_1_1'
                            977 ;------------------------------------------------------------
                            978 ;	main.c:397: void check_radio_packet (){
                            979 ;	-----------------------------------------
                            980 ;	 function check_radio_packet
                            981 ;	-----------------------------------------
   022E                     982 _check_radio_packet:
                            983 ;	main.c:401: if ( GDO0 ){
   022E 30 86 6B            984 	jnb	_P0_6,00109$
                            985 ;	main.c:404: cc1100_read(RX_fifo|BURST, &length, 1);		// Length byte = payload length + 1
   0231 75 19 0F            986 	mov	_cc1100_read_PARM_2,#_check_radio_packet_length_1_1
   0234 75 1A 00            987 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0237 75 1B 40            988 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   023A 75 1C 01            989 	mov	_cc1100_read_PARM_3,#0x01
   023D 75 82 7F            990 	mov	dpl,#0x7F
   0240 12 05 63            991 	lcall	_cc1100_read
                            992 ;	main.c:406: cc1100_read(RX_fifo|BURST, &address, 1); 	// Address byte (unused!)
   0243 75 19 10            993 	mov	_cc1100_read_PARM_2,#_check_radio_packet_address_1_1
   0246 75 1A 00            994 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0249 75 1B 40            995 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   024C 75 1C 01            996 	mov	_cc1100_read_PARM_3,#0x01
   024F 75 82 7F            997 	mov	dpl,#0x7F
   0252 12 05 63            998 	lcall	_cc1100_read
                            999 ;	main.c:409: for (i=0; i < (length - 1); i++){
   0255 7A 00              1000 	mov	r2,#0x00
   0257                    1001 00103$:
   0257 AB 0F              1002 	mov	r3,_check_radio_packet_length_1_1
   0259 7C 00              1003 	mov	r4,#0x00
   025B 1B                 1004 	dec	r3
   025C BB FF 01           1005 	cjne	r3,#0xff,00117$
   025F 1C                 1006 	dec	r4
   0260                    1007 00117$:
   0260 8A 05              1008 	mov	ar5,r2
   0262 7E 00              1009 	mov	r6,#0x00
   0264 C3                 1010 	clr	c
   0265 ED                 1011 	mov	a,r5
   0266 9B                 1012 	subb	a,r3
   0267 EE                 1013 	mov	a,r6
   0268 64 80              1014 	xrl	a,#0x80
   026A 8C F0              1015 	mov	b,r4
   026C 63 F0 80           1016 	xrl	b,#0x80
   026F 95 F0              1017 	subb	a,b
   0271 50 26              1018 	jnc	00106$
                           1019 ;	main.c:410: cc1100_read(RX_fifo|BURST, &x, 1);
   0273 75 19 0E           1020 	mov	_cc1100_read_PARM_2,#_check_radio_packet_x_1_1
   0276 75 1A 00           1021 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0279 75 1B 40           1022 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   027C 75 1C 01           1023 	mov	_cc1100_read_PARM_3,#0x01
   027F 75 82 7F           1024 	mov	dpl,#0x7F
   0282 C0 02              1025 	push	ar2
   0284 12 05 63           1026 	lcall	_cc1100_read
                           1027 ;	main.c:411: send_byte(x);
   0287 85 0E 82           1028 	mov	dpl,_check_radio_packet_x_1_1
   028A 12 04 58           1029 	lcall	_send_byte
   028D D0 02              1030 	pop	ar2
                           1031 ;	main.c:412: if (x == EOT)
   028F 74 04              1032 	mov	a,#0x04
   0291 B5 0E 02           1033 	cjne	a,_check_radio_packet_x_1_1,00119$
   0294 80 03              1034 	sjmp	00106$
   0296                    1035 00119$:
                           1036 ;	main.c:409: for (i=0; i < (length - 1); i++){
   0296 0A                 1037 	inc	r2
   0297 80 BE              1038 	sjmp	00103$
   0299                    1039 00106$:
                           1040 ;	main.c:415: start_rx();	
   0299 02 01 44           1041 	ljmp	_start_rx
   029C                    1042 00109$:
   029C 22                 1043 	ret
                           1044 ;------------------------------------------------------------
                           1045 ;Allocation info for local variables in function 'check_radio_input'
                           1046 ;------------------------------------------------------------
                           1047 ;length                    Allocated with name '_check_radio_input_length_1_1'
                           1048 ;address                   Allocated with name '_check_radio_input_address_1_1'
                           1049 ;n                         Allocated to registers r2 
                           1050 ;x                         Allocated with name '_check_radio_input_x_1_1'
                           1051 ;------------------------------------------------------------
                           1052 ;	main.c:437: void check_radio_input (){
                           1053 ;	-----------------------------------------
                           1054 ;	 function check_radio_input
                           1055 ;	-----------------------------------------
   029D                    1056 _check_radio_input:
                           1057 ;	main.c:444: if (length == 0){
   029D E5 11              1058 	mov	a,_check_radio_input_length_1_1
   029F 70 34              1059 	jnz	00112$
                           1060 ;	main.c:445: n = cc1100_read_status_reg_otf(RXBYTES) & 0x7f;
   02A1 75 82 FB           1061 	mov	dpl,#0xFB
   02A4 12 05 DB           1062 	lcall	_cc1100_read_status_reg_otf
   02A7 E5 82              1063 	mov	a,dpl
   02A9 54 7F              1064 	anl	a,#0x7F
                           1065 ;	main.c:446: if (n > 2) {
   02AB FA                 1066 	mov  r2,a
   02AC 24 FD              1067 	add	a,#0xff - 0x02
   02AE 40 01              1068 	jc	00122$
   02B0 22                 1069 	ret
   02B1                    1070 00122$:
                           1071 ;	main.c:447: cc1100_read(RX_fifo|BURST, &length, 1);		// Length byte = payload length + 1 address byte
   02B1 75 19 11           1072 	mov	_cc1100_read_PARM_2,#_check_radio_input_length_1_1
   02B4 75 1A 00           1073 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02B7 75 1B 40           1074 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02BA 75 1C 01           1075 	mov	_cc1100_read_PARM_3,#0x01
   02BD 75 82 7F           1076 	mov	dpl,#0x7F
   02C0 12 05 63           1077 	lcall	_cc1100_read
                           1078 ;	main.c:448: cc1100_read(RX_fifo|BURST, &address, 1); 	// Address byte (unused!)
   02C3 75 19 12           1079 	mov	_cc1100_read_PARM_2,#_check_radio_input_address_1_1
   02C6 75 1A 00           1080 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02C9 75 1B 40           1081 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02CC 75 1C 01           1082 	mov	_cc1100_read_PARM_3,#0x01
   02CF 75 82 7F           1083 	mov	dpl,#0x7F
   02D2 02 05 63           1084 	ljmp	_cc1100_read
   02D5                    1085 00112$:
                           1086 ;	main.c:455: if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) == MARCSTATE_IDLE) {
   02D5 75 82 F5           1087 	mov	dpl,#0xF5
   02D8 12 05 DB           1088 	lcall	_cc1100_read_status_reg_otf
   02DB E5 82              1089 	mov	a,dpl
   02DD 54 1F              1090 	anl	a,#0x1F
   02DF FB                 1091 	mov	r3,a
   02E0 BB 01 28           1092 	cjne	r3,#0x01,00109$
                           1093 ;	main.c:456: while (length > 1) {
   02E3                    1094 00103$:
   02E3 E5 11              1095 	mov	a,_check_radio_input_length_1_1
   02E5 24 FE              1096 	add	a,#0xff - 0x01
   02E7 50 1C              1097 	jnc	00105$
                           1098 ;	main.c:457: cc1100_read(RX_fifo|BURST, &x, 1);
   02E9 75 19 13           1099 	mov	_cc1100_read_PARM_2,#_check_radio_input_x_1_1
   02EC 75 1A 00           1100 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02EF 75 1B 40           1101 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02F2 75 1C 01           1102 	mov	_cc1100_read_PARM_3,#0x01
   02F5 75 82 7F           1103 	mov	dpl,#0x7F
   02F8 12 05 63           1104 	lcall	_cc1100_read
                           1105 ;	main.c:458: send_byte(x);	
   02FB 85 13 82           1106 	mov	dpl,_check_radio_input_x_1_1
   02FE 12 04 58           1107 	lcall	_send_byte
                           1108 ;	main.c:459: length--;
   0301 15 11              1109 	dec	_check_radio_input_length_1_1
   0303 80 DE              1110 	sjmp	00103$
   0305                    1111 00105$:
                           1112 ;	main.c:461: length = 0;
   0305 75 11 00           1113 	mov	_check_radio_input_length_1_1,#0x00
                           1114 ;	main.c:462: start_rx();	
   0308 02 01 44           1115 	ljmp	_start_rx
   030B                    1116 00109$:
                           1117 ;	main.c:465: n = cc1100_read_status_reg_otf(RXBYTES) & 0x7f;	
   030B 75 82 FB           1118 	mov	dpl,#0xFB
   030E 12 05 DB           1119 	lcall	_cc1100_read_status_reg_otf
   0311 E5 82              1120 	mov	a,dpl
   0313 54 7F              1121 	anl	a,#0x7F
                           1122 ;	main.c:468: if (n > 1){
   0315 FA                 1123 	mov  r2,a
   0316 24 FE              1124 	add	a,#0xff - 0x01
   0318 50 1A              1125 	jnc	00114$
                           1126 ;	main.c:469: cc1100_read(RX_fifo|BURST, &x, 1);
   031A 75 19 13           1127 	mov	_cc1100_read_PARM_2,#_check_radio_input_x_1_1
   031D 75 1A 00           1128 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0320 75 1B 40           1129 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   0323 75 1C 01           1130 	mov	_cc1100_read_PARM_3,#0x01
   0326 75 82 7F           1131 	mov	dpl,#0x7F
   0329 12 05 63           1132 	lcall	_cc1100_read
                           1133 ;	main.c:470: send_byte(x);	
   032C 85 13 82           1134 	mov	dpl,_check_radio_input_x_1_1
   032F 12 04 58           1135 	lcall	_send_byte
                           1136 ;	main.c:471: length--;	
   0332 15 11              1137 	dec	_check_radio_input_length_1_1
   0334                    1138 00114$:
   0334 22                 1139 	ret
                           1140 ;------------------------------------------------------------
                           1141 ;Allocation info for local variables in function 'main'
                           1142 ;------------------------------------------------------------
                           1143 ;------------------------------------------------------------
                           1144 ;	main.c:498: void main(void) {
                           1145 ;	-----------------------------------------
                           1146 ;	 function main
                           1147 ;	-----------------------------------------
   0335                    1148 _main:
                           1149 ;	main.c:508: P0M1 &= 0x6c;		// 0110 1100 -> P0M1
   0335 53 84 6C           1150 	anl	_P0M1,#0x6C
                           1151 ;	main.c:509: P0M2 |= 0x92;		// 1001 0010 -> P0M2
   0338 43 85 92           1152 	orl	_P0M2,#0x92
                           1153 ;	main.c:511: P0 = 0x83;			// 1000 0011 -> Port0
   033B 75 80 83           1154 	mov	_P0,#0x83
                           1155 ;	main.c:518: P1M1 &= 0x3e;		// 0011 1110
   033E 53 91 3E           1156 	anl	_P1M1,#0x3E
                           1157 ;	main.c:519: P1M2 |= 0xcd;		// 1100 1101
   0341 43 92 CD           1158 	orl	_P1M2,#0xCD
                           1159 ;	main.c:521: P1 = 0xcd;			// 1100 1101 -> Port1
   0344 75 90 CD           1160 	mov	_P1,#0xCD
                           1161 ;	main.c:525: P3M1 &= 0xfe;
   0347 53 B1 FE           1162 	anl	_P3M1,#0xFE
                           1163 ;	main.c:526: P3M2 |= 0x01;
   034A 43 B2 01           1164 	orl	_P3M2,#0x01
                           1165 ;	main.c:528: P3 = 0;
   034D 75 B0 00           1166 	mov	_P3,#0x00
                           1167 ;	main.c:549: AUXR1 |= (1<<6);
   0350 43 A2 40           1168 	orl	_AUXR1,#0x40
                           1169 ;	main.c:551: RTCCON |= 1<<RTCS1;
   0353 43 D1 40           1170 	orl	_RTCCON,#0x40
                           1171 ;	main.c:552: RTCCON |= 1<<RTCS0;
   0356 43 D1 20           1172 	orl	_RTCCON,#0x20
                           1173 ;	main.c:553: RTCH = 0xff;
   0359 75 D2 FF           1174 	mov	_RTCH,#0xFF
                           1175 ;	main.c:554: RTCL = 0xff;
   035C 75 D3 FF           1176 	mov	_RTCL,#0xFF
                           1177 ;	main.c:556: initSerial(384);		// Serial baudrate 38400
   035F 90 01 80           1178 	mov	dptr,#0x0180
   0362 12 03 95           1179 	lcall	_initSerial
                           1180 ;	main.c:557: SSTAT |= (1<<CIDIS);	// Combined interrupt disabled, RX and TX generate different interrupts
   0365 43 BA 20           1181 	orl	_SSTAT,#0x20
                           1182 ;	main.c:559: cc1100_init();
   0368 12 04 C2           1183 	lcall	_cc1100_init
                           1184 ;	main.c:561: ESR = 1;
   036B D2 AC              1185 	setb	_IEN0_4
                           1186 ;	main.c:562: EA = 1;
   036D D2 AF              1187 	setb	_IEN0_7
                           1188 ;	main.c:622: got_etx = 0;
   036F C2 00              1189 	clr	_got_etx
                           1190 ;	main.c:623: got_eot = 0;
   0371 C2 01              1191 	clr	_got_eot
                           1192 ;	main.c:625: buffer_init();
   0373 12 00 8D           1193 	lcall	_buffer_init
                           1194 ;	main.c:627: start_rx();								// Start receiving via radio
   0376 12 01 44           1195 	lcall	_start_rx
                           1196 ;	main.c:629: while (1) {						/* Forever: */
   0379                    1197 00107$:
                           1198 ;	main.c:631: check_etx();
   0379 12 02 1B           1199 	lcall	_check_etx
                           1200 ;	main.c:639: if (radio_mode == RADIO_RX)
   037C 74 01              1201 	mov	a,#0x01
   037E B5 08 05           1202 	cjne	a,_radio_mode,00102$
                           1203 ;	main.c:642: check_radio_input();
   0381 12 02 9D           1204 	lcall	_check_radio_input
   0384 80 03              1205 	sjmp	00103$
   0386                    1206 00102$:
                           1207 ;	main.c:646: re_enter_rx();
   0386 12 01 9B           1208 	lcall	_re_enter_rx
   0389                    1209 00103$:
                           1210 ;	main.c:651: if (radio_mode != RADIO_TX)
   0389 74 02              1211 	mov	a,#0x02
   038B B5 08 02           1212 	cjne	a,_radio_mode,00116$
   038E 80 E9              1213 	sjmp	00107$
   0390                    1214 00116$:
                           1215 ;	main.c:652: handle_tx();
   0390 12 01 AB           1216 	lcall	_handle_tx
   0393 80 E4              1217 	sjmp	00107$
                           1218 	.area CSEG    (CODE)
                           1219 	.area CONST   (CODE)
                           1220 	.area XINIT   (CODE)
                           1221 	.area CABS    (ABS,CODE)
