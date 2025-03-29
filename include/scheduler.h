#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "process.h"

// * Returns the number of completed processes.
int round_robin_scheduler(Process prc_lst[], int nprc);

#endif