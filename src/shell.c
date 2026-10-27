#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "executor.h"

#define INPUT_SIZE 1024

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

        // Remove the trailing newline.
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
