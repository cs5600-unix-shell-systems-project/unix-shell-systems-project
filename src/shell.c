#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "executor.h"
#include "scheduler.h"
#include "priority.h"

#define INPUT_SIZE 1024

#define MAX_PENDING_JOBS 128

typedef struct {
    Job job;
    char args[MAX_ARGS - 1][INPUT_SIZE];
} PendingJob;

static PendingJob pending_jobs[MAX_PENDING_JOBS];
static int pending_count = 0;
static int next_job_id = 1;

static void submit_job(char *args)
{
    if (pending_count >= MAX_PENDING_JOBS) {
        fprintf(stderr, "Batch queue is full\n");
        return;
    }

    char *priority_text = strtok(args, " \t");
    if (priority_text == NULL) {
        fprintf(stderr, "Usage: submit <1-3> <command> [args...]\n");
        return;
    }

    char *end = NULL;
    long priority = strtol(priority_text, &end, 10);

    if (*end != '\0' || priority < 1 || priority > 3) {
        fprintf(stderr, "Priority must be 1, 2, or 3\n");
        return;
    }

    char *token = strtok(NULL, " \t");
    if (token == NULL) {
        fprintf(stderr, "Missing command\n");
        return;
    }

    PendingJob *pending = &pending_jobs[pending_count];
    memset(pending, 0, sizeof(*pending));

    int argc = 0;

    while (token != NULL && argc < MAX_ARGS - 1) {
        size_t length = strlen(token);

        if (length >= INPUT_SIZE) {
            fprintf(stderr, "Argument is too long\n");
            return;
        }

        memcpy(pending->args[argc], token, length + 1);
        pending->job.argv[argc] = pending->args[argc];

        argc++;
        token = strtok(NULL, " \t");
    }

    if (token != NULL) {
        fprintf(stderr, "Too many arguments\n");
        return;
    }

    pending->job.argv[argc] = NULL;
    pending->job.id = next_job_id;
    pending->job.priority = (int)priority;
    pending->job.submission_order = next_job_id;

    pending_count++;
    next_job_id++;

    printf("Job %d submitted (priority %ld)\n",
           pending->job.id, priority);
}

static void run_fifo_jobs(void)
{
    if (pending_count == 0) {
        printf("No pending jobs\n");
        return;
    }

    FifoQueue queue;
    fifo_init(&queue);

    for (int i = 0; i < pending_count; i++) {
        if (!fifo_enqueue(&queue, pending_jobs[i].job)) {
            fprintf(stderr, "Failed to enqueue Job %d\n",
                    pending_jobs[i].job.id);
            return;
        }
    }

    while (!fifo_is_empty(&queue)) {
        Job job = fifo_dequeue(&queue);

        printf("Running Job %d (priority %d)\n",
               job.id, job.priority);
        fflush(stdout);

        int status = run_job(&job);

        printf("Job %d finished with status %d\n",
               job.id, status);
    }

    pending_count = 0;
}

static void run_priority_jobs(void)
{
    if (pending_count == 0) {
        printf("No pending jobs\n");
        return;
    }

    PriorityQueue queue;
    priority_init(&queue);

    for (int i = 0; i < pending_count; i++) {
        if (!priority_enqueue(&queue, pending_jobs[i].job)) {
            fprintf(stderr, "Failed to enqueue Job %d\n",
                    pending_jobs[i].job.id);
            return;
        }
    }

    while (!priority_is_empty(&queue)) {
        Job job = priority_dequeue(&queue);

        printf("Running Job %d (priority %d)\n",
               job.id, job.priority);
        fflush(stdout);

        int status = run_job(&job);

        printf("Job %d finished with status %d\n",
               job.id, status);
    }

    pending_count = 0;
}

int main(void)
{
    char input[INPUT_SIZE];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        // Read one line of user input.
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        // Reject input that does not fit in the buffer.
        size_t length = strlen(input);

        if (length > 0 && input[length - 1] != '\n' && !feof(stdin)) {
            int ch = getchar();

            if (ch != '\n' && ch != EOF) {
                while ((ch = getchar()) != '\n' && ch != EOF) {
                    // Discard the remaining characters.
                }

                fprintf(stderr, "Input too long (maximum 1023 characters)\n");
                continue;
            }
          }

        // Remove the trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Ignore empty commands.
        if (input[0] == '\0') {
            continue;
        }

        // Exit the shell.
        if (strcmp(input, "exit") == 0) {
            break;
        }

        Job job = {0};
        int argc = 0;

        // Submit a batch job without executing it immediately.
        if (strncmp(input, "submit ", 7) == 0) {
            submit_job(input + 7);
            continue;
        }

        if (strcmp(input, "submit") == 0) {
            fprintf(stderr, "Usage: submit <1-3> <command> [args...]\n");
            continue;
        }

        // Execute pending jobs in FIFO order.
        if (strcmp(input, "run fifo") == 0) {
            run_fifo_jobs();
            continue;
        }

       // Execute pending jobs in priority order.
       if (strcmp(input, "run priority") == 0) {
            run_priority_jobs();
            continue;
        }

        // Split input into command and arguments.
        char *token = strtok(input, " \t");

        while (token != NULL && argc < MAX_ARGS - 1) {
            job.argv[argc++] = token;
            token = strtok(NULL, " \t");
        }

        job.argv[argc] = NULL;

        if (argc == 0) {
            continue;
        }

        // Execute the command.
        int status = run_job(&job);

        if (status != 0) {
            fprintf(stderr, "Command exited with status %d\n", status);
        }
    }

    return 0;
}
