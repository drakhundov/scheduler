#include <stdio.h>
#include <fcntl.h>

#include "logs.h"

void testLogs() {
	init_logs("testlogs.log", LOGMODE);
	logMsg(INFO_LOG, "I'm just testiiiin");
	logMsg(ERROR_LOG, "Something terrible happened");
	end_logs();
}