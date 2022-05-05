#include <string.h>
#include <ctype.h>

#include "strmanage.h"


bool s_is_empty(const char *s) {
    if (s == NULL) {
        return true;
    }

    while (isspace(*s)) {
        s++;
    }
	
	if(*s == '\0')
		return true;
	else 
		return false;
}

char *remove_ext(char *s) {
    char *last_dot = findchr(s, '.');

    if (last_dot != NULL) {
        *last_dot = '\0';
    }
    return s;
}

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

int s_splitting(char *s, char *sub_str[], int max_s, const char *borders)
{
    int i;

    if (s  == NULL || sub_str == NULL || borders == NULL || !*s || !*borders || max_s < 1)
        return 0;

    sub_str[0] = strtok(s, borders);
    if (sub_str[0] == NULL)
        return 0;

    for (i = 1; i < max_s && (sub_str[i] = strtok(NULL, borders)) != NULL; i++) { }

    return i;
}