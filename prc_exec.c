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

void run_process(char *prc_id, int total_time) {
    int elapsed_time = 0;
    while (elapsed_time < total_time) {
        if (stopped) {
            pause();  // Wait until SIGCONT is received.
        }
        sleep(1);  // Simulate real execution.
        elapsed_time++;
        printf("%s running... %d/%d seconds completed\n", prc_id, elapsed_time, total_time);
    }
    printf("%s finished execution.\n", prc_id);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <prc_id:string> <execution_time>\n", argv[0]);
        exit(1);
    }
    char *prc_id = argv[1];
    int execution_time = atoi(argv[2]);
    signal(SIGSTOP, handle_signal);
    signal(SIGCONT, handle_signal);
    run_process(prc_id, execution_time);
    return 0;
}