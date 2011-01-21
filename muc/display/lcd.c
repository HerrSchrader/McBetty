/*
    lcd.c - lcd control & graphics primitives
    Copyright (C) 2007  Ch. Klippel <ck@mamalala.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "global.h"
#include "lpc2220.h"
#include "kernel.h"
#include "lcd.h"
#include "fonty.h"


/* These variables are used to store function pointers.
	They are needed to access functions which are in ".fastcode" segments.
	TODO some functions have type definitions, others are explicitly stated. Change that.
*/
void (*write_buf) (uint8 row, uint8 col, uint8 w, uint8 h);
void (*draw_block) (uint8 row, uint8 col, uint8 w, uint8 h, uint8 fg_col);
static void (*draw_hline) (uint8 row, uint8 col, uint8 l, uint8 fg_col);
static void (*draw_vline) (uint8 row, uint8 col, uint8 l, uint8 fg_col);
static void(*do_scroll) (uint8 row, uint8 col, uint8 width, uint8 bitmask, uint8 offset);
static void (*write_pattern) (uint8 row, uint8 col, uint8 l, uint8 mask, uint8 pattern0, uint8 pattern1);

/* 
	The display is organized as 160 (internally 176) rows and 128 columns of pixels with 2 bits each of gray scale information.
	We can address the LCD only bytewise. A single byte holds one bit of color information of 8 consecutive rows at a specific column.
	We can really address the LCD only in terms of pages. A page is 128 pixels wide and 8 pixels high.
	Pages start on a row boundary which is divisble by 8.
	
	We can address up to 22 pages of LCD RAM. (Only 20 are shown)
	Each page contains 256 bytes.
	They are organized as 8 rows and 128 colums per row.
	The first byte in each page contains the msb of the color of 8 bits of column 0.
	The second byte in each page contains the lsb of the color of 8 bits of column 0.

	The third and fourth byte determine the color of 8 bits of column 1 and so on.
*/

/* Before drawing to a given page, we have to tell the LCD the page number (0..21). Only pages 0..19 are visible.  */
#define lcd_set_page(page_no) {LCD_CMD = 0xB0;LCD_CMD = page_no;}

/* We often do not want to address a specific page, but a given row. 
	We set the page from the row number (0..175). Only rows 0..159 are visible.
*/
#define lcd_set_rowadr(row) lcd_set_page((row) >> 3)

/* Set column address where to begin the drawing or reading.
	Valid are values from 0 ... 127.
	NOTE does not check for a valid column
*/
#define lcd_set_coladr(col) {LCD_CMD = 0x10 + ((col) >> 4);LCD_CMD = (col) & 0x0F;}



/* Here we keep two small buffers to simplify drawing 
	drawbuf16[] can hold 2 pages, 
	
	rcubuf holds information read from lcd, it can hold only one page.
	The rcubuf is simply a partial (1 page) copy of the current lcd contents
*/
static uint8 rcubuf[2][128];

/* For up to 16 bit high operations */
static uint16 drawbuf16[2][128];

/* 
	This buffer is used for pop-up windows.
	It saves a rectangular region in the middle of the screen
	with a size of POPUP_PAGES*8 rows and 128 columns.
	That are around 5 kBytes.
	Indexed by page and column
*/
static uint8 popup_buf[POPUP_PAGES][2 * 128];


/* Read w columns into rcu_buf */
static void __attribute__ ((section(".text.fastcode"))) 
_read_lcd(uint8 row, uint8 col, uint8 w){
	uint8 d, i;

	lcd_set_rowadr(row);
	lcd_set_coladr(col);
	
	d = LCD_DATA;		// Dummy Data Read; LCD chip needs this
		
	for (i=0; i<w; i++){
		rcubuf[0][i] = LCD_DATA;
		rcubuf[1][i] = LCD_DATA;
	};
	
	d = LCD_DATA;		// Another read, why that ? Maybe bug in chip, does not work correctly without
}


/* Read LCD contents into popup_buf 
	Need not be in fastcode section
*/
void
popup_save(){
	uint8 page, col, d, i;
	
	for (page=0; page < POPUP_PAGES; page++){
		lcd_set_page(POPUP_STARTPAGE + page);
		lcd_set_coladr(0);
		d = LCD_DATA;		// Dummy Data Read; LCD chip needs this
		for (i=0, col=0; col<128; col++){
			popup_buf[page][i++] = LCD_DATA;
			popup_buf[page][i++] = LCD_DATA;
		};
		d = LCD_DATA;		// Another read, why that ? Maybe bug in chip, does not work correctly without
	};

}

/*
	Write popup_buf contents back into LCD
*/
void
popup_restore(){
	uint8 page, col, i;
			
	for (page=0; page < POPUP_PAGES; page++){
		lcd_set_page(POPUP_STARTPAGE + page);
		lcd_set_coladr(0);
		for (i=0, col=0; col<128; col++){
			LCD_DATA = popup_buf[page][i++];
			LCD_DATA = popup_buf[page][i++];
		};
	};
}

/* 
	Write a fixed pattern into LCD.
	The LCD is only changed at those positions, where mask bits are 1.
*/
static void __attribute__ ((section(".text.fastcode"))) 
_write_pattern(uint8 row, uint8 col, uint8 l, uint8 mask, uint8 pattern0, uint8 pattern1){
	uint8 i;

	/* No need to read lcd if all of lcd is overwritten */
	if (mask == 0xff){
		lcd_set_rowadr(row);
		lcd_set_coladr(col);
		for (i=0; i<l; i++){
			LCD_DATA = pattern0;
			LCD_DATA = pattern1;
		}
	} else {
		_read_lcd(row, col, l);
		lcd_set_rowadr(row);
		lcd_set_coladr(col);

		for (i=0; i<l; i++){
			LCD_DATA = (rcubuf[0][i] & ~mask) | (pattern0 & mask);
			LCD_DATA = (rcubuf[1][i] & ~mask) | (pattern1 & mask);
		}
	};
}

/* 
	Overlay the contents of LCD with contents of drawbuf16[].
	The LCD is only changed at those positions, where mask bits are 1.
	Before the contents are mixed, drawbuf16 is shifted by a certain amount.
	The shift factor s means right shift s positions 
*/
static void __attribute__ ((section(".text.fastcode"))) 
_overlay_shiftr(uint8 row, uint8 col, uint8 l, uint8 mask, uint8 s){
	uint8 i;

	/* No need to use rcubuf if all rows of this page are changed */
	if (mask == 0xff){
		lcd_set_rowadr(row);
		lcd_set_coladr(col);
		for (i=0; i<l; i++){
			LCD_DATA = drawbuf16[0][i] >> s;
			LCD_DATA = drawbuf16[1][i] >> s;
		}
	} else {
		_read_lcd(row, col, l);
		lcd_set_rowadr(row);
		lcd_set_coladr(col);

		for (i=0; i<l; i++){
			LCD_DATA = (rcubuf[0][i] & ~mask) | ((drawbuf16[0][i] >> s) & mask);
			LCD_DATA = (rcubuf[1][i] & ~mask) | ((drawbuf16[1][i] >> s) & mask);
		}
	};
}

/* 
	Overlay the contents of LCD with contents of drawbuf16[].
	The LCD is only changed at those positions, where mask bits are 1.
	Before the contents are mixed, drawbuf16 is shifted by a certain amount.
	The shift factor s means left shift s positions 
*/
static void __attribute__ ((section(".text.fastcode"))) 
_overlay_shiftl(uint8 row, uint8 col, uint8 l, uint8 mask, uint8 s){
	uint8 i;

	/* No need to use rcubuf if all rows of this page are changed */
	if (mask == 0xff){
		lcd_set_rowadr(row);
		lcd_set_coladr(col);
		for (i=0; i<l; i++){
			LCD_DATA = drawbuf16[0][i] << s;
			LCD_DATA = drawbuf16[1][i] << s;
		}
	} else {
		_read_lcd(row, col, l);
		lcd_set_rowadr(row);
		lcd_set_coladr(col);

		for (i=0; i<l; i++){
			LCD_DATA = (rcubuf[0][i] & ~mask) | ((drawbuf16[0][i] << s) & mask);
			LCD_DATA = (rcubuf[1][i] & ~mask) | ((drawbuf16[1][i] << s) & mask);
		}
	};
}

static uint8 col_bits0[4]={0x00, 0x00, 0xff, 0xff};
static uint8 col_bits1[4]={0x00, 0xff, 0x00, 0xff};

static uint16 col_bits16_0[4]={0x0000, 0x0000, 0xffff, 0xffff};
static uint16 col_bits16_1[4]={0x0000, 0xffff, 0x0000, 0xffff};


/* 
	We want to store a pattern with a height of 16 bits into drawbuf16[0..1][pos] with the given color 
	The pattern has bits set to 1 where we want to use the foreground color and bits set to 0 where to use the
	background color.
*/
void 
store_buf (uint8 pos, uint16 pattern, uint8 fg_col, uint8 bg_col){
	uint16 bits0, bits1;
		
	bits0 = (col_bits16_0[fg_col] & pattern) | (col_bits16_0[bg_col] & ~pattern);
	bits1 = (col_bits16_1[fg_col] & pattern) | (col_bits16_1[bg_col] & ~pattern);
	
	drawbuf16[0][pos] = bits0;
	drawbuf16[1][pos] = bits1;
};

/* 
	Put contents of drawbuf16 into lcd.
	This function needs some setup. We assume the first 2 pages of drawbuf contain some pixels (text or drawing), 
	that have to be drawn on lcd. The colors of these pixels are already correctly set up.
	We only want the first w pixels to be drawn.
	We may not need the complete height of the drawbuf. The variable h is the relevant height.
*/
static void __attribute__ ((section(".text.fastcode"))) 
_write_buf(uint8 row, uint8 col, uint8 w, uint8 h){
	uint8 s;
	uint8 bitmask;
	int height = h;

	/* We have to spread the rows on 2 or 3 consecutive pages depending on s and h. */
	
	/* -- First page in LCD -- */		
	
	s = 8 - (row & 7);			/* s is how many rows we want to draw in the current page. */

	bitmask = 0xff << (8-s);		// The lower s rows will be changed
	
	_overlay_shiftl(row, col, w, bitmask, 8-s);
	/* We have just drawn s rows. Adjust height and row */
	height -= s;
	row += s;

	/* -- Second page in LCD -- ( Now we are on a page boundary) */
	
	/* If we have still 8 or more rows to draw, we fill one complete page */ 	
	if (height >= 8)
		bitmask = 0xff;						// All 8 rows are changed
	else 
		bitmask = 0xff >> (8 - height);	// Upper height bits are changed */ 
	_overlay_shiftr(row, col, w, bitmask, s);
	height -= 8;
	row += 8;
	
	/* -- Third page in LCD -- */	
	
	if (height <= 0) return;
	
	bitmask = 0xff >> (8 - height);				/* Upper height bits are changed */
	_overlay_shiftr(row, col, w, bitmask, 8+s); 
}



/* Scroll part of a page of LCD RAM horizontally (0 .. 8 rows).
	Start at column col. The area to shift is width pixels wide.
	The last horizontal column is NOT erased.
	Only those bits set in bitmask are shifted, the others are unaffected.
*/
static void __attribute__ ((section(".text.fastcode"))) 
_do_scroll(uint8 row, uint8 col, uint8 width, uint8 bitmask, uint8 offset){
	uint8 com_bitmask = ~bitmask;
	int i;
	
	_read_lcd(row, col, width);
	lcd_set_rowadr(row);
	lcd_set_coladr(col);
	for (i=0; i < width - offset; i++){
		LCD_DATA = (rcubuf[0][i] & com_bitmask) | (rcubuf[0][i+offset] & bitmask);
		LCD_DATA = (rcubuf[1][i] & com_bitmask) | (rcubuf[1][i+offset] & bitmask);
	};
}
	

/* Scroll an area on LCD with height =  0..16 
	We can only efficiently scroll those pixels at a time, which are on the same page.
	So we shift in phases.
*/
void 
lcd_scroll(uint8 row, uint8 col, uint8 width, int height, int offset){
	uint8 s;	// s is our shift factor. It tells us how many rows we shift in the current page
	uint8 bitmask;
	
	s = 8 - (row & 7);
	bitmask = 0xff << (8 - s);		// The first (8-s) rows are 0 and will not be changed, the last s rows are 1
	/* We want to scroll s rows. 
		If this is more than the total height, we modify the bitmask */
	if (s > height){
		bitmask &= 0xff >> (s - height);	/* The last (s - height) rows are set to 0 */ 
		s = height;							/* We only scroll height rows */
	};	
	do_scroll (row, col, width, bitmask, offset);
	
	height -= s;	// We have just shifted s rows. height is reduced.
	row += s;		// We are now on a page boundary !
	
	if (height >= 8){
		do_scroll (row, col, width, 0xff, offset);
		height -= 8;	// We have shifted 8 more rows.
		row += 8;
	};
	
	if (height > 0){		// still more to do ? 
		bitmask = 0xff >> (8 - height);			// the first height bits will be set
		do_scroll (row, col, width, bitmask, offset); 
	};		
};



/* Draw a rectangular block with a given width (w) and height (h), starting at column col and row row 
	We do this in three phases like in _draw_vline() below
*/
static void __attribute__ ((section(".text.fastcode"))) 
_draw_block(uint8 row, uint8 col, uint8 w, uint8 h, uint8 fg_col) {
	uint8 s, bitmask;
	
	if (h == 0) return;
	s = 8 - (row & 7);			/* s is how many rows we want to draw in the current page. */

	// We only need the first phase, if the starting row is not on a page boundary
	if ( s != 8) {
		// First Phase 
		bitmask = 0xff << (8-s);		// The upper  (8-s) rows will NOT be changed

		if (s > h){						// for short blocks we must not use all rows in bitmask		
			bitmask &= 0xff >> (s - h);
			s = h;
		};
		_write_pattern(row, col, w, bitmask, col_bits0[fg_col], col_bits1[fg_col]);
		
		row += s;	// Now row is on a page boundary
		h -= s;
	};
	
	// Second phase
	while(h >= 8) {
		// draw 8 rows at once
		_write_pattern(row, col, w, 0xff, col_bits0[fg_col], col_bits1[fg_col] );
		row += 8;
		h -= 8;	
	};
	
	// Third phase (h < 8)
	if (h > 0){						// still lines to draw
		bitmask = 0xff >> (8-h);
		_write_pattern(row, col, w, bitmask, col_bits0[fg_col], col_bits1[fg_col]);
	};
}

/* Draw a horizontal line 
	l is the lenth of the line in pixels
*/
static void __attribute__ ((section(".text.fastcode"))) 
_draw_hline(uint8 row, uint8 col, uint8 l, uint8 fg_col) {
	uint8 bitmask;

	if (l == 0) return;
	
	// p is the row bitmask within a page, i.e. if we want to change the rth row in a page, p=(1<<r)
	bitmask = 1 << (row & 0x07);

	_write_pattern( row, col, l, bitmask, col_bits0[fg_col], col_bits1[fg_col] );
}


/* Draw a vertical line 
	First draw all the pixels that are above the first page boundary
	Then draw columns of 8 rows simultaneously
	until the number of remaining rows is less than 8.
	Then draw the pixels that are below the last page boundary. 
*/
static void __attribute__ ((section(".text.fastcode"))) 
_draw_vline(uint8 row, uint8 col, uint8 l, uint8 fg_col) {
	uint8 cur_row;
	uint8 row_offset, bitmask_rows;
	uint8 bitmask;
	
	if ( l== 0) return;

	cur_row = row;			/* starting row */
	row_offset = cur_row & 0x07;
	
	// We only need the first phase, if the starting row is not on a page boundary
	if ( 0 != row_offset) {
		// First Phase 
		bitmask = 0xff << row_offset;
		bitmask_rows = 8 - row_offset;	// this many rows are filled by bitmask
		if (bitmask_rows > l){		// for short lines we must not use all rows in bitmask		
			bitmask = bitmask & (0xff >> (bitmask_rows - l));
			bitmask_rows = l;
		};
		_write_pattern(cur_row, col, 1, bitmask, col_bits0[fg_col], col_bits1[fg_col] );		

		cur_row += bitmask_rows;	// Now cur_row is on a page boundary
		l -= bitmask_rows;
	};
	
	// Second phase
	while(l >= 8) {
		// draw 8 rows at once
		_write_pattern(cur_row, col, 1, 0xff, col_bits0[fg_col], col_bits1[fg_col] );		
		cur_row += 8;
		l -= 8;	
	};

	// Third phase (l < 8)
	if (l > 0){		// still lines to draw
		bitmask = 0xff >> (8-l);
		_write_pattern(cur_row, col, 1, bitmask, col_bits0[fg_col], col_bits1[fg_col] );
	};
}

/* Draw a non-filled rectangle. The parameter lw gives the thickness of the line */
void 
draw_rect(uint8 col, uint8 row, uint8 w, uint8 h, uint8 lw, uint8 fg_col){
	if (lw) {
		if(lw == 1) {
			draw_hline(row,     col,     w,   fg_col);
			draw_hline(row+h-1, col,     w,   fg_col);
			draw_vline(row+1,     col,   h-2, fg_col);
			draw_vline(row+1, col+w-1,   h-2, fg_col);
		} else{
			draw_block(row,      col,      w,  lw,       fg_col);
			draw_block(row+h-lw, col,      w,  lw,       fg_col);
			draw_block(row+lw,   col,      lw, h-(2*lw), fg_col);
			draw_block(row+lw,   col+w-lw, lw, h-(2*lw), fg_col);
		}
	}
}

/* This routine fills a rectangular block on LCD bewtween oldval and newval 
	The range of oldval and newval is bewtween 0 and 100.
	The lowest pixel is not drawm.
	So at most 100 pixels are drawn (from col+1 to col+100).
	If newval > oldval, the foreground color is used, else the background color.
	We do a progressbar this way to allow incremental updates, so that we do not have to redraw
	the whole bar all the time.
*/
void
draw_progressbar(uint8 row, uint8 col, uint8 height, uint8 fg_col, uint8 bg_col, uint8 oldval, uint8 newval){
	if ( (newval < 0) || (newval > 100)) return;
	if ( (oldval < 0) || (oldval > 100)) return;
	
	if (newval > oldval){
		draw_block(row, col + oldval + 1, newval - oldval, height, fg_col);
		return;
	};

	if (newval < oldval)
		draw_block(row, col + newval + 1, oldval - newval, height, bg_col);

};

static void 
draw_pixel (uint8 row,  uint8 col, uint8 fg_col){
	uint8 bitmask;

	bitmask = 1 << (row & 0x07);
	write_pattern(row, col, 1, bitmask, col_bits0[fg_col], col_bits1[fg_col] );
}


/* Triangle progress bar. 
###	A bit of a hack at the moment. We only need this at one place, so we have hardcoded some values.
*/

/* The normal draw_vline draws a line from top to bottom, i.e. from row+0 to row + h (exclusive).
	We need a routine here to draw a line from bottom to top. 
	We are given the bottom row and draw h pixels upward. (i. e. from row+0 to row - h (exclusive)
*/
#define vline_up(row, col, h, c) (draw_vline( (row) - (h) + 1, (col), (h), (c)))

/* At each column we have a certain height that is linearily increasing */
static int
triangle_height(int col){
	return (2 + (col >> 2)); 
};


void
draw_trianglebar(uint8 row, uint8 col, uint8 width, uint8 height, uint8 fg_col, uint8 bg_col, uint8 oldval, uint8 newval){

	if ( (newval < 0) || (newval > 50)) return;
	if (oldval > 50) oldval = 50;
		
	if (newval > oldval){
		while (newval >= oldval){	
			vline_up (row + height - 1, col + oldval, triangle_height(oldval), fg_col);
			oldval++;
		} 
	} else {
		while (newval < oldval){
			vline_up (row + height - 1, col + oldval, triangle_height(oldval), bg_col);
			oldval--;
		};
	};
};

void
trianglebar_border(uint8 row, uint8 col, uint8 width, uint8 height, uint8 fg_col, uint8 bg_col){
	int i;
	
	/* Erase everything */
	draw_trianglebar(row, col, width, height, fg_col, bg_col, 50, 0);
	
	/* Show the first bar (val = 0) */
	vline_up (row + height, col, triangle_height(0) , fg_col);
	
	/* Show the last bar (val = 51) */
	vline_up (row + height, col+51, triangle_height(51) + 1, fg_col);
	
	for (i=1; i<51; i++){
		draw_pixel(row+height, col + i, fg_col); 	/* The bottom pixels */
		draw_pixel(row+height - triangle_height(i) - 1, col + i, fg_col); 	/* 1 more than normal height */
	};	
};


/* Fill the complete visible LCD RAM with a specified byte value 
	The inner loop is executed 20 * 128 = 2560 times. This should be well under 10 ms. TODO simply measure it.
*/
void lcd_fill(unsigned char f){
	unsigned char col,page;
	for(page=0; page<20; page++) {
		lcd_set_page(page);
		lcd_set_coladr(0);
		
		for(col=0; col<128; col++){
			LCD_DATA = f;
			LCD_DATA = f;
		}
	}
}


/* This command switches between normal and upside-down mode */
static void 
lcd_set(unsigned char s) {
	if(s == 0)
	{
		LCD_CMD = 0xA0;		// set segment remap (00H mapped to seg0)
		LCD_CMD = 0xC8;		// set com output scan direction (remapped mode)
	}
	else
	{
		LCD_CMD = 0xA1;		// set segment remap (7FH mapped to seg0)
		LCD_CMD = 0xC0;		// set com output scan direction (normal mode)
	}
}

void 
lcd_enable(unsigned char e){
	if(e) {
		LCD_CMD = 0xAF;		// set display on
	} else {
		LCD_CMD = 0xAE;		// set display off
	};
}

void 
lcd_init(unsigned char s) {
	LCD_CMD = 0xE1;		// exit power save mode
	LCD_CMD = 0xE2;		// software reset
	delay(100000);		// around 10 ms
	LCD_CMD = 0xAB;		// start internal oscillator
	LCD_CMD = 0x27;		// set internal regulator resistor ratio (8.1)
	LCD_CMD = 0x81;		// volume cmd
	LCD_CMD = 0x3A; 	// volume cmd value
	LCD_CMD = 0x65;		// set DC-DC converter factor (4x)
	LCD_CMD = 0x60;		// set upper window corner ax cmd
	LCD_CMD = 0x1C;		// set upper window corner ax value
	LCD_CMD = 0x61;		// set upper window corner ay cmd
	LCD_CMD = 0x0A;		// set upper window corner ay value
	LCD_CMD = 0x62;		// set lower window corner bx cmd
	LCD_CMD = 0x75;		// set lower window corner bx value
	LCD_CMD = 0x63;		// set lower window corner by cmd
	LCD_CMD = 0x81;		// set lower window corner by value
	LCD_CMD = 0x90;		// set PWM and FRC (4-frames)
	LCD_CMD = 0x88;		// set white mode frame  2nd and 1st cmd
	LCD_CMD = 0x00;		// set white mode frame  2nd and 1st value
	LCD_CMD = 0x89;		// set white mode frame  4th and 3rd cmd
	LCD_CMD = 0x00;		// set white mode frame  4th and 3rd value
	
	LCD_CMD = 0x8A;		// set light gray mode frame  2nd and 1st cmd
//	LCD_CMD = 0x36;		// set light gray mode frame  2nd and 1st value
	LCD_CMD = 0x33;

	LCD_CMD = 0x8B;		// set light gray mode frame  4th and 3rd cmd
	LCD_CMD = 0x63;		// set light gray mode frame  4th and 3rd vakue
//	LCD_CMD = 0x33;
	
	LCD_CMD = 0x8C;		// set dark gray mode frame  2nd and 1st cmd
	LCD_CMD = 0x67;		// set dark gray mode frame  2nd and 1st value
	LCD_CMD = 0x8D;		// set dark gray mode frame  4th and 3rd cmd
	LCD_CMD = 0x76;		// set dark gray mode frame  4th and 3rd vakue
	LCD_CMD = 0x8E;		// set black mode frame  2nd and 1st cmd
	LCD_CMD = 0xAC;		// set black mode frame  2nd and 1st value
	LCD_CMD = 0x8F;		// set black mode frame  4th and 3rd cmd
	LCD_CMD = 0xCA;		// set black mode frame  4th and 3rd value
	LCD_CMD = 0x66;		// set DC-DC converter factor (5x)
	lcd_set(s);

//	LCD_CMD = 0x2E;		// set power control register (boost on, reg on, buffer off)

//	delay(100000);		// around 10 ms	

	LCD_CMD = 0x2F;		// set power control register (boost on, reg on, buffer on)
	LCD_CMD = 0xA4;		// set entire display on, normal mode)
	LCD_CMD = 0xA6;		// set normal display mode, inverse = 0xA7

	/* Stubs to call fastcode routines from normal code */
	write_buf = _write_buf;
	draw_block = _draw_block;
	draw_hline = _draw_hline;
	draw_vline = _draw_vline;
	do_scroll = _do_scroll;
	write_pattern = _write_pattern;
	
	lcd_fill(0x00);

	LCD_CMD = 0xAF;		// set display on
}
