#include "job.h"

#include <stdlib.h>

Job *job_constr(char *job_id, int arr, int pri, int exec_t) {
	Job *new_job = malloc(sizeof(Job));
	new_job->job_id = job_id;
	new_job->arr = arr;
	new_job->pri = pri;
	new_job->exec_t = exec_t;
}

void job_destr(Job *_job) {
	free(_job->job_id);
	free(_job);
}