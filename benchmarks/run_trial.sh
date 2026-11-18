#!/bin/bash
# run_trial.sh
# Runs one trial of a workload on the base shell.
# Run this script from the repository root.
# Usage:   benchmarks/run_trial.sh <workload_file> <trial_name>
# Example: benchmarks/run_trial.sh benchmarks/small.txt small_trial1
# Output files:
#   benchmarks/trials/<trial_name>_jobs.csv   one line per job
#   benchmarks/trials/<trial_name>_batch.csv  batch start and end time

# Step 1: check that we got exactly 2 arguments.
if [ $# -ne 2 ]; then
    echo "Usage: benchmarks/run_trial.sh <workload_file> <trial_name>"
    exit 1
fi

# Step 2: check that we are in the repository root.
if [ ! -x ./shell ]; then
    echo "Error: ./shell not found. Run this script from the repository root."
    exit 1
fi

# Step 3: read the arguments and build the file names.
workload_file=$1
trial_name=$2
jobs_log=benchmarks/trials/${trial_name}_jobs.csv
batch_log=benchmarks/trials/${trial_name}_batch.csv
batch_file=/tmp/${trial_name}_batch.txt

# Step 4: make sure the output directory exists.
mkdir -p benchmarks/trials

# Step 5: write the CSV header lines.
# This also clears old results that have the same trial name.
echo "id,priority,duration,start,end" > "$jobs_log"
echo "batch_start,batch_end" > "$batch_log"

# Step 6: convert the workload into a batch file for the base shell.
python3 benchmarks/make_batch.py "$workload_file" "$jobs_log" "$batch_file" > /dev/null

# Step 7: record the batch start time.
# All jobs arrive at this moment.
batch_start=$(date +%s.%N)

# Step 8: run the base shell on the batch file.
# Throw away the shell output. The jobs write their own timestamps.
./shell < "$batch_file" > /dev/null

# Step 9: record the batch end time.
batch_end=$(date +%s.%N)

# Step 10: save the batch start and end time.
echo "$batch_start,$batch_end" >> "$batch_log"

echo "Trial $trial_name done."
