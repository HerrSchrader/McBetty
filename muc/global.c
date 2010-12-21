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
int str_len( char *s){
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

/* Standard C string copy EXCEPT that the destination string is always 0 terminated ! 
	The resulting string uses at most n bytes (incl. trailing 0) 
*/
char *strn_cpy(char *dest, const char *src, int n){
	int i;
	char *dst = dest;
	for (i=0; i<n; i++){
		*dst = *src;
		if (*src) src++;
		else break;
		dst++;
	};
	*dst = 0;
	return dest;
};

// Not needed
#if 0
/* Standard C string concatenation  */
char *strncat(char *dest, const char *src, int n){
	int i;
	char *dst = dest;
	
	while (*(dst)) dst++;
	
	for (i=0; i<n; i++){
		if (*src) *(dst++) = *(src++);
		else break;
	};
	*dst = 0;
	return dest;
};
#endif


/* Non-Standard C string concatenation: The resulting string uses at most n bytes (incl. terminating 0) */
char *
str_cat_max(char *dest, const char *src, int n){
	int i = 0;
	char *dst = dest;
	
	/* Find the current end of *dst */
	for (i=0; i< n-1; i++, dst++){
		if (*dst == '\0')
			break;
	};
	
	for (; i < n-1; i++){
		if (*src) 
			*(dst++) = *(src++);
		else break;
	};
	*dst = '\0';
	return dest;
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
strn_cpy_cmp(char *str1, char *str2, int n, int *length){
	int ident = 1;
	int len = 0;

	do {	
		if (*str1 != *str2){
			*str1 = *str2;
			ident = 0;
		};
		str1++;
	} while ( *(str2++) && ( ++len < n ) );
	/* Now *(str2-1) is 0 or len == n 
		If *(str2-1) is 0 we have just terminated the string str1 and we are finished.
		if (len == n) we have found no terminating 0 in str2. Write one and return not identical. 
	*/
	if (len == n) {
		*(str1-1) = '\0';
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

/* Given a string pos, we return the index into str[] corresponding to this number.
	If the number is outside of our list, we return -1.
*/
int
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
	or NULL if the pos is outside our cache.
	NULL might also mean we do not have information about that string.
*/
char *
cache_entry(STR_CACHE *pc, int pos){
	int i = cache_index(pc, pos);
	if (i < 0) return NULL;
	return pc->str[i];
};

/* 
	Given a string, store this info in our cache (if it fits) 
	If the string is NULL, a NULL is stored else the string is copied length-limited by CACHE_ENTRY_LEN.
*/
void
cache_store(STR_CACHE *pc, int pos, char *content){
	int idx;
	
	idx = cache_index(pc, pos);
	if (idx < 0)
		return;
	if (NULL == content)
		pc->str[idx] = NULL;
	else {
		strn_cpy(pc->cache_entry[idx], content, CACHE_ENTRY_LEN); 
		pc->str[idx] = pc->cache_entry[idx];
	};	
};

/* All the cache entries starting at pos are made empty (unknown). */
void 
cache_empty(STR_CACHE *pc, int pos){
	int i;
	
	if (pos > last_pos(pc)) 
		return;				/* not in our cache */
	
	pos = max(pc->first_pos, pos);		/* Need not start before begin of cache */

	for (i=pos; i <= last_pos(pc); i++)
		cache_store(pc, i, NULL);
};

void
cache_init(STR_CACHE *pc){
	pc->first_idx = 0;
	pc->first_pos = 0;
	cache_empty(pc, 0);
};
	
/* We are shifting the start of our tracklist up by 1 position.
	I.e. if the cache started at pos 17, it now starts at pos 18
	We are not changing most of the information in str[], only the start (and end) of the array.
	We are setting the information of the previous first track to NULL, because it is no longer valid.
		(That information now belongs to the new last track)
*/
static void
cache_inc_first(STR_CACHE *pc){
	pc->first_pos++;
	pc->str[pc->first_idx] = NULL;
	pc->first_idx++;
	if (pc->first_idx > CACHE_MAX)
		pc->first_idx = 0;
};

/* We are shifting the start of our tracklist down by 1 pos 
	I.e. if the cache started at pos 17, it now starts at pos 16
	We are not changing most of the information in tracklist[], only the start (and end) of the array.
	We are setting the information of the new first track no. to NULL, because we do not know it.
*/
static void
cache_dec_first(STR_CACHE *pc){
	pc->first_pos--;
	pc->first_idx--;
	if (pc->first_idx < 0)
		pc->first_idx = CACHE_MAX;
	pc->str[pc->first_idx] = NULL;
};

void
cache_shift_up(STR_CACHE *pc, int diff){
	for (;diff > 0; diff --)
		cache_inc_first(pc);
};

void
cache_shift_down(STR_CACHE *pc, int diff){
	for (;diff > 0; diff --)
		cache_dec_first(pc);
};

/* Returns the first empty pos in our cache 
	or -1 if every pos has a non-NULL entry.
*/
int
cache_find_empty(STR_CACHE *pc){
	int pos;
	for (pos=pc->first_pos; pos <= last_pos(pc); pos++)
		if (cache_entry(pc, pos) == NULL)
			return pos;
	return -1;	
}

/* ==================================== End of string cache functions ========================================= */


