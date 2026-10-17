# Glossary

Key terms, data structures, and interfaces for the UNIX Shell with Priority-Based Batch Scheduling project.

## Core terms

**Batch job**
A command submitted to run later as part of a batch, instead of running interactively one at a time.

**Batch scheduler**
The part of the shell that decides the order in which submitted batch jobs run. It controls launch order only. It does not change the Linux kernel CPU scheduler.

**FIFO scheduling**
First In, First Out. Jobs run in the order they were submitted. The baseline policy.

**Priority scheduling**
Each job has a priority level. The scheduler runs the highest-priority job first. Ties are broken by submission order.

**Binary heap**
A data structure that keeps the highest-priority item fast to retrieve. It will back the priority scheduler in Sprint 3. The Sprint 2 skeleton uses a linear scan instead.

**Waiting time**
Start time minus submission time. How long a job waits before it starts.

**Turnaround time**
Completion time minus submission time. Total time from submission to finish.

## Core data structures

**Job** (`src/job.h`)
One batch job. Fields:
- `id` — job number, starts at 1
- `argv` — command and its arguments, NULL-terminated for execvp
- `priority` — bigger number means higher priority (1 low, 2 medium, 3 high)
- `submission_order` — breaks ties between equal priorities; earlier runs first

## Core interface

**run_job(job)** (owned by shell core)
The scheduler selects the next job and calls this function to run it. The function does fork, execvp, and waitpid, then returns the exit status. The scheduler decides order. run_job runs one job.

## Metrics output format

One CSV row per job per run. Columns:

job_id, priority, submission_order, submit_time, start_time, finish_time, exit_status

Waiting time and turnaround time are computed from these columns. This format is shared so the benchmarking code writes it and the analysis code reads it.