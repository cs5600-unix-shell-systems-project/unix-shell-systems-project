# generate_workload.py
# Generates a workload file to feed the shell for scheduling experiments.
# One job per line. Format: id priority command
# Usage:   python3 generate_workload.py <job_count> <output_file>
# Example: python3 generate_workload.py 10 small.txt

import sys       # Read command-line arguments.
import random    # Generate random numbers.

# Fixed random seed.
# This makes every run produce the same workload.
# Reproducibility is a basic requirement for experiments.
random.seed(42)

# Three priority levels. A bigger number means higher priority.
# 1 = low, 2 = medium, 3 = high
PRIORITY_LEVELS = [1, 2, 3]

# Three job durations in seconds. These model short, medium, and long jobs.
JOB_DURATIONS = [0.1, 0.3, 0.5]


# Main function: build the workload and write it to a file.
def main():
    # Step 1: check that we got enough arguments.
    # sys.argv[0] is the script name itself, so we need 3 in total.
    if len(sys.argv) != 3:
        print("Usage: python3 generate_workload.py <job_count> <output_file>")
        return

    # Step 2: read the arguments.
    job_count = int(sys.argv[1])    # How many jobs to generate.
    output_file = sys.argv[2]       # Where to write the output.

    # Step 3: open the file for writing.
    f = open(output_file, "w")

    # Step 4: generate the jobs one by one.
    job_id = 1
    while job_id <= job_count:
        # Pick a random priority.
        priority = random.choice(PRIORITY_LEVELS)
        # Pick a random duration.
        duration = random.choice(JOB_DURATIONS)
        # Build one line: id priority sleep duration
        line = str(job_id) + " " + str(priority) + " sleep " + str(duration)
        # Write it to the file, with a newline at the end.
        f.write(line + "\n")
        job_id = job_id + 1

    # Step 5: close the file.
    f.close()

    # Print a short confirmation.
    print("Generated " + str(job_count) + " jobs, written to " + output_file)


# Standard Python pattern: run main only when this script is run directly.
if __name__ == "__main__":
    main()