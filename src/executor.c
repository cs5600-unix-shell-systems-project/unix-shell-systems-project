#include "executor.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int run_job(Job *job)
{
    // Check that the job contains a valid command.
    if (job == NULL || job->argv[0] == NULL) {
        fprintf(stderr, "run_job: invalid job\n");
        return -1;
    }

    // Create a child process.
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Child: replace this process with the command.
        execvp(job->argv[0], job->argv);

        // execvp returns only when execution fails.
        int saved_errno = errno;
        perror("execvp");
        _exit(saved_errno == ENOENT ? 127 : 126);
    }

    // Parent: wait for the child to finish.
    int status;

    while (waitpid(pid, &status, 0) == -1) {
        if (errno == EINTR) {
            continue;
        }

        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }

    return -1;
}
