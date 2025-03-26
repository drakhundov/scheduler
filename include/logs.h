#ifndef _LOGS_H
#define _LOGS_H

#define LOGFILENAME "Scheduler.log"

typedef enum LOGTYPES { ERROR_LOG, INFO_LOG } Logtype;

void init_logs(const char* filename);
void end_logs();
void logMsg(Logtype log_t, const char* fmt, ...);

#endif