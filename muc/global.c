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

