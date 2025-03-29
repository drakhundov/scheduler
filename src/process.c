#include "process.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "logs.h"

char *gExecPath;

Process process_constr(char *prc_id, int arr, int pri, int exec_t) {
  Process new_prc;
  new_prc.prc_id = (char *)malloc(strlen(prc_id) + 1);
  if (new_prc.prc_id == NULL) {
    logMsg(ERROR_LOG, "Failed to allocate space for process id");
    perror("Failed to allocate space for process id.");
    exit(1);
  }
  strcpy(new_prc.prc_id, prc_id);
  new_prc.arr = arr;
  new_prc.pri = pri;
  new_prc.exec_t = new_prc.rem_t = exec_t;
  new_prc.pid = -1;
  return new_prc;
}

// ! Must be called if Process is not in use
// ! anymore to avoid memory leack.
void process_destr(Process _prc) { free(_prc.prc_id); }

int process_comp(const void *a, const void *b) {
  Process *j1 = (Process *)a;
  Process *j2 = (Process *)b;
  // Compare by priority.
  if (j1->pri != j2->pri) {
    return j1->pri - j2->pri;
  }
  // If priorities are equal, compare by arrival time.
  if (j1->arr != j2->arr) {
    return j1->arr - j2->arr;
  }
  // If arrival times are equal, compare by remaining time.
  if (j1->rem_t != j2->rem_t) {
    return j1->rem_t - j2->rem_t;
  }
  // If all are equal, use the original order  in the input.
  return 0;
}

int run_process(Process *prc, bool *fin_prc) {
  extern int gTimeQ;
  //* fin_prc tells if the Process is finished,
  //* so that it can be removed from the list.
  *fin_prc = false;
  int alloc_t = (prc->rem_t <= gTimeQ) ? prc->rem_t : gTimeQ;
  if (prc->pid == -1) {
    // If the process is not started yet, fork and execute it.
    logMsg(INFO_LOG, "Forking new process for %s", prc->prc_id);
    pid_t pid = fork();
    if (pid == 0) {
      // Executing the process.
      logMsg(INFO_LOG, "Executing %s (PID: %d) using exec", prc->prc_id,
             getpid());
      char buffer[20];
      snprintf(buffer, sizeof(buffer), "%d", alloc_t);
      execlp(gExecPath, gExecPath, prc->prc_id, buffer, NULL);
      perror("Failed to execute the process.");
      exit(1);
    } else if (pid > 0) {
      prc->pid = pid;
      sleep(alloc_t);
      prc->rem_t -= alloc_t;
      if (prc->rem_t > 0) {
        logMsg(INFO_LOG,
               "%s ran for %d seconds. Time slice expired - Sending SIGSTOP",
               prc->prc_id, alloc_t);
        kill(prc->pid, SIGSTOP);
      } else {
        logMsg(INFO_LOG, "%s completed execution. Terminating (PID: %d)",
               prc->prc_id, prc->pid);
        // ! Ensure the process safely ends.
        waitpid(prc->pid, NULL, 0);
        *fin_prc = true;  // To be removed from the process list.
      }
    } else {
      logMsg(ERROR_LOG, "Fork failed");
      perror("Fork failed");
      exit(1);
    }
  } else {
    // Resume existing process.
    kill(prc->pid, SIGCONT);
    sleep(alloc_t);
    prc->rem_t -= alloc_t;
    if (prc->rem_t > 0) {
      logMsg(INFO_LOG,
             "%s ran for %d seconds. Time slice expired - Sending SIGSTOP",
             prc->prc_id, alloc_t);
      kill(prc->pid, SIGSTOP);
    } else {
      logMsg(INFO_LOG, "%s completed execution. Terminating (PID: %d)",
             prc->prc_id, prc->pid);
      waitpid(prc->pid, NULL, WNOHANG);
      *fin_prc = true;
    }
    logMsg(INFO_LOG, "%s remaining time: %d", prc->prc_id, prc->rem_t);
  }
  return alloc_t;
}