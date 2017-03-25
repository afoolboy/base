#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "fl_log.h"


#define FL_LOGFILE_MAXSIZE ( 1 * 1024 * 1024 *1024 )  // 1GB
#define FL_LOGLINE_MAXSIZE 4096

int currentLogLevel = LOG_LEVEL_INFO;


static int logToFile = 0;
static int logToConsole = 1;

char *get_logLevel(char *levelStr,uint32_t size)
{
    /*if( NULL == levelStr ) { abort(); }*/
    switch(currentLogLevel)
    {
        case LOG_LEVEL_TRACE:
            snprintf(levelStr,size,"TRACE");
            break;
        case LOG_LEVEL_DEBUG:
            snprintf(levelStr,size,"DEBUG");
            break;
        case LOG_LEVEL_INFO:
            snprintf(levelStr,size,"INFO");
            break;
        case LOG_LEVEL_ERR:
            snprintf(levelStr,size,"ERR");
            break;
        default:
            snprintf(levelStr,size,"UNKNOWN");
            break;
    }
    return levelStr;
}


char* get_currentTime(char *timeStr, uint32_t size)
{
    time_t t_t = time(NULL);
    struct tm *t_tm = localtime(&t_t);

   /*struct tm {*/
   /*int tm_sec;         [> seconds <]*/
   /*int tm_min;         [> minutes <]*/
   /*int tm_hour;        [> hours <]*/
   /*int tm_mday;        [> day of the month <]*/
   /*int tm_mon;         [> month <]*/
   /*int tm_year;        [> year <]*/
   /*int tm_wday;        [> day of the week <]*/
   /*int tm_yday;        [> day in the year <]*/
   /*int tm_isdst;       [> daylight saving time <]*/
   /*};*/
    // 20170325_00:00:00
    snprintf(timeStr,size,"%04d%02d%02d_%02d%02d%02d",
                t_tm->tm_year + 1900,
                t_tm->tm_mon,
                t_tm->tm_mday,
                t_tm->tm_hour,
                t_tm->tm_min,
                t_tm->tm_sec
            );
    return timeStr;
}


void logfile(char *output)
{

    return ;
}

void fl_log_output(int level,char *file,char *func,int line,char *fmt,...)
{
    char output[FL_LOGLINE_MAXSIZE]={0};
    char szTime[64]={0};
    char szLevel[16]={0};

    int index=0;

    // pid,time,level,file,func,line,fmt,...
    index = snprintf(output,FL_LOGLINE_MAXSIZE-1,"[%d][%s][%s][%s::%s(%d)]:",
            getpid(),get_currentTime(szTime,64),get_logLevel(szLevel,16),
            file,func,line);

    if(index < 1) return;

    va_list ap;
    va_start(ap,fmt);
    vsnprintf(output+index,FL_LOGLINE_MAXSIZE -1 -index, fmt, ap);
    va_end(ap);

    if(logToFile) logfile(output);

    if(logToConsole) fprintf(stderr,"%s\n",output);

    return;
}

void fl_setLogLevel(int level)
{
    currentLogLevel = level;
}
