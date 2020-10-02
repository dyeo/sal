#ifndef _SAL_STRUTIL_H_
#define _SAL_STRUTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "utf8.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* counts the number of occurrences of a character in a given string */
static inline
size_t cntchr(const unsigned char *str, unsigned char chr)
{
    const unsigned char *tmp = str;
    int count = 0;
    while(tmp != NULL)
	{
		tmp = utf8chr(tmp, chr);
		if(tmp)
		{
			++tmp;
			++count;
		}
	}
    return count;
}

/* returns a new null-terminated substring of the string given by the pointer range [beg, end) */
static inline
unsigned char *substr(unsigned char *beg, unsigned char *end)
{
	char *res = (char*)malloc(end-beg+1);
	memcpy(res, beg, end-beg);
	res[end-beg] = '\0';
	return res;
}

/* replaces all occurrences of a given character in a string */
static inline
unsigned char *repchr(unsigned char *str, unsigned char fnd, unsigned char rep)
{
	unsigned char *cur = strchr(str,fnd);
    while (cur)
	{
        *cur = rep;
        cur = utf8chr(cur,fnd);
    }
    return str;
}

static inline
bool strtob(const unsigned char *str)
{
	return utf8cmp(str,"false") ? false : true;
}

#ifdef __cplusplus
}
#endif

#endif//_SAL_STRUTIL_H_