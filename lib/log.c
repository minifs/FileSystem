#include "log.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

void funclog (const char *format,... )
{
    if(format==NULL) {
        return;
    }
//---------------get and print time-------------------
    time_t rawtime=time(0); // get time now
    struct tm *timeinfo=localtime ( &rawtime );
    char *time_output = asctime(timeinfo);
    time_output[strlen(time_output) - 1] = 0;

    va_list args_vprintf;
    va_list args_vfprintf;
    va_start(args_vfprintf, format);
    va_copy(args_vprintf,args_vfprintf);

    if(LOG_TARGET == WRITE_TO_FILE) {
        FILE *fp;
        fp = fopen(LOG_PATH,"a");
	fprintf(fp, "%s -- ", time_output);
        vfprintf(fp, format, args_vfprintf);
        fclose(fp);
    } else if(LOG_TARGET == WRITE_TO_CONSOLE) {
	printf("%s -- ", time_output);
        vprintf(format, args_vprintf);//print information on console
    } else {
        FILE *fp;
        fp = fopen(LOG_PATH, "a");
	fprintf(fp, "%s -- ", time_output);
        vfprintf(fp, format, args_vfprintf);
	printf("%s -- ", time_output);
        vprintf(format, args_vprintf);
        fclose(fp);
    }

    va_end(args_vprintf);
    va_end(args_vfprintf);
}


