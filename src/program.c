/*
 *              NOTE 
 *              
 * This program was generated using chatgpt4o.
 * The dispatcher runs this program with arg <service_time> at or after
 * <arrival time>.
 * 
 * */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define DEFAULT_TIME 60

static volatile sig_atomic_t signal_SIGINT = 0;
static volatile sig_atomic_t signal_SIGQUIT = 0;
static volatile sig_atomic_t signal_SIGHUP = 0;
static volatile sig_atomic_t signal_SIGTERM = 0;
static volatile sig_atomic_t signal_SIGABRT = 0;
static volatile sig_atomic_t signal_SIGCONT = 0;
static volatile sig_atomic_t signal_SIGTSTP = 0;

void SignalHandler(int sig) {
    switch (sig) {
        case SIGINT: signal_SIGINT = 1; break;
        case SIGQUIT: signal_SIGQUIT = 1; break;
        case SIGHUP: signal_SIGHUP = 1; break;
        case SIGCONT: signal_SIGCONT = 1; break;
        case SIGTSTP: signal_SIGTSTP = 1; break;
        case SIGABRT: signal_SIGABRT = 1; break;
        case SIGTERM: signal_SIGTERM = 1; break;
    }
}

void PrintUsage(char *pgmName) {
    printf("\nUsage: %s [seconds]\n", pgmName);
    printf(" where [seconds] is the lifetime of the program - default = %d seconds.\n", DEFAULT_TIME);
    exit(127);
}

int main(int argc, char *argv[]) {
    pid_t pid = getpid();
    int i, cycle;

    if (argc > 2 || (argc == 2 && !isdigit(argv[1][0]))) {
        PrintUsage(argv[0]);
    }

    printf("Process ID: %d\n", (int)pid);

    signal(SIGINT, SignalHandler);
    signal(SIGQUIT, SignalHandler);
    signal(SIGHUP, SignalHandler);
    signal(SIGTERM, SignalHandler);
    signal(SIGABRT, SignalHandler);
    signal(SIGCONT, SignalHandler);
    signal(SIGTSTP, SignalHandler);

    cycle = argc < 2 ? DEFAULT_TIME : atoi(argv[1]);
    if (cycle <= 0) cycle = 1;

    for (i = 0; i < cycle;) {
        sleep(1);
        printf("Tick %d\n", ++i);

        if (signal_SIGINT) { printf("SIGINT received\n"); exit(0); }
        if (signal_SIGQUIT) { printf("SIGQUIT received\n"); exit(0); }
        if (signal_SIGHUP) { printf("SIGHUP received\n"); exit(0); }
        if (signal_SIGTERM) { printf("SIGTERM received\n"); exit(0); }
        if (signal_SIGABRT) { printf("SIGABRT received\n"); exit(0); }
        if (signal_SIGCONT) { signal_SIGCONT = 0; printf("SIGCONT received\n"); }
        if (signal_SIGTSTP) { signal_SIGTSTP = 0; printf("SIGTSTP received\n"); raise(SIGTSTP); }
    }

    return 0;
}
