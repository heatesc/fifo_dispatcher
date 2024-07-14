#ifndef FIFO_DISPATCHER_PCB_QUEUE_H
#define FIFO_DISPATCHER_PCB_QUEUE_H

#define DEFAULT_ARRIVAL_TIME (0)
#define DEFAULT_PID_VALUE (-1)
#define DEFAULT_SERVICE_TIME (0)
#define DEFAULT_REMAINING_TIME (0)

#include <stdlib.h>
#include <unistd.h>
#include <aio.h> // for pid_t

#define DEFAULT

typedef enum
{
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} status_t;

typedef struct proc_ctrl_block
{
    pid_t pid;
    char* args[3]; // process name and args, followed by NULL
    int arrival_time;
    int service_time;
    int remaining_time;
    status_t status;
    struct proc_ctrl_block* next;
} pcb_t;

typedef struct pcb_queue
{   
    pcb_t* head;
    pcb_t* tail;
} pcb_queue;


// create empty pcb, returns NULL on failure
pcb_t* pcb_init();

void pcb_destroy(pcb_t* pcb);

void pcb_queue_destroy(pcb_queue* pcbq);

// create empty pcb_q, returns NULL on failure
pcb_queue* pcb_queue_init();

void pcb_queue_enqueue(pcb_queue* q, pcb_t* new_elem);

pcb_t* pcb_queue_dequeue(pcb_queue* q);

#endif //FIFO_DISPATCHER_PCB_QUEUE_H
