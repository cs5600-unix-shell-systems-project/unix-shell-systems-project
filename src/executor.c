#define _POSIX_C_SOURCE 200809L 
#include "executor.h"
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

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
// Execute multiple commands connected by pipes.
int run_pipeline(Job *jobs, int job_count, const char *output_file)
{
    if (jobs == NULL || job_count < 2 || job_count > MAX_ARGS) {
        fprintf(stderr, "run_pipeline: invalid arguments\n");
        return -1;
    }

    for (int i = 0; i < job_count; i++) {
        if (jobs[i].argv[0] == NULL) {
            fprintf(stderr, "run_pipeline: empty command\n");
            return -1;
        }
    }

    pid_t pids[MAX_ARGS];
    int started = 0;
    int previous_read = -1;
    int failed = 0;
    int last_status = 0;

    fflush(stdout);

    for (int i = 0; i < job_count; i++) {
        int pipefd[2] = {-1, -1};

        // Every stage except the last needs an output pipe.
        if (i < job_count - 1 && pipe(pipefd) < 0) {
            perror("pipe");
            failed = 1;
            break;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            if (pipefd[0] != -1) close(pipefd[0]);
            if (pipefd[1] != -1) close(pipefd[1]);
            failed = 1;
            break;
        }

        if (pid == 0) {
            // Read input from the previous stage.
            if (previous_read != -1) {
                if (dup2(previous_read, STDIN_FILENO) < 0) {
                    perror("dup2");
                    _exit(1);
                }
            }

            // Send output to the next stage.
            if (pipefd[1] != -1) {
                if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
                    perror("dup2");
                    _exit(1);
                }
            }

            if (previous_read != -1) close(previous_read);
            if (pipefd[0] != -1) close(pipefd[0]);
            if (pipefd[1] != -1) close(pipefd[1]);

            // Only the final stage can redirect stdout to a file.
            if (i == job_count - 1 && output_file != NULL) {
                int fd = open(output_file,
                              O_WRONLY | O_CREAT | O_TRUNC, 0666);

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

            execvp(jobs[i].argv[0], jobs[i].argv);

            int saved_errno = errno;
            perror("execvp");
            _exit(saved_errno == ENOENT ? 127 : 126);
        }

        pids[started++] = pid;

        // Parent keeps only the read end for the next stage.
        if (previous_read != -1) close(previous_read);
        if (pipefd[1] != -1) close(pipefd[1]);
        previous_read = pipefd[0];
    }

    if (previous_read != -1) close(previous_read);

    // orphaned processes need to be killed before they're waited on, 
    // behavior before this change would keep an orphaned child running while waiting on it.
    if (failed) {
        for(int i = 0; i < started; i++){
            kill(pids[i], SIGTERM);
        }
    }

    // Wait for every child, including after a pipe or fork error.
    for (int i = 0; i < started; i++) {
        int status = 0;
        pid_t result;

        do {
            result = waitpid(pids[i], &status, 0);
        } while (result < 0 && errno == EINTR);

        if (result < 0) {
            perror("waitpid");
            failed = 1;
        } else if (i == job_count - 1) {
            last_status = status;
        }
    }

    if (failed) return -1;

    if (WIFEXITED(last_status)) {
        return WEXITSTATUS(last_status);
    }

    if (WIFSIGNALED(last_status)) {
        return 128 + WTERMSIG(last_status);
    }

    return -1;
}
