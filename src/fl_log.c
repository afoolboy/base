#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "fl_log.h"


//#define FL_LOGFILE_MAXSIZE ( 1 * 1024 * 1024 *1024 )  // 1GB
#define FL_LOGFILE_MAXSIZE ( 1 * 256 )  // 256B for test

#define FL_LOGLINE_MAXSIZE 4096

int currentLogLevel = FL_LOG_LEVEL;

static int logToFile = 0;
static int logToConsole = 1;

void fl_setLogLevel(int level)
{
    currentLogLevel = level;
}

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
    snprintf(timeStr,size,"%04d%02d%02d_%02d:%02d:%02d",
                t_tm->tm_year + 1900,
                t_tm->tm_mon,
                t_tm->tm_mday,
                t_tm->tm_hour,
                t_tm->tm_min,
                t_tm->tm_sec
            );
    return timeStr;
}

static FILE *fp;
static char logFileName[256]={0};


void logfile_open()
{
    logToFile = 1;
}

void setLogfileName(char *name)
{
    if(*logFileName!=0) return;
    /*fprintf(stderr,"set logfile:%s\n",name);*/
    snprintf(logFileName,sizeof(logFileName)-1,"%s",name);
}


// return:
// 1 for need open
// 0 for no need open
// -1 for err
int checkNeedOpenLogFile()
{
    // check file name
    if(fp==NULL){
        if(*logFileName==0)
            snprintf(logFileName,sizeof(logFileName)-1,"../log/log");
        return 1;
    }

    // check size
    struct stat stat_info;
    if(fstat(fileno(fp),&stat_info)){
        fprintf(stderr,"[%s::%s(%d)] err:%s\n",__FILE__,__FUNCTION__,\
                __LINE__,strerror(errno));
        return -1;
    }

    if(stat_info.st_size < FL_LOGFILE_MAXSIZE ) return 0;
    return 1;
}

// return:
// 0 for Ok
// -1 for failed
int checkFileOpen()
{
    int iRet = checkNeedOpenLogFile();
    if(iRet != 1) return iRet;

    char oldLogFileName[512]={0};
    char newLogFileName[512]={0};
    int i = 8;
    if( fp!=NULL ){
        fclose(fp);
        while(i>=0){
            snprintf(newLogFileName,sizeof(newLogFileName)-1,"%s_%d.log",logFileName,i+1);
            snprintf(oldLogFileName,sizeof(oldLogFileName)-1,"%s_%d.log",logFileName,i);
            rename(oldLogFileName,newLogFileName);
            i--;
        }
    }
    else{
            snprintf(newLogFileName,sizeof(newLogFileName)-1,"%s_0.log",logFileName);
    }

    fp = fopen(newLogFileName,"a+");
    if(NULL==fp){
        fprintf(stderr,"fopen(%s) failed(%s)\n",newLogFileName,strerror(errno));
        iRet = -1;
    }

    dup2(fileno(fp),STDOUT_FILENO);
    dup2(fileno(fp),STDERR_FILENO);

    return iRet==-1?-1:0;
}

void logfile(char *output)
{
    if(!output) return;
    int iRet = checkFileOpen();
    if(iRet==-1) { fprintf(stderr,"checkFileOpen failed\n");return;}
    if(!fp){
        fprintf(stderr,"[-]can't get log file fp\n");
        return;
    }

    fprintf(fp,"%s\n",output);
    fflush(fp);

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

    if(logToFile) 
        logfile(output);
    else
        fprintf(stderr,"%s\n",output);

    return;
}

