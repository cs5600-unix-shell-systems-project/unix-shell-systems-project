# Sprint 2 Design Note: UML Sequence Diagrams

## Decision

We did not change the three Sprint 1 sequence diagrams. They still describe the control flow of our system accurately.

## Why the diagrams are still accurate

The Sprint 2 skeleton follows the same flow the diagrams show. Each participant in the diagrams maps to real code:

| Diagram participant | Sprint 2 code |
|---|---|
| UNIX Shell | `src/shell.c`, the REPL loop in `main` |
| Priority Queue | `src/priority.c` and `src/priority.h` |
| FIFO baseline | `src/fifo.c` and `src/fifo.h` |
| Priority Scheduler | `run_priority_jobs` in `src/shell.c` |
| Process Manager | `run_job` in `src/executor.c` |
| Child Process | the child branch after `fork` in `run_job` |
| Operating System | `fork`, `execvp`, and `waitpid` system calls |

The literature we reviewed in Sprint 2 also supports this design. ghOSt [2] and sched_ext [10] both keep the execution mechanism fixed and make the scheduling policy replaceable. Our design does the same: only the queue and the selection rule change between FIFO and priority.

## Differences between the diagrams and the current code

These are planned stages of the same design, not changes to it.

1. **Priority Queue internals.** Diagram 3 labels the priority queue as a binary heap. The Sprint 2 skeleton uses a linear scan with the same interface. Sprint 3 replaces the internals with a binary heap. The interface in `priority.h` stays the same.
2. **Metrics Collector.** Diagram 3 records start and finish times. This is not implemented yet. It is planned for Sprint 3, as part of the benchmarking rig.
3. **Pipeline Execution.** Diagram 2 shows pipelines. Pipes and redirection are planned for Sprint 3, as part of the base shell.
4. **Batch commands.** The diagrams show "submit jobs" and "start batch execution" as abstract steps. In the code, these are the `submit <priority> <command>` command and the `run fifo` and `run priority` commands.