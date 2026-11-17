# summarize.py
# Combines all trials of one experiment group into a summary table.
# For each workload size and each metric, it computes mean, std, min, and max.
# Run this script from the repository root.
# Usage:   python3 benchmarks/summarize.py <prefix>
# Example: python3 benchmarks/summarize.py baseline
# Output:  benchmarks/results/<prefix>_summary.csv

import sys           # Read command-line arguments.
import os            # Create the output directory.
import statistics    # Compute mean and standard deviation.
import analyze_trial # Reuse compute_metrics() from analyze_trial.py.

# Experiment settings. These must match run_all.sh.
SIZES = [10, 50, 100]
TRIALS = 5

# The metrics we want to summarize.
METRIC_NAMES = [
    "total_time",
    "avg_waiting",
    "avg_turnaround",
    "avg_waiting_p3",
    "avg_waiting_p2",
    "avg_waiting_p1",
    "avg_turnaround_p3",
    "avg_turnaround_p2",
    "avg_turnaround_p1",
]


# Main function: summarize all trials and write the summary file.
def main():
    # Step 1: check that we got the prefix.
    if len(sys.argv) != 2:
        print("Usage: python3 benchmarks/summarize.py <prefix>")
        return
    prefix = sys.argv[1]

    # Step 2: make sure the output directory exists, then open the file.
    os.makedirs("benchmarks/results", exist_ok=True)
    output_file = "benchmarks/results/" + prefix + "_summary.csv"
    f = open(output_file, "w")
    f.write("size,metric,mean,std,min,max\n")

    # Step 3: process each workload size.
    for size in SIZES:
        # Make one empty list for each metric.
        values = {}
        for name in METRIC_NAMES:
            values[name] = []

        # Step 4: read every trial of this size and collect the values.
        trial = 1
        while trial <= TRIALS:
            trial_name = prefix + "_jobs" + str(size) + "_trial" + str(trial)
            metrics = analyze_trial.compute_metrics(trial_name)
            for name in METRIC_NAMES:
                values[name].append(metrics[name])
            trial = trial + 1

        # Step 5: compute the statistics and save them.
        print("Workload size: " + str(size) + " jobs")
        for name in METRIC_NAMES:
            mean = statistics.mean(values[name])
            std = statistics.stdev(values[name])    # Sample standard deviation.
            low = min(values[name])
            high = max(values[name])

            # Write one row to the summary file.
            row = (str(size) + "," + name + ","
                   + str(round(mean, 6)) + "," + str(round(std, 6)) + ","
                   + str(round(low, 6)) + "," + str(round(high, 6)))
            f.write(row + "\n")

            # Print a short line on the screen.
            print("  " + name + ": mean = " + str(round(mean, 4))
                  + " s, std = " + str(round(std, 4)) + " s")

    # Step 6: close the file.
    f.close()
    print("Summary written to " + output_file)


# Standard Python pattern: run main only when this script is run directly.
if __name__ == "__main__":
    main()
