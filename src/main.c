// main.c
// Entry point for the shell project.
// For Sprint 2, this tests both the FIFO queue and the priority queue.

#include <stdio.h>
#include "scheduler.h"
#include "priority.h"

// Build a fake job with the given id, priority, and submission order.
// argv is left empty for now. Real commands come later.
Job make_job(int id, int priority, int order) {
    Job job;
    job.id = id;
    job.priority = priority;
    job.submission_order = order;
    job.argv[0] = NULL;   // No real command yet.
    return job;
}

int main(void) {
    // ---- Test 1: FIFO queue ----
    // Jobs should come out in submission order: 1, 2, 3.
    printf("FIFO test:\n");

    FifoQueue fifo;
    fifo_init(&fifo);

    int i;
    for (i = 1; i <= 3; i++) {
        Job job = make_job(i, 2, i);
        fifo_enqueue(&fifo, job);
    }

    while (fifo_is_empty(&fifo) == 0) {
        Job job = fifo_dequeue(&fifo);
        printf("  Ran job %d\n", job.id);
    }

    // ---- Test 2: priority queue ----
    // Three jobs with different priorities.
    // Job 1: priority 1 (low)
    // Job 2: priority 3 (high)
    // Job 3: priority 2 (medium)
    // They should come out high to low: job 2, job 3, job 1.
    printf("Priority test:\n");

    PriorityQueue pq;
    priority_init(&pq);

    priority_enqueue(&pq, make_job(1, 1, 1));   // low
    priority_enqueue(&pq, make_job(2, 3, 2));   // high
    priority_enqueue(&pq, make_job(3, 2, 3));   // medium

    while (priority_is_empty(&pq) == 0) {
        Job job = priority_dequeue(&pq);
        printf("  Ran job %d (priority %d)\n", job.id, job.priority);
    }

    return 0;
}