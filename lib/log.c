#include "log.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

void funclog (const char *format,... )
{
    if(format==NULL) {
        return;
    }

    va_list args;
    memset(&args,0,sizeof(va_list));
    va_start(args, format);

    vprintf(format, args);//print information on console
    va_end(args);
}


