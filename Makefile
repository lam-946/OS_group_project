CC = gcc

CFLAGS = -Wall -Iinclude

SRCS = src/main.c \
       src/task/task.c \
       src/queue/queue.c \
       src/gpu/gpu.c \
       src/benchmark/benchmark.c \
       src/scheduler/scheduler.c \
       src/scheduler/fcfs.c \
       src/scheduler/priority.c \
       src/scheduler/round_robin.c \
       src/scheduler/sjf-non-preemptive.c \
       src/scheduler/sjf-preemptive.c \
       src/scheduler/mlfq.c

TARGET = gpu_sim

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe