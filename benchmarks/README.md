# Benchmarks

This directory contains the benchmark rig for our shell.
The rig treats the shell as a black box.
It feeds a batch file to the shell and measures each job from outside.

## Files

| File | Purpose |
|---|---|
| `generate_workload.py` | Generates a workload file. Each line is `id priority sleep duration`. The random seed is fixed to 42. |
| `make_batch.py` | Converts a workload file into a batch file that the base shell can run. |
| `job.sh` | A test job. It sleeps for the given duration and logs its own start and end time. |
| `run_trial.sh` | Runs one trial and saves the raw data. |
| `run_all.sh` | Runs all workload sizes, 5 trials each. |
| `analyze_trial.py` | Computes the metrics of one trial. |
| `summarize.py` | Combines all trials into a summary table with mean, std, min, and max. |
| `workloads/` | Fixed workload files with 10, 50, and 100 jobs. |
| `trials/` | Raw data. Two CSV files per trial. |
| `results/` | Summary tables. |

## How to Run

Run all commands from the repository root.

1. Build the shell:

```bash
make
```

2. Run all trials. This takes about 4 minutes.

```bash
benchmarks/run_all.sh
```

3. Compute the summary:

```bash
python3 benchmarks/summarize.py baseline
```

The summary is written to `benchmarks/results/baseline_summary.csv`.

To look at one trial only:

```bash
python3 benchmarks/analyze_trial.py baseline_jobs10_trial1
```

## What We Measure

All jobs in a batch arrive at the same time.
The arrival time is the batch start time.
`run_trial.sh` records it before the shell starts.

| Metric | Definition |
|---|---|
| Waiting time | Job start time minus arrival time |
| Turnaround time | Job end time minus arrival time |
| Total batch time | Batch end time minus batch start time |
| Completion order | The order in which jobs finish |

We report each metric for all jobs and for each priority level.
Priority 3 is high, 2 is medium, and 1 is low.

## Experiment Design

- Independent variable: the number of jobs. We use 10, 50, and 100.
- Dependent variables: waiting time, turnaround time, and total batch time.
- Controlled variables: the same workload files, the same job durations, the same machine.
- Trials: 5 per workload size. We report the mean and the sample standard deviation.
- Job durations: 0.1, 0.3, and 0.5 seconds. These model short, medium, and long jobs.

## Raw Data Format

Each trial writes two files to `trials/`.

`<trial_name>_jobs.csv` has one line per job:

```text
id,priority,duration,start,end
```

`<trial_name>_batch.csv` has one line per trial:

```text
batch_start,batch_end
```

All times are Unix timestamps in seconds, with nanosecond precision.

## Measurement Overhead

Each job starts `date` twice and `sleep` once.
This adds a few milliseconds to every job.
The overhead is the same for every job and every experiment group.
So it does not change the comparison between groups.

## Environment

To be filled in after the baseline run on the LXC container.

## Sprint 4

The same rig will measure the priority scheduler.
Trial names use the prefix `baseline_` for the base shell.
The extension will use the prefix `priority_`.
