// src/gpu/gpu.c
#include "gpu.h"

int can_allocate(GPU* gpu, int mem) {
    return (gpu->used_memory + mem <= gpu->total_memory);
}

int allocate(GPU* gpu, int mem) {
    if (!can_allocate(gpu, mem))
        return 0;

    gpu->used_memory += mem;
    gpu->running_tasks++;

    return 1;
}

void release(GPU* gpu, int mem) {
    gpu->used_memory -= mem;
    gpu->running_tasks--;
}