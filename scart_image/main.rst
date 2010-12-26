                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.7 #5820 (May  6 2010) (Linux)
                              4 ; This file was generated Sun Dec 26 12:18:23 2010
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
                             18 	.globl _start_tx
                             19 	.globl _start_rx
                             20 	.globl _rx_overflow_reset
                             21 	.globl _has_room
                             22 	.globl _buffer_out
                             23 	.globl _serial_isr
                             24 	.globl _buffer_init
                             25 	.globl _feed_wd
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
   0023 02 00 A2            514 	ljmp	_serial_isr
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
   0026 12 03 20            555 	lcall	_main
                            556 ;	return from main will lock up
   0029 80 FE               557 	sjmp .
                            558 ;--------------------------------------------------------
                            559 ; code
                            560 ;--------------------------------------------------------
                            561 	.area CSEG    (CODE)
                            562 ;------------------------------------------------------------
                            563 ;Allocation info for local variables in function 'feed_wd'
                            564 ;------------------------------------------------------------
                            565 ;------------------------------------------------------------
                            566 ;	main.c:156: feed_wd(){
                            567 ;	-----------------------------------------
                            568 ;	 function feed_wd
                            569 ;	-----------------------------------------
   008D                     570 _feed_wd:
                    0002    571 	ar2 = 0x02
                    0003    572 	ar3 = 0x03
                    0004    573 	ar4 = 0x04
                    0005    574 	ar5 = 0x05
                    0006    575 	ar6 = 0x06
                    0007    576 	ar7 = 0x07
                    0000    577 	ar0 = 0x00
                    0001    578 	ar1 = 0x01
                            579 ;	main.c:157: EA = 0;
   008D C2 AF               580 	clr	_IEN0_7
                            581 ;	main.c:158: WFEED1 = 0xA5;
   008F 75 C2 A5            582 	mov	_WFEED1,#0xA5
                            583 ;	main.c:159: WFEED2 = 0x5A;
   0092 75 C3 5A            584 	mov	_WFEED2,#0x5A
                            585 ;	main.c:160: EA = 1;
   0095 D2 AF               586 	setb	_IEN0_7
   0097 22                  587 	ret
                            588 ;------------------------------------------------------------
                            589 ;Allocation info for local variables in function 'buffer_init'
                            590 ;------------------------------------------------------------
                            591 ;------------------------------------------------------------
                            592 ;	main.c:163: void buffer_init(){
                            593 ;	-----------------------------------------
                            594 ;	 function buffer_init
                            595 ;	-----------------------------------------
   0098                     596 _buffer_init:
                            597 ;	main.c:164: bufcnt = 0;			// Number of bytes in the buffer
   0098 75 0A 00            598 	mov	_bufcnt,#0x00
                            599 ;	main.c:165: bufnxt = 0;			// Index of next free place in buffer
   009B 75 0B 00            600 	mov	_bufnxt,#0x00
                            601 ;	main.c:166: bufstart = 0;		// Index of first data byte in buffer
   009E 75 09 00            602 	mov	_bufstart,#0x00
   00A1 22                  603 	ret
                            604 ;------------------------------------------------------------
                            605 ;Allocation info for local variables in function 'serial_isr'
                            606 ;------------------------------------------------------------
                            607 ;x                         Allocated to registers r2 
                            608 ;------------------------------------------------------------
                            609 ;	main.c:182: void serial_isr (void) __interrupt (4) {
                            610 ;	-----------------------------------------
                            611 ;	 function serial_isr
                            612 ;	-----------------------------------------
   00A2                     613 _serial_isr:
   00A2 C0 E0               614 	push	acc
   00A4 C0 02               615 	push	ar2
   00A6 C0 03               616 	push	ar3
   00A8 C0 00               617 	push	ar0
   00AA C0 D0               618 	push	psw
   00AC 75 D0 00            619 	mov	psw,#0x00
                            620 ;	main.c:185: x=SBUF;
   00AF AA 99               621 	mov	r2,_SBUF
                            622 ;	main.c:186: RI = 0;
   00B1 C2 98               623 	clr	_SCON_0
                            624 ;	main.c:191: if (x == ETX){
   00B3 BA 03 04            625 	cjne	r2,#0x03,00102$
                            626 ;	main.c:192: got_etx = 1;
   00B6 D2 00               627 	setb	_got_etx
                            628 ;	main.c:193: return;
   00B8 80 32               629 	sjmp	00114$
   00BA                     630 00102$:
                            631 ;	main.c:197: if (got_eot){
   00BA 30 01 02            632 	jnb	_got_eot,00104$
                            633 ;	main.c:198: return;
   00BD 80 2D               634 	sjmp	00114$
   00BF                     635 00104$:
                            636 ;	main.c:202: if (bufcnt >= BUFSIZE){
   00BF 74 B2               637 	mov	a,#0x100 - 0x4E
   00C1 25 0A               638 	add	a,_bufcnt
   00C3 50 0D               639 	jnc	00109$
                            640 ;	main.c:203: bufcnt--;
   00C5 15 0A               641 	dec	_bufcnt
                            642 ;	main.c:205: if (bufnxt == 0)
   00C7 E5 0B               643 	mov	a,_bufnxt
   00C9 70 05               644 	jnz	00106$
                            645 ;	main.c:206: bufnxt = BUFSIZE-1;
   00CB 75 0B 4D            646 	mov	_bufnxt,#0x4D
   00CE 80 02               647 	sjmp	00109$
   00D0                     648 00106$:
                            649 ;	main.c:208: bufnxt--;
   00D0 15 0B               650 	dec	_bufnxt
   00D2                     651 00109$:
                            652 ;	main.c:212: buf[bufnxt++] = x;
   00D2 AB 0B               653 	mov	r3,_bufnxt
   00D4 05 0B               654 	inc	_bufnxt
   00D6 EB                  655 	mov	a,r3
   00D7 24 4D               656 	add	a,#_buf
   00D9 F8                  657 	mov	r0,a
   00DA A6 02               658 	mov	@r0,ar2
                            659 ;	main.c:213: if (bufnxt >= BUFSIZE)
   00DC 74 B2               660 	mov	a,#0x100 - 0x4E
   00DE 25 0B               661 	add	a,_bufnxt
   00E0 50 03               662 	jnc	00111$
                            663 ;	main.c:214: bufnxt = 0;
   00E2 75 0B 00            664 	mov	_bufnxt,#0x00
   00E5                     665 00111$:
                            666 ;	main.c:215: bufcnt++;
   00E5 05 0A               667 	inc	_bufcnt
                            668 ;	main.c:217: if (x == EOT)
   00E7 BA 04 02            669 	cjne	r2,#0x04,00113$
                            670 ;	main.c:218: got_eot = 1;
   00EA D2 01               671 	setb	_got_eot
   00EC                     672 00113$:
                            673 ;	main.c:220: return;
   00EC                     674 00114$:
   00EC D0 D0               675 	pop	psw
   00EE D0 00               676 	pop	ar0
   00F0 D0 03               677 	pop	ar3
   00F2 D0 02               678 	pop	ar2
   00F4 D0 E0               679 	pop	acc
   00F6 32                  680 	reti
                            681 ;	eliminated unneeded push/pop ar1
                            682 ;	eliminated unneeded push/pop dpl
                            683 ;	eliminated unneeded push/pop dph
                            684 ;	eliminated unneeded push/pop b
                            685 ;------------------------------------------------------------
                            686 ;Allocation info for local variables in function 'buffer_out'
                            687 ;------------------------------------------------------------
                            688 ;x                         Allocated to registers r2 
                            689 ;------------------------------------------------------------
                            690 ;	main.c:227: char buffer_out(){
                            691 ;	-----------------------------------------
                            692 ;	 function buffer_out
                            693 ;	-----------------------------------------
   00F7                     694 _buffer_out:
                            695 ;	main.c:230: if (bufcnt == 0) return (0);
   00F7 E5 0A               696 	mov	a,_bufcnt
   00F9 70 03               697 	jnz	00102$
   00FB F5 82               698 	mov	dpl,a
   00FD 22                  699 	ret
   00FE                     700 00102$:
                            701 ;	main.c:231: x = buf[bufstart++];
   00FE AA 09               702 	mov	r2,_bufstart
   0100 05 09               703 	inc	_bufstart
   0102 EA                  704 	mov	a,r2
   0103 24 4D               705 	add	a,#_buf
   0105 F8                  706 	mov	r0,a
   0106 86 02               707 	mov	ar2,@r0
                            708 ;	main.c:232: if (bufstart >= BUFSIZE)
   0108 74 B2               709 	mov	a,#0x100 - 0x4E
   010A 25 09               710 	add	a,_bufstart
   010C 50 03               711 	jnc	00104$
                            712 ;	main.c:233: bufstart = 0;
   010E 75 09 00            713 	mov	_bufstart,#0x00
   0111                     714 00104$:
                            715 ;	main.c:236: bufcnt--;
   0111 15 0A               716 	dec	_bufcnt
                            717 ;	main.c:238: return x;
   0113 8A 82               718 	mov	dpl,r2
   0115 22                  719 	ret
                            720 ;------------------------------------------------------------
                            721 ;Allocation info for local variables in function 'has_room'
                            722 ;------------------------------------------------------------
                            723 ;------------------------------------------------------------
                            724 ;	main.c:251: unsigned char has_room(){
                            725 ;	-----------------------------------------
                            726 ;	 function has_room
                            727 ;	-----------------------------------------
   0116                     728 _has_room:
                            729 ;	main.c:252: return ( (BUFSIZE - bufcnt) > (MPDTOOL_PKTSIZE + 2) );
   0116 AA 0A               730 	mov	r2,_bufcnt
   0118 7B 00               731 	mov	r3,#0x00
   011A 74 4E               732 	mov	a,#0x4E
   011C C3                  733 	clr	c
   011D 9A                  734 	subb	a,r2
   011E FA                  735 	mov	r2,a
   011F E4                  736 	clr	a
   0120 9B                  737 	subb	a,r3
   0121 FB                  738 	mov	r3,a
   0122 C3                  739 	clr	c
   0123 74 12               740 	mov	a,#0x12
   0125 9A                  741 	subb	a,r2
   0126 74 80               742 	mov	a,#(0x00 ^ 0x80)
   0128 8B F0               743 	mov	b,r3
   012A 63 F0 80            744 	xrl	b,#0x80
   012D 95 F0               745 	subb	a,b
   012F E4                  746 	clr	a
   0130 33                  747 	rlc	a
   0131 F5 82               748 	mov	dpl,a
   0133 22                  749 	ret
                            750 ;------------------------------------------------------------
                            751 ;Allocation info for local variables in function 'rx_overflow_reset'
                            752 ;------------------------------------------------------------
                            753 ;------------------------------------------------------------
                            754 ;	main.c:262: rx_overflow_reset(){
                            755 ;	-----------------------------------------
                            756 ;	 function rx_overflow_reset
                            757 ;	-----------------------------------------
   0134                     758 _rx_overflow_reset:
                            759 ;	main.c:263: if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) == RX_OVERFLOW){
   0134 75 82 F5            760 	mov	dpl,#0xF5
   0137 12 05 3F            761 	lcall	_cc1100_read_status_reg_otf
   013A E5 82               762 	mov	a,dpl
   013C 54 1F               763 	anl	a,#0x1F
   013E FA                  764 	mov	r2,a
   013F BA 11 0C            765 	cjne	r2,#0x11,00103$
                            766 ;	main.c:264: cc1100_strobe(SFRX);
   0142 75 82 3A            767 	mov	dpl,#0x3A
   0145 12 05 30            768 	lcall	_cc1100_strobe
                            769 ;	main.c:265: cc1100_strobe(SRX);
   0148 75 82 34            770 	mov	dpl,#0x34
   014B 02 05 30            771 	ljmp	_cc1100_strobe
   014E                     772 00103$:
   014E 22                  773 	ret
                            774 ;------------------------------------------------------------
                            775 ;Allocation info for local variables in function 'start_rx'
                            776 ;------------------------------------------------------------
                            777 ;------------------------------------------------------------
                            778 ;	main.c:271: void start_rx() {
                            779 ;	-----------------------------------------
                            780 ;	 function start_rx
                            781 ;	-----------------------------------------
   014F                     782 _start_rx:
                            783 ;	main.c:273: rx_overflow_reset();
   014F 12 01 34            784 	lcall	_rx_overflow_reset
                            785 ;	main.c:274: switch_to_idle();
   0152 12 05 6F            786 	lcall	_switch_to_idle
                            787 ;	main.c:276: cc1100_strobe(SFRX);
   0155 75 82 3A            788 	mov	dpl,#0x3A
   0158 12 05 30            789 	lcall	_cc1100_strobe
                            790 ;	main.c:277: cc1100_strobe(SCAL);
   015B 75 82 33            791 	mov	dpl,#0x33
   015E 12 05 30            792 	lcall	_cc1100_strobe
                            793 ;	main.c:278: cc1100_strobe(SRX);
   0161 75 82 34            794 	mov	dpl,#0x34
   0164 12 05 30            795 	lcall	_cc1100_strobe
                            796 ;	main.c:279: radio_mode = RADIO_RX;
   0167 75 08 01            797 	mov	_radio_mode,#0x01
   016A 22                  798 	ret
                            799 ;------------------------------------------------------------
                            800 ;Allocation info for local variables in function 'start_tx'
                            801 ;------------------------------------------------------------
                            802 ;------------------------------------------------------------
                            803 ;	main.c:284: void start_tx() {	
                            804 ;	-----------------------------------------
                            805 ;	 function start_tx
                            806 ;	-----------------------------------------
   016B                     807 _start_tx:
                            808 ;	main.c:285: rx_overflow_reset();
   016B 12 01 34            809 	lcall	_rx_overflow_reset
                            810 ;	main.c:286: switch_to_idle();
   016E 12 05 6F            811 	lcall	_switch_to_idle
                            812 ;	main.c:288: cc1100_strobe(SCAL);
   0171 75 82 33            813 	mov	dpl,#0x33
   0174 12 05 30            814 	lcall	_cc1100_strobe
                            815 ;	main.c:289: cc1100_strobe(STX);
   0177 75 82 35            816 	mov	dpl,#0x35
   017A 12 05 30            817 	lcall	_cc1100_strobe
                            818 ;	main.c:290: radio_mode = RADIO_TX;
   017D 75 08 02            819 	mov	_radio_mode,#0x02
   0180 22                  820 	ret
                            821 ;------------------------------------------------------------
                            822 ;Allocation info for local variables in function 're_enter_rx'
                            823 ;------------------------------------------------------------
                            824 ;------------------------------------------------------------
                            825 ;	main.c:296: void re_enter_rx(){
                            826 ;	-----------------------------------------
                            827 ;	 function re_enter_rx
                            828 ;	-----------------------------------------
   0181                     829 _re_enter_rx:
                            830 ;	main.c:297: if ((radio_mode == RADIO_TX) && tx_finished())
   0181 74 02               831 	mov	a,#0x02
   0183 B5 08 0F            832 	cjne	a,_radio_mode,00104$
   0186 12 05 84            833 	lcall	_cc1100_tx_finished
   0189 E5 82               834 	mov	a,dpl
   018B 85 83 F0            835 	mov	b,dph
   018E 45 F0               836 	orl	a,b
   0190 60 03               837 	jz	00104$
                            838 ;	main.c:298: start_rx();
   0192 02 01 4F            839 	ljmp	_start_rx
   0195                     840 00104$:
   0195 22                  841 	ret
                            842 ;------------------------------------------------------------
                            843 ;Allocation info for local variables in function 'handle_tx'
                            844 ;------------------------------------------------------------
                            845 ;tx_state                  Allocated with name '_handle_tx_tx_state_1_1'
                            846 ;tx_cnt                    Allocated with name '_handle_tx_tx_cnt_1_1'
                            847 ;x                         Allocated to registers 
                            848 ;------------------------------------------------------------
                            849 ;	main.c:322: void handle_tx(){
                            850 ;	-----------------------------------------
                            851 ;	 function handle_tx
                            852 ;	-----------------------------------------
   0196                     853 _handle_tx:
                            854 ;	main.c:329: switch (tx_state){
   0196 E4                  855 	clr	a
   0197 B5 0C 02            856 	cjne	a,_handle_tx_tx_state_1_1,00121$
   019A 80 0E               857 	sjmp	00101$
   019C                     858 00121$:
   019C 74 01               859 	mov	a,#0x01
   019E B5 0C 02            860 	cjne	a,_handle_tx_tx_state_1_1,00122$
   01A1 80 45               861 	sjmp	00107$
   01A3                     862 00122$:
   01A3 74 02               863 	mov	a,#0x02
                            864 ;	main.c:331: case TX_IDLE:
   01A5 B5 0C 5D            865 	cjne	a,_handle_tx_tx_state_1_1,00113$
   01A8 80 55               866 	sjmp	00111$
   01AA                     867 00101$:
                            868 ;	main.c:333: if (got_eot) {
   01AA 30 01 1C            869 	jnb	_got_eot,00105$
                            870 ;	main.c:334: tx_cnt = bufcnt;	/* = number of payload bytes to be transferred to TXFIFO (not counting address and length byte) */
                            871 ;	main.c:337: cc1100_write1(TX_fifo, tx_cnt + 1);
   01AD E5 0A               872 	mov	a,_bufcnt
   01AF F5 0D               873 	mov	_handle_tx_tx_cnt_1_1,a
   01B1 04                  874 	inc	a
   01B2 F5 18               875 	mov	_cc1100_write1_PARM_2,a
   01B4 75 82 3F            876 	mov	dpl,#0x3F
   01B7 12 04 AA            877 	lcall	_cc1100_write1
                            878 ;	main.c:340: cc1100_write1(TX_fifo, DEV_ADDR);	
   01BA 75 18 01            879 	mov	_cc1100_write1_PARM_2,#0x01
   01BD 75 82 3F            880 	mov	dpl,#0x3F
   01C0 12 04 AA            881 	lcall	_cc1100_write1
                            882 ;	main.c:342: tx_state = TX_COPY;
   01C3 75 0C 01            883 	mov	_handle_tx_tx_state_1_1,#0x01
                            884 ;	main.c:343: got_eot=0;
   01C6 C2 01               885 	clr	_got_eot
                            886 ;	main.c:345: break;
   01C8 22                  887 	ret
   01C9                     888 00105$:
                            889 ;	main.c:347: } else if ( bufcnt >= MAX_TX_PAYLOAD ) {
   01C9 74 C4               890 	mov	a,#0x100 - 0x3C
   01CB 25 0A               891 	add	a,_bufcnt
   01CD 50 36               892 	jnc	00113$
                            893 ;	main.c:350: tx_cnt = MAX_TX_PAYLOAD;	
   01CF 75 0D 3C            894 	mov	_handle_tx_tx_cnt_1_1,#0x3C
                            895 ;	main.c:353: cc1100_write1(TX_fifo, MAX_TX_PAYLOAD + 1);
   01D2 75 18 3D            896 	mov	_cc1100_write1_PARM_2,#0x3D
   01D5 75 82 3F            897 	mov	dpl,#0x3F
   01D8 12 04 AA            898 	lcall	_cc1100_write1
                            899 ;	main.c:356: cc1100_write1(TX_fifo, DEV_ADDR);	
   01DB 75 18 01            900 	mov	_cc1100_write1_PARM_2,#0x01
   01DE 75 82 3F            901 	mov	dpl,#0x3F
   01E1 12 04 AA            902 	lcall	_cc1100_write1
                            903 ;	main.c:358: tx_state = TX_COPY;
   01E4 75 0C 01            904 	mov	_handle_tx_tx_state_1_1,#0x01
                            905 ;	main.c:360: break;
                            906 ;	main.c:362: case TX_COPY:
   01E7 22                  907 	ret
   01E8                     908 00107$:
                            909 ;	main.c:364: if (tx_cnt > 0){
   01E8 E5 0D               910 	mov	a,_handle_tx_tx_cnt_1_1
   01EA 60 0F               911 	jz	00109$
                            912 ;	main.c:365: x = buffer_out();
   01EC 12 00 F7            913 	lcall	_buffer_out
   01EF 85 82 18            914 	mov	_cc1100_write1_PARM_2,dpl
                            915 ;	main.c:366: cc1100_write1(TX_fifo, x);	
   01F2 75 82 3F            916 	mov	dpl,#0x3F
   01F5 12 04 AA            917 	lcall	_cc1100_write1
                            918 ;	main.c:367: tx_cnt--;
   01F8 15 0D               919 	dec	_handle_tx_tx_cnt_1_1
   01FA 22                  920 	ret
   01FB                     921 00109$:
                            922 ;	main.c:370: tx_state = TX_SEND;
   01FB 75 0C 02            923 	mov	_handle_tx_tx_state_1_1,#0x02
                            924 ;	main.c:372: break;
                            925 ;	main.c:374: case TX_SEND:
   01FE 22                  926 	ret
   01FF                     927 00111$:
                            928 ;	main.c:375: start_tx();
   01FF 12 01 6B            929 	lcall	_start_tx
                            930 ;	main.c:376: tx_state = TX_IDLE;
   0202 75 0C 00            931 	mov	_handle_tx_tx_state_1_1,#0x00
                            932 ;	main.c:378: }
   0205                     933 00113$:
   0205 22                  934 	ret
                            935 ;------------------------------------------------------------
                            936 ;Allocation info for local variables in function 'check_etx'
                            937 ;------------------------------------------------------------
                            938 ;------------------------------------------------------------
                            939 ;	main.c:382: void check_etx(){
                            940 ;	-----------------------------------------
                            941 ;	 function check_etx
                            942 ;	-----------------------------------------
   0206                     943 _check_etx:
                            944 ;	main.c:383: if (got_etx) {	/* Is the sender waiting for an ACK ? */		
   0206 30 00 0F            945 	jnb	_got_etx,00105$
                            946 ;	main.c:384: if (has_room()){	/* Still enough space in buffer ? */
   0209 12 01 16            947 	lcall	_has_room
   020C E5 82               948 	mov	a,dpl
   020E 60 08               949 	jz	00105$
                            950 ;	main.c:385: got_etx = 0;		// Atomic Operation ! (see sdcc manual)
   0210 C2 00               951 	clr	_got_etx
                            952 ;	main.c:386: send_byte(ACK);
   0212 75 82 06            953 	mov	dpl,#0x06
   0215 02 03 F1            954 	ljmp	_send_byte
   0218                     955 00105$:
   0218 22                  956 	ret
                            957 ;------------------------------------------------------------
                            958 ;Allocation info for local variables in function 'check_radio_packet'
                            959 ;------------------------------------------------------------
                            960 ;x                         Allocated with name '_check_radio_packet_x_1_1'
                            961 ;i                         Allocated to registers r2 
                            962 ;length                    Allocated with name '_check_radio_packet_length_1_1'
                            963 ;address                   Allocated with name '_check_radio_packet_address_1_1'
                            964 ;------------------------------------------------------------
                            965 ;	main.c:397: void check_radio_packet (){
                            966 ;	-----------------------------------------
                            967 ;	 function check_radio_packet
                            968 ;	-----------------------------------------
   0219                     969 _check_radio_packet:
                            970 ;	main.c:401: if ( GDO0 ){
   0219 30 86 6B            971 	jnb	_P0_6,00109$
                            972 ;	main.c:404: cc1100_read(RX_fifo|BURST, &length, 1);		// Length byte = payload length + 1
   021C 75 19 0F            973 	mov	_cc1100_read_PARM_2,#_check_radio_packet_length_1_1
   021F 75 1A 00            974 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0222 75 1B 40            975 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   0225 75 1C 01            976 	mov	_cc1100_read_PARM_3,#0x01
   0228 75 82 7F            977 	mov	dpl,#0x7F
   022B 12 04 C7            978 	lcall	_cc1100_read
                            979 ;	main.c:406: cc1100_read(RX_fifo|BURST, &address, 1); 	// Address byte (unused!)
   022E 75 19 10            980 	mov	_cc1100_read_PARM_2,#_check_radio_packet_address_1_1
   0231 75 1A 00            981 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0234 75 1B 40            982 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   0237 75 1C 01            983 	mov	_cc1100_read_PARM_3,#0x01
   023A 75 82 7F            984 	mov	dpl,#0x7F
   023D 12 04 C7            985 	lcall	_cc1100_read
                            986 ;	main.c:409: for (i=0; i < (length - 1); i++){
   0240 7A 00               987 	mov	r2,#0x00
   0242                     988 00103$:
   0242 AB 0F               989 	mov	r3,_check_radio_packet_length_1_1
   0244 7C 00               990 	mov	r4,#0x00
   0246 1B                  991 	dec	r3
   0247 BB FF 01            992 	cjne	r3,#0xff,00117$
   024A 1C                  993 	dec	r4
   024B                     994 00117$:
   024B 8A 05               995 	mov	ar5,r2
   024D 7E 00               996 	mov	r6,#0x00
   024F C3                  997 	clr	c
   0250 ED                  998 	mov	a,r5
   0251 9B                  999 	subb	a,r3
   0252 EE                 1000 	mov	a,r6
   0253 64 80              1001 	xrl	a,#0x80
   0255 8C F0              1002 	mov	b,r4
   0257 63 F0 80           1003 	xrl	b,#0x80
   025A 95 F0              1004 	subb	a,b
   025C 50 26              1005 	jnc	00106$
                           1006 ;	main.c:410: cc1100_read(RX_fifo|BURST, &x, 1);
   025E 75 19 0E           1007 	mov	_cc1100_read_PARM_2,#_check_radio_packet_x_1_1
   0261 75 1A 00           1008 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   0264 75 1B 40           1009 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   0267 75 1C 01           1010 	mov	_cc1100_read_PARM_3,#0x01
   026A 75 82 7F           1011 	mov	dpl,#0x7F
   026D C0 02              1012 	push	ar2
   026F 12 04 C7           1013 	lcall	_cc1100_read
                           1014 ;	main.c:411: send_byte(x);
   0272 85 0E 82           1015 	mov	dpl,_check_radio_packet_x_1_1
   0275 12 03 F1           1016 	lcall	_send_byte
   0278 D0 02              1017 	pop	ar2
                           1018 ;	main.c:412: if (x == EOT)
   027A 74 04              1019 	mov	a,#0x04
   027C B5 0E 02           1020 	cjne	a,_check_radio_packet_x_1_1,00119$
   027F 80 03              1021 	sjmp	00106$
   0281                    1022 00119$:
                           1023 ;	main.c:409: for (i=0; i < (length - 1); i++){
   0281 0A                 1024 	inc	r2
   0282 80 BE              1025 	sjmp	00103$
   0284                    1026 00106$:
                           1027 ;	main.c:415: start_rx();	
   0284 02 01 4F           1028 	ljmp	_start_rx
   0287                    1029 00109$:
   0287 22                 1030 	ret
                           1031 ;------------------------------------------------------------
                           1032 ;Allocation info for local variables in function 'check_radio_input'
                           1033 ;------------------------------------------------------------
                           1034 ;length                    Allocated with name '_check_radio_input_length_1_1'
                           1035 ;address                   Allocated with name '_check_radio_input_address_1_1'
                           1036 ;n                         Allocated to registers r2 
                           1037 ;x                         Allocated with name '_check_radio_input_x_1_1'
                           1038 ;------------------------------------------------------------
                           1039 ;	main.c:437: void check_radio_input (){
                           1040 ;	-----------------------------------------
                           1041 ;	 function check_radio_input
                           1042 ;	-----------------------------------------
   0288                    1043 _check_radio_input:
                           1044 ;	main.c:444: if (length == 0){
   0288 E5 11              1045 	mov	a,_check_radio_input_length_1_1
   028A 70 34              1046 	jnz	00112$
                           1047 ;	main.c:445: n = cc1100_read_status_reg_otf(RXBYTES) & 0x7f;
   028C 75 82 FB           1048 	mov	dpl,#0xFB
   028F 12 05 3F           1049 	lcall	_cc1100_read_status_reg_otf
   0292 E5 82              1050 	mov	a,dpl
   0294 54 7F              1051 	anl	a,#0x7F
                           1052 ;	main.c:446: if (n > 2) {
   0296 FA                 1053 	mov  r2,a
   0297 24 FD              1054 	add	a,#0xff - 0x02
   0299 40 01              1055 	jc	00122$
   029B 22                 1056 	ret
   029C                    1057 00122$:
                           1058 ;	main.c:447: cc1100_read(RX_fifo|BURST, &length, 1);		// Length byte = payload length + 1 address byte
   029C 75 19 11           1059 	mov	_cc1100_read_PARM_2,#_check_radio_input_length_1_1
   029F 75 1A 00           1060 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02A2 75 1B 40           1061 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02A5 75 1C 01           1062 	mov	_cc1100_read_PARM_3,#0x01
   02A8 75 82 7F           1063 	mov	dpl,#0x7F
   02AB 12 04 C7           1064 	lcall	_cc1100_read
                           1065 ;	main.c:448: cc1100_read(RX_fifo|BURST, &address, 1); 	// Address byte (unused!)
   02AE 75 19 12           1066 	mov	_cc1100_read_PARM_2,#_check_radio_input_address_1_1
   02B1 75 1A 00           1067 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02B4 75 1B 40           1068 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02B7 75 1C 01           1069 	mov	_cc1100_read_PARM_3,#0x01
   02BA 75 82 7F           1070 	mov	dpl,#0x7F
   02BD 02 04 C7           1071 	ljmp	_cc1100_read
   02C0                    1072 00112$:
                           1073 ;	main.c:455: if ( (cc1100_read_status_reg_otf(MARCSTATE) & 0x1f) == MARCSTATE_IDLE) {
   02C0 75 82 F5           1074 	mov	dpl,#0xF5
   02C3 12 05 3F           1075 	lcall	_cc1100_read_status_reg_otf
   02C6 E5 82              1076 	mov	a,dpl
   02C8 54 1F              1077 	anl	a,#0x1F
   02CA FB                 1078 	mov	r3,a
   02CB BB 01 28           1079 	cjne	r3,#0x01,00109$
                           1080 ;	main.c:456: while (length > 1) {
   02CE                    1081 00103$:
   02CE E5 11              1082 	mov	a,_check_radio_input_length_1_1
   02D0 24 FE              1083 	add	a,#0xff - 0x01
   02D2 50 1C              1084 	jnc	00105$
                           1085 ;	main.c:457: cc1100_read(RX_fifo|BURST, &x, 1);
   02D4 75 19 13           1086 	mov	_cc1100_read_PARM_2,#_check_radio_input_x_1_1
   02D7 75 1A 00           1087 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   02DA 75 1B 40           1088 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   02DD 75 1C 01           1089 	mov	_cc1100_read_PARM_3,#0x01
   02E0 75 82 7F           1090 	mov	dpl,#0x7F
   02E3 12 04 C7           1091 	lcall	_cc1100_read
                           1092 ;	main.c:458: send_byte(x);	
   02E6 85 13 82           1093 	mov	dpl,_check_radio_input_x_1_1
   02E9 12 03 F1           1094 	lcall	_send_byte
                           1095 ;	main.c:459: length--;
   02EC 15 11              1096 	dec	_check_radio_input_length_1_1
   02EE 80 DE              1097 	sjmp	00103$
   02F0                    1098 00105$:
                           1099 ;	main.c:461: length = 0;
   02F0 75 11 00           1100 	mov	_check_radio_input_length_1_1,#0x00
                           1101 ;	main.c:462: start_rx();	
   02F3 02 01 4F           1102 	ljmp	_start_rx
   02F6                    1103 00109$:
                           1104 ;	main.c:465: n = cc1100_read_status_reg_otf(RXBYTES) & 0x7f;	
   02F6 75 82 FB           1105 	mov	dpl,#0xFB
   02F9 12 05 3F           1106 	lcall	_cc1100_read_status_reg_otf
   02FC E5 82              1107 	mov	a,dpl
   02FE 54 7F              1108 	anl	a,#0x7F
                           1109 ;	main.c:468: if (n > 1){
   0300 FA                 1110 	mov  r2,a
   0301 24 FE              1111 	add	a,#0xff - 0x01
   0303 50 1A              1112 	jnc	00114$
                           1113 ;	main.c:469: cc1100_read(RX_fifo|BURST, &x, 1);
   0305 75 19 13           1114 	mov	_cc1100_read_PARM_2,#_check_radio_input_x_1_1
   0308 75 1A 00           1115 	mov	(_cc1100_read_PARM_2 + 1),#0x00
   030B 75 1B 40           1116 	mov	(_cc1100_read_PARM_2 + 2),#0x40
   030E 75 1C 01           1117 	mov	_cc1100_read_PARM_3,#0x01
   0311 75 82 7F           1118 	mov	dpl,#0x7F
   0314 12 04 C7           1119 	lcall	_cc1100_read
                           1120 ;	main.c:470: send_byte(x);	
   0317 85 13 82           1121 	mov	dpl,_check_radio_input_x_1_1
   031A 12 03 F1           1122 	lcall	_send_byte
                           1123 ;	main.c:471: length--;	
   031D 15 11              1124 	dec	_check_radio_input_length_1_1
   031F                    1125 00114$:
   031F 22                 1126 	ret
                           1127 ;------------------------------------------------------------
                           1128 ;Allocation info for local variables in function 'main'
                           1129 ;------------------------------------------------------------
                           1130 ;------------------------------------------------------------
                           1131 ;	main.c:498: void main(void) {
                           1132 ;	-----------------------------------------
                           1133 ;	 function main
                           1134 ;	-----------------------------------------
   0320                    1135 _main:
                           1136 ;	main.c:508: P0M1 &= 0x6c;		// 0110 1100 -> P0M1
   0320 53 84 6C           1137 	anl	_P0M1,#0x6C
                           1138 ;	main.c:509: P0M2 |= 0x92;		// 1001 0010 -> P0M2
   0323 43 85 92           1139 	orl	_P0M2,#0x92
                           1140 ;	main.c:511: P0 = 0x83;			// 1000 0011 -> Port0
   0326 75 80 83           1141 	mov	_P0,#0x83
                           1142 ;	main.c:518: P1M1 &= 0x3e;		// 0011 1110
   0329 53 91 3E           1143 	anl	_P1M1,#0x3E
                           1144 ;	main.c:519: P1M2 |= 0xcd;		// 1100 1101
   032C 43 92 CD           1145 	orl	_P1M2,#0xCD
                           1146 ;	main.c:521: P1 = 0xcd;			// 1100 1101 -> Port1
   032F 75 90 CD           1147 	mov	_P1,#0xCD
                           1148 ;	main.c:525: P3M1 &= 0xfe;
   0332 53 B1 FE           1149 	anl	_P3M1,#0xFE
                           1150 ;	main.c:526: P3M2 |= 0x01;
   0335 43 B2 01           1151 	orl	_P3M2,#0x01
                           1152 ;	main.c:528: P3 = 0;
   0338 75 B0 00           1153 	mov	_P3,#0x00
                           1154 ;	main.c:549: AUXR1 |= (1<<6);
   033B 43 A2 40           1155 	orl	_AUXR1,#0x40
                           1156 ;	main.c:551: RTCCON |= 1<<RTCS1;
   033E 43 D1 40           1157 	orl	_RTCCON,#0x40
                           1158 ;	main.c:552: RTCCON |= 1<<RTCS0;
   0341 43 D1 20           1159 	orl	_RTCCON,#0x20
                           1160 ;	main.c:553: RTCH = 0xff;
   0344 75 D2 FF           1161 	mov	_RTCH,#0xFF
                           1162 ;	main.c:554: RTCL = 0xff;
   0347 75 D3 FF           1163 	mov	_RTCL,#0xFF
                           1164 ;	main.c:556: initSerial(384);		// Serial baudrate 38400
   034A 90 01 80           1165 	mov	dptr,#0x0180
   034D 12 03 93           1166 	lcall	_initSerial
                           1167 ;	main.c:557: SSTAT |= (1<<CIDIS);	// Combined interrupt disabled, RX and TX generate different interrupts
   0350 43 BA 20           1168 	orl	_SSTAT,#0x20
                           1169 ;	main.c:559: cc1100_init();
   0353 12 04 26           1170 	lcall	_cc1100_init
                           1171 ;	main.c:561: ESR = 1;
   0356 D2 AC              1172 	setb	_IEN0_4
                           1173 ;	main.c:562: EA = 1;
   0358 D2 AF              1174 	setb	_IEN0_7
                           1175 ;	main.c:622: got_etx = 0;
   035A C2 00              1176 	clr	_got_etx
                           1177 ;	main.c:623: got_eot = 0;
   035C C2 01              1178 	clr	_got_eot
                           1179 ;	main.c:625: buffer_init();
   035E 12 00 98           1180 	lcall	_buffer_init
                           1181 ;	main.c:627: start_rx();								// Start receiving via radio
   0361 12 01 4F           1182 	lcall	_start_rx
                           1183 ;	main.c:630: WDL = 0xFF;			// WDT counter
   0364 75 C1 FF           1184 	mov	_WDL,#0xFF
                           1185 ;	main.c:631: EA = 0;
   0367 C2 AF              1186 	clr	_IEN0_7
                           1187 ;	main.c:632: WDCON = 0xE5;		// Start WDT
   0369 75 A7 E5           1188 	mov	_WDCON,#0xE5
                           1189 ;	main.c:633: WFEED1 = 0xA5;
   036C 75 C2 A5           1190 	mov	_WFEED1,#0xA5
                           1191 ;	main.c:634: WFEED2 = 0x5A;
   036F 75 C3 5A           1192 	mov	_WFEED2,#0x5A
                           1193 ;	main.c:635: EA = 1;
   0372 D2 AF              1194 	setb	_IEN0_7
                           1195 ;	main.c:637: while (1) {						/* Forever: */
   0374                    1196 00107$:
                           1197 ;	main.c:639: feed_wd();
   0374 12 00 8D           1198 	lcall	_feed_wd
                           1199 ;	main.c:642: check_etx();
   0377 12 02 06           1200 	lcall	_check_etx
                           1201 ;	main.c:650: if (radio_mode == RADIO_RX)
   037A 74 01              1202 	mov	a,#0x01
   037C B5 08 05           1203 	cjne	a,_radio_mode,00102$
                           1204 ;	main.c:652: check_radio_input();
   037F 12 02 88           1205 	lcall	_check_radio_input
   0382 80 03              1206 	sjmp	00103$
   0384                    1207 00102$:
                           1208 ;	main.c:656: re_enter_rx();
   0384 12 01 81           1209 	lcall	_re_enter_rx
   0387                    1210 00103$:
                           1211 ;	main.c:661: if (radio_mode != RADIO_TX)
   0387 74 02              1212 	mov	a,#0x02
   0389 B5 08 02           1213 	cjne	a,_radio_mode,00116$
   038C 80 E6              1214 	sjmp	00107$
   038E                    1215 00116$:
                           1216 ;	main.c:662: handle_tx();
   038E 12 01 96           1217 	lcall	_handle_tx
   0391 80 E1              1218 	sjmp	00107$
                           1219 	.area CSEG    (CODE)
                           1220 	.area CONST   (CODE)
                           1221 	.area XINIT   (CODE)
                           1222 	.area CABS    (ABS,CODE)
