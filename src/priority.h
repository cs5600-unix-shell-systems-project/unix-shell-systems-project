// priority.h
// Declares the priority scheduler interface.
// For Sprint 2 this is a skeleton. The real binary heap comes in Sprint 3.

#ifndef PRIORITY_H
#define PRIORITY_H

#include "job.h"

// Max number of jobs the priority queue can hold.
#define MAX_PRIORITY_JOBS 128

// A priority queue of jobs.
// For now it stores jobs in a plain array.
// Sprint 3 will replace the internals with a real binary heap.
typedef struct {
    Job jobs[MAX_PRIORITY_JOBS];   // Storage for the jobs.
    int count;                     // How many jobs are in the queue now.
} PriorityQueue;

// Set up an empty priority queue before use.
void priority_init(PriorityQueue *queue);

// Add one job to the queue.
// Returns 1 on success, 0 if the queue is full.
int priority_enqueue(PriorityQueue *queue, Job job);

// Remove and return the highest-priority job.
// Ties are broken by submission order: earlier goes first.
// The caller must check priority_is_empty first.
Job priority_dequeue(PriorityQueue *queue);

// Check if the queue has no jobs.
// Returns 1 if empty, 0 if it still has jobs.
int priority_is_empty(PriorityQueue *queue);

#endif