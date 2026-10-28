# Research Extension Update

Our research direction has not changed since Sprint 1. We still compare two batch scheduling policies, FIFO and priority, measuring how each distributes waiting time and turnaround time across jobs of different priority levels.

The literature we reviewed confirmed this direction. The Lottery Scheduling paper (Waldspurger and Weihl, 1994) shows that proportional-share policies are a well-studied way to reason about fairness, which is one of our metrics. The ghOSt paper (Humphries et al., SOSP 2021) shows that running scheduling policies in user space, without changing the kernel, is a valid and current approach at production scale. This matches our own design, where the batch scheduler runs in user space and controls launch order only.

Based on these sources, we keep our original hypothesis: priority scheduling does not reduce total makespan, but redistributes waiting time from high-priority jobs to low-priority jobs. No scope or hypothesis change is needed for this sprint.

------------------------------------------------------------------

# Annotated Bibliography

This bibliography collects at least 12 sources across five categories: academic papers, books, engineering blogs, community discourse, and source code. At least two entries are source code linked to a specific function or struct. Each team member maintains their own section below.

## Changming Qiu

Scheduling-focused sources: proportional-share scheduling, OSTEP scheduling chapters, and a binary heap implementation for the priority scheduler.

### [Academic paper] Lottery Scheduling: Flexible Proportional-Share Resource Management

- **Citation**: C. A. Waldspurger and W. E. Weihl. "Lottery Scheduling: Flexible Proportional-Share Resource Management." Proceedings of the First USENIX Symposium on Operating Systems Design and Implementation (OSDI '94), Monterey, CA, November 1994. https://www.usenix.org/legacy/publications/library/proceedings/osdi/full_papers/waldspurger.pdf (retrieved 2026-10-17).
- **Category**: Academic paper
- **Summary**: This paper introduces lottery scheduling, a randomized way to share a resource in proportion to each job's weight. Each job holds tickets. A random draw picks the next job, so jobs with more tickets run more often. The authors built a prototype on the Mach microkernel and measured low overhead.
- **Relevance**: Our research extension compares scheduling policies by how they divide waiting time across jobs of different importance. Lottery scheduling is a proportional-share baseline. It gives us a reference point for thinking about fairness, which is one of our evaluation metrics. We studied it in HW3, so it directly informs our policy design.

### [Academic paper] ghOSt: Fast and Flexible User-Space Delegation of Linux Scheduling

- **Citation**: J. T. Humphries, N. Natu, A. Chaugule, O. Weisse, B. Rhoden, J. Don, L. Rizzo, O. Rombakh, P. Turner, and C. Kozyrakis. "ghOSt: Fast and Flexible User-Space Delegation of Linux Scheduling." Proceedings of the ACM SIGOPS 28th Symposium on Operating Systems Principles (SOSP '21), 2021, pp. 588–604. https://dl.acm.org/doi/10.1145/3477132.3483542 (retrieved 2026-10-17).
- **Category**: Academic paper
- **Summary**: ghOSt is a framework from Google that moves Linux scheduling decisions out of the kernel and into user-space agents. The kernel only reports events. A user-space agent decides what runs where, then commits the decision back to the kernel. It supports many policy styles with low overhead.
- **Relevance**: Our batch scheduler also runs in user space and controls launch order without changing the kernel. ghOSt is a modern, real-world example of the same design choice at production scale. It confirms that user-space scheduling is a valid and current research direction, which supports our project's approach.

### [Source code] bheap — a binary heap library in C

- **Citation**: R. Kota. bheap: Binary heap library written in C. GitHub repository, 2018. https://github.com/octarect/bheap (retrieved 2026-10-17).
- **Category**: Source code
- **Specific code**: The push and pop functions in bheap.c, bheap_push and bheap_pop. https://github.com/octarect/bheap/blob/master/bheap.c
- **Summary**: A small, readable C library that implements a binary heap. bheap_push inserts an item with a key and swims it up to keep heap order. bheap_pop removes the top item and sinks the last element down to restore order. The README shows a three-item example where pop returns items in key order.
- **Relevance**: Our priority scheduler uses a linear scan in the Sprint 2 skeleton. Sprint 3 replaces it with a binary heap for O(log n) insert and remove. This library shows a clean push/pop pattern we can learn from when we write our own heap. It connects directly to our design challenge of keeping the highest-priority job fast to retrieve.

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

## Parker McKillop

## Rowan Lowden
