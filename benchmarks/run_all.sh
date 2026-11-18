#!/bin/bash
# run_all.sh
# Runs every workload size several times on the base shell.
# Run this script from the repository root.
# Usage: benchmarks/run_all.sh

# Step 1: check that we are in the repository root.
if [ ! -x ./shell ]; then
    echo "Error: ./shell not found. Run this script from the repository root."
    exit 1
fi

# Step 2: experiment settings.
# sizes  = the number of jobs in each workload
# trials = how many times we run each workload
sizes="10 50 100"
trials=5

# Step 3: run every size, and every trial for that size.
for size in $sizes; do
    workload_file=benchmarks/workloads/jobs_${size}.txt

    trial=1
    while [ $trial -le $trials ]; do
        trial_name=baseline_jobs${size}_trial${trial}
        benchmarks/run_trial.sh "$workload_file" "$trial_name"
        trial=$((trial + 1))
    done
done

echo "All trials done."
