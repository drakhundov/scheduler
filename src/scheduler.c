#include "scheduler.h"

#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include "process.h"

int round_robin_scheduler(Process prc_lst[], int nprc) {
  int time = 0, runtime;
  int completed_prc = 0;
  bool fin_prc, no_prc;
  while (nprc > 0) {
    // Sort prc_lst by priority, arrival, etc.
    qsort(prc_lst, nprc, sizeof(Process), process_comp);
    no_prc = false;
    for (int i = 0; i < nprc; i++) {
      if (prc_lst[i].arr <= time && prc_lst[i].rem_t > 0) {
        // Execute or resume the process.
        runtime = run_process(prc_lst + i, &fin_prc);
        time += runtime;
        if (fin_prc) {
          process_destr(prc_lst[i]);
          // Shift remaining processes.
          for (int j = i; j < nprc - 1; j++) {
            prc_lst[j] = prc_lst[j + 1];
          }
          nprc--;
          i--;
          completed_prc++;
        }
        no_prc = false;
      } else if (i == 0) {
        no_prc = true;
      } else if (i == nprc - 1 && no_prc) {
        // If there is no process now, wait some time.
        sleep(1);
        time += 1;
        no_prc = false;
      }
    }
  }
  return completed_prc;
}