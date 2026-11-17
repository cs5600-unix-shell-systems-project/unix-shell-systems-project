#!/bin/bash
# job.sh
# A test job. It logs its own start time and end time.
# Usage:   job.sh <id> <priority> <duration> <log_file>
# Example: benchmarks/job.sh 1 3 0.1 benchmarks/trials/log.csv

# Step 1: check that we got exactly 4 arguments.
if [ $# -ne 4 ]; then
    echo "Usage: job.sh <id> <priority> <duration> <log_file>"
    exit 1
fi

# Step 2: read the 4 arguments.
job_id=$1
priority=$2
duration=$3
log_file=$4

# Step 3: record the start time in seconds, with nanosecond precision.
start_time=$(date +%s.%N)

# Step 4: run the job itself. The job sleeps for the given duration.
sleep "$duration"

# Step 5: record the end time.
end_time=$(date +%s.%N)

# Step 6: append one CSV line to the log file.
# Column order: id, priority, duration, start time, end time
echo "$job_id,$priority,$duration,$start_time,$end_time" >> "$log_file"
