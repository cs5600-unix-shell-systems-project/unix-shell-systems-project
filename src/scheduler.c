// scheduler.c
// Implements the FIFO scheduler declared in scheduler.h.

#include "scheduler.h"

// Set up an empty queue before use.
void fifo_init(FifoQueue *queue) {
    queue->count = 0;   // No jobs yet.
    queue->head = 0;    // Next job to remove is at index 0.
}

// Add one job to the back of the queue.
// Returns 1 on success, 0 if the queue is full.
int fifo_enqueue(FifoQueue *queue, Job job) {
    // Check if there is room.
    if (queue->count >= MAX_JOBS) {
        return 0;   // Queue is full.
    }
    int tail = queue->head + queue->count;   // Index of the back.
    queue->jobs[tail] = job;                  // Store the job.
    queue->count = queue->count + 1;          // One more job now.
    return 1;
}

// Remove and return the job at the front.
// The caller must check fifo_is_empty first.
Job fifo_dequeue(FifoQueue *queue) {
    Job job = queue->jobs[queue->head];   // Take the front job.
    queue->head = queue->head + 1;        // Move head forward.
    queue->count = queue->count - 1;      // One less job now.
    return job;
}

// Check if the queue has no jobs.
// Returns 1 if empty, 0 if it still has jobs.
int fifo_is_empty(FifoQueue *queue) {
    if (queue->count == 0) {
        return 1;   // Empty.
    }
    return 0;       // Still has jobs.
}
