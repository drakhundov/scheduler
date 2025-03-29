#ifndef _PROCESS_H
#define _PROCESS_H

#include <stdbool.h>
#include <sys/types.h>  // pid_t

typedef struct {
  char *prc_id;
  int arr,     // Arrival
      pri,     // Priority
      exec_t,  // Execution time
      rem_t;   // Remaining time
  pid_t pid;
} Process;

Process process_constr(char *prc_id, int arr, int pri, int exec_t);
void process_destr(Process _prc);
int process_comp(const void *a, const void *b);  // ! To be used by qsort.
//* fin_prc tells if the Process is finished,
//* so that it can be removed from the list.
// Returns the amount of time allocated for the process.
int run_process(Process *prc, bool *fin_prc);

#endif