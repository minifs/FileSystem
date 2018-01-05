#include "log.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

void funclog (const char *format,... )
{
    if(format==NULL) {
        return;
    }


    va_list args_vprintf;
    va_list args_vfprintf;
    va_start(args_vfprintf, format);
    va_copy(args_vprintf,args_vfprintf);

    if(LOG_TARGET == WRITE_TO_FILE) {
        FILE *fp;
        fp = fopen(LOG_PATH,"a");
        vfprintf(fp, format, args_vfprintf);
        fclose(fp);
    } else if(LOG_TARGET == WRITE_TO_CONSOLE) {
        vprintf(format, args_vprintf);//print information on console
    } else {
        FILE *fp;
        fp = fopen(LOG_PATH, "a");
        vfprintf(fp, format, args_vfprintf);
        vprintf(format, args_vprintf);
        fclose(fp);
    }

    va_end(args_vprintf);
    va_end(args_vfprintf);
}


