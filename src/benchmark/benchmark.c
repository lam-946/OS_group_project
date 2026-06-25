#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int get_next_run_number(int vram) {
    int vgb = vram >= 1000 ? vram / 1000 : vram;
    char filename[256];
    sprintf(filename, "src/data/%dgb_gpu/benchmark_results.csv", vgb);
    
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return 1;
    }

    char line[512];
    int max_run = 0;
    int is_first = 1;
    while (fgets(line, sizeof(line), f)) {
        if (is_first) {
            is_first = 0;
            continue;
        }
        int current_run = 0;
        if (sscanf(line, "%d,", &current_run) == 1) {
            if (current_run > max_run) {
                max_run = current_run;
            }
        }
    }
    fclose(f);
    return max_run + 1;
}

void export_benchmark_csv(Task tasks[], int num_tasks, const char* policy_name, int vram, int run_number) {
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

    float avg_waiting = total_waiting / num_tasks;
    float avg_turnaround = total_turnaround / num_tasks;
    float throughput = makespan > 0 ? (float)num_tasks / makespan : 0;

    // Create directories
    system("if not exist \"src\\data\" mkdir \"src\\data\"");
    
    int vgb = vram >= 1000 ? vram / 1000 : vram;
    char dir_cmd[256];
    sprintf(dir_cmd, "if not exist \"src\\data\\%dgb_gpu\" mkdir \"src\\data\\%dgb_gpu\"", vgb, vgb);
    system(dir_cmd);

    // Open file
    char filename[256];
    sprintf(filename, "src/data/%dgb_gpu/benchmark_results.csv", vgb);
    
    FILE *f = fopen(filename, "r");
    int write_header = 0;
    if (f == NULL) {
        write_header = 1;
    } else {
        fclose(f);
    }

    f = fopen(filename, "a");
    if (f == NULL) {
        printf("Khong the mo file de ghi CSV!\n");
        return;
    }

    if (write_header) {
        // UTF-8 BOM
        fprintf(f, "\xEF\xBB\xBF");
        fprintf(f, "Lần chạy,Thuật toán,Số lượng Task,Makespan,Thời gian chờ (Avg),Thời gian lưu (Avg),Throughput\n");
    }

    fprintf(f, "%d,%s,%d,%d,%.2f,%.2f,%.4f\n", 
            run_number, policy_name, num_tasks, makespan, avg_waiting, avg_turnaround, throughput);
    
    fclose(f);
}

typedef struct {
    char policy[50];
    int num_tasks;
    int count;
    float sum_makespan;
    float sum_waiting;
    float sum_turnaround;
    float sum_throughput;
} AlgoStats;

void generate_algorithm_recommendation(int vram) {
    int vgb = vram >= 1000 ? vram / 1000 : vram;
    char in_file[256], out_file[256];
    sprintf(in_file, "src/data/%dgb_gpu/benchmark_results.csv", vgb);
    sprintf(out_file, "src/data/%dgb_gpu/algorithm_recommendation.csv", vgb);

    FILE *fin = fopen(in_file, "r");
    if (!fin) return;

    AlgoStats stats[20];
    int num_stats = 0;

    char line[512];
    int is_first = 1;
    while (fgets(line, sizeof(line), fin)) {
        if (is_first) {
            is_first = 0;
            continue;
        }
        int run, tasks, makespan; 
        float wait, turn, thr; 
        char pol[50];
        if (sscanf(line, "%d,%49[^,],%d,%d,%f,%f,%f", &run, pol, &tasks, &makespan, &wait, &turn, &thr) == 7) {
            int found = -1;
            for (int i = 0; i < num_stats; i++) {
                if (strcmp(stats[i].policy, pol) == 0) {
                    found = i;
                    break;
                }
            }
            if (found == -1 && num_stats < 20) {
                strcpy(stats[num_stats].policy, pol);
                stats[num_stats].num_tasks = 0;
                stats[num_stats].count = 1;
                stats[num_stats].sum_makespan = makespan;
                stats[num_stats].sum_waiting = wait;
                stats[num_stats].sum_turnaround = turn;
                stats[num_stats].sum_throughput = thr;
                num_stats++;
            } else if (found != -1) {
                stats[found].count++;
                stats[found].sum_makespan += makespan;
                stats[found].sum_waiting += wait;
                stats[found].sum_turnaround += turn;
                stats[found].sum_throughput += thr;
            }
        }
    }
    fclose(fin);

    FILE *fout = fopen(out_file, "w");
    if (!fout) return;

    fprintf(fout, "\xEF\xBB\xBF");
    fprintf(fout, "Thuật toán,Trung bình Makespan,Trung bình Thời gian chờ,Trung bình Thời gian lưu,Trung bình Throughput,Ghi chú\n");

    float min_turn = 1e9, min_wait = 1e9, min_makespan = 1e9, max_thr = -1;
    float max_turn = -1, max_wait = -1, max_makespan = -1, min_thr = 1e9;
    
    for (int i = 0; i < num_stats; i++) {
        if (stats[i].count > 0) {
            float avg_turn = stats[i].sum_turnaround / stats[i].count;
            float avg_wait = stats[i].sum_waiting / stats[i].count;
            float avg_make = stats[i].sum_makespan / stats[i].count;
            float avg_thr = stats[i].sum_throughput / stats[i].count;
            if (avg_turn < min_turn) min_turn = avg_turn;
            if (avg_wait < min_wait) min_wait = avg_wait;
            if (avg_make < min_makespan) min_makespan = avg_make;
            if (avg_thr > max_thr) max_thr = avg_thr;

            if (avg_turn > max_turn) max_turn = avg_turn;
            if (avg_wait > max_wait) max_wait = avg_wait;
            if (avg_make > max_makespan) max_makespan = avg_make;
            if (avg_thr < min_thr) min_thr = avg_thr;
        }
    }

    for (int i = 0; i < num_stats; i++) {
        if (stats[i].count > 0) {
            float avg_turn = stats[i].sum_turnaround / stats[i].count;
            float avg_wait = stats[i].sum_waiting / stats[i].count;
            float avg_make = stats[i].sum_makespan / stats[i].count;
            float avg_thr = stats[i].sum_throughput / stats[i].count;
            
            char note[256] = "";
            if (avg_turn <= min_turn + 0.001 && min_turn < max_turn - 0.001) strcat(note, "Best Turnaround; ");
            if (avg_wait <= min_wait + 0.001 && min_wait < max_wait - 0.001) strcat(note, "Best Waiting; ");
            if (avg_make <= min_makespan + 0.001 && min_makespan < max_makespan - 0.001) strcat(note, "Best Makespan; ");
            if (avg_thr >= max_thr - 0.001 && max_thr > min_thr + 0.001) strcat(note, "Best Throughput; ");
            
            fprintf(fout, "%s,%.2f,%.2f,%.2f,%.4f,%s\n", 
                    stats[i].policy, avg_make, avg_wait, avg_turn, avg_thr, note);
        }
    }
    fclose(fout);
}