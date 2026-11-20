#include <stdio.h>
#include <unistd.h>
#include "../src/executor.h"
//cases to look through:
    // yes prints "y" forever, but head only takes the first line of output and exits
    // once head is gone nothing reads the end of the yes pipe, so yes gets killed
    // the shell needs to close it's side of the pipe, otherwise yes wont die and the shell will hang forever
    // since yes needs user input to end

static int test_broken_pipe(void){
    Job stages[2] = {0};

    stages[0].argv[0]="yes";
    stages[0].argv[1]=NULL;

    // stage 2: head -n 1
    stages[1].argv[0] = "head";
    stages[1].argv[1] = "-n";
    stages[1].argv[2] = "1";
    stages[1].argv[3] = NULL;

    // if this hangs, kill the test after 5 seconds instead of waiting forever
    alarm(5);
    int status = run_pipeline(stages, 2, NULL);
    alarm(0);

    if (status != 0) {
        printf("FAIL: broken pipe (status %d)\n", status);
        return 1;
    }

    printf("PASS: broken pipe\n");
    return 0;
}

int main(void){
    int failures = 0;

    failures += test_broken_pipe();

    if (failures==0){
        return 0;
    } else {
        return 1;
    }
}