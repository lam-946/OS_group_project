// src/scheduler/mlfq.c
#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "queue.h"
#include "gpu.h"

void run_mlfq(Task tasks[], int n, GPU* gpu, int quanta[], int num_queues) {
    Queue** qs = malloc(sizeof(Queue*) * num_queues);
    for (int i = 0; i < num_queues; i++) {
        qs[i] = init_queue(n * 2);
    }

    int completed = 0;
    int current_time = 0;
    
    int* arrived = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        tasks[i].remaining_time = tasks[i].compute_time;
        tasks[i].queue_level = 0;
    }

    Task* active_task = NULL;
    int current_quantum_used = 0;

    // Process initial arrivals at current_time = 0
    for (int i = 0; i < n; i++) {
        if (!arrived[i] && tasks[i].arrive_time <= current_time) {
            enqueue(qs[0], &tasks[i]);
            arrived[i] = 1;
        }
    }

    while (completed < n) {
        if (active_task != NULL) {
            // Check preemption
            int active_q = active_task->queue_level;
            int higher_priority_exists = 0;
            for (int i = 0; i < active_q; i++) {
                if (!is_empty(qs[i])) {
                    higher_priority_exists = 1;
                    break;
                }
            }

            if (higher_priority_exists) {
                release(gpu, active_task->memory_required);
                enqueue(qs[active_task->queue_level], active_task);
                active_task = NULL;
                current_quantum_used = 0;
            }
        }

        if (active_task == NULL) {
            for (int i = 0; i < num_queues; i++) {
                if (!is_empty(qs[i])) {
                    Task* t = dequeue(qs[i]);
                    if (!allocate(gpu, t->memory_required)) {
                        enqueue(qs[i], t);
                        break; 
                    }
                    active_task = t;
                    current_quantum_used = 0;
                    if (active_task->remaining_time == active_task->compute_time) {
                        active_task->start_time = current_time;
                    }
                    break;
                }
            }
        }

        if (active_task == NULL) {
            current_time++;
            for (int i = 0; i < n; i++) {
                if (!arrived[i] && tasks[i].arrive_time <= current_time) {
                    enqueue(qs[0], &tasks[i]);
                    arrived[i] = 1;
                }
            }
            continue;
        }

        // Execute active_task for 1 time step
        current_time++;
        active_task->remaining_time--;
        current_quantum_used++;

        // Process new arrivals
        for (int i = 0; i < n; i++) {
            if (!arrived[i] && tasks[i].arrive_time <= current_time) {
                enqueue(qs[0], &tasks[i]);
                arrived[i] = 1;
            }
        }

        if (active_task->remaining_time == 0) {
            release(gpu, active_task->memory_required);
            completed++;
            active_task->finish_time = current_time;
            active_task->turnaround_time = active_task->finish_time - active_task->arrive_time;
            active_task->waiting_time = active_task->turnaround_time - active_task->compute_time;
            active_task = NULL;
        } else if (current_quantum_used == quanta[active_task->queue_level]) {
            release(gpu, active_task->memory_required);
            if (active_task->queue_level < num_queues - 1) {
                active_task->queue_level++;
            }
            enqueue(qs[active_task->queue_level], active_task);
            active_task = NULL;
        }
    }

    for (int i = 0; i < num_queues; i++) {
        free_queue(qs[i]);
    }
    free(qs);
    free(arrived);
}
