// include/queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "task.h"

typedef struct Queue {
    Task** tasks;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

Queue* init_queue(int capacity);
void enqueue(Queue* q, Task* t);
Task* dequeue(Queue* q);
int is_empty(Queue* q);
void free_queue(Queue* q);

#endif