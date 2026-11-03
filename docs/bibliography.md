# Research Extension Update

Our research direction has not changed since Sprint 1. We still compare two batch scheduling policies, FIFO and priority, measuring how each distributes waiting time and turnaround time across jobs of different priority levels.

The literature we reviewed confirmed this direction. The Lottery Scheduling paper (Waldspurger and Weihl, 1994) shows that proportional-share policies are a well-studied way to reason about fairness, which is one of our metrics. The ghOSt paper (Humphries et al., SOSP 2021) shows that running scheduling policies in user space, without changing the kernel, is a valid and current approach at production scale. This matches our own design, where the batch scheduler runs in user space and controls launch order only.

Based on these sources, we keep our original hypothesis: priority scheduling does not reduce total makespan, but redistributes waiting time from high-priority jobs to low-priority jobs. No scope or hypothesis change is needed for this sprint.

------------------------------------------------------------------

# Annotated Bibliography

This bibliography collects at least 12 sources across five categories: academic papers, books, engineering blogs, community discourse, and source code. At least two entries are source code linked to a specific function or struct. Each team member maintains their own section below.

## Changming Qiu

Scheduling-focused sources: proportional-share scheduling, user-space scheduling, a binary heap implementation, and Linux process APIs.

### [1] C. A. Waldspurger and W. E. Weihl, "Lottery Scheduling: Flexible Proportional-Share Resource Management," Proceedings of the First USENIX Symposium on Operating Systems Design and Implementation (OSDI '94), 1994. [Link](https://www.usenix.org/conference/osdi-94/lottery-scheduling-flexible-proportional-share-resource-management)

**Category:** Academic Paper

**Summary:** This paper introduces lottery scheduling, a randomized way to share a resource in proportion to each job's weight. Each job holds tickets. A random draw picks the next job, so jobs with more tickets run more often.

**Relevance:** Our research extension compares how scheduling policies divide waiting time across jobs of different importance. Lottery scheduling is a classic proportional-share reference point for fairness, which is one of our evaluation concerns.

### [2] J. T. Humphries, N. Natu, A. Chaugule, O. Weisse, B. Rhoden, J. Don, L. Rizzo, O. Rombakh, P. Turner, and C. Kozyrakis, "ghOSt: Fast and Flexible User-Space Delegation of Linux Scheduling," Proceedings of the 28th ACM Symposium on Operating Systems Principles (SOSP '21), 2021, pp. 588–604. [Link](https://dl.acm.org/doi/10.1145/3477132.3483542)

**Category:** Academic Paper

**Summary:** ghOSt is a framework from Google that moves Linux scheduling decisions into user-space agents. The kernel reports events, and a user-space agent decides what runs where. It supports many policies with low overhead.

**Relevance:** Our batch scheduler also runs in user space and controls launch order without changing the kernel. ghOSt is a recent, production-scale example of the same design choice, which supports our approach.

### [3] R. Kota, "bheap: Binary heap library written in C," GitHub repository, 2018. [Link](https://github.com/octarect/bheap/blob/master/bheap.c)

**Category:** Source Code

**Specific code:** `bheap_push` and `bheap_pop` in `bheap.c`.

**Summary:** A small, readable C library that implements a binary heap. `bheap_push` inserts an item and moves it up to keep heap order. `bheap_pop` removes the top item and moves the last element down to restore order.

**Relevance:** Our priority scheduler uses a linear scan in the Sprint 2 skeleton. In Sprint 3 we will replace it with a binary heap for O(log n) insert and remove. This code shows a clean push and pop pattern we can follow, while we keep our own tie-break by submission order.

### [7] M. Kerrisk, "The Linux Programming Interface: A Linux and UNIX System Programming Handbook," No Starch Press, 2010. Chapters 24–27. [Link](https://www.nostarch.com/tlpi)

**Category:** Book

**Summary:** Chapters 24 to 27 cover the life cycle of a process on Linux. Chapter 24 explains fork(), and shows that after fork() returns, two processes exist and both continue from that point. Chapters 25 to 27 cover termination with exit() and _exit(), waiting for children with waitpid(), and running a new program with the exec() family.

**Relevance:** Our executor's run_job() follows this exact pattern: fork, execvp in the child, waitpid in the parent, then decode the exit status. These chapters explain why the child calls _exit() after a failed exec, and how WIFEXITED and WEXITSTATUS read the status our scheduler reports for each job.

### [9] W. R. Stevens and S. A. Rago, "Advanced Programming in the UNIX Environment," 3rd ed., Addison-Wesley, 2013. Chapters 8 and 9. [Link](https://www.pearson.com/en-us/subject-catalog/p/advanced-programming-in-the-unix-environment/P200000000628)

**Category:** Book

**Summary:** Chapter 8 covers process control: fork, the exit functions, wait and waitpid, and process scheduling with nice values. Chapter 9 covers process relationships: process groups, sessions, job control, and how a shell executes programs.

**Relevance:** Section 8.16 explains that the kernel already schedules running processes, which clarifies what our user-space scheduler controls: only the launch order of batch jobs. Section 9.9 shows how a real shell executes programs, which is the model our executor follows. Sections 9.4 and 9.8 describe job control, which we plan for background processes in Sprint 3.

### [10] J. Corbet, "The extensible scheduler class," LWN.net, 2023. [Link](https://lwn.net/Articles/922405/)

**Category:** Blog

**Summary:** This article describes sched_ext, a patch set that adds a new Linux scheduling class called SCHED_EXT. The scheduling policy is written as BPF programs and can be loaded and replaced without rebooting. If no BPF scheduler is loaded, tasks in this class run as normal tasks, and the kernel keeps safety checks so a faulty scheduler cannot take over the system.

**Relevance:** Our research extension swaps one scheduling policy for another on the same workload. sched_ext shows the same idea in the real Linux kernel: keep the mechanism fixed and make the policy replaceable. Like ghOSt, it supports our view that comparing pluggable policies is a current and practical research direction.

## Zihan Guo

### [4] S. R. Bourne, "UNIX Time-Sharing System: The UNIX Shell," Bell System Technical Journal, Vol. 57, No. 6, pp. 1971–1990, 1978. [Link](https://doi.org/10.1002/j.1538-7305.1978.tb02139.x)

**Category:** Academic Paper

**Summary:** Bourne presents the UNIX shell as both a command interpreter and a programming language. The paper describes command interpretation, argument passing, and the use of command return codes in execution control. It establishes the shell's role as an interface between users and UNIX system facilities.

**Relevance:** Our `shell.c` implements a command loop that accepts user input and dispatches commands, while `executor.c` launches external programs and handles their exit statuses. Bourne's discussion of command interpretation and execution provides a foundational reference for these components. Our project extends this basic shell model with `submit`, `run fifo`, and `run priority` commands to compare different batch job ordering policies.

### [5] GNU Bash Project, "GNU Bash 5.3: execute_cmd.c," GNU Bash Source Repository, 2025. [Link](https://gitea.psi.ch/pmodules/bash/src/commit/b8c60bc9ca365f8261fa97900b6fa939f6ebc303/execute_cmd.c)

**Category:** Source Code

**Specific code:** `execute_disk_command()` (line 5771) and `shell_execve()` (line 6126) in `execute_cmd.c`, GNU Bash 5.3, commit `b8c60bc9ca365f8261fa97900b6fa939f6ebc303`. The linked file is a fixed-commit mirror of the [official GNU Bash repository](https://git.savannah.gnu.org/git/bash.git).

**Summary:** The `execute_disk_command()` function searches for external commands and uses `make_child()` to create a child process when needed. It prepares arguments and invokes `shell_execve()`, which calls `execve()` to execute the selected program. The implementation also handles execution failures and command-related file descriptor operations.

**Relevance:** Our shell uses `fork()`, `execvp()`, and `waitpid()` to execute commands and retrieve exit statuses. Examining Bash's implementation illustrates how process creation, program execution, and error handling can be separated into distinct responsibilities. This informs our `run_job()` design, while the FIFO and priority schedulers determine which submitted job should execute next.

### [6] Kristian Köhntopp, "fork, exec, wait and exit," Percona Community, 2021. [Link](https://percona.community/blog/2021/01/04/fork-exec-wait-and-exit/)

**Category:** Blog

**Summary:** Köhntopp explains the UNIX process lifecycle through practical C examples involving `fork()`, `wait()`, `execl()`, and process termination. The article demonstrates how parent processes create children, execute external programs, and collect termination statuses. It also uses `strace` output to examine how Bash launches commands and waits for their completion.

**Relevance:** Our shell executor uses `fork()`, `execvp()`, and `waitpid()` to launch external commands and retrieve their exit statuses. The article clarifies the distinction between process creation and program execution, as well as the importance of collecting child termination statuses. These concepts support our sequential batch execution model, where FIFO or priority scheduling selects the next submitted job.

### [8] D.C., "How to avoid starving out lower-priorly work item sources," Stack Overflow, Feb. 7, 2017. [Link](https://stackoverflow.com/questions/42098351/how-to-avoid-starving-out-lower-priorly-work-item-sources)

**Category:** Community Discourse

**Summary:** This Stack Overflow discussion examines starvation in priority-based work scheduling. It introduces priority aging as a way to increase the effective priority of jobs that have waited for a long time, reducing the risk that lower-priority jobs will be indefinitely delayed.

**Relevance:** Our project implements FIFO and priority-based batch scheduling to compare job execution order and scheduling behavior. The discussion highlights a potential fairness limitation of strict priority scheduling, where lower-priority jobs may experience long waiting times. Priority aging is a possible future improvement to our scheduler, although it is not implemented in Sprint 2.

### [11] A. Verma, L. Pedrosa, M. R. Korupolu, D. Oppenheimer, E. Tune, and J. Wilkes, "Large-scale cluster management at Google with Borg," Proceedings of the Tenth European Conference on Computer Systems (EuroSys '15), 2015. [Link](https://research.google/pubs/large-scale-cluster-management-at-google-with-borg/)

**Category:** Academic Paper

**Summary:** This paper describes Google's Borg cluster management system, which schedules jobs across large numbers of machines. Borg uses task priorities, admission control, and resource allocation policies to manage different workloads, including long-running services and batch jobs. The paper explains how scheduling decisions balance resource utilization and workload requirements.

**Relevance:** Our project compares FIFO and priority-based batch scheduling to study how execution order affects waiting time and turnaround time. Borg provides a real-world example of how task priorities influence scheduling decisions. Although our scheduler runs jobs sequentially on a single system rather than distributing tasks across a cluster, the paper helps explain why priority-based policies are useful and what trade-offs they introduce.

### [12] Linux Kernel Project, "Real-Time Scheduling Class (kernel/sched/rt.c)," Linux Kernel Source Code, version 6.12, 2024. [Link](https://github.com/torvalds/linux/blob/v6.12/kernel/sched/rt.c)

**Category:** Source Code

**Specific code:** `enqueue_task_rt()` (line 1383), `pick_next_rt_entity()` (line 1606), and `pick_task_rt()` (line 1637) in `kernel/sched/rt.c`, Linux v6.12.

**Summary:** This source file implements the Linux real-time scheduling class used by SCHED_FIFO and SCHED_RR. The `enqueue_task_rt()` function adds runnable tasks to the real-time scheduling queues. The `pick_next_rt_entity()` function uses a priority bitmap to identify the highest-priority nonempty queue and selects the first entity from that queue. The `pick_task_rt()` function checks whether real-time tasks are runnable and retrieves the next task selected by the scheduler.

**Relevance:** Our project implements FIFO and priority-based batch scheduling in user space. The Linux real-time scheduler provides a more advanced example of priority-based task selection and queue management. Examining these functions helps us understand how tasks can be organized by priority while preserving queue order within a priority level. Unlike the kernel scheduler, our project controls only the order in which submitted jobs are launched, without implementing CPU preemption or kernel-level scheduling policies.

## Parker McKillop

Exempt from Sprint 2 by instructor accommodation.

## Rowan Lowden

No longer on the team.
