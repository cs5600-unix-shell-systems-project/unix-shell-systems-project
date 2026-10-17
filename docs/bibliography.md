# Annotated Bibliography

This bibliography collects at least 12 sources across five categories: academic papers, books, engineering blogs, community discourse, and source code. At least two entries are source code linked to a specific function or struct. Each team member maintains their own section below.

## Changming Qiu

Scheduling-focused sources: proportional-share scheduling, OSTEP scheduling chapters, and a binary heap implementation for the priority scheduler.

### [Academic paper] Lottery Scheduling: Flexible Proportional-Share Resource Management

- **Citation**: C. A. Waldspurger and W. E. Weihl. "Lottery Scheduling: Flexible Proportional-Share Resource Management." Proceedings of the First USENIX Symposium on Operating Systems Design and Implementation (OSDI '94), Monterey, CA, November 1994. https://www.usenix.org/legacy/publications/library/proceedings/osdi/full_papers/waldspurger.pdf (retrieved 2026-10-17).
- **Category**: Academic paper
- **Summary**: This paper introduces lottery scheduling, a randomized way to share a resource in proportion to each job's weight. Each job holds tickets. A random draw picks the next job, so jobs with more tickets run more often. The authors built a prototype on the Mach microkernel and measured low overhead.
- **Relevance**: Our research extension compares scheduling policies by how they divide waiting time across jobs of different importance. Lottery scheduling is a proportional-share baseline. It gives us a reference point for thinking about fairness, which is one of our evaluation metrics. We studied it in HW3, so it directly informs our policy design.

### [Book] Operating Systems: Three Easy Pieces — Scheduling chapters

- **Citation**: R. H. Arpaci-Dusseau and A. C. Arpaci-Dusseau. Operating Systems: Three Easy Pieces. Arpaci-Dusseau Books, version 1.10. Chapters 7 (Scheduling: Introduction), 8 (Multi-Level Feedback Queue), and 9 (Lottery Scheduling). https://pages.cs.wisc.edu/~remzi/OSTEP/ (retrieved 2026-10-17).
- **Category**: Book
- **Summary**: These chapters explain core CPU scheduling ideas. Chapter 7 covers basic metrics like turnaround time and response time. Chapter 8 covers the multi-level feedback queue. Chapter 9 covers proportional-share and lottery scheduling.
- **Relevance**: The metrics defined in Chapter 7, especially turnaround time and waiting time, are the exact metrics our benchmarking framework measures. These chapters set the vocabulary and the baseline concepts for our FIFO versus priority comparison.

### [Source code] bheap — a binary heap library in C

- **Citation**: R. Kota. bheap: Binary heap library written in C. GitHub repository, 2018. https://github.com/octarect/bheap (retrieved 2026-10-17).
- **Category**: Source code
- **Specific code**: The push and pop functions in bheap.c, bheap_push and bheap_pop. https://github.com/octarect/bheap/blob/master/bheap.c
- **Summary**: A small, readable C library that implements a binary heap. bheap_push inserts an item with a key and swims it up to keep heap order. bheap_pop removes the top item and sinks the last element down to restore order. The README shows a three-item example where pop returns items in key order.
- **Relevance**: Our priority scheduler uses a linear scan in the Sprint 2 skeleton. Sprint 3 replaces it with a binary heap for O(log n) insert and remove. This library shows a clean push/pop pattern we can learn from when we write our own heap. It connects directly to our design challenge of keeping the highest-priority job fast to retrieve.

## Zihan Guo

## Parker McKillop

## Rowan Lowden