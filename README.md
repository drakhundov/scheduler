# Round Robin Scheduler

## Overview
This project implements a Round Robin process scheduler with priority-based selection. It reads Process information from an input file, creates child processes using `fork()` and `exec()` on a dummy program that imitates a running process, and schedules them according to their priority, arrival time, and execution time. The scheduler preempts running processes using `SIGSTOP` and resumes them with `SIGCONT` when they are scheduled again.

## Features
- Round Robin scheduling with a configurable time quantum.
- Priority-based Process selection (lower value = higher priority).
- Preempted processes are resumed correctly.
- Processs are managed with `fork()` and executed using `execv()`.
- Handles Process arrival times to ensure correct scheduling order.

## How to Run the Scheduler
1. Compile the program:
   ```
   make
   ```
2. Provide a Process list (`Jobs.txt`) in the project directory with the format:
   Arrival time, priority, execution time.
   ```
   TimeSlice 3
   ProcessA 0 1 6
   ProcessB 2 2 9
   ProcessC 4 1 4
   ```
3. Run the scheduler:
   ```
   make run
   ```

## Implementation Details
### `run_Process()` Function
The function `run_Process(Process *Process, bool *fin_prc)` in `Process.c` is responsible for executing and managing Process processes.
`*fin_prc` is used as a flag to tell the scheduler to remove the process from the list.

- **First Execution:**
  - Uses `fork()` to create a new process.
  - Executes `ProcessExec` (the dummy program) using `execlp()` to simulate Process execution.
  - Sleeps for the allocated time slice and then sends `SIGSTOP` if the Process is not finished.

- **Resuming Execution:**
  - Sends `SIGCONT` to resume a preempted Process.
  - Sleeps for the allocated time slice and checks if the Process is complete.
  - Terminates the Process if finished.

### Example Execution Flow:
1. A Process is started using `fork()` and `execlp()`.
2. It runs for at most the time quantum (`gTimeQ`).
3. If it is not finished, it is paused using `SIGSTOP` and re-added to the scheduling queue.
4. When scheduled again, it is resumed using `SIGCONT` and continues execution.