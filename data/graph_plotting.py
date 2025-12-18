import csv
import matplotlib.pyplot as plt

files = {
    "OLD Depth vs Time (1 Core)": ("timing_results1.csv", "depth_time_1_core_old.png"),
    "OLD Depth vs Time (8 Cores)": ("timing_results8.csv", "depth_time_8_core_old.png"),
    "Depth vs Time (8 Cores)": ("new_results8.csv", "depth_time_8_core_new.png"),
}

for title, (filename, output_png) in files.items():
    depth = []
    seq = []
    thread = []
    omp = []

    with open(filename, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            depth.append(int(row["Depth"]))
            seq.append(float(row["Search"]))
            thread.append(float(row["ThreadSearch"]))
            omp.append(float(row["OMPSearch"]))

    plt.figure()
    plt.plot(depth, seq, label="Sequential")
    plt.plot(depth, thread, label="Threads")
    plt.plot(depth, omp, label="OpenMP")

    plt.xlabel("Depth")
    plt.ylabel("Time (seconds)")
    plt.title(title)
    plt.grid(True)

    plt.legend(
        fontsize=8,
        framealpha=0.8,
        borderpad=0.3,
        labelspacing=0.3,
    )

    plt.tight_layout()
    plt.savefig(output_png, dpi=300)
    plt.close()
