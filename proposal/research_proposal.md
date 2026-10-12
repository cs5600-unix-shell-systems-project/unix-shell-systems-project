# UNIX Shell with Priority-Based Batch Scheduling

## 1. Project Title & Team

**Project:** G.2 UNIX Shell

**Title:** UNIX Shell with Priority-Based Batch Scheduling

**Course:** Computer Systems

**Team Size:** 4 members

**Team Members:**
- Zihan Guo
- Changming Qiu
- Parker McKillop
- Rowan Lowden

## 2. Abstract

This project aims to develop a UNIX shell that supports
basic command execution, pipelines, input/output redirection,
and process management.

The shell will use standard UNIX process APIs, including
fork(), execvp(), waitpid(), pipe(), and dup2().

In addition to these standard features, we propose a
priority-based batch scheduling mechanism.

The system will allow users to submit batch jobs with
different priority levels and execute them according to
a scheduling policy.

We will compare FIFO scheduling with priority-based
scheduling using different workloads.

Our goal is to evaluate how job selection policies affect
waiting time, turnaround time, and scheduling fairness.

## 3. Research Extension (Hypothesis)

Our research extension is a priority-based batch scheduler.

The standard implementation will execute batch jobs using
a First-In-First-Out (FIFO) queue.

The extended implementation will use a priority queue to
select jobs based on their assigned priority levels.

Jobs with the same priority will be executed in submission
order.

We hypothesize that priority-based scheduling will reduce
the average waiting time of high-priority jobs compared
with FIFO scheduling.

However, lower-priority jobs may experience longer waiting
times.

This trade-off will be measured experimentally.

## 4. Research Question

Our primary research question is:

**Does priority-based batch scheduling reduce the average
waiting time of high-priority jobs compared with FIFO
scheduling in a UNIX shell?**

We will evaluate the following metrics:

- Average waiting time
- Average turnaround time
- Waiting time by priority level
- Job completion order
- Scheduling fairness

Waiting time:

Waiting Time = Start Time - Submission Time

Turnaround time:

Turnaround Time = Completion Time - Submission Time

We will compare both scheduling policies using identical
sets of batch jobs.

## 5. Proposed Scope

We plan to complete the following technical tasks:

1. Implement a basic UNIX shell using fork(), execvp(),
   and waitpid() for command execution.

2. Add support for input/output redirection, pipelines,
   and background process execution.

3. Implement a FIFO batch job queue as the baseline
   scheduling mechanism.

4. Implement a priority-based batch scheduler using
   a priority queue with FIFO ordering for equal-priority jobs.

5. Develop a benchmarking framework to measure waiting
   time, turnaround time, and scheduling fairness under
   different workloads.

## 6. Experimental Design

We will evaluate two batch scheduling policies: FIFO and
priority-based scheduling.

Both policies will execute the same set of batch jobs on
the same Ubuntu virtual machine.

Each job will have a predefined priority level and an
estimated workload size. All jobs will be submitted before
batch execution begins.

The scheduler will execute one job at a time to ensure
that the comparison focuses on job selection order.

For each experiment, we will record job submission time,
start time, completion time, and execution order.

We will compare average waiting time, average turnaround
time, and waiting time across different priority levels.

We will also examine whether high-priority jobs benefit
at the expense of low-priority jobs.

Each workload will be tested under both scheduling policies
using repeated trials.

The results will help us evaluate the benefits and
trade-offs of priority-based scheduling.
