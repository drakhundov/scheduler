#include "process.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "logs.h"

Process *process_constr(char *prc_id, int arr, int pri, int exec_t) {
  Process *new_prc = malloc(sizeof(Process));
  if (new_prc == NULL) {
    logMsg(ERROR_LOG, "Failed to allocate memory for new Process");
    perror("Failed to allocate memory for new Process.");
    exit(1);
  }
  strcpy(new_prc->prc_id, prc_id);
  new_prc->arr = arr;
  new_prc->pri = pri;
  new_prc->exec_t = new_prc->rem_t = exec_t;
  new_prc->pid = -1;
  return new_prc;
}

// ! Must be called if Process is not in use
// ! anymore to avoid memory leack.
void process_destr(Process *_prc) {
  free(_prc->prc_id);
  free(_prc);
}

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

int run_process(Process *process, bool *fin_prc) {
  extern int gTimeQ;
  //* fin_prc tells if the Process is finished,
  //* so that it can be removed from the list.
  *fin_prc = false;
  int alloc_t = (process->rem_t <= gTimeQ) ? process->rem_t : gTimeQ;
  if (process->pid == -1) {
    // The process hasn't been initialized yet.
    logMsg(INFO_LOG, "Forking new process for %s", process->prc_id);
    pid_t pid = fork();
    if (pid == 0) {
      // Executing the process.
      logMsg(INFO_LOG, "Executing %s (PID: %d) using exec", process->prc_id,
             getpid());
      char buffer[20];
      sprintf(buffer, "%d", alloc_t);
      execlp("./ProcessExec", "./ProcessExec", process->prc_id, buffer, NULL);
    } else if (pid > 0) {
      process->pid = pid;
      sleep(alloc_t);
      process->rem_t -= alloc_t;
      if (process->rem_t > 0) {
        logMsg(INFO_LOG,
               "%s ran for %d seconds. Time slice expired - Sending SIGSTOP",
               process->prc_id, alloc_t);
        kill(process->pid, SIGSTOP);
      } else {
        logMsg(INFO_LOG, "%s completed execution. Terminating (PID: %d)",
               process->prc_id, process->pid);
        // ! Ensure the process safely ends.
        waitpid(process->pid, NULL, 0);
        *fin_prc = true;  // To be removed from the process list.
      }
    } else {
      logMsg(ERROR_LOG, "Fork failed");
      perror("Fork failed");
      exit(1);
    }
  } else {
    // Resume existing process.
    kill(process->pid, SIGCONT);
    sleep(alloc_t);
    process->rem_t -= alloc_t;
    if (process->rem_t > 0) {
      logMsg(INFO_LOG,
             "%s ran for %d seconds. Time slice expired - Sending SIGSTOP",
             process->prc_id, alloc_t);
      kill(process->pid, SIGSTOP);
    } else {
      logMsg(INFO_LOG, "%s completed execution. Terminating (PID: %d)",
             process->prc_id, process->pid);
      waitpid(process->pid, NULL, 0);
      *fin_prc = true;
    }
    logMsg(INFO_LOG, "%s remaining time: %d\n", process->prc_id, process->rem_t);
  }
  return alloc_t;
}

// int run_Process(Process *Process, bool *fin_prc) {
//   extern int gTimeQ;
//   *fin_prc =
//       false;    // A flag that specifies whether the process should be
//       finished.
//   int alloc_t;  // Allocated time.
//   int t = Process->rem_t;
//   if (t <= gTimeQ) {
//     alloc_t = t;
//   } else {
//     alloc_t = gTimeQ;
//   }
//   if (Process->pid == -1) {
//     pid_t pid = fork();
//     if (pid == -1) {
//       logMsg(ERROR_LOG,
//              "Fork failed: Resource temporarily unavailable for Process %s",
//              Process->prc_id);
//       perror("Fork failed");
//       exit(1);
//     }
//     logMsg(INFO_LOG, "Forking new process for %s", Process->prc_id);
//     if (pid == 0) {
//       // Child process: Executing the Process.
//       logMsg(INFO_LOG, "Executing %s (PID: %d) using exec", Process->prc_id,
//              getpid());
//       char buffer[20];
//       sprintf(buffer, "%d", t);
//       execlp("./Process_exec", "./Process_exec", buffer, NULL);
//       // If execlp fails
//       logMsg(ERROR_LOG, "execlp failed for Process %s", Process->prc_id);
//       perror("execlp failed");
//       exit(1);
//     } else if (pid > 0) {
//       // Parent process: Managing the Process.
//       Process->pid = pid;
//       sleep(alloc_t);
//       Process->rem_t -= alloc_t;
//       if (Process->rem_t > 0) {
//         logMsg(INFO_LOG,
//                "%s ran for %d seconds. Time slice expired - Sending SIGSTOP",
//                Process->prc_id, alloc_t);
//         kill(Process->pid, SIGSTOP);
//       } else {
//         logMsg(INFO_LOG, "%s completed execution. Terminating (PID: %d)",
//                Process->prc_id, Process->pid);
//         waitpid(Process->pid, NULL, 0);
//         *fin_prc = true;
//       }
//     }
//   } else {
//     // Resume the existing Process.
//     kill(Process->pid, SIGCONT);
//     sleep(alloc_t);
//     Process->rem_t -= alloc_t;
//     if (Process->rem_t > 0) {
//       logMsg(INFO_LOG,
//              "%s ran for %d seconds. Time slice expired - Sending SIGSTOP",
//              Process->prc_id, alloc_t);
//       kill(Process->pid, SIGSTOP);
//     } else {
//       logMsg(INFO_LOG, "%s completed execution. Terminating (PID: %d)",
//              Process->prc_id, Process->pid);
//       waitpid(Process->pid, NULL, 0);
//       *fin_prc = true;
//     }
//   }
//   logMsg(INFO_LOG, "%s remaining time: %d\n", Process->prc_id, Process->rem_t);
//   return alloc_t;
// }