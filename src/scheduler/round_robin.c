// src/scheduler/rr.c
#include <stdio.h>
#include "task.h"
#include "queue.h"
#include "gpu.h"

void run_rr(Task tasks[], int n, GPU* gpu, int quantum) {

    Queue* q = init_queue(n * 2);

    int completed = 0;
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        tasks[i].remaining_time = tasks[i].compute_time;
    }

    int arrived[n];

    for (int i = 0; i < n; i++)
        arrived[i] = 0;

    while (completed < n) {

        for (int i = 0; i < n; i++) {
            if (!arrived[i] && tasks[i].arrive_time <= current_time) {
                enqueue(q, &tasks[i]);
                arrived[i] = 1;
            }
        }

        if (is_empty(q)) {
            current_time++;
            continue;
        }

        Task* current = dequeue(q);

        if (!allocate(gpu, current->memory_required)) {
            enqueue(q, current);
            current_time++;
            continue;
        }

        if (current->remaining_time == current->compute_time) {
            current->start_time = current_time;
        }

        int exec_time;

        if (current->remaining_time > quantum)
            exec_time = quantum;
        else
            exec_time = current->remaining_time;

        current_time += exec_time;
        current->remaining_time -= exec_time;

        release(gpu, current->memory_required);

        for (int i = 0; i < n; i++) {
            if (!arrived[i] && tasks[i].arrive_time <= current_time) {
                enqueue(q, &tasks[i]);
                arrived[i] = 1;
            }
        }

        if (current->remaining_time > 0) {
            enqueue(q, current);
        }
        else {
            completed++;

            current->finish_time = current_time;
            current->turnaround_time =
                current->finish_time - current->arrive_time;
}
    }

    free_queue(q);
}