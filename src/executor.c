#include "executor.h"
#include <fcntl.h>
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

// Execute a command with stdout redirected to a file.
int run_job_redirect(Job *job, const char *output_file)
{
    if (job == NULL || job->argv[0] == NULL ||
        output_file == NULL || output_file[0] == '\0') {
        fprintf(stderr, "run_job_redirect: invalid arguments\n");
        return -1;
    }

    fflush(stdout);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Create or overwrite the output file.
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);

        if (fd < 0) {
            perror("open");
            _exit(1);
        }

        // Replace stdout with the output file.
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            close(fd);
            _exit(1);
        }

        close(fd);

        execvp(job->argv[0], job->argv);

        int saved_errno = errno;
        perror("execvp");
        _exit(saved_errno == ENOENT ? 127 : 126);
    }

    // Wait for the child process.
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
