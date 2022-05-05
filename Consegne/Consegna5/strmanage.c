#include <string.h>
#include "strmanage.h"

char* findchr(const char* str, char c)
{
    while ( *str && *str != c ) {
		++str;
	}
	if(c == *str)
		return (char *)str;
	else
		return NULL;
}

void strconcat(char *dest, const char *s1, const char *s2, const char *s3){
	strcat(dest,s1);
	strcat(dest,s2);
	strcat(dest,s3);
}

void dectobinary(int num, char* string)
{
    *(string+15) = '\0';
    int mask = 0x4000 << 1;
    while(mask >>= 1)
        *string++ = !!(mask & num) + '0';
}

