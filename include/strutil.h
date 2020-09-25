#ifndef _SAL_STRUTIL_H_
#define _SAL_STRUTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include "stdbool.h"

static inline
int count_chars(const char *str, char chr)
{
    const char *p = str;
    int count = 0;
    do {
        if (*p == chr)
            count++;
    } while (*(p++));
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