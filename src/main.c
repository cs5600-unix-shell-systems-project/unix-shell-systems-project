// main.c
// Entry point for the shell project.
// For Sprint 2, this just tests the FIFO queue.

#include <stdio.h>
#include "scheduler.h"

int main(void) {
    // Make an empty queue.
    FifoQueue queue;
    fifo_init(&queue);

    // Build three fake jobs and enqueue them.
    // We only set id and priority here. argv is filled later.
    int i;
    for (i = 1; i <= 3; i++) {
        Job job;
        job.id = i;
        job.priority = 2;
        job.submission_order = i;
        job.argv[0] = NULL;   // No real command yet.

        fifo_enqueue(&queue, job);
        printf("Enqueued job %d\n", job.id);
    }

    printf("---\n");

    // Dequeue all jobs and print the order.
    // FIFO means they should come out as 1, 2, 3.
    while (fifo_is_empty(&queue) == 0) {
        Job job = fifo_dequeue(&queue);
        printf("Dequeued job %d\n", job.id);
    }

    return 0;
}