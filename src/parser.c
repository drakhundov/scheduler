#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "logs.h"

void parse_prcs(char *data_fn, Process prc_lst[], int *nprc, int max_prc) {
  FILE *file = fopen(data_fn, "r");
  if (file == NULL) {
    char error_msg[128];
    sprintf(error_msg, "Unable to open file %s.", data_fn);
    logMsg(ERROR_LOG, error_msg);
    perror(error_msg);
    exit(1);
  }
  extern int gTimeQ;
  int _nprc;
  fscanf(file, "TimeSlice %d\n", &gTimeQ);
  _nprc = 0;
  char buf[BUFSIZ];
  int arr, pri, exec_t;
  while (fscanf(file, "%s %d %d %d\n", buf, &arr, &pri, &exec_t) != EOF &&
         _nprc < max_prc) {
    prc_lst[_nprc] = process_constr(buf, arr, pri, exec_t);
    _nprc++;
  }
  *nprc = _nprc;
  fclose(file);
}