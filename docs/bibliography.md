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

## Parker McKillop

## Rowan Lowden
