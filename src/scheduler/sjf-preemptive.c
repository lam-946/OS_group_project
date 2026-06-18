// src/scheduler/sjf-preemptive.c
#include <stdio.h>
#include "task.h"
#include "gpu.h"

int all_done_sjf_p(Task tasks[], int n) {
    for (int i = 0; i < n; i++) {
        if (tasks[i].remaining_time > 0)
            return 0;
    }
    return 1;
}

int select_shortest_remaining(Task tasks[], int n, int current_time) {
    int best = -1;
    for (int i = 0; i < n; i++) {
        if (tasks[i].arrive_time <= current_time && tasks[i].remaining_time > 0) {
            if (best == -1 || tasks[i].remaining_time < tasks[best].remaining_time) {
                best = i;
            }
        }
    }
    return best;
}

void run_sjf_p(Task tasks[], int n, GPU* gpu) {
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        tasks[i].remaining_time = tasks[i].compute_time;
    }

    while (!all_done_sjf_p(tasks, n)) {
        int idx = select_shortest_remaining(tasks, n, current_time);

        if (idx == -1) {
            current_time++;
            continue;
        }

        Task* current = &tasks[idx];

        if (!allocate(gpu, current->memory_required)) {
            current_time++;
            continue;
        }

        if (current->remaining_time == current->compute_time) {
            current->start_time = current_time;
        }

        int exec_time = current->remaining_time;
        int next_arrival = -1;
        
        for (int i = 0; i < n; i++) {
            if (tasks[i].arrive_time > current_time) {
                if (next_arrival == -1 || tasks[i].arrive_time < next_arrival) {
                    next_arrival = tasks[i].arrive_time;
                }
            }
        }

        if (next_arrival != -1 && (next_arrival - current_time) < exec_time) {
            exec_time = next_arrival - current_time;
        }

        current_time += exec_time;
        current->remaining_time -= exec_time;

        release(gpu, current->memory_required);

        if (current->remaining_time == 0) {
            current->finish_time = current_time;
            current->turnaround_time = current->finish_time - current->arrive_time;
            current->waiting_time = current->turnaround_time - current->compute_time;
        }
    }
}
