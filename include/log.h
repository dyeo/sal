#ifndef _SAL_LOG_H_
#define _SAL_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#define _LOG_LEVEL_TRACE 0
#define _LOG_LEVEL_DEBUG 1
#define _LOG_LEVEL_INFO 2
#define _LOG_LEVEL_WARNING 3
#define _LOG_LEVEL_ERROR 4
#define _LOG_LEVEL_FATAL 5

#define log_trace(...) __log(_LOG_LEVEL_TRACE, __VA_ARGS__)
#define log_debug(...) __log(_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define log_info(...) __log(_LOG_LEVEL_INFO, __VA_ARGS__)
#define log_warn(...) __log(_LOG_LEVEL_WARNING, __VA_ARGS__)
#define log_error(...) __log(_LOG_LEVEL_ERROR, __VA_ARGS__)
#define log_fatal(...) __log(_LOG_LEVEL_FATAL, __VA_ARGS__)

#define logf_trace(FILE, LINE, COL, ...) __logf(_LOG_LEVEL_TRACE, FILE, LINE, COL, __VA_ARGS__)
#define logf_debug(FILE, LINE, COL, ...) __logf(_LOG_LEVEL_DEBUG, FILE, LINE, COL, __VA_ARGS__)
#define logf_info(FILE, LINE, COL, ...)	__logf(_LOG_LEVEL_INFO, FILE, LINE, COL, __VA_ARGS__)
#define logf_warn(FILE, LINE, COL, ...) __logf(_LOG_LEVEL_WARNING, FILE, LINE, COL, __VA_ARGS__)
#define logf_error(FILE, LINE, COL, ...) __logf(_LOG_LEVEL_ERROR, FILE, LINE, COL, __VA_ARGS__)
#define logf_fatal(FILE, LINE, COL, ...) __logf(_LOG_LEVEL_FATAL, FILE, LINE, COL, __VA_ARGS__)

static const char *_fmt[] = {
	"TRACE ",
	"DEBUG ",
	" INFO ",
	" WARN ",
	"ERROR ",
	"FATAL "
};

static const char *_col[] = {
	"\e[0;36m",
	"\e[0;32m",
	"\e[0;35m",
	"\e[01;33m",
	"\e[0;31m",
	"\e[1;31m"
};

inline static
void __log_time()
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer [9];
	time(&rawtime);
	timeinfo = localtime(&rawtime);	
	strftime(buffer, 9,"%H:%M:%S", timeinfo);
    printf("\e[90m%s\e[0m ", buffer);
}

inline static
void __log_level(const int level)
{	
	printf(_col[level]);
	printf(_fmt[level]);
	printf("\e[0m");
}

inline static
void __log(const int level, const char *fmt, ...)
{
	__log_time();
	__log_level(level);
	va_list argptr;
	va_start(argptr, fmt);
	vfprintf(stdout, fmt, argptr);
	va_end(argptr);
	printf("\n");
}

inline static
void __logf(const int level, const char *file, const int line, const int col, const char *fmt, ...)
{
	__log_time();
	__log_level(level);
	printf("\e[90m%s:%i:%i\e[0m ", file, line, col);
	va_list argptr;
	va_start(argptr, fmt);
	vfprintf(stdout, fmt, argptr);
	va_end(argptr);
	printf("\n");
}

#ifdef __cplusplus
}
#endif

#endif//_SAL_LOG_H_