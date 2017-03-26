
#include <stdio.h>
#include "fl_log.h"


int main()
{
    logfile_open();
    setLogfileName("./mylog");

    fl_setLogLevel(LOG_LEVEL_TRACE);

    LOG_TRACE("trace");
    LOG_DEBUG("debug");
    LOG_INFO("info");
    LOG_WARN("warn");
    LOG_ERR("err");

    return 0;
}


