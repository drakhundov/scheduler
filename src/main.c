#include <stdio.h>
#include <stdlib.h>

#define MAX_PRC 100

#include "logs.h"
#include "parser.h"
#include "scheduler.h"
#include "process.h"

Process prc_lst[MAX_PRC];
int nprc = 0;

int gTimeQ;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    perror("Usage: ./scheduler <process_list> <process_exec>");
    return 1;
  }
  char *data_fn = argv[1];
  extern char *gExecPath;
  gExecPath = argv[2];
  init_logs(LOGFILENAME, LOGMODE);
  parse_prcs(data_fn, prc_lst, &nprc, MAX_PRC);
  logMsg(INFO_LOG, "The processes have been parsed");
  logMsg(INFO_LOG, "Running the scheduler");
  round_robin_scheduler(prc_lst, nprc);
  end_logs();
  return 0;
}