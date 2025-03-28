# Round Robin Scheduler

## Overview
This project implements a Round Robin process scheduler with priority-based selection. It reads job information from an input file, creates child processes using `fork()` and `exec()` on a dummy program that imitates a running process, and schedules them according to their priority, arrival time, and execution time. The scheduler preempts running processes using `SIGSTOP` and resumes them with `SIGCONT` when they are scheduled again.

## Features
- Round Robin scheduling with a configurable time quantum.
- Priority-based job selection (lower value = higher priority).
- Preempted processes are resumed correctly.
- Jobs are managed with `fork()` and executed using `execv()`.
- Handles job arrival times to ensure correct scheduling order.

## How to Run the Scheduler
1. Compile the program:
   ```
   make
   ```
2. Provide a job list (`jobs.txt`) with the format:
   Arrival time, priority, execution time.
   ```
   TimeSlice 3
   jobA 0 1 6
   jobB 2 2 9
   jobC 4 1 4
   ```
3. Run the scheduler:
   ```
   make run
   ```

## Implementation Details
### `run_job()` Function
The function `run_job(Job *job, bool *fin_prc)` in `job.c` is responsible for executing and managing job processes.
`*fin_prc` is used as a flag to tell the scheduler to remove the process from the list.

- **First Execution:**
  - Uses `fork()` to create a new process.
  - Executes `jobExec` (the dummy program) using `execlp()` to simulate job execution.
  - Sleeps for the allocated time slice and then sends `SIGSTOP` if the job is not finished.

- **Resuming Execution:**
  - Sends `SIGCONT` to resume a preempted job.
  - Sleeps for the allocated time slice and checks if the job is complete.
  - Terminates the job if finished.

### Example Execution Flow:
1. A job is started using `fork()` and `execlp()`.
2. It runs for at most the time quantum (`quan_t`).
3. If it is not finished, it is paused using `SIGSTOP` and re-added to the scheduling queue.
4. When scheduled again, it is resumed using `SIGCONT` and continues execution.

## Future Improvements
- Implement **aging** to prevent starvation.
- Allow **dynamic time quantum adjustment**.
- Support **I/O-bound processes** with sleep/wait handling.