#include "logs.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>

int logfd;

void init_logs(const char *filename) {
	logfd = open(filename, O_WRONLY | O_CREAT, 0644);
	if (logfd == -1) {
		perror("Failed to open log file.");
		exit(1);
	}
}

// ! This is a helper function.
// ! It is not supposed to be used
// ! outside of the logging implementation.
char *_get_timestamp() {
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	char *timestamp = (char*) malloc(20);
	if (timestamp == NULL) {
		perror("Failed to allocate memory for timestamp.");
		exit(1);
	}
	sprintf(timestamp, "%02d-%02d-%04d %02d:%02d:%02d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900,
		local->tm_hour, local->tm_min, local->tm_sec);
	return timestamp;
}

void end_logs() {
	close(logfd);
}

void logMsg(Logtype log_t, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	char tmp[1024];
	char *type_msg = (log_t == ERROR_LOG) ? "ERROR" : "INFO";
	vsnprintf(tmp, sizeof(tmp), fmt, args);
	va_end(args);
	char msg[1024];
	sprintf(msg, "[%s] [%s] %s\n", _get_timestamp(), type_msg, tmp);
	write(logfd, msg, strlen(msg));
	fsync(logfd);  // Ensure the log message is written to disk.
}
