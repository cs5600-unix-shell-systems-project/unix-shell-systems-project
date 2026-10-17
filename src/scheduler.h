// scheduler.h
// Declares the FIFO scheduler interface.
// Other files include this header to use the scheduler.

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"

// Max number of jobs the queue can hold.
#define MAX_JOBS 128

// A simple FIFO queue of jobs.
// Jobs leave in the same order they were added.
typedef struct {
    Job jobs[MAX_JOBS];   // Storage for the jobs.
    int count;            // How many jobs are in the queue now.
    int head;             // Index of the next job to remove.
} FifoQueue;

// Set up an empty queue before use.
void fifo_init(FifoQueue *queue);

// Add one job to the back of the queue.
// Returns 1 on success, 0 if the queue is full.
int fifo_enqueue(FifoQueue *queue, Job job);

// Remove and return the job at the front.
// The caller must check fifo_is_empty first.
Job fifo_dequeue(FifoQueue *queue);

// Check if the queue has no jobs.
// Returns 1 if empty, 0 if it still has jobs.
int fifo_is_empty(FifoQueue *queue);

#endif