#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

volatile sig_atomic_t stopped = 0;

void handle_signal(int sig) {
    if (sig == SIGTSTP || sig == SIGSTOP) {
        stopped = 1;
    } else if (sig == SIGCONT) {
        stopped = 0;
    }
}

void run_job(char *job_id, int total_time) {
    int elapsed_time = 0;
    while (elapsed_time < total_time) {
        if (stopped) {
            pause();  // Wait until SIGCONT is received.
        }
        sleep(1);  // Simulate real execution.
        elapsed_time++;
        printf("%s running... %d/%d seconds completed\n", job_id, elapsed_time, total_time);
    }
    printf("%s finished execution.\n", job_id);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <job_id:string> <execution_time>\n", argv[0]);
        exit(1);
    }
    char *job_id = argv[1];
    int execution_time = atoi(argv[2]);
    signal(SIGSTOP, handle_signal);
    signal(SIGCONT, handle_signal);
    run_job(job_id, execution_time);
    return 0;
}
