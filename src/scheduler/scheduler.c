#include "../../include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>

void run_scheduler(Task tasks[], int num_tasks, GPU* gpu, PolicyType policy) {
    switch (policy) {
        case POLICY_FCFS:
            run_fcfs(tasks, num_tasks, gpu); 
            break;
        case POLICY_SJF_NON_PREEMPTIVE:
            run_sjf_np(tasks, num_tasks, gpu); 
            break;
        case POLICY_SJF_PREEMPTIVE:
            run_sjf_p(tasks, num_tasks, gpu); 
            break;
        case POLICY_PRIORITY:
            run_priority(tasks, num_tasks, gpu); 
            break;
        case POLICY_ROUND_ROBIN:
            run_rr(tasks, num_tasks, gpu, 10); // quantum = 10ms
            break;
        case POLICY_MLFQ:
            {
                int quantums[] = {10, 20, 40};
                run_mlfq(tasks, num_tasks, gpu, quantums, 3);
            }
            break;
        default:
            printf("Loi: Khong tim thay chinh sach!\n");
    }
}