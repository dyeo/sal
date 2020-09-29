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

static inline
int count_chars(const char *str, char chr)
{
    const char *tmp = str;
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

/*
Returns a substring of the string given by the pointer range [beg, end).
This string is allocated and must be freed later.
*/
static inline
char *substr(char *beg, char *end)
{
	char *res = (char*)malloc(end-beg+1);
	memcpy(res, beg, end-beg);
	res[end-beg] = '\0';
	return res;
}

#ifdef __cplusplus
}
#endif

#endif//_SAL_STRUTIL_H_