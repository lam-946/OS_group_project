#include <stdio.h>
#include <stdlib.h>
#include "task.h"

Task* create_task(int id, int arrive, int compute, int transfer,
    int transfer_back, int mem_req, int prio){
    Task* t = (Task*)malloc(sizeof(Task));

    if (t == NULL){
        return NULL;
    }   

    t->id = id;
    t->arrive_time = arrive;
    t->compute_time = compute;
    t->transfer_time = transfer;
    t->transfer_back_time = transfer_back;
    t->memory_required = mem_req;
    t->priority = prio;

    t->remaining_time = compute;
    t->queue_level = 0;
    t->start_time = -1;
    t->finish_time = -1;
    t->waiting_time = 0;
    t->turnaround_time = 0;
    return t;
}

void free_task(Task* t){
    if (t !=NULL){
        free(t);
    }
}