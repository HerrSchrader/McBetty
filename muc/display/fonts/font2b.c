/*
 * font2b.c - convert linux console font data to betty lcd ram layout
 *
 * author: based loosely on a tool from hackbard@hackdaworld.org
 *
 * usage: font2b >font_output.h
 *
 * You have to compile this file with either SMOOTH, PEARL, MISC (see comments!)
 * don't forget to rebuild.
 * 
 * ./font2b > font_name.h
 *
 *
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define PSF2_MAGIC0     0x72
#define PSF2_MAGIC1     0xb5
#define PSF2_MAGIC2     0x4a
#define PSF2_MAGIC3     0x86

typedef unsigned int u32;
typedef unsigned char u8;

/* One of these defines must be given by compile time option */
//#define PEARL
//#define SMOOTH
//#define MISC

/* --------------------------- The following lines must be adjusted for each font ----------------------- */ 
/* We include a .inc file as produced when we run a psf file through psf2inc from pdftools. */


const u8 font_data[]={ 
// put your font data here!
#ifdef SMOOTH
#include "smooth.inc"
#endif

#ifdef PEARL
//#include "pearl-8x8.inc"
#include "pearl.inc"
#endif

#ifdef MISC
#include "misc19.inc"
#endif
};

#ifdef SMOOTH

#define FONT_NAME "smooth_font"

/* Width of a space glyph in this font */
#define SPACE_WIDTH	3

/* All numerals (0..9) should have the same width. Smaller numberals are padded with 0's. */
#define NUMERAL_WIDTH 5

/* Some fonts do not really use the top rows.
	We start using the rows beginnig from TOP_ROW
*/
#define TOP_ROW 2

/* Some fonts do not really use the bottom rows.
	We ignore this many rows at the bottom
*/
#define IGNORE_BOTTOM 0

/* All characters in the original font below this are assumed to be blank,
	because they are non-printable chararcters.
*/
#define FIRST_CHAR 32

/* Some characters may be not encoded in this specific font. 
	We substitute characters with a width of 0, which need no space.
*/
#define FIRST_MISS	0x80
#define LAST_MISS	0x9F

/* All characters above this in the original font are ignored.
	We do not want to store more than this number of characters + 1 !
*/
#define MAX_CHAR 255 

#endif

#ifdef PEARL
#define FONT_NAME "pearl_font"

/* Width of a space glyph in this font */
#define SPACE_WIDTH	5

/* All numerals (0..9) should have the same width. Smaller numberals are padded with 0's. */
#define NUMERAL_WIDTH 7

/* Some fonts do not really use the top rows.
	We start using the rows beginnig from TOP_ROW
*/
#define TOP_ROW 0

/* Some fonts do not really use the bottom rows.
	We ignore this many rows at the bottom
*/
#define IGNORE_BOTTOM 0

/* All characters in the original font below this are assumed to be blank,
	because they are non-printable chararcters.
*/
#define FIRST_CHAR 32

/* Some characters may be not encoded in this specific font. 
	We substitute characters with a width of 0, which need no space.
*/
#define FIRST_MISS	256
#define LAST_MISS	255

/* All characters above this in the original font are ignored.
	We do not want to store more than this number of characters + 1 !
*/
#define MAX_CHAR 179 

#endif 

#ifdef MISC

#define FONT_NAME "misc_font"

/* Width of a space glyph in this font */
#define SPACE_WIDTH	6

/* All numerals (0..9) should have the same width. Smaller numberals are padded with 0's. */
#define NUMERAL_WIDTH 8

/* Some fonts do not really use the top rows.
	We start using the rows beginnig from TOP_ROW
*/
#define TOP_ROW 2

/* Some fonts do not really use the bottom rows.
	We ignore this many rows at the bottom
*/
#define IGNORE_BOTTOM 2

/* All characters in the original font below this are assumed to be blank,
	because they are non-printable chararcters, even though they are present in the font data.
	They will be encoded with a width of 0.
*/
#define FIRST_CHAR 32

/* Some characters may be not encoded in this specific font. 
	We substitute characters with a width of 0, which need no space.
*/
#define FIRST_MISS	0x80
#define LAST_MISS	0x9F

/* All characters above this in the original font are ignored.
	We do not want to store more than this number of characters + 1 !
*/
#define MAX_CHAR 255 

#endif

/* --------------------------- End of font specific lines --------------------------------------------- */ 

/* Do we want to show each glyph on stderr ? */
#define SHOW_GLYPH 0

#define SHOW_OUTPUT 0

#define SHOW_BOTTOM 0

#define SHOW_TOP 0

int makeint(int index){
	return font_data[index] + (font_data[index+1] << 8)  + (font_data[index+2] << 16) + (font_data[index+3] << 24);
};


int main(int argc,char **argv) {
	int glyph,col,row, idx;
	
	int headersize;		// offset of character data from start of file
	int length;			// number of characters in the font
	int	charsize;		// number of bytes for each character
	int height;			// height of each character
	int width;			// width of each character
	
	int bytes_per_row;	// number of bytes for each row. (input format)
	int bytes_per_col;	// (output format)
	
	int first_col;
	int last_col;
	
	u32 colbuf;
	u8 tmp;
	
	int max_width=0;
	
	unsigned char outbyte;

	u8 bittable[32][32];
	
	/* The real width of each glyph */
	int real_width[MAX_CHAR+1];
	
	/* Check the magic number for PSF Version 2 */
	if ( (font_data[0] != PSF2_MAGIC0) || (font_data[1] != PSF2_MAGIC1) || (font_data[2] != PSF2_MAGIC2) || (font_data[3] != PSF2_MAGIC3) ){
		fprintf(stderr, "Not a PSF2 file!\n");
		exit(20);
	};
	
	headersize = makeint(8);
	length = makeint(16);
	charsize = makeint(20);
	height = makeint(24);
	width = makeint(28);
	
	fprintf(stderr, "Headersize = %d\nLength=%d\nCharsize=%d\nHeight=%d\nWidth=%d\n\n", headersize, length, charsize, height, width);
	
	if (height > 32){
		fprintf(stderr, "Can't handle height > 32, sorry.\n");
		exit(20);
	};
	
	if (width > 32){
		fprintf(stderr, "Can't handle width > 32, sorry.\n");
		exit(20);
	};	
	bytes_per_row = (width + 7 ) / 8;

	bytes_per_col = ((height - TOP_ROW - IGNORE_BOTTOM + 7) / 8) ;
	fprintf(stderr, "Bytes per column = %d\n", bytes_per_col);
	
	printf("const unsigned char %s_bits[]={\n", FONT_NAME);

	for(idx=0,glyph=0; glyph<=MAX_CHAR; glyph++, idx++) {

		for (row=0; row<32; row++)
			for(col=0; col<32; col++)
				bittable[row][col] = 0;
		
		if ( (glyph >= FIRST_MISS) && (glyph <= LAST_MISS) ){
			real_width[glyph] = 0;
			idx--;
			continue;
		};
			
		if (glyph >= FIRST_CHAR){
			for (row=0; row<height-IGNORE_BOTTOM; row++)
				for (col=0; col<width; col++)
					bittable[row][col] = font_data[headersize+idx*charsize+row*bytes_per_row+(col/8)] & (1<<(7- (col%8)));
		} else {
			real_width[glyph] = 0;
			continue;
		};
		
		/* Determine the actual width of this glyph */
		/* Find the first column which is used */
		for (col=0; col<width; col++){
			for (row=0; row<height; row++)
				if (bittable[row][col]) 
					break;
			if (row < height)
				break;
		};
		first_col = col;
		
		/* Find the last column which is used */
		for (col=width-1; col>=0; col--){
			for (row=0; row<height; row++)
				if (bittable[row][col]) 
					break;
			if (row < height)
				break;
		};		
		last_col = col;
		
		if ( (first_col >= width) || (last_col < 0) ) {
			fprintf(stderr,"%02x: Width is zero, maybe space character\n", glyph);
			real_width[glyph] = SPACE_WIDTH;
			first_col = 0; last_col = SPACE_WIDTH - 1;
		} else
			real_width[glyph] = last_col - first_col + 1;
//		fprintf(stderr,"Real width = %d\n", real_width[glyph]);

		if ( (glyph >= '0') && (glyph <= '9')){
			real_width[glyph] = NUMERAL_WIDTH;
			first_col = 0; last_col = NUMERAL_WIDTH - 1;
		};
			
		if (real_width[glyph] > max_width)
			max_width=real_width[glyph];
		
#if SHOW_GLYPH		
		fprintf(stderr,"\n");
		for (row=0; row<height; row++){
			for (col=0; col<real_width[glyph]; col++)
				fprintf(stderr, "%c",(bittable[row][col] ? 'X' : '.'));
			fprintf(stderr, "\n");
		};
#endif
		
#if SHOW_TOP
		/* Show all glyphs which use the top row */
		for (col=0; col<real_width[glyph]; col++){
			if (bittable[TOP_ROW][col]) 
					break;
		};

		if (col < real_width[glyph]){	// we did find a pixel in row TOP_ROW
			fprintf(stderr, "\n");
			for (row=0; row<height; row++){
				for (col=0; col<real_width[glyph]; col++)
					fprintf(stderr, "%c",(bittable[row][col] ? 'X' : '.'));
				fprintf(stderr, "\n");
			};	
			
		};
#endif
		
#if SHOW_BOTTOM
		/* Show all glyphs which use the last row */
		for (col=0; col<real_width[glyph]; col++){
			if (bittable[height-IGNORE_BOTTOM-1][col]) 
					break;
		};

		if (col < real_width[glyph]){	// we did find a pixel in row (height-IGNORE_BOTTOM-1)
				fprintf(stderr, "\n");
				for (row=0; row<height; row++){
				for (col=0; col<real_width[glyph]; col++)
					fprintf(stderr, "%c",(bittable[row][col] ? 'X' : '.'));
				fprintf(stderr, "\n");
			};	
		};
#endif	
		/* Now output the gyplh in column/row order. Row 0 must be Least Significant Bit */
		for (col = first_col; col<=last_col; col++){
				
			colbuf=0;
			for (row=TOP_ROW; row < height-IGNORE_BOTTOM; row++){
				colbuf += (bittable[row][col] ? 1 : 0) << (row-TOP_ROW);
#if SHOW_OUTPUT				
				fprintf(stderr, "%c",(bittable[row][col] ? 'X' : '.'));
#endif
			};
#if SHOW_OUTPUT	
			fprintf(stderr,"\n");
#endif			
			/* Output bytes, least significant first */
			printf("0x%02x", colbuf & 0xff);
			if (bytes_per_col >= 2) printf(", 0x%02x", (colbuf >> 8) & 0xff);
			if (bytes_per_col >= 3) printf(", 0x%02x", (colbuf >> 16) & 0xff);
			if (bytes_per_col >= 4) printf(", 0x%02x", (colbuf >> 24) & 0xff);
				
			if ((glyph != MAX_CHAR) || (col != last_col) )
					 printf(", ");
		};
	
		/* print the font number as a comment */
		printf("\t/* %3d 0x%02x", glyph, glyph);
			if((glyph>0x1f)&(glyph<0x7f))
				printf(" %c */\n", glyph);
			else
				printf("   */\n");
	};
	printf("};\n\n");

	printf("const unsigned char %s_info[]={\n", FONT_NAME);
	for (glyph = 0; glyph <= MAX_CHAR; glyph++){
		printf("%d", real_width[glyph]);
		if ( (glyph + 1) <= MAX_CHAR )
			printf(", ");
		if ( ( (glyph+1) & 0x0f) == 0 )
			printf("\n");
	};
	
	printf("};\n\n");
	
	fprintf(stderr, "Max Width = %d\n", max_width);
	
	printf("const u_int16_t %s_cpos[]={\n", FONT_NAME);
	for (glyph = 0; glyph <= MAX_CHAR; glyph++){
		int sum = 0;
		int i;
		for (i=0; i<glyph; i++)
			sum += real_width[i] * bytes_per_col;
		printf("%d", sum);
		if ( (glyph + 1) <= MAX_CHAR )
			printf(", ");
		if ( ( (glyph+1) & 0x0f) == 0 )
			printf("\n");
	};
	
	printf("};\n");
	
	printf("const unsigned int %s_height=%d;\n", FONT_NAME, height - TOP_ROW- IGNORE_BOTTOM);
	printf("const unsigned int %s_maxchar=%d;\n", FONT_NAME, MAX_CHAR);
	
	return 0;
}
