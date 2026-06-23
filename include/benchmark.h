#ifndef BENCHMARK_H
#define BENCHMARK_H
#include "task.h"

void print_benchmark_header();
void print_benchmark_row(Task tasks[], int num_tasks, const char* policy_name);
int get_next_run_number(int vram);
void export_benchmark_csv(Task tasks[], int num_tasks, const char* policy_name, int vram, int run_number);
void generate_algorithm_recommendation(int vram);

#endif