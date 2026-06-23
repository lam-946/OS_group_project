#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/task.h"
#include "../include/gpu.h"
#include "../include/scheduler.h"
#include "../include/benchmark.h" 

// Hàm tạo bản sao task để lúc chạy thuật toán tác động vào task không làm thay đổi input của thuật toán kế tiếp
void copy_task_list(Task d[], Task src[], int num_tasks) {
    for (int i = 0; i < num_tasks; i++) {
        d[i] = src[i];
    }
}

int main() {
    srand(time(NULL));

    int num_tasks, total_vram;
    printf("Nhap so luong Task: ");
    scanf("%d", &num_tasks);
    printf("Nhap dung luong VRAM (MB): ");
    scanf("%d", &total_vram);

    int run_number = get_next_run_number(total_vram);

    // khởi tạo GPU
    GPU my_gpu;
    my_gpu.total_memory = total_vram;
    my_gpu.used_memory = 0;
    my_gpu.max_concurrent = 10;
    my_gpu.running_tasks = 0;

    // tạo task ngẫu nhiên và lưu vào original_tasks
    Task *original_tasks = malloc(num_tasks * sizeof(Task));    
    printf("========================================================\n");
    printf(" DANH SACH %d TASK KHOI TAO\n", num_tasks);
    printf("========================================================\n");
    
    for (int i = 0; i < num_tasks; i++) {
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

    Task *tasks_for_algo = malloc(num_tasks * sizeof(Task));

    // --- Chạy FCFS ---
    copy_task_list(tasks_for_algo, original_tasks, num_tasks); 
    my_gpu.used_memory = 0;    // Reset RAM
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_algo, num_tasks, &my_gpu, POLICY_FCFS);
    print_benchmark_row(tasks_for_algo, num_tasks, "FCFS");
    export_benchmark_csv(tasks_for_algo, num_tasks, "FCFS", total_vram, run_number);
    
    // --- Chạy Round Robin ---
    copy_task_list(tasks_for_algo, original_tasks, num_tasks); 
    my_gpu.used_memory = 0;    // Reset RAM
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_algo, num_tasks, &my_gpu, POLICY_ROUND_ROBIN);
    print_benchmark_row(tasks_for_algo, num_tasks, "Round Robin");
    export_benchmark_csv(tasks_for_algo, num_tasks, "Round Robin", total_vram, run_number);

    // --- Chạy Priority ---
    copy_task_list(tasks_for_algo, original_tasks, num_tasks); 
    my_gpu.used_memory = 0;    // Reset RAM
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_algo, num_tasks, &my_gpu, POLICY_PRIORITY);
    print_benchmark_row(tasks_for_algo, num_tasks, "Priority");
    export_benchmark_csv(tasks_for_algo, num_tasks, "Priority", total_vram, run_number);

    // --- Chạy SJF Non-Preemptive ---
    copy_task_list(tasks_for_algo, original_tasks, num_tasks); 
    my_gpu.used_memory = 0;    // Reset RAM
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_algo, num_tasks, &my_gpu, POLICY_SJF_NON_PREEMPTIVE);
    print_benchmark_row(tasks_for_algo, num_tasks, "SJF (NP)");
    export_benchmark_csv(tasks_for_algo, num_tasks, "SJF (NP)", total_vram, run_number);

    // --- Chạy SJF Preemptive ---
    copy_task_list(tasks_for_algo, original_tasks, num_tasks); 
    my_gpu.used_memory = 0;    // Reset RAM
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_algo, num_tasks, &my_gpu, POLICY_SJF_PREEMPTIVE);
    print_benchmark_row(tasks_for_algo, num_tasks, "SJF (Pre)");
    export_benchmark_csv(tasks_for_algo, num_tasks, "SJF (Pre)", total_vram, run_number);

    // --- Chạy MLFQ ---
    copy_task_list(tasks_for_algo, original_tasks, num_tasks); 
    my_gpu.used_memory = 0;    // Reset RAM
    my_gpu.running_tasks = 0;
    run_scheduler(tasks_for_algo, num_tasks, &my_gpu, POLICY_MLFQ);
    print_benchmark_row(tasks_for_algo, num_tasks, "MLFQ");
    export_benchmark_csv(tasks_for_algo, num_tasks, "MLFQ", total_vram, run_number);

    free(tasks_for_algo);
    free(original_tasks);

    generate_algorithm_recommendation(total_vram);

    printf("\nHoan thanh mo phong!\n");
    return 0;
}