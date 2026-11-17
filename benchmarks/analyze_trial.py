# analyze_trial.py
# Reads the raw data of one trial and computes the metrics.
# Run this script from the repository root.
# Usage:   python3 benchmarks/analyze_trial.py <trial_name>
# Example: python3 benchmarks/analyze_trial.py test1
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


# Main function: read the raw data and print the metrics.
def main():
    # Step 1: check that we got the trial name.
    if len(sys.argv) != 2:
        print("Usage: python3 benchmarks/analyze_trial.py <trial_name>")
        return

    # Step 2: build the file names.
    trial_name = sys.argv[1]
    jobs_log = "benchmarks/trials/" + trial_name + "_jobs.csv"
    batch_log = "benchmarks/trials/" + trial_name + "_batch.csv"

    # Step 3: read the batch start and end time.
    batch_file = open(batch_log, "r")
    reader = csv.DictReader(batch_file)
    for row in reader:
        batch_start = float(row["batch_start"])
        batch_end = float(row["batch_end"])
    batch_file.close()

    # Step 4: prepare empty lists for the results.
    waiting_times = []
    turnaround_times = []
    completion_order = []

    # One list per priority level: 1 = low, 2 = medium, 3 = high
    waiting_by_priority = {1: [], 2: [], 3: []}
    turnaround_by_priority = {1: [], 2: [], 3: []}

    # Step 5: read the jobs one by one.
    # Each job writes its line when it finishes.
    # So the line order in the file is the completion order.
    jobs_file = open(jobs_log, "r")
    reader = csv.DictReader(jobs_file)
    for row in reader:
        job_id = int(row["id"])
        priority = int(row["priority"])
        start = float(row["start"])
        end = float(row["end"])

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

    # Step 6: print the summary.
    print("Trial: " + trial_name)
    print("Jobs: " + str(len(waiting_times)))
    print("Total batch time (s): " + str(round(batch_end - batch_start, 4)))
    print("Average waiting time (s): " + str(round(average(waiting_times), 4)))
    print("Average turnaround time (s): " + str(round(average(turnaround_times), 4)))

    # Print the metrics for each priority level.
    for priority in [3, 2, 1]:
        count = len(waiting_by_priority[priority])
        avg_wait = average(waiting_by_priority[priority])
        avg_turn = average(turnaround_by_priority[priority])
        print("Priority " + str(priority) + ": jobs = " + str(count)
              + ", avg waiting (s) = " + str(round(avg_wait, 4))
              + ", avg turnaround (s) = " + str(round(avg_turn, 4)))

    print("Completion order: " + str(completion_order))


# Standard Python pattern: run main only when this script is run directly.
if __name__ == "__main__":
    main()
