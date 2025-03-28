#ifndef _LOGS_H
#define _LOGS_H

#include <fcntl.h>

// Default configuration.
#define LOGFILENAME "Scheduler.log"
#define LOGMODE O_WRONLY | O_APPEND | O_CREAT

typedef enum LOGTYPES { ERROR_LOG, INFO_LOG } Logtype;

void init_logs(const char* filename, int mode);
void end_logs();
void logMsg(Logtype log_t, const char* fmt, ...);

#endif