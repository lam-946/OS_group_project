#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/task.h"
#include "../include/gpu.h"
#include "../include/scheduler.h"
#include "../include/benchmark.h" 

#define NUM_TASKS 20
#define TOTAL_VRAM 8000 // Giả sử GPU có 8000 MB VRAM

// Hàm tạo bản sao task để lúc chạy thuật toán tác động vào task không làm thay đổi input của thuật toán kế tiếp
void copy_task_list(Task d[], Task src[], int num_tasks) {
    for (int i = 0; i < num_tasks; i++) {
        d[i] = src[i];
    }
}

int main() {
    srand(time(NULL));

    // khởi tạo GPU

    GPU my_gpu;
    my_gpu.total_memory = TOTAL_VRAM;
    my_gpu.used_memory = 0;
    my_gpu.max_concurrent = 10;
    my_gpu.running_tasks = 0;

    // tạo task ngẫu nhiên và lưu vào original_tasks
    Task original_tasks[NUM_TASKS];    
    printf("========================================================\n");
    printf(" DANH SACH %d TASK KHOI TAO\n", NUM_TASKS);
    printf("========================================================\n");
    
    for (int i = 0; i < NUM_TASKS; i++) {
        int id = i + 1;
        int arr = rand() % 50;            
        int comp = (rand() % 10 + 1) * 5; 
        int mem = (rand() % 8 + 1) * 500; 
        int prio = rand() % 5 + 1;        
        int trans = mem / 500;            
        int trans_back = mem / 1000;      
        
        Task* t = create_task(id, arr, comp, trans, trans_back, mem, prio);
        
        original_tasks[i] = *t; 
        
        printf("Task_%02d | Arrive: %2d | Compute: %2d | Mem: %4d MB | Pri: %d\n",
               original_tasks[i].id, arr, comp, mem, prio);
               
        free(t); 
    }

    // In phần Header (tiêu đề cột) của bảng Benchmark ra màn hình
    print_benchmark_header();
// --- Chạy FCFS ---
    Task tasks_for_fcfs[NUM_TASKS]; 
    copy_task_list(tasks_for_fcfs, original_tasks, NUM_TASKS); 
    my_gpu.used_memory = 0;   
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_fcfs, NUM_TASKS, &my_gpu, POLICY_FCFS);
    print_benchmark_row(tasks_for_fcfs, NUM_TASKS, "FCFS");
    
    // --- Chạy Round Robin ---
    Task tasks_for_rr[NUM_TASKS];
    copy_task_list(tasks_for_rr, original_tasks, NUM_TASKS); 
    my_gpu.used_memory = 0;    
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_rr, NUM_TASKS, &my_gpu, POLICY_ROUND_ROBIN);
    print_benchmark_row(tasks_for_rr, NUM_TASKS, "Round Robin");

    // --- Chạy Priority ---
    Task tasks_for_pri[NUM_TASKS]; 
    copy_task_list(tasks_for_pri, original_tasks, NUM_TASKS); 
    my_gpu.used_memory = 0;  
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_pri, NUM_TASKS, &my_gpu, POLICY_PRIORITY);
    print_benchmark_row(tasks_for_pri, NUM_TASKS, "Priority");
    // Chạy SJF Non-Preemptive
    Task tasks_for_sjf_np[NUM_TASKS]; 
    copy_task_list(tasks_for_sjf_np, original_tasks, NUM_TASKS); 
    my_gpu.used_memory = 0;    
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_sjf_np, NUM_TASKS, &my_gpu, POLICY_SJF_NON_PREEMPTIVE);
    print_benchmark_row(tasks_for_sjf_np, NUM_TASKS, "SJF (Non-Preep)");

    // Chạy SJF Preemptive 
    Task tasks_for_sjf_p[NUM_TASKS]; 
    copy_task_list(tasks_for_sjf_p, original_tasks, NUM_TASKS); 
    my_gpu.used_memory = 0;    
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_sjf_p, NUM_TASKS, &my_gpu, POLICY_SJF_PREEMPTIVE);
    print_benchmark_row(tasks_for_sjf_p, NUM_TASKS, "SJF (Preemptive)");

    // Chạy MLFQ
    Task tasks_for_mlfq[NUM_TASKS]; 
    copy_task_list(tasks_for_mlfq, original_tasks, NUM_TASKS); 
    my_gpu.used_memory = 0;    
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_mlfq, NUM_TASKS, &my_gpu, POLICY_MLFQ);
    print_benchmark_row(tasks_for_mlfq, NUM_TASKS, "MLFQ");
    printf("\nHoan thanh mo phong!\n");

    return 0;


}