/*
    global.h

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

#ifndef GLOBAL_H
#define GLOBAL_H

#define VERSION "1.2.0"

/* This device address is used for radio communication.
	Betty and SCART-Adapter must have equal addresses  !
*/
#define DEVICE_ADDRESS	0x01

// Used by "pt.h"
// selects which kind of pt implementation we use
#define LC_INCLUDE "lc-addrlabels.h"

#include "pt.h"

#define TRACE

/* included here so that all routines have access to debug_out */
#include "serial.h"


/* Frequency of PCLK in Hz 
		Here: 15 MHz
		When processor speed is changed, make sure to keep PCLK constant! 
*/
#define PCLK 15000000

typedef unsigned char BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;


// TODO use only c99 types here
typedef unsigned char uint8;
typedef unsigned char uint8_t;
typedef unsigned short uint16;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long uint32;

typedef signed char int8_t;

/* Structure used to give format values to snprintf()
	Very specialized, just 2 integers and 1 string
	convenient for exec_action
*/
typedef struct format_values {
	int arg1;
	int arg2;
	char *str; 
} FORMAT_VALUES;

#define false	0
#define FALSE	0
#define true	1
#define TRUE	1

#define NULL	0

#define	SPEED_30	0
#define	SPEED_60	1

/* P0.4 directly controls the LCD backlight LEDs. It can be PWMed to dim the light. */
#define BACKLIGHT_PWM_PIN (1<<4)
/* P0.11 is 1, if sound output is enabled and 0 otherwise */
#define SOUND_ENABLE_PIN (1<<11)

#define EINT0 (1<<0)
#define EINT1 (1<<1)
#define EINT2 (1<<2)
#define EINT3 (1<<3)

// PCONP bits
#define PCSPI1	10
#define PCSSP	21

#define EOT 0x04

#define CACHE_LIM	33
#define CACHE_MAX	(CACHE_LIM -1)

#define CACHE_ENTRY_SIZE 64
/* Max length of a string stored in our cache (final 0 is not counted) */
#define CACHE_ENTRY_LEN (CACHE_ENTRY_SIZE - 1)

/* 
	This structure is an indexed cache of consecutive string values.
	It is a ring buffer.

	We associate a numeric positional index with each string (called pos) 
	pos starts at 0 (for the first string we can possibly store)
	and each following string has its pos incremented by 1 
	pos == -1 means information is not yet known
	pos == -2 means information is not available (non-existant)
	We always store only a small portion of all strings.
	The first string that we currently have in our cache is given by the variable first_pos.
	The variable first_idx gives the index into our array that corresponds to first_pos.
	The constant CACHE_LIM gives the maximum total number of entries in our cache.

	The real strings are stored in cache_entry[i].cache_str. 

*/

#define NOT_KNOWN	-1
#define NOT_AVAIL	-2

struct cache_entry {
	int pos;		// positional id of this entry, or NOT_KNOWN or NOT_AVAIL
	char cache_str[CACHE_ENTRY_SIZE];	// the cached string
};

typedef struct str_cache {
	struct cache_entry entry[CACHE_LIM];
	int first_pos;	// positional id of first cached information
	int first_idx;	// index of cache-entry corresponding to first_pos.
	int pos_lim;	// positional index of the last available info + 1 (-1 means unknown) 
} STR_CACHE;

/* 
	pos of last string in our cache 
	This changes every time that first.pos changes
*/
#define last_pos(pcache) ((pcache)->first_pos + CACHE_MAX)


int max(int a, int b);
int min(int a, int b);
int abs(int a);
int strlen( char *s);
int strstart( char *s1, char *s2);
int strlcpy(char *dst, const char *src, int size);
int strlcat(char *dst, const char *src, int size);
int strn_cpy_cmp(char *str1, char *str2, int n, int *length);
int atoi(const char *s);
int str_del(char *s, int pos);
char *strchr(const char *s, int c);
char *get_hex_digits(unsigned long v, char *s);
char *get_digits(unsigned int val, char *s, int z);

void sec2hms(char *s, int sec);
void rand_seed(int s);
int rand(void);

char *cache_info(STR_CACHE *pc, int pos);
void cache_init(STR_CACHE *pc);
void cache_unknown(STR_CACHE *pc, int pos);
void cache_clear(STR_CACHE *pc, int pos);
void cache_store(STR_CACHE *pc, int pos, char *content);
int cache_find_unknown(STR_CACHE *pc);
void cache_range_set(STR_CACHE *pc, int start_pos, int end_pos);
void cache_set_limit(STR_CACHE *pc, int limit);

#endif
