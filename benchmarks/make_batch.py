# make_batch.py
# Converts a workload file into a batch file for the base shell.
# Input line format:  id priority sleep duration
# Output line format: benchmarks/job.sh id priority duration log_file
# Usage:   python3 make_batch.py <workload_file> <log_file> <batch_file>
# Example: python3 benchmarks/make_batch.py benchmarks/small.txt benchmarks/trials/log.csv benchmarks/small_batch.txt

import sys    # Read command-line arguments.


# Main function: read the workload and write the batch file.
def main():
    # Step 1: check that we got enough arguments.
    # sys.argv[0] is the script name itself, so we need 4 in total.
    if len(sys.argv) != 4:
        print("Usage: python3 make_batch.py <workload_file> <log_file> <batch_file>")
        return

    # Step 2: read the arguments.
    workload_file = sys.argv[1]    # The workload file to read.
    log_file = sys.argv[2]         # Where job.sh will write its timestamps.
    batch_file = sys.argv[3]       # Where to write the batch file.

    # Step 3: open both files.
    input_file = open(workload_file, "r")
    output_file = open(batch_file, "w")

    # Step 4: convert the lines one by one.
    line_count = 0
    for line in input_file:
        # Remove the newline and spaces at both ends.
        line = line.strip()

        # Skip empty lines.
        if line == "":
            continue

        # Split the line into words.
        # Example: "1 3 sleep 0.1" becomes ["1", "3", "sleep", "0.1"]
        parts = line.split()
        job_id = parts[0]
        priority = parts[1]
        duration = parts[3]

        # Build one batch line that calls job.sh.
        new_line = "benchmarks/job.sh " + job_id + " " + priority + " " + duration + " " + log_file
        output_file.write(new_line + "\n")
        line_count = line_count + 1

    # Step 5: close both files.
    input_file.close()
    output_file.close()

    # Print a short confirmation.
    print("Converted " + str(line_count) + " jobs, written to " + batch_file)


# Standard Python pattern: run main only when this script is run directly.
if __name__ == "__main__":
    main()
