#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "job.h"

// Execute one job using fork, execvp, and waitpid.
// Returns the command's exit status, or -1 on system error.
int run_job(Job *job);

// Execute a job and redirect its standard output to a file.
// Returns the command's exit status, or -1 on system error.
int run_job_redirect(Job *job, const char *output_file);

// Execute multiple commands connected by pipes.
// Returns the last command's exit status, or -1 on system error.
int run_pipeline(Job *jobs, int job_count, const char *output_file);

#endif
