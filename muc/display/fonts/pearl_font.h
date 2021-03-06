const unsigned char pearl_font_bits[]={
0x00, 0x00, 0x00, 0x00, 0x00, 	/*  32 0x20   */
0x0e, 0x5f, 0x5f, 0x0e, 	/*  33 0x21 ! */
0x03, 0x03, 0x00, 0x03, 0x03, 	/*  34 0x22 " */
0x14, 0x7f, 0x7f, 0x14, 0x7f, 0x7f, 0x14, 	/*  35 0x23 # */
0x24, 0x2e, 0x6b, 0x6b, 0x3a, 0x12, 	/*  36 0x24 $ */
0x46, 0x66, 0x30, 0x18, 0x0c, 0x66, 0x62, 	/*  37 0x25 % */
0x30, 0x7e, 0x4f, 0x59, 0x37, 0x7a, 0x48, 	/*  38 0x26 & */
0x04, 0x07, 0x03, 	/*  39 0x27 ' */
0x1c, 0x3e, 0x63, 0x41, 	/*  40 0x28 ( */
0x41, 0x63, 0x3e, 0x1c, 	/*  41 0x29 ) */
0x08, 0x2a, 0x3e, 0x1c, 0x1c, 0x3e, 0x2a, 0x08, 	/*  42 0x2a * */
0x08, 0x08, 0x3e, 0x3e, 0x08, 0x08, 	/*  43 0x2b + */
0x80, 0xe0, 0x60, 	/*  44 0x2c , */
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 	/*  45 0x2d - */
0x60, 0x60, 	/*  46 0x2e . */
0x40, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x01, 	/*  47 0x2f / */
0x3e, 0x7f, 0x59, 0x5d, 0x4d, 0x7f, 0x3e, 	/*  48 0x30 0 */
0x00, 0x02, 0x02, 0x7f, 0x7f, 0x00, 0x00, 	/*  49 0x31 1 */
0x42, 0x63, 0x71, 0x59, 0x4d, 0x47, 0x42, 	/*  50 0x32 2 */
0x22, 0x63, 0x41, 0x49, 0x49, 0x7f, 0x36, 	/*  51 0x33 3 */
0x18, 0x1c, 0x16, 0x13, 0x7f, 0x7f, 0x10, 	/*  52 0x34 4 */
0x27, 0x67, 0x45, 0x45, 0x45, 0x7d, 0x39, 	/*  53 0x35 5 */
0x3c, 0x7e, 0x4b, 0x49, 0x49, 0x78, 0x30, 	/*  54 0x36 6 */
0x01, 0x61, 0x71, 0x19, 0x0d, 0x07, 0x03, 	/*  55 0x37 7 */
0x36, 0x7f, 0x49, 0x49, 0x49, 0x7f, 0x36, 	/*  56 0x38 8 */
0x06, 0x0f, 0x49, 0x49, 0x69, 0x3f, 0x1e, 	/*  57 0x39 9 */
0x66, 0x66, 	/*  58 0x3a : */
0x80, 0xe6, 0x66, 	/*  59 0x3b ; */
0x08, 0x1c, 0x36, 0x63, 0x41, 	/*  60 0x3c < */
0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 	/*  61 0x3d = */
0x41, 0x63, 0x36, 0x1c, 0x08, 	/*  62 0x3e > */
0x02, 0x03, 0x51, 0x59, 0x0f, 0x06, 	/*  63 0x3f ? */
0x3e, 0x7f, 0x41, 0x5d, 0x5d, 0x5f, 0x1e, 	/*  64 0x40 @ */
0x78, 0x7c, 0x16, 0x13, 0x16, 0x7c, 0x78, 	/*  65 0x41 A */
0x7f, 0x7f, 0x49, 0x49, 0x49, 0x7f, 0x36, 	/*  66 0x42 B */
0x3e, 0x7f, 0x41, 0x41, 0x41, 0x63, 0x22, 	/*  67 0x43 C */
0x7f, 0x7f, 0x41, 0x41, 0x41, 0x7f, 0x3e, 	/*  68 0x44 D */
0x7f, 0x7f, 0x49, 0x49, 0x49, 0x41, 0x41, 	/*  69 0x45 E */
0x7f, 0x7f, 0x09, 0x09, 0x09, 0x01, 0x01, 	/*  70 0x46 F */
0x3e, 0x7f, 0x41, 0x41, 0x49, 0x7b, 0x3a, 	/*  71 0x47 G */
0x7f, 0x7f, 0x08, 0x08, 0x08, 0x7f, 0x7f, 	/*  72 0x48 H */
0x41, 0x41, 0x7f, 0x7f, 0x41, 0x41, 	/*  73 0x49 I */
0x30, 0x70, 0x40, 0x40, 0x40, 0x7f, 0x3f, 	/*  74 0x4a J */
0x7f, 0x7f, 0x08, 0x1c, 0x36, 0x63, 0x41, 	/*  75 0x4b K */
0x7f, 0x7f, 0x40, 0x40, 0x40, 0x40, 0x40, 	/*  76 0x4c L */
0x7f, 0x7e, 0x0c, 0x18, 0x0c, 0x7e, 0x7f, 	/*  77 0x4d M */
0x7f, 0x7f, 0x06, 0x0c, 0x18, 0x7f, 0x7f, 	/*  78 0x4e N */
0x3e, 0x7f, 0x41, 0x41, 0x41, 0x7f, 0x3e, 	/*  79 0x4f O */
0x7f, 0x7f, 0x09, 0x09, 0x09, 0x0f, 0x06, 	/*  80 0x50 P */
0x3e, 0x7f, 0x51, 0x71, 0x61, 0xff, 0xbe, 	/*  81 0x51 Q */
0x7f, 0x7f, 0x09, 0x19, 0x39, 0x6f, 0x46, 	/*  82 0x52 R */
0x22, 0x67, 0x4d, 0x49, 0x59, 0x73, 0x22, 	/*  83 0x53 S */
0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01, 	/*  84 0x54 T */
0x3f, 0x7f, 0x40, 0x40, 0x40, 0x7f, 0x3f, 	/*  85 0x55 U */
0x03, 0x0f, 0x3c, 0x70, 0x70, 0x3c, 0x0f, 0x03, 	/*  86 0x56 V */
0x7f, 0x7f, 0x30, 0x18, 0x30, 0x7f, 0x7f, 	/*  87 0x57 W */
0x41, 0x63, 0x36, 0x1c, 0x1c, 0x36, 0x63, 0x41, 	/*  88 0x58 X */
0x03, 0x07, 0x0c, 0x78, 0x78, 0x0c, 0x07, 0x03, 	/*  89 0x59 Y */
0x41, 0x61, 0x71, 0x59, 0x4d, 0x47, 0x43, 	/*  90 0x5a Z */
0x7f, 0x7f, 0x41, 0x41, 	/*  91 0x5b [ */
0x01, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x40, 	/*  92 0x5c \ */
0x41, 0x41, 0x7f, 0x7f, 	/*  93 0x5d ] */
0x08, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x08, 	/*  94 0x5e ^ */
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 	/*  95 0x5f _ */
0x03, 0x07, 0x04, 	/*  96 0x60 ` */
0x20, 0x74, 0x54, 0x54, 0x54, 0x7c, 0x78, 	/*  97 0x61 a */
0x7f, 0x7f, 0x44, 0x44, 0x44, 0x7c, 0x38, 	/*  98 0x62 b */
0x38, 0x7c, 0x44, 0x44, 0x44, 0x6c, 0x28, 	/*  99 0x63 c */
0x38, 0x7c, 0x44, 0x44, 0x44, 0x7f, 0x7f, 	/* 100 0x64 d */
0x38, 0x7c, 0x54, 0x54, 0x54, 0x5c, 0x18, 	/* 101 0x65 e */
0x08, 0x7e, 0x7f, 0x09, 0x01, 0x03, 0x02, 	/* 102 0x66 f */
0x18, 0xbc, 0xa4, 0xa4, 0xa4, 0xfc, 0x7c, 	/* 103 0x67 g */
0x7f, 0x7f, 0x04, 0x04, 0x04, 0x7c, 0x78, 	/* 104 0x68 h */
0x04, 0x7d, 0x7d, 	/* 105 0x69 i */
0x40, 0xc0, 0x80, 0x80, 0x80, 0xfd, 0x7d, 	/* 106 0x6a j */
0x7f, 0x7f, 0x10, 0x38, 0x6c, 0x44, 	/* 107 0x6b k */
0x01, 0x7f, 0x7f, 	/* 108 0x6c l */
0x7c, 0x7c, 0x0c, 0x18, 0x0c, 0x7c, 0x78, 	/* 109 0x6d m */
0x7c, 0x7c, 0x04, 0x04, 0x04, 0x7c, 0x78, 	/* 110 0x6e n */
0x38, 0x7c, 0x44, 0x44, 0x44, 0x7c, 0x38, 	/* 111 0x6f o */
0xfc, 0xfc, 0x24, 0x24, 0x24, 0x3c, 0x18, 	/* 112 0x70 p */
0x18, 0x3c, 0x24, 0x24, 0x24, 0xfc, 0xfc, 	/* 113 0x71 q */
0x7c, 0x7c, 0x08, 0x04, 0x04, 0x0c, 0x08, 	/* 114 0x72 r */
0x48, 0x5c, 0x54, 0x54, 0x54, 0x74, 0x24, 	/* 115 0x73 s */
0x04, 0x3f, 0x7f, 0x44, 0x64, 0x20, 	/* 116 0x74 t */
0x3c, 0x7c, 0x40, 0x40, 0x40, 0x7c, 0x3c, 	/* 117 0x75 u */
0x1c, 0x3c, 0x60, 0x40, 0x60, 0x3c, 0x1c, 	/* 118 0x76 v */
0x3c, 0x7c, 0x60, 0x30, 0x60, 0x7c, 0x3c, 	/* 119 0x77 w */
0x44, 0x6c, 0x38, 0x10, 0x38, 0x6c, 0x44, 	/* 120 0x78 x */
0x04, 0x8c, 0xd8, 0x70, 0x30, 0x18, 0x0c, 0x04, 	/* 121 0x79 y */
0x44, 0x64, 0x74, 0x54, 0x5c, 0x4c, 0x44, 	/* 122 0x7a z */
0x08, 0x08, 0x3e, 0x77, 0x41, 0x41, 	/* 123 0x7b { */
0x7f, 0x7f, 	/* 124 0x7c | */
0x41, 0x41, 0x77, 0x3e, 0x08, 0x08, 	/* 125 0x7d } */
0x02, 0x01, 0x01, 0x03, 0x02, 0x02, 0x01, 	/* 126 0x7e ~ */
0x70, 0x78, 0x4c, 0x46, 0x4c, 0x78, 0x70, 	/* 127 0x7f   */
0x1e, 0xbf, 0xa1, 0xa1, 0xe1, 0x73, 0x12, 	/* 128 0x80   */
0x3d, 0x7d, 0x40, 0x40, 0x3d, 0x7d, 0x40, 	/* 129 0x81   */
0x38, 0x7c, 0x54, 0x56, 0x57, 0x5d, 0x18, 	/* 130 0x82   */
0x22, 0x75, 0x55, 0x55, 0x3d, 0x79, 0x42, 	/* 131 0x83   */
0x21, 0x75, 0x54, 0x54, 0x3c, 0x79, 0x41, 	/* 132 0x84   */
0x20, 0x74, 0x55, 0x57, 0x3e, 0x78, 0x40, 	/* 133 0x85   */
0x20, 0x74, 0x57, 0x57, 0x3c, 0x78, 0x40, 	/* 134 0x86   */
0x18, 0x3c, 0xa4, 0xa4, 0xe4, 0x64, 0x24, 	/* 135 0x87   */
0x3a, 0x7d, 0x55, 0x55, 0x55, 0x5d, 0x1a, 	/* 136 0x88   */
0x39, 0x7d, 0x54, 0x54, 0x54, 0x5d, 0x19, 	/* 137 0x89   */
0x38, 0x7c, 0x55, 0x57, 0x56, 0x5c, 0x18, 	/* 138 0x8a   */
0x01, 0x45, 0x7c, 0x7c, 0x41, 0x01, 	/* 139 0x8b   */
0x02, 0x01, 0x45, 0x7d, 0x7d, 0x41, 0x02, 	/* 140 0x8c   */
0x49, 0x7b, 0x7a, 0x40, 	/* 141 0x8d   */
0x79, 0x7d, 0x16, 0x12, 0x16, 0x7d, 0x79, 	/* 142 0x8e   */
0x78, 0x7e, 0x17, 0x15, 0x17, 0x7e, 0x78, 	/* 143 0x8f   */
0x7c, 0x7c, 0x56, 0x57, 0x55, 0x44, 0x44, 	/* 144 0x90   */
0x20, 0x74, 0x54, 0x7c, 0x7c, 0x54, 0x54, 	/* 145 0x91   */
0x7c, 0x7e, 0x0b, 0x09, 0x7f, 0x7f, 0x49, 	/* 146 0x92   */
0x3a, 0x7d, 0x45, 0x45, 0x45, 0x7d, 0x3a, 	/* 147 0x93   */
0x39, 0x7d, 0x44, 0x44, 0x44, 0x7d, 0x39, 	/* 148 0x94   */
0x38, 0x7c, 0x45, 0x47, 0x46, 0x7c, 0x38, 	/* 149 0x95   */
0x3a, 0x79, 0x41, 0x41, 0x39, 0x7a, 0x40, 	/* 150 0x96   */
0x3c, 0x7d, 0x43, 0x42, 0x3c, 0x7c, 0x40, 	/* 151 0x97   */
0x9d, 0xbd, 0xa0, 0xa0, 0xa0, 0xfd, 0x7d, 	/* 152 0x98   */
0x19, 0x3d, 0x66, 0x42, 0x66, 0x3d, 0x19, 	/* 153 0x99   */
0x3d, 0x7d, 0x40, 0x40, 0x40, 0x7d, 0x3d, 	/* 154 0x9a   */
0x18, 0x3c, 0x24, 0xe7, 0xe7, 0x24, 0x24, 	/* 155 0x9b   */
0x48, 0x7e, 0x7f, 0x49, 0x43, 0x66, 0x20, 	/* 156 0x9c   */
0x2b, 0x2f, 0xfc, 0xfc, 0x2f, 0x2b, 	/* 157 0x9d   */
0xff, 0xff, 0x09, 0x09, 0x2f, 0xf6, 0xf8, 0xa0, 	/* 158 0x9e   */
0x20, 0x60, 0x48, 0x7e, 0x3f, 0x09, 0x03, 0x02, 	/* 159 0x9f   */
0x20, 0x74, 0x56, 0x57, 0x3d, 0x78, 0x40, 	/* 160 0xa0   */
0x48, 0x7a, 0x7b, 0x41, 	/* 161 0xa1   */
0x38, 0x7c, 0x44, 0x46, 0x47, 0x7d, 0x38, 	/* 162 0xa2   */
0x3c, 0x7c, 0x42, 0x43, 0x3d, 0x7c, 0x40, 	/* 163 0xa3   */
0x0a, 0x7b, 0x71, 0x0b, 0x0a, 0x7b, 0x71, 	/* 164 0xa4   */
0x7a, 0x7b, 0x19, 0x33, 0x62, 0x7b, 0x79, 	/* 165 0xa5   */
0x26, 0x2f, 0x29, 0x2f, 0x2f, 0x28, 	/* 166 0xa6   */
0x26, 0x2f, 0x29, 0x2f, 0x26, 	/* 167 0xa7   */
0x20, 0x70, 0x5d, 0x4d, 0x40, 0x60, 0x20, 	/* 168 0xa8   */
0x38, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 	/* 169 0xa9   */
0x08, 0x08, 0x08, 0x08, 0x08, 0x38, 0x38, 	/* 170 0xaa   */
0x42, 0x6f, 0x3f, 0x18, 0xcc, 0xee, 0xbb, 0x91, 	/* 171 0xab   */
0x42, 0x6f, 0x3f, 0x58, 0x6c, 0xd6, 0xfb, 0x41, 	/* 172 0xac   */
0x3e, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x3e, 	/* 173 0xad   */
0x08, 0x1c, 0x36, 0x22, 0x08, 0x1c, 0x36, 0x22, 	/* 174 0xae   */
0x22, 0x36, 0x1c, 0x08, 0x22, 0x36, 0x1c, 0x08, 	/* 175 0xaf   */
0x0c, 0x0c, 0x0c, 0x0c, 0x3f, 0x1e, 0x0c, 	/* 176 0xb0   */
0x0c, 0x1e, 0x3f, 0x0c, 0x0c, 0x0c, 0x0c, 	/* 177 0xb1   */
0x04, 0x06, 0x7f, 0x7f, 0x06, 0x04, 	/* 178 0xb2   */
0x10, 0x30, 0x7f, 0x7f, 0x30, 0x10	/* 179 0xb3   */
};

const unsigned char pearl_font_info[]={
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
5, 4, 5, 7, 6, 7, 7, 3, 4, 4, 8, 6, 3, 6, 2, 8, 
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 3, 5, 6, 5, 6, 
7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 6, 7, 8, 7, 8, 8, 7, 4, 8, 4, 7, 7, 
3, 7, 7, 7, 7, 7, 7, 7, 7, 3, 7, 6, 3, 7, 7, 7, 
7, 7, 7, 7, 6, 7, 7, 7, 7, 8, 7, 6, 2, 6, 7, 7, 
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 7, 4, 7, 7, 
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 8, 8, 
7, 4, 7, 7, 7, 7, 6, 5, 7, 7, 7, 8, 8, 8, 8, 8, 
7, 7, 6, 6};

const u_int16_t pearl_font_cpos[]={
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 5, 9, 14, 21, 27, 34, 41, 44, 48, 52, 60, 66, 69, 75, 77, 
85, 92, 99, 106, 113, 120, 127, 134, 141, 148, 155, 157, 160, 165, 171, 176, 
182, 189, 196, 203, 210, 217, 224, 231, 238, 245, 251, 258, 265, 272, 279, 286, 
293, 300, 307, 314, 321, 327, 334, 342, 349, 357, 365, 372, 376, 384, 388, 395, 
402, 405, 412, 419, 426, 433, 440, 447, 454, 461, 464, 471, 477, 480, 487, 494, 
501, 508, 515, 522, 529, 535, 542, 549, 556, 563, 571, 578, 584, 586, 592, 599, 
606, 613, 620, 627, 634, 641, 648, 655, 662, 669, 676, 683, 689, 696, 700, 707, 
714, 721, 728, 735, 742, 749, 756, 763, 770, 777, 784, 791, 798, 805, 811, 819, 
827, 834, 838, 845, 852, 859, 866, 872, 877, 884, 891, 898, 906, 914, 922, 930, 
938, 945, 952, 958};
const unsigned int pearl_font_height=8;
const unsigned int pearl_font_maxchar=179;
