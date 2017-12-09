#ifndef __LOG_H_
#define __LOG_H_
#include <time.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

void funclog (const char *format,... );

#define __FUNC__	(char *)__FUNCTION__// get function name	

#define PRINTFUNCTION(format, ...) funclog(format, __VA_ARGS__)

#define _FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
#define NO_LOG          0x00
#define FATAL_LEVEL		0x01
#define ERROR_LEVEL     0x02
#define WARN_LEVEL		0x03
#define INFO_LEVEL      0x04
#define DEBUG_LEVEL     0x05

#ifndef LOG_LEVEL
#define LOG_LEVEL   DEBUG_LEVEL
#endif
#define NEWLINE     "\n"

#define FATAL_TAG	"FATAL"
#define ERROR_TAG   "ERROR"
#define INFO_TAG    "INFO"
#define WARN_TAG	"WARN"
#define DEBUG_TAG   "DEBUG"
#define LOG_FMT             "[%-5s] %-15s:[%d]:%s: "
#define LOG_ARGS(LOG_TAG)   LOG_TAG, _FILE,__LINE__ , __FUNCTION__

#if LOG_LEVEL >= DEBUG_LEVEL
#define LOG_DEBUG(message, args...)     PRINTFUNCTION(LOG_FMT message, LOG_ARGS(DEBUG_TAG), ## args)
#else
#define LOG_DEBUG(message, args...)
#endif

#if LOG_LEVEL >= INFO_LEVEL
#define LOG_INFO(message, args...)      PRINTFUNCTION(LOG_FMT message, LOG_ARGS(INFO_TAG), ## args)
#else
#define LOG_INFO(message, args...)
#endif


#if LOG_LEVEL >= WARN_LEVEL
#define LOG_WARN(message, args...)      PRINTFUNCTION(LOG_FMT message, LOG_ARGS(WARN_TAG), ## args)
#else
#define LOG_WARN(message, args...)
#endif

#if LOG_LEVEL >= ERROR_LEVEL
#define LOG_ERROR(message, args...)     PRINTFUNCTION(LOG_FMT message , LOG_ARGS(ERROR_TAG), ## args)
#else
#define LOG_ERROR(message, args...)
#endif

#if LOG_LEVEL >= FATAL_LEVEL
#define LOG_FATAL(message, args...) 	    PRINTFUNCTION(LOG_FMT message, LOG_ARGS(ERROR_TAG), ## args)
#else
#define LOG_FATAL(message, args...)
#endif


#endif
