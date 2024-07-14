#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define DISPATCHER_TIME (50)

#include "../include/pcb_queue.h"
#include "../include/utils.h"

#define EXPECTED_ARGC (2)
#define MAX_LEN_SCHED_FILE_LINE (100)
#define DUMMY_PROG "program"

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"

// parse line from file, advancing pointer by a line.
// store parsed line into a new pcb_t to return.
// Also, sets the args fields of the pcb
pcb_t* parse_line(FILE* f)
{
    char line[MAX_LEN_SCHED_FILE_LINE + 1] = {0};
    if (!fgets(line, sizeof(line), f))
    {
        return NULL;
    }
    remove_newline(line);
    if (0 == *line) return NULL;

    pcb_t* pcb = pcb_init();
    if (!pcb) return NULL;

    /* get arrival time */
    
    char* tok = strtok(line, ",");
    if (!tok) goto cleanup;
    int arrival_time;
    if (str_to_int(tok, &arrival_time)) goto cleanup;
    pcb->arrival_time = arrival_time;

    /* get service time */
    
    tok = strtok(NULL, ",");
    if (!tok) goto cleanup;
    int service_time;
    if (str_to_int(tok, &service_time)) goto cleanup;
    pcb->remaining_time = service_time;

    /* set the args field */
    
    pcb->args[0] = malloc(strlen(DUMMY_PROG) + 1);
    strcpy(pcb->args[0], DUMMY_PROG);
    pcb->args[1] = malloc(MAX_INT_STR_SZ);
    sprintf(pcb->args[1], "%d", service_time);
    pcb->args[2] = NULL;

    /* ensure there are no trailing characters */
    
    tok = strtok(NULL, ",");
    if (tok) goto cleanup;

    return pcb;

    cleanup:
    free(pcb->args[0]);
    free(pcb->args[1]);
    free(pcb);
    return NULL;
}

// load job schedule from specified file into pcb_queue
// returns NULL on failure
// behaviour: load all tasks till eof, or invalid line
pcb_queue* load_pcb_queue(char* path)
{
    printf(COLOR_CYAN "Loading PCB queue from file: %s\n" COLOR_RESET, path);

    pcb_queue* q = pcb_queue_init();
    if (!q) return NULL;

    FILE* f = fopen(path, "r");
    if (!f)
    {
        printf(COLOR_RED "Failed to open file: %s\n" COLOR_RESET, path);
        free(q);
        return NULL;
    }

    pcb_t* new_pcb;
    int job_count = 0;
    while ((new_pcb = parse_line(f)))
    {
        pcb_queue_enqueue(q, new_pcb);
        job_count++;
    }
    fclose(f);

    printf(COLOR_GREEN "PCB queue loaded successfully. Number of jobs loaded: %d\n" COLOR_RESET, job_count);
    return q;
}

pcb_t* start_pcb(pcb_t* pcb)
{
    printf(COLOR_YELLOW "Starting PCB with arrival time: %d and service time: %d\n" COLOR_RESET,
           pcb->arrival_time, pcb->remaining_time);

    /* run program with arg <service time> */
    
    pid_t pid = fork();
    if (pid < 0) return NULL;
    pcb->pid = pid;
    if (0 == pid)
    {
        execvp(pcb->args[0], pcb->args);
        exit(1);
    }
    printf(COLOR_GREEN "PCB started with PID: %d\n" COLOR_RESET, pcb->pid);
    return pcb;
}

void run_dispatcher(pcb_queue* pcbq)
{
    int timer = DISPATCHER_TIME;
    pcb_t* currently_running = NULL;
    while (pcbq->head || currently_running)
    {
        printf(COLOR_BLUE "Timer: %d\n" COLOR_RESET, timer);
        int elapsed_time = DISPATCHER_TIME - timer;

        if (!currently_running && pcbq->head && pcbq->head->arrival_time <= elapsed_time)
        {
            currently_running = pcb_queue_dequeue(pcbq);
            if (!currently_running) break;
            currently_running->status = RUNNING;
            if (NULL == start_pcb(currently_running))
            {
                if (DEFAULT_PID_VALUE == currently_running->pid)
                {
                    printf(COLOR_RED "Dispatch unsuccessful: fork() failed\n" COLOR_RESET);
                }
                else
                {
                    waitpid(currently_running->pid, NULL, 0);
                    printf(COLOR_RED "Dispatch failed for process with PID: %d\n" COLOR_RESET,
                           currently_running->pid);
                }
                pcb_destroy(currently_running);
                currently_running = NULL;
            }
        }
        else if (currently_running)
        {
            if (0 == currently_running->remaining_time)
            {
                pid_t res = waitpid(currently_running->pid, NULL, WNOHANG);
                if (0 == res)
                {
                    kill(currently_running->pid, SIGINT);
                    waitpid(currently_running->pid, NULL, 0);
                }
                pcb_destroy(currently_running);
                currently_running = NULL;
                printf(COLOR_MAGENTA "PCB terminated and cleaned up.\n" COLOR_RESET);
            }
            else currently_running->remaining_time--;
        }

        sleep(1);
        timer--;
    }
    printf(COLOR_CYAN "Dispatcher finished.\n" COLOR_RESET);
}

int main(int argc, char** argv)
{
    if (EXPECTED_ARGC != argc)
    {
        printf(COLOR_RED "Invalid number of arguments.\n" COLOR_RESET);
        return 1;
    }
    if ('\0' == *argv[1])
    {
        printf(COLOR_RED "Invalid file path.\n" COLOR_RESET);
        return 2;
    }
    pcb_queue* pcbq = load_pcb_queue(argv[1]);
    if (!pcbq)
    {
        printf(COLOR_RED "Failed to load PCB queue.\n" COLOR_RESET);
        return 3;
    }
    run_dispatcher(pcbq);
    return 0;
}
