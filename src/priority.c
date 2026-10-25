// priority.c
// Skeleton implementation of the priority scheduler.
// For Sprint 2, it uses a simple linear scan to find the best job.
// Sprint 3 will replace this with a real binary heap.

#include "priority.h"

// Set up an empty priority queue before use.
void priority_init(PriorityQueue *queue) {
    queue->count = 0;   // No jobs yet.
}

// Add one job to the queue.
// Returns 1 on success, 0 if the queue is full.
int priority_enqueue(PriorityQueue *queue, Job job) {
    // Check if there is room.
    if (queue->count >= MAX_PRIORITY_JOBS) {
        return 0;   // Queue is full.
    }
    queue->jobs[queue->count] = job;     // Put the job at the end.
    queue->count = queue->count + 1;     // One more job now.
    return 1;
}

// Remove and return the highest-priority job.
// Ties are broken by submission order: earlier goes first.
// The caller must check priority_is_empty first.
Job priority_dequeue(PriorityQueue *queue) {
    // Step 1: scan all jobs and find the best one.
    // "Best" means highest priority.
    // If two jobs tie on priority, the smaller submission_order wins.
    int best = 0;   // Index of the best job found so far.
    int i;
    for (i = 1; i < queue->count; i++) {
        int higher_priority = queue->jobs[i].priority > queue->jobs[best].priority;
        int same_priority = queue->jobs[i].priority == queue->jobs[best].priority;
        int earlier = queue->jobs[i].submission_order < queue->jobs[best].submission_order;

        if (higher_priority == 1) {
            best = i;   // This job has higher priority.
        } else if (same_priority == 1 && earlier == 1) {
            best = i;   // Same priority, but submitted earlier.
        }
    }

    // Step 2: save the best job to return it later.
    Job best_job = queue->jobs[best];

    // Step 3: remove it by shifting the last job into its slot.
    // This keeps the array packed with no holes.
    queue->jobs[best] = queue->jobs[queue->count - 1];
    queue->count = queue->count - 1;

    return best_job;
}

// Check if the queue has no jobs.
// Returns 1 if empty, 0 if it still has jobs.
int priority_is_empty(PriorityQueue *queue) {
    if (queue->count == 0) {
        return 1;   // Empty.
    }
    return 0;       // Still has jobs.
}