# analyze_trial.py
# Reads the raw data of one trial and computes the metrics.
# Other scripts can import compute_metrics() from this file.
# Run this script from the repository root.
# Usage:   python3 benchmarks/analyze_trial.py <trial_name>
# Example: python3 benchmarks/analyze_trial.py baseline_jobs10_trial1
#
# Definitions:
#   arrival time    = batch start time (all jobs arrive together)
#   waiting time    = job start time - arrival time
#   turnaround time = job end time - arrival time

import sys    # Read command-line arguments.
import csv    # Read CSV files.


# Compute the average of a list of numbers.
# Return 0 if the list is empty, so we never divide by zero.
def average(numbers):
    if len(numbers) == 0:
        return 0
    total = 0
    for number in numbers:
        total = total + number
    return total / len(numbers)


# Read the raw data of one trial and compute its metrics.
# Return the metrics in a dictionary.
def compute_metrics(trial_name):
    # Step 1: build the file names.
    jobs_log = "benchmarks/trials/" + trial_name + "_jobs.csv"
    batch_log = "benchmarks/trials/" + trial_name + "_batch.csv"

    # Step 2: read the batch start and end time.
    batch_file = open(batch_log, "r")
    reader = csv.DictReader(batch_file)
    for row in reader:
        batch_start = float(row["batch_start"])
        batch_end = float(row["batch_end"])
    batch_file.close()

    # Step 3: prepare empty lists for the results.
    waiting_times = []
    turnaround_times = []
    completion_order = []
    total_duration = 0    # Sum of all job durations, in seconds.

    # One list per priority level: 1 = low, 2 = medium, 3 = high
    waiting_by_priority = {1: [], 2: [], 3: []}
    turnaround_by_priority = {1: [], 2: [], 3: []}

    # Step 4: read the jobs one by one.
    # Each job writes its line when it finishes.
    # So the line order in the file is the completion order.
    jobs_file = open(jobs_log, "r")
    reader = csv.DictReader(jobs_file)
    for row in reader:
        job_id = int(row["id"])
        priority = int(row["priority"])
        start = float(row["start"])
        end = float(row["end"])
        duration = float(row["duration"])
        total_duration = total_duration + duration

        # Compute the two metrics for this job.
        waiting = start - batch_start
        turnaround = end - batch_start

        # Save the results.
        waiting_times.append(waiting)
        turnaround_times.append(turnaround)
        completion_order.append(job_id)
        waiting_by_priority[priority].append(waiting)
        turnaround_by_priority[priority].append(turnaround)
    jobs_file.close()

    # Step 5: put all metrics into one dictionary.
    metrics = {}
    metrics["jobs"] = len(waiting_times)
    metrics["total_time"] = batch_end - batch_start
    metrics["avg_waiting"] = average(waiting_times)
    metrics["avg_turnaround"] = average(turnaround_times)

    # Overhead is the time that is not spent inside the jobs.
    # Divide it by the number of jobs to get the average overhead per job.
    total_time = batch_end - batch_start
    metrics["overhead_per_job"] = (total_time - total_duration) / len(waiting_times)

    # Add the metrics for each priority level.
    for priority in [3, 2, 1]:
        p = str(priority)
        metrics["count_p" + p] = len(waiting_by_priority[priority])
        metrics["avg_waiting_p" + p] = average(waiting_by_priority[priority])
        metrics["avg_turnaround_p" + p] = average(turnaround_by_priority[priority])

    metrics["completion_order"] = completion_order
    return metrics


# Main function: compute the metrics of one trial and print them.
def main():
    # Step 1: check that we got the trial name.
    if len(sys.argv) != 2:
        print("Usage: python3 benchmarks/analyze_trial.py <trial_name>")
        return

    # Step 2: compute the metrics.
    trial_name = sys.argv[1]
    metrics = compute_metrics(trial_name)

    # Step 3: print the summary.
    print("Trial: " + trial_name)
    print("Jobs: " + str(metrics["jobs"]))
    print("Total batch time (s): " + str(round(metrics["total_time"], 4)))
    print("Average waiting time (s): " + str(round(metrics["avg_waiting"], 4)))
    print("Average turnaround time (s): " + str(round(metrics["avg_turnaround"], 4)))
    print("Overhead per job (ms): " + str(round(metrics["overhead_per_job"] * 1000, 2)))

    # Print the metrics for each priority level.
    for priority in [3, 2, 1]:
        p = str(priority)
        print("Priority " + p + ": jobs = " + str(metrics["count_p" + p])
              + ", avg waiting (s) = " + str(round(metrics["avg_waiting_p" + p], 4))
              + ", avg turnaround (s) = " + str(round(metrics["avg_turnaround_p" + p], 4)))

    print("Completion order: " + str(metrics["completion_order"]))


# Standard Python pattern: run main only when this script is run directly.
if __name__ == "__main__":
    main()
