#ifndef _JOB_H
#define _JOB_H

struct job {
  char *job_id;
  int arr, pri, exec_t;
};

typedef struct job Job;

Job *job_constr(char *job_id, int arr, int pri, int exec_t);

#endif