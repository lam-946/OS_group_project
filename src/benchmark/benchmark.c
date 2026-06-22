#include <stdio.h>
#include "benchmark.h"

void print_benchmark_header() {
    printf("\n=================================================================================\n");
    printf("| %-15s | %-12s | %-20s | %-20s |\n", "Thuat Toan", "Makespan", "Thoi gian cho (Avg)", "Thoi gian luu (Avg)");
    printf("=================================================================================\n");
}

void print_benchmark_row(Task tasks[], int num_tasks, const char* policy_name) {
    float total_waiting = 0;
    float total_turnaround = 0;
    int makespan = 0;

    for (int i = 0; i < num_tasks; i++) {
        total_waiting += tasks[i].waiting_time;
        total_turnaround += tasks[i].turnaround_time;
        
        if (tasks[i].finish_time > makespan) {
            makespan = tasks[i].finish_time;
        }
    }

    printf("| %-15s | %-12d | %-20.2f | %-20.2f |\n", 
           policy_name, makespan, 
           total_waiting / num_tasks, 
           total_turnaround / num_tasks);
    printf("---------------------------------------------------------------------------------\n");
}