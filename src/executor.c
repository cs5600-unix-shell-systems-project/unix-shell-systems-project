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

// Execute two commands connected by a pipe.
int run_pipeline(Job *left, Job *right, const char *output_file)
{
    if (left == NULL || right == NULL ||
        left->argv[0] == NULL || right->argv[0] == NULL) {
        fprintf(stderr, "run_pipeline: invalid arguments\n");
        return -1;
    }

    int pipefd[2];

    if (pipe(pipefd) < 0) {
        perror("pipe");
        return -1;
    }

    fflush(stdout);

    // First child writes to the pipe.
    pid_t left_pid = fork();

    if (left_pid < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (left_pid == 0) {
        close(pipefd[0]);

        if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
            perror("dup2");
            _exit(1);
        }

        close(pipefd[1]);
        execvp(left->argv[0], left->argv);

        int saved_errno = errno;
        perror("execvp");
        _exit(saved_errno == ENOENT ? 127 : 126);
    }

    // Second child reads from the pipe.
    pid_t right_pid = fork();

    if (right_pid < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);

        int status;
        while (waitpid(left_pid, &status, 0) < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("waitpid");
            break;
        }
        return -1;
    }

    if (right_pid == 0) {
        close(pipefd[1]);

        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            perror("dup2");
            _exit(1);
        }

        close(pipefd[0]);

        // Redirect the last command's stdout if requested.
        if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);

            if (fd < 0) {
                perror("open");
                _exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                close(fd);
                _exit(1);
            }

            close(fd);
        }

        execvp(right->argv[0], right->argv);

        int saved_errno = errno;
        perror("execvp");
        _exit(saved_errno == ENOENT ? 127 : 126);
    }

    // Parent closes both pipe ends.
    close(pipefd[0]);
    close(pipefd[1]);

    int left_status = 0;
    int right_status = 0;
    int wait_error = 0;

    // Always attempt to reap both children.
    while (waitpid(left_pid, &left_status, 0) < 0) {
        if (errno == EINTR) {
            continue;
        }

        perror("waitpid");
        wait_error = 1;
        break;
    }

    while (waitpid(right_pid, &right_status, 0) < 0) {
        if (errno == EINTR) {
            continue;
        }

        perror("waitpid");
        wait_error = 1;
        break;
    }

    if (wait_error) {
        return -1;
    }

    // Like a typical shell, return the last command's status.
    if (WIFEXITED(right_status)) {
        return WEXITSTATUS(right_status);
    }

    if (WIFSIGNALED(right_status)) {
        return 128 + WTERMSIG(right_status);
    }

    return -1;
}
