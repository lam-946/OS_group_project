// include/gpu.h
#ifndef GPU_H
#define GPU_H

typedef struct GPU {
    int total_memory;
    int used_memory;
    int max_concurrent;
    int running_tasks;
} GPU;

int can_allocate(GPU* gpu, int mem);
int allocate(GPU* gpu, int mem);
void release(GPU* gpu, int mem);

#endif