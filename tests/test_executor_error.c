#include <stdio.h>
#include "../src/executor.h"

int main(void)
{
    Job job = {0};

    job.id = 2;
    job.priority = 2;
    job.submission_order = 2;

    job.argv[0] = "command_that_does_not_exist_12345";
    job.argv[1] = NULL;

    printf("Testing invalid command...\n");
    fflush(stdout);

    int status = run_job(&job);

    printf("Exit status: %d\n", status);

    return status == 127 ? 0 : 1;
}
