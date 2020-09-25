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

#ifdef __cplusplus
}
#endif

#endif//_SAL_STRUTIL_H_