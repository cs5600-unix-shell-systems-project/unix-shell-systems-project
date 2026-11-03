#include <stdio.h>
#include "../src/executor.h"

int main(void)
{
    Job job = {0};

    job.id = 1;
    job.priority = 2;
    job.submission_order = 1;

    job.argv[0] = "echo";
    job.argv[1] = "Hello from UNIX Shell!";
    job.argv[2] = NULL;

    printf("Testing run_job()...\n");
    fflush(stdout);

    int status = run_job(&job);

    printf("Exit status: %d\n", status);

    return status == 0 ? 0 : 1;
}
