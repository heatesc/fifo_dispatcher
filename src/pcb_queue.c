#include "../include/pcb_queue.h"
#include "../include/utils.h"

// create empty pcb, returns NULL on failure
pcb_t* pcb_init()
{
    pcb_t* ret = malloc(sizeof(pcb_t));
    if (!ret) return NULL;
    ret->args[0] = NULL;
    ret->pid = DEFAULT_PID_VALUE;
    ret->args[1] = NULL;
    ret->args[2] = NULL;
    ret->arrival_time = DEFAULT_ARRIVAL_TIME;
    ret->remaining_time = DEFAULT_REMAINING_TIME;
    ret->service_time = DEFAULT_SERVICE_TIME;
    ret->next = NULL;
    return ret;
}

// create empty pcb_q, returns NULL on failure
pcb_queue* pcb_queue_init()
{
    pcb_queue* ret = malloc(sizeof(pcb_queue));
    if (!ret) return NULL;
    ret->head = NULL;
    ret->tail = NULL;
    return ret;
}

void pcb_queue_enqueue(pcb_queue* q, pcb_t* new_elem)
{
    if (!q) return;
    new_elem->next = NULL;
    if (!q->head)
    {
        q->head = new_elem;
        q->tail = new_elem;
    }
    else
    {
        q->tail->next = new_elem;
        q->tail = new_elem;
    }
}

pcb_t* pcb_queue_dequeue(pcb_queue* q)
{
    if (!q || !q->head) return NULL;
    pcb_t* ret = q->head;
    q->head = q->head->next;
    if (!q->head)
    {
        q->tail = NULL;
    }
    return ret;
}

void pcb_destroy(pcb_t* pcb)
{
    if (!pcb) return;
    if (pcb->args[0]) free(pcb->args[0]);
    if (pcb->args[1]) free(pcb->args[1]);
    free(pcb);
}

void pcb_queue_destroy(pcb_queue* pcbq)
{
    while (pcbq->head) pcb_destroy(pcb_queue_dequeue(pcbq));
}
