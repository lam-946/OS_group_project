// src/queue/queue.c
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* init_queue(int capacity) {
    Queue* q = (Queue*)malloc(sizeof(Queue));

    q->tasks = (Task**)malloc(sizeof(Task*) * capacity);
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->capacity = capacity;

    return q;
}

void enqueue(Queue* q, Task* t) {
    if (q->size == q->capacity)
        return;

    q->rear = (q->rear + 1) % q->capacity;
    q->tasks[q->rear] = t;
    q->size++;
}

Task* dequeue(Queue* q) {
    if (q->size == 0)
        return NULL;

    Task* t = q->tasks[q->front];

    q->front = (q->front + 1) % q->capacity;
    q->size--;

    return t;
}

int is_empty(Queue* q) {
    return q->size == 0;
}

void free_queue(Queue* q) {
    free(q->tasks);
    free(q);
}