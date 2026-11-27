#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "executor.h"
#include "fifo.h"
#include "priority.h"
#include "history.h"

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


static void change_directory(int argc, char *argv[]){
    // arg safety check
    if (argc > 2) {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    const char *path = argv[1];

    if(!path) {
        // if no path is included, change to root
        path = getenv("HOME");
        // if HOME path isn't set in environment
        if (!path){
            fprintf(stderr, "HOME not set in global environment variables\n");
            return;
        }
    }

    // fire off the directory change system call, pass the error through if it fails
    if (chdir(path)!=0){
        perror("cd");
    }
}


int main(void)
{
    char input[INPUT_SIZE];
    bool cmd_history = true;

    Vector history;
    if (!init_vector(&history)){
        fprintf(stderr, "Could not allocate command history; history will not be available this session\n");
        cmd_history = false;
    }

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

        //history handling

        // replaces input with old command
        if (input[0] == '!' && input[1] >= '0' && input[1]<='9') {
            char *end = NULL;
            size_t i = (size_t)strtol(input + 1, &end, 10);

            if (!cmd_history || *end != '\0' || i >= history.length) {
                fprintf(stderr, "%s: command not found\n", input);
                continue;
            }

            strcpy(input, history.commands[i]);
            printf("myshell> %s\n", input);
        }

        // push all commands into history except for history commands
        if(cmd_history && input[0]!='!'){
            vector_push(&history, input);
        }

        // print out the command history mapping 
        if(strcmp(input, "!!")==0){
            print_history(&history);
            continue;
        }

        // Exit the shell.
        if (strcmp(input, "exit") == 0) {
            break;
        }


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

        // Parse output redirection before tokenizing the command.
        char *output_file = NULL;
        char *redirect = strchr(input, '>');

        if (redirect != NULL) {
            *redirect = '\0';
            char *filename = redirect + 1;

            // Skip whitespace after '>'.
            while (*filename == ' ' || *filename == '\t') {
                filename++;
            }

            if (*filename == '\0') {
                fprintf(stderr, "Redirection: missing output file\n");
                continue;
            }

            if (strchr(filename, '>') != NULL) {
                fprintf(stderr, "Redirection: multiple '>' operators\n");
                continue;
            }

            // Trim trailing whitespace from the filename.
            size_t file_len = strlen(filename);
            while (file_len > 0 &&
                   (filename[file_len - 1] == ' ' ||
                    filename[file_len - 1] == '\t')) {
                filename[--file_len] = '\0';
            }

            // This version supports one filename without spaces.
            if (strchr(filename, ' ') || strchr(filename, '\t')) {
                fprintf(stderr, "Redirection: invalid output filename\n");
                continue;
            }

            output_file = filename;
        }

        // Split the input into pipeline stages.
        // Each stage contains one command and its arguments.
        Job stages[MAX_ARGS] = {0};
        int stage_count = 0;
        int parse_error = 0;
        char *segment = input;

        while (segment != NULL) {
            if (stage_count >= MAX_ARGS) {
                fprintf(stderr, "Pipeline: too many stages\n");
                parse_error = 1;
                break;
            }

            char *next_pipe = strchr(segment, '|');

            if (next_pipe != NULL) {
                *next_pipe = '\0';
            }

            Job *current = &stages[stage_count];
            int arg_count = 0;
            char *token = strtok(segment, " \t");

            while (token != NULL && arg_count < MAX_ARGS - 1) {
                current->argv[arg_count++] = token;
                token = strtok(NULL, " \t");
            }

            if (token != NULL) {
                fprintf(stderr, "Pipeline: too many arguments\n");
                parse_error = 1;
                break;
            }

            current->argv[arg_count] = NULL;

            if (arg_count == 0) {
                fprintf(stderr, "Pipeline: missing command\n");
                parse_error = 1;
                break;
            }

            stage_count++;

            if (next_pipe == NULL) {
                break;
            }

            segment = next_pipe + 1;
        }

        if (parse_error || stage_count == 0) {
            continue;
        }

        // cd must execute in the parent shell process.
        if (strcmp(stages[0].argv[0], "cd") == 0) {
            if (stage_count > 1 || output_file != NULL) {
                fprintf(stderr, "cd: pipes and redirection not supported\n");
                continue;
            }

            int cd_argc = 0;
            while (stages[0].argv[cd_argc] != NULL) {
                cd_argc++;
            }

            change_directory(cd_argc, stages[0].argv);
            continue;
        }

        // Execute a pipeline or a single command.
        int status;

        if (stage_count > 1) {
            status = run_pipeline(stages, stage_count, output_file);
        } else if (output_file != NULL) {
            status = run_job_redirect(&stages[0], output_file);
        } else {
            status = run_job(&stages[0]);
        }

        if (status != 0) {
            fprintf(stderr, "Command exited with status %d\n", status);
        }
    }

    // if the cmd_history flag is set, call vector_free here

    if(cmd_history) {
        vector_free(&history);
    }

    return 0;
}
