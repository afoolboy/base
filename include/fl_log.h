


#ifndef FL_LOG_H
#define FL_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum LogLevel
{
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERR,

    LOG_LEVEL_OFF,
}LogLevel;


#ifdef FL_LOG_TRACE 
#define FL_LOG_LEVEL LOG_LEVEL_TRACE
#endif

#ifdef FL_LOG_DEBUG
#define FL_LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#ifdef FL_LOG_INFO
#define FL_LOG_LEVEL LOG_LEVEL_INFO
#endif

#ifdef FL_LOG_WARN
#define FL_LOG_LEVEL LOG_LEVEL_WARN
#endif

#ifdef FL_LOG_ERR
#define FL_LOG_LEVEL LOG_LEVEL_ERR
#endif 

#ifdef FL_LOG_OFF
#define FL_LOG_LEVEL LOG_LEVEL_OFF
#endif


#ifndef FL_LOG_LEVEL
#define FL_LOG_LEVEL LOG_LEVEL_INFO
#endif

extern int currentLogLevel;

void fl_log_output(int level,char *file,char *func,int line,char *fmt,...);
void fl_setLogLevel(int level);


#ifdef FL_CLOSE_LEVEL
#define FL_LOG(level,fmt,...)
#else
#define FL_LOG(level,fmt,...)   \
    do{ \
        if( level >= currentLogLevel ) \
        { \
            fl_log_output( level, (char *)__FILE__ , (char *)__FUNCTION__ , __LINE__ , (char *)fmt, ##__VA_ARGS__); \
        } \
    }while(0); 
#endif 




#define LOG_TRACE(fmt,...)          FL_LOG(LOG_LEVEL_TRACE, fmt,##__VA_ARGS__);
#define LOG_DEBUG(fmt,...)          FL_LOG(LOG_LEVEL_DEUBG, fmt,##__VA_ARGS__);  
#define LOG_INFO(fmt,...)           FL_LOG(LOG_LEVEL_INFO,  fmt,##__VA_ARGS__);
#define LOG_WARN(fmt,...)           FL_LOG(LOG_LEVEL_WARN,  fmt,##__VA_ARGS__);
#define LOG_ERR(fmt,...)            FL_LOG(LOG_LEVEL_ERR,   fmt,##__VA_ARGS__);

#ifdef __cplusplus
}
#endif

#endif
