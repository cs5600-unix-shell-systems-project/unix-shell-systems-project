# make_table.py
# Turns a summary file into a LaTeX table for the paper.
# Run this script from the repository root.
# Usage:   python3 benchmarks/make_table.py <prefix>
# Example: python3 benchmarks/make_table.py baseline
# Input:   benchmarks/results/<prefix>_summary.csv
# Output:  benchmarks/results/<prefix>_table.tex

import sys    # Read command-line arguments.
import csv    # Read CSV files.

# Workload sizes. These must match run_all.sh.
SIZES = [10, 50, 100]

# Table columns. Each column has four parts:
#   1. the metric name in the summary file
#   2. the column title in the table
#   3. the scale factor. 1 keeps seconds. 1000 turns seconds into milliseconds.
#   4. the number of decimal places to show
# Columns in seconds use 4 decimal places, so small standard deviations stay visible.
COLUMNS = [
    ("total_time", "Total (s)", 1, 4),
    ("avg_waiting_p3", "Wait, high (s)", 1, 4),
    ("avg_waiting_p2", "Wait, medium (s)", 1, 4),
    ("avg_waiting_p1", "Wait, low (s)", 1, 4),
    ("avg_waiting", "Wait, all (s)", 1, 4),
    ("overhead_per_job", "Overhead/job (ms)", 1000, 2),
]


# Main function: read the summary and write the LaTeX table.
def main():
    # Step 1: check that we got the prefix.
    if len(sys.argv) != 2:
        print("Usage: python3 benchmarks/make_table.py <prefix>")
        return
    prefix = sys.argv[1]
    input_file = "benchmarks/results/" + prefix + "_summary.csv"
    output_file = "benchmarks/results/" + prefix + "_table.tex"

    # Step 2: read the summary into a dictionary.
    # The key is (size, metric). The value is (mean, std).
    data = {}
    f = open(input_file, "r")
    reader = csv.DictReader(f)
    for row in reader:
        key = (int(row["size"]), row["metric"])
        data[key] = (float(row["mean"]), float(row["std"]))
    f.close()

    # Step 3: build the table line by line.
    lines = []
    lines.append("\\begin{table*}[t]")
    lines.append("\\centering")
    lines.append("\\footnotesize")    # Smaller font, so the wide table fits the page.

    # Write a caption that fits the experiment group.
    if prefix == "baseline":
        lines.append("\\caption{Baseline results for the base shell, which runs jobs in FIFO order. "
                     "Values are mean $\\pm$ standard deviation over five trials.}")
    else:
        lines.append("\\caption{Results for the " + prefix + " group. "
                     "Values are mean $\\pm$ standard deviation over five trials.}")
    lines.append("\\label{tab:" + prefix + "}")

    # One centered column for the job count, plus one for each metric.
    column_format = "|c|"
    for column in COLUMNS:
        column_format = column_format + "c|"
    lines.append("\\begin{tabular}{" + column_format + "}")
    lines.append("\\hline")

    # The header row.
    header = "Jobs"
    for metric, title, scale, decimals in COLUMNS:
        header = header + " & " + title
    lines.append(header + " \\\\")
    lines.append("\\hline")

    # One row for each workload size.
    for size in SIZES:
        row_text = str(size)
        for metric, title, scale, decimals in COLUMNS:
            mean, std = data[(size, metric)]
            # Round both numbers to the decimal places of this column.
            number_format = "{:." + str(decimals) + "f}"
            cell = number_format.format(mean * scale) + " $\\pm$ " + number_format.format(std * scale)
            row_text = row_text + " & " + cell
        lines.append(row_text + " \\\\")

    lines.append("\\hline")
    lines.append("\\end{tabular}")
    lines.append("\\end{table*}")

    # Step 4: write all lines to the output file.
    f = open(output_file, "w")
    for line in lines:
        f.write(line + "\n")
    f.close()
    print("Table written to " + output_file)


# Standard Python pattern: run main only when this script is run directly.
if __name__ == "__main__":
    main()
