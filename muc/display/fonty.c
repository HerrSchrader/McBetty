/*
    fonty.c - drawing text & font handling
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
#include "kernel.h"
#include "lcd.h"

#include "fonty.h"

#include <sys/types.h>

#include "fonts/pearl_font.h"
#include "fonts/smooth_font.h"
#include "fonts/misc_font.h"



/* The following variables are set according to font information in include files. */
uint8 font_height;
static const unsigned char *font_bits, *font_info; 
static const uint16_t *char_pos;
static int bytes_per_col, max_char;

static int ic_space;			// inter character space (depends on font)
static enum FONT activefont;		// the currently active font


void 
init_font(){
	activefont = NO_FONT;	/* not yet set ! */
};

/* Set the current font.
	This is a reasonably fast routine, but it is better not called before every single character that is drawn.
	We do a little cache trick here and set the new font only if it has changed from before.
	The variable activefont must be initialized to NO_FONT by init_font() before using any routine here.
*/
void 
set_font(enum FONT f)
{	
	if (f == activefont)
		return;			// already correct
	
	switch(f) {

		case BIGFONT:
			activefont = BIGFONT;
			font_height = misc_font_height;
			font_bits = misc_font_bits;
			font_info = misc_font_info;
			char_pos = misc_font_cpos;
			bytes_per_col = 2;
			max_char = misc_font_maxchar;
			ic_space=2;
			break;
		
		case MEDIUMFONT:
			activefont = MEDIUMFONT;
			font_height = smooth_font_height;
			font_bits = smooth_font_bits;
			font_info = smooth_font_info;
			char_pos = smooth_font_cpos;
			bytes_per_col = 2;	
			max_char = smooth_font_maxchar;
			ic_space = 1;
			break;
		
		default:			// font not implemented, use the smallest
			
		case SMALLFONT:
			activefont = SMALLFONT;
			font_height = pearl_font_height;
			font_bits = pearl_font_bits;
			font_info = pearl_font_info;
			char_pos = pearl_font_cpos;
			bytes_per_col = 1;	
			max_char = pearl_font_maxchar;
			ic_space = 1;
			break;
	}
}

/* The fonts used are bitmap fonts. Each character can have a different width. The height of all characters is supposed to be 
	the same.
	They are derived from the Linux Font Project (in bdf or pcf format) or from linux console driver fonts.
	
	The fonts are included as C include files.
	
	The width of each character is counted in pixels and called bit_width.
	The fonts are stored with a fixed height between 1 and 16 pixels.
	The number of bytes needed per column is called bytes_per_col.
	Each character is stored as bytes_per_col * bit_width consecutive bytes. The first bytes_per_col bytes represent the first
	column of the first character, the next the second column and so on.
	If the font height is not a multiple of 8, the unneeded bits are in the MSB of the last byte per column and set to 0 
*/

/* The font format uses a compressed format to store the width of each character in the current font 
	Each byte stores 4 bits of width information, so we have a width from 0 to 15 pixels.
*/
// This gives the width of the specified character in the current font
#define char_width(c) (font_info[ (unsigned char) c])


/* 
	Given a character position, returns the corresponding start column at which the character would be drawn.
	Is used by cursor drawing routine.
*/
uint8_t
char_start_col(char *txt, int pos){ 
	unsigned char ch;
	unsigned int i, w = 0, cpos;
	
	for (i = 0; i < pos; i++){
		ch = txt[i];
		
		if (ch <= max_char){ 
			cpos = char_pos[ch];		// Start of glyph bits in font_bits[]
			w += char_width(ch) + ic_space;
		};
	};
	return w;
};

/* The most important routine here!
	Draw a single character 
	 
	width tells us how many display columns we can use at most to draw the character
	
	returns 0 iff the character did not fit completely (with inter character space) into the given width
	if it did fit, returns the number of columns drawn (with inter character space) 
	
*/

uint8_t 
draw_char(uint8_t start_row, uint8_t start_col, unsigned char ch, uint8_t fg_col, uint8_t bg_col, uint8_t width) {
	unsigned int w;
	unsigned int i, cpos, cnt;
	uint8 res = 0;
	
	// NOTE maybe print a ? or something
	if (ch > max_char) return 1;	/* We print nothing, character not encoded */
	
	cpos = char_pos[ch];		// Start of glyph bits in font_bits[]
	
	w = char_width(ch);		// Width of the character without inter character space
	if (w > width) w = width;	// w is now min(char_width(ch), width)

	// draw w columns into drawbuf
	// each column is copied (with correct color) to drawbuf directly from font_bits
	for (cnt = 0; cnt < w; cnt++, cpos+=bytes_per_col)
		store_buf(cnt, font_bits[cpos] | (font_bits[cpos+1] << 8), fg_col, bg_col);
	if ( (width - cnt) >= ic_space ){
		for (i=0; i<ic_space; i++)
			store_buf(cnt++, 0, fg_col, bg_col);		// inter character space
		res = cnt;
	};
	write_buf(start_row, start_col, cnt, font_height);
	return res;	
}


static int
is_whitespace(char c){
	return ( (' ' == c) || ('\n' == c) );
};

/*
	Draw the next word in s on screen. We stop at the next white space character.

	We return the number of characters in the word.
	If the word did not fit in the given width, we return 0.
	
	If the variable cnt is not NULL, we do NOT draw the text, but simply count the width in pixels
	that we would draw and store the result in *cnt.
	If the word does not fit completely, cnt is returned as greater than width.
	If we are already at a whitespace character, we draw only this character.
*/
static int
draw_text_word(char *s, int start_row, int start_col, int width, int fg_col, int bg_col, int *cnt){
	int c=0;	
	char ch;	
	int n=0;
	
	while ( (ch = *s++) ){
		
		/* can we put the next character on the line ? */
		if ( c + char_width(ch) <= width ){
			
			/* draw this character */
			if (NULL == cnt)  draw_char(start_row, start_col + c, ch, fg_col, bg_col, width - c );
			
			c += (char_width(ch) + ic_space);
			n++;
			
			if ( ('\0' == *s) || is_whitespace(*s) ){
				/* This word did fit completely */
				break;
			}
		} else {
			/* This word did not fit completely */
			c = width + 1;
			n = 0;
			break;
		};
	};
	
	if (NULL != cnt)
		*cnt = c;
	return n;
};

/*
	Draw the next line in s on screen. We wrap at word boundaries.
	We return a pointer to the next character in s after the current line.
	
	If the variable cnt is not NULL, we do NOT draw the text, but simply count the width in pixels
	that we would draw and store the result in *cnt.
*/
static char * 
draw_text_line(char *s, int start_row, int start_col, int width, int fg_col, int bg_col, int *cnt){
	int c = 0;
	int cnt_wid;
	int n;
	
	while ( *s ){
		if ('\n' == *s) 		// newline character
			break;
		
		/* count how many pixels we will draw with the current word and remember start of next word */
		n = draw_text_word(s, start_row, start_col + c, width - c, fg_col, bg_col, &cnt_wid);
				
		/* can we put the current word on the line ? */
		if ( (c + cnt_wid) <= width ){
			
			/* shall we really draw this word ? */
			if (NULL == cnt)
				draw_text_word(s, start_row, start_col + c, width - c, fg_col, bg_col, NULL);
		
			c += cnt_wid;
			s = s + n;
		} else {
			/* word does not fit completely on this line, finished */
			break;
		};
	};
	
	if (cnt != NULL) *cnt = c;
	
	return s;
};


/* Draw a text into a given rectangle 
	We wrap the text at white space boundaries	
	If the variable cnt is TRUE, we do NOT draw the text, but simply count the height in pixels
	that we would draw.
	If lftadj is <>0, the text is not centered horizontally, but left adjusted
*/
int
draw_text_space (char *s, int start_row, int start_col, int width, int height, int fg_col, int bg_col, int cnt, int lftadj){
	int r=0;
	int cnt_wid = 0;
	char *s2;
	int offset;
	
	if (font_height > height) return 0;		// No vertical space to draw anything
	
	while (*s ){

		/* count how many pixels we will draw on the current line and remember start of line break */
		s2 = draw_text_line(s, start_row + r, start_col, width, fg_col, bg_col, &cnt_wid);
		
		/* Shall we really draw ? */
		if (0 == cnt){
			/* If this is less than the line width, center the text horizontally */
			if ( (cnt_wid <= width) && !lftadj)
				offset = (width - cnt_wid) / 2;
			else
				offset = 0;
		
			/* And now draw for real (and maybe centered) */
			draw_text_line(s, start_row + r, start_col + offset, width - offset, fg_col, bg_col, NULL);
		};
		
		/* Go to the beginning of next line */
		s = s2;
		if (*s) s++;		// this is white space, skip it.
		
		r += font_height;	/* We have drawn a line font_height pixels high */
		cnt_wid = 0;		/* We have not drawn anything on this line */
		
		/* is there enough room vertically for another line */
		if ((r + 2 + font_height) > height)
			break;
		r += 2;			/* inter line space */
	};
	
	if (cnt_wid > 0) r += font_height;
	
	return r;
};




/* 
	Scroll a text horizontally. 
	Number of rows depend on font height.
*/
void
scroll(int start_row, int start_col, int width, int offset){
	lcd_scroll(start_row, start_col, width, font_height, offset);
};

