// include/schedular.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "gpu.h"
#include "queue.h"
#include "task.h"

typedef enum {
    POLICY_FCFS,
    POLICY_MLFQ,
    POLICY_PRIORITY,
    POLICY_ROUND_ROBIN,
    POLICY_SJF_NON_PREEMPTIVE,
    POLICY_SJF_PREEMPTIVE
} PolicyType;

void run_fcfs(Task tasks[], int n, GPU* gpu);
void run_mlfq(Task tasks[], int n, GPU* gpu, int quanta[], int num_queues);
void run_priority(Task tasks[], int n, GPU* gpu);
void run_rr(Task tasks[], int n, GPU* gpu, int quantum);
void run_sjf_np(Task tasks[], int n, GPU* gpu);
void run_sjf_p(Task tasks[], int n, GPU* gpu);

void run_scheduler(Task tasks[], int num_tasks, GPU* gpu, PolicyType policy);
#endif