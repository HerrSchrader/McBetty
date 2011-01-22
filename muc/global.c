/*
    global.c - Useful functions

	H. Raap 2010

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

int max(int a, int b){
	return ( (a > b) ? a : b );
}

int min(int a, int b){
	return ( (a < b) ? a : b );
}

int abs(int a){
	if (a >= 0) return a;
	else return -a;
};


/* Returns TRUE iff string1 starts with string2 */
int strstart( char *s1, char *s2){ 
	while ( (*s2 != 0) && (*s1 != 0) ){
		if (*s1 != *s2) return 0; 
		s1++; s2++;
	};
	return (*s2 == 0);
};

/* Standard C string length */
int strlen( char *s){
	int cnt;
	for (cnt = 0; *s++; cnt++);
	return cnt;
};

/* Returns a pointer to the first occurrence of the character c in string s 
	Just like the standard c function
*/
char *
strchr(const char *s, int c){
	while (*s){
		if (*s == c)
			return (char *) s;
		s++;
	};
	return NULL;
};

/* BSD string length copy. The destination string is always 0 terminated ! 
	The resulting string uses at most n bytes (incl. trailing 0) 
	Returns strlen(src).
*/
int
strlcpy(char *dst, const char *src, int size){
	int i;

	for (i=0; i < (size -1); i++){
		if (0 == (*(dst++) = *(src++)) )		// we have just copied the NUL byte.
			return i;			// this is the length of src
	};
	if (size != 0)
		*dst = 0;				// i == max(0, size-1), last byte of destination string, must be terminated.
	while (* (src++) ) i++;		// add remaining bytes of src
	return i;
};

/* BSD string length concatenation.
	The resulting string uses at most n bytes (incl. terminating 0) and is always 0 terminated
	Returns strlen(dst) + strlen(src) 
*/
int
strlcat(char *dst, const char *src, int size){
	int i = 0;
	
	/* Find the current end of dst */
	for (i=0; i< (size - 1); i++, dst++){
		if (0 == *dst)
			break;
	};
								// now i is len(dst)
	for (; i < (size - 1); i++){
		if (0 == (*(dst++) = *(src++)) )		// we have just copied the NUL byte.
			return i;			// this is the length of src + length of dst
	};
	if (size != 0)
		*dst = 0;
	
	while (* (src++) ) i++;		// add remaining bytes of src
	return i;
};


/* Delete the character at position pos in the string 
	Deletes in place, original string is changed
	Returns new strlen(s). 
*/
int
str_del(char *s, int pos){
	int i;
	for (i=0; s[i]; i++)
		if (i == pos) break;
	for (; s[i]; i++)
		s[i] = s[i+1];
	return i;
};

/* String copy and compare.
	Copies str2 to str1.
	Returns 1 if both strings were identical.
	Else returns 0.
	The length of the resulting string is <= n bytes (incl terminating 0).
	The resulting string is guaranteed to be 0 terminated.
	The length of the resulting string is stored in *length:
*/
int
strn_cpy_cmp(char *dest, char *str2, int n, int *length){
	int ident = 1;
	int len = 0;

	do {	
		if (*dest != *str2){
			*dest = *str2;
			ident = 0;
		};
		dest++;
	} while ( *(str2++) && ( ++len < n ) );
	/* Now *(str2-1) is 0 or len == n 
		If *(str2-1) is 0 we have just terminated the string dest and we are finished.
		if (len == n) we have found no terminating 0 in str2. Write 0 to dest and return not identical. 
	*/
	if (len == n) {
		*(dest-1) = '\0';
		len--;
		ident = 0;
	};
	*length = len;
	return ident;	
};


/* Simple atoi version. 
	A string is converted to a positive integer.
*/	
int atoi(const char *s){
	int n=0;
	
	while ( (*s >= '0') && (*s <= '9') ){
		n = n*10 + (*s - '0');
		s++;
	};
	return n;
};


static const char hval[16] = "0123456789ABCDEF";

/* Stores the ASCII representation of the hex digits of val into character array s.
	Returns pointer to s.
	Generates exactly 2 digits
*/
char *
get_hex_digits(unsigned long v, char *s){
	s[0] = hval[ (v & 0xf0) >>4];
	s[1] = hval[ (v & 0x0f) ];
	return s;
};

/* Division by repeated subtraction
	Computes (*pu_longval % tval)
	and leaves remainder in *pu_longval
*/
static int 
div(unsigned int tval, unsigned int *pu_longval){
	int count = 0;
	while(*pu_longval >= tval){
		count++;
		*pu_longval -= tval;
	}
	return count;
};

/* Divide val by powers of 10 and store digits in *s 
	Gives exactly 10 digits.
	s will be null terminated
	returns a pointer to the first printable character in s
	if z is 1, all characters are printable.
	if z is 0, leading zeroes are suppressed
*/
char *
get_digits(unsigned int val, char *s, int z){
	unsigned int u_longval = val;
	char *s1 = s;
	int pos;
	
	*s++ = '0' + div(1000000000, &u_longval);
	*s++ = '0' + div(100000000, &u_longval);
	*s++ = '0' + div(10000000, &u_longval);
	*s++ = '0' + div(1000000, &u_longval);
	*s++ = '0' + div(100000, &u_longval);
	*s++ = '0' + div(10000, &u_longval);
	*s++ = '0' + div(1000, &u_longval);
	*s++ = '0' + div(100, &u_longval);
	*s++ = '0' + div(10, &u_longval);
	*s++ = '0' + u_longval;
	*s = 0;
	
	pos = 0;
	/* shall we omit leading zeroes? */
	if (z==0){
		for (pos=0; pos < 9; pos++)
			if (s1[pos] != '0')
				break; 
	};
	return &(s1[pos]);
}


/* Store a time value given in seconds in a string with exactly 5 characters.
	Leading 0's in the hour will be replaced by spaces.
	max. 99 hours
	s will be 0 terminated
*/
void
sec2hms(char *s, int sec){
	unsigned int rest = sec;
	
	if (sec < 0){
		strlcpy(s, "--:--", 20);
		return;
	};
	
	if (sec >= 3600){		// an hour or more ? 
		*s = '0' + div(36000, &rest);
		if (*s == '0') 
			*s = ' ';
		s++;
	 
		*s++ = '0' + div(3600, &rest); 
		*s++ = 'h';
	};

	/* rest is between 0 and 60 minutes */
	*s++ = '0' + div(600, &rest);
	*s++ = '0' + div(60, &rest);

	if (sec < 3600){		// less than an hour? show the seconds
		/* rest is between 0 and 59 seconds */
		*s++ = ':';
		*s++ = '0' + div(10, &rest);
		*s++ = '0' + rest;
	}
	*s = '\0';
};


/* Random number generator. Returns random number between 0 and 65535.
	 extracted from mother-of-all */
static int seed;

void 
rand_seed(int s){
	seed = s;
};

int rand(){
	seed=30903*(seed&65535)+(seed>>16); 
	return(seed&65535);
};


/* ==================================== String cache functions ========================================= */

/* Given a string pos, we return the index of the cache entry corresponding to this number.
	If the number is outside of our list, we return -1.
*/
static int
cache_index(STR_CACHE *pc, int pos){
	int idx;

	if ( (pos < pc->first_pos) || (pos > last_pos(pc)) )
		return -1;
		
	idx = pc->first_idx + pos - pc->first_pos;
	if (idx >= CACHE_LIM)
		idx -= CACHE_LIM;	
	return idx;
};

/* We are given a string pos. We return the corresponding string 
	or NULL if the pos is outside of our cache.
	NULL might also mean we do not have information about that string.
*/
static char *
cache_string(STR_CACHE *pc, int pos){
	int i = cache_index(pc, pos);
	if (i < 0) return NULL;
	return pc->entry[i].cache_str;
};

static int
cache_pos(STR_CACHE *pc, int pos){
	int i = cache_index(pc, pos);
	if (i < 0) return NOT_KNOWN;
	return pc->entry[i].pos;
};

char *
cache_info(STR_CACHE *pc, int pos){
	int p = cache_pos(pc, pos);
	
	if (p == NOT_KNOWN)
		return "...";
	
	if (p == NOT_AVAIL)
		return "";
	return cache_string(pc, pos);	
};

	
static void inline 
entry_unknown(STR_CACHE *pc, int idx){
	pc->entry[idx].pos = NOT_KNOWN;
	pc->entry[idx].cache_str[0] = '\0';	
};

static void inline 
entry_not_avail(STR_CACHE *pc, int idx){
	pc->entry[idx].pos = NOT_AVAIL;
	pc->entry[idx].cache_str[0] = '\0';
};

static void inline
pos_unknown(STR_CACHE *pc, int pos){
	int idx;
	idx = cache_index(pc, pos);	
	if (idx >= 0){
		entry_unknown(pc, idx);
	}
}

static void inline
pos_not_avail (STR_CACHE *pc, int pos){
	int idx;
	idx = cache_index(pc, pos);	
	if (idx >= 0){
		entry_not_avail(pc, idx);
	}
}
	

/* 
	Given a string, store this info in our cache (if it fits) 
	If the string is NULL, pos is set to NOT_KNOWN and "" is stored 
	else the string is copied length-limited by CACHE_ENTRY_LEN
	and pos is set to pos
*/
void
cache_store(STR_CACHE *pc, int pos, char *content){
	int idx;
	
	idx = cache_index(pc, pos);
	if (idx < 0)
		return;
	if (NULL == content){
		entry_unknown(pc,idx);
	} else {
		strlcpy(pc->entry[idx].cache_str, content, CACHE_ENTRY_SIZE); 
		pc->entry[idx].pos = pos;
	};	
};

/* All the cache entries starting at pos are made unknown. */
void 
cache_unknown(STR_CACHE *pc, int pos){
	int p;
	
	if (pos > last_pos(pc)) 
		return;				/* not in our cache */
	
	pos = max(pc->first_pos, pos);		/* Need not start before begin of cache */

	for (p=pos; p <= last_pos(pc); p++)
		pos_unknown(pc, p);
};

#if 0
/* All the cache entries starting at pos are set to NOT_AVAIL. */
void 
cache_clear(STR_CACHE *pc, int pos){
	int p;
	
	if (pos > last_pos(pc)) 
		return;				/* not in our cache */
	
	pos = max(pc->first_pos, pos);		/* Need not start before begin of cache */

	for (p=pos; p <= last_pos(pc); p++)
		pos_not_avail(pc, p);
};
#endif

void
cache_init(STR_CACHE *pc){
	pc->first_idx = 0;
	pc->first_pos = 0;
	pc->pos_lim = -1;
	cache_unknown(pc, 0);
};
	
/* We are shifting the start of our tracklist up by 1 position.
	I.e. if the cache started at pos 17, it now starts at pos 18
	We are not changing most of the information in str[], only the start and end of the array.

	The last information is set to UNKNOWN or NOT_AVAIL depending on pos_lim
*/
static void
cache_inc_first(STR_CACHE *pc){
	pc->first_pos++;			// sets last_pos() also

	// the cache is a ring buffer, so the old first element will be the new last element
	if ( (pc->pos_lim >= 0) && (last_pos(pc) >= pc->pos_lim) )
		entry_not_avail(pc,pc->first_idx);
	else
		entry_unknown(pc,pc->first_idx);
	
	pc->first_idx++;
	if (pc->first_idx > CACHE_MAX)
		pc->first_idx = 0;
};

/* We are shifting the start of our cache down by 1 pos 
	I.e. if the cache started at pos 17, it now starts at pos 16
	We are not changing most of the information in cache, only the start (and end) of the array.
	We are setting the information of the new first info to NOT_KNOWN,, because we do not know it,
	except when it is above the absolute information limit, then it is NOT_AVAIL
*/
static void
cache_dec_first(STR_CACHE *pc){
	pc->first_pos--;
	pc->first_idx--;
	if (pc->first_idx < 0)
		pc->first_idx = CACHE_MAX;
	
	if ( (pc->pos_lim >= 0)	&& (pc->first_pos >= pc->pos_lim) )			// valid limit known and start is above it
		entry_not_avail(pc,pc->first_idx);
	else 
		entry_unknown(pc,pc->first_idx);
};

static void
cache_shift_up(STR_CACHE *pc, int diff){
	for (;diff > 0; diff --)
		cache_inc_first(pc);
};

static void
cache_shift_down(STR_CACHE *pc, int diff){
	for (;diff > 0; diff --)
		cache_dec_first(pc);
};

/* Returns the first unknown pos in our cache 
	or -1 if every pos is either known or not available.
*/
int
cache_find_unknown(STR_CACHE *pc){
	int pos;
	for (pos=pc->first_pos; pos <= last_pos(pc); pos++)
		if (cache_pos(pc, pos) == NOT_KNOWN)
			return pos;
	return -1;
}


/* 
	The cache containing track info has to follow the information that we show on screen.
	Here we tell the cache which positions we want to show,
	namely all infos between start_pos and end_pos inclusive.
	We adjust the positions given to us so that the start is >= 0
	and the end is below the total length of the information (if we know it)
	Also start must not be > total length of the information (if we know it)
	The last parameter is the absolut limit of the information,
	i.e. playlistlength if cache is tracklist cache
	and num_playlists if cache is playlist cache
	
	Returns the start_pos that we actually used.
	
	This routine is important. We ask the cache which positions are empty and 
	send commands to MPD to ask for that information.
	So the cache has to know which positions really make sense to fill,
	i.e. which positions actually exist and may be shown on screen.
	
*/
void
cache_range_set(STR_CACHE *pc, int start_pos, int end_pos){
	int p, margin, left_margin;
	
	// ideally we want to keep the needed info in the middle of our cache,
	// so that the user can scroll back and forth relatively fast without
	// reloading a lot of cache values.
	// We center the info in the cache like a text on a page
	
	// Check if the needed info fits completely within our cache
	if ( end_pos - start_pos + 1 <= CACHE_LIM ) {
		margin = CACHE_LIM - (end_pos - start_pos + 1);				// size of cache - size of needed info
		left_margin = margin / 2;
		start_pos = start_pos - left_margin;
	};
		
	// start < 0 does not make sense
	start_pos = max(0, start_pos);
	
	// lets say the end_pos is much larger than our cache_size.
	// we assume that the starting values are more important than the later ones
	// because these will be shown first.
	// so we will truncate the end if too large
	// it would be too complicated to keep a dynamic length of the cache
	// so we allocate the complete cache space even if the real end is lower than that
	end_pos = start_pos + CACHE_MAX;
		
	// Maybe some of the values in our cache are still good
	// for ex. the user just shifted the range by 1
	if (pc->first_pos > start_pos)
		cache_shift_down(pc, pc->first_pos - start_pos);
	// pc->first_pos is now <= start_pos
	
	if (pc->first_pos < start_pos)
		cache_shift_up(pc, start_pos - pc->first_pos);
	// pc->first_pos is now == start_pos
	

	// Now maybe some infos are not available, i.e. there pos is bigger than the actual pos limit
	
	// is the limit already known ?
	if (pc->pos_lim >= 0){				// 0 is valid and means list of information is empty
		// all infos in our cache >= pos_lim are NOT_AVAIL
		for (p = max(pc->first_pos, pc->pos_lim); p < last_pos(pc); p++)
			pos_not_avail(pc,p);
	};
	
#if 0	
	if (pc->pos_lim >= 0){								// valid limit information
		end_pos = min (end_pos, pc->pos_lim - 1);
		start_pos = min (start_pos, pc->pos_lim - 1);
	};

	if (end_pos > last_pos(pc))				// user wants more information than we currently have
		cache_shift_up(pc, end_pos - last_pos(pc));
	else
		if (start_pos < pc->first_pos)		// user wants information from earlier tracks than we have
			cache_shift_down(pc, pc->first_pos - start_pos);
#endif			
};

void
cache_set_limit(STR_CACHE *pc, int limit){
	int p;
	
	// Limit unknown ?
	// All entries above old limit are now unknown
	if (limit == -1){
		for (p = max(pc->first_pos, pc->pos_lim); p <= last_pos(pc); p++)
			pos_unknown(pc,p);
		pc->pos_lim = -1;
		return;
	};
	
	/* Was the current limit expanded ?
		Then all subsequent entries are NOT_KNOWN
	*/
	if (limit > pc->pos_lim){
		for (p = max(pc->first_pos, pc->pos_lim); p <= min(last_pos(pc), limit -1); p++)
			pos_unknown(pc, p);	
			
	// or did the limit shrink?
	// all entries between new limit and old limit are NOT_AVAIL	
	} else if (limit < pc->pos_lim){
		for (p = max(pc->first_pos, limit); p <= min(last_pos(pc), pc->pos_lim - 1); p++)
			pos_not_avail(pc, p);
	};
	pc->pos_lim = limit;

};

/* ==================================== End of string cache functions ========================================= */


