#ifndef BENCHMARK_H
#define BENCHMARK_H
#include "task.h"

void print_benchmark_header();
void print_benchmark_row(Task tasks[], int num_tasks, const char* policy_name);

#endif