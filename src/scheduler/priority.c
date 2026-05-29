// src/scheduler/priority.c
#include <stdio.h>
#include "task.h"
#include "gpu.h"

int all_done(Task tasks[], int n) {
    for (int i = 0; i < n; i++) {
        if (tasks[i].remaining_time > 0)
            return 0;
    }

    return 1;
}

int select_highest_priority(Task tasks[], int n, int current_time) {

    int best = -1;

    for (int i = 0; i < n; i++) {

        if (tasks[i].arrive_time <= current_time &&
            tasks[i].remaining_time > 0) {

            if (best == -1 ||
                tasks[i].priority < tasks[best].priority) {

                best = i;
            }
        }
    }

    return best;
}

void run_priority(Task tasks[], int n, GPU* gpu) {

    int current_time = 0;

    for (int i = 0; i < n; i++) {
        tasks[i].remaining_time = tasks[i].compute_time;
    }

    while (!all_done(tasks, n)) {

        int idx = select_highest_priority(tasks, n, current_time);

        if (idx == -1) {
            current_time++;
            continue;
        }

        Task* current = &tasks[idx];

        if (!allocate(gpu, current->memory_required)) {
            current_time++;
            continue;
        }

        current->start_time = current_time;

        current_time += current->compute_time;

        current->remaining_time = 0;

        current->finish_time = current_time;

        current->turnaround_time =
            current->finish_time - current->arrive_time;

        current->waiting_time =
            current->turnaround_time - current->compute_time;

        release(gpu, current->memory_required);
    }
}