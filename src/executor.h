#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "job.h"

// Execute one job using fork, execvp, and waitpid.
// Returns the command's exit status, or -1 on system error.
int run_job(Job *job);

#endif
