#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include "../src/executor.h"

// bool to trigger fork failure and int to iterate and fail on the right step
static bool fake_fork_on = false;
static int fork_calls = 0;

pid_t __real_fork(void);

// wrapper to force a failure on a specific fork
pid_t __wrap_fork(void)
{
    if (fake_fork_on) {
        // counts calls to allow failure on the right fork
        fork_calls++;
        // fail on on the second fork
        if (fork_calls == 2) {
            errno = EAGAIN;
            return -1;
        }
    }
    //allows the actual fork process to resolve as normal
    return __real_fork();
}

//pipeline setup
static int test_second_fork_fails(void)
{
    Job stages[3] = {0};

    stages[0].argv[0] = "sleep";
    stages[0].argv[1] = "10";
    stages[0].argv[2] = NULL;

    stages[1].argv[0] = "cat";
    stages[1].argv[1] = NULL;

    stages[2].argv[0] = "cat";
    stages[2].argv[1] = NULL;

    //trigger the failed fork
    fake_fork_on = true;

    // sleep is running when the second fork fails
    // check if the sleep process is killed, if its not the alarm
    // kills the process
    alarm(2);
    int status = run_pipeline(stages, 3, NULL);
    alarm(0);

    fake_fork_on = false;

    if (status != -1) {
        printf("FAIL: second fork fails (status %d)\n", status);
        return 1;
    }

    printf("PASS: second fork fails\n");
    return 0;
}

int main(void)
{
    int failures = 0;

    failures += test_second_fork_fails();

    if (failures == 0) {
        return 0;
    } else {
        return 1;
    }
}
