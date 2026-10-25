// job.h
// Defines what one batch job looks like.
// Both the scheduler and the execution module use this struct.

#ifndef JOB_H
#define JOB_H

// Max number of arguments a job can hold.
// This includes the command itself and the ending NULL.
#define MAX_ARGS 64

// Priority rule: a bigger number means higher priority.
// The scheduler runs higher-priority jobs first.
// Suggestion: HIGH = 3, MEDIUM = 2, LOW = 1.

// One batch job.
typedef struct
{
    int id;               // Job number, starting from 1.
    char *argv[MAX_ARGS]; // Command and its arguments. Last element must be NULL.
    int priority;         // Priority. Bigger number runs first.
    int submission_order; // Submission order. Breaks ties between equal priorities.
} Job;

#endif