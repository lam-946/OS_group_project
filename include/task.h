// include/task.h
#ifndef TASK_H
#define TASK_H

typedef struct Task {
    int id;
    int arrive_time;
    int compute_time;
    int transfer_time;
    int transfer_back_time;
    int memory_required;
    int priority;

    int remaining_time;
    int queue_level;

    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;

} Task;

Task* create_task(int id, int arrive, int compute, int transfer, int transfer_back, int mem_req, int prio);
void free_task(Task* t);
void print_task(Task* t);

#endif