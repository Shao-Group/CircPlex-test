#!/usr/bin/env python3
import os
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# ------------------------------------------------------------
# Helper: Plot histogram with dynamic ylim
# ------------------------------------------------------------
def plot_hist(values, title, output_file, color, tissue):
    values = np.array(values.dropna())
    if len(values) < 2:
        print(f"[!] Not enough data points for {title}")
        return

    plt.figure(figsize=(7,5))
    plt.hist(values, bins=100, color=color, edgecolor='black', alpha=0.7)
    plt.axvline(0, color='gray', lw=1, ls='--')
    plt.xlabel(r"$1 - |X_M| / |X|$")
    plt.ylabel("Number of complex reads")

    # --------------------------------------------------------
    # NEW: tissue-based ylim
    # --------------------------------------------------------
    if tissue.lower() == "testis":
        plt.ylim(0, 21000)
    elif tissue.lower() == "brain":
        plt.ylim(0, 28000)
    else:
        plt.ylim(0, 28000)  # default

    plt.tight_layout()
    plt.savefig(output_file, dpi=300)
    plt.close()
    print(f" → Saved: {output_file}")

# ------------------------------------------------------------
# Extract tissue name from filename like: testis_123_A.tsv
# ------------------------------------------------------------
def extract_tissue(path):
    base = os.path.basename(path)
    return base.split("_")[0]

# ------------------------------------------------------------
# Main
# ------------------------------------------------------------
def main():
    if len(sys.argv) != 4:
        print("Usage: python plot_XM_ratio.py <A_file> <AAbar_file> <output_dir>")
        sys.exit(1)

    A_file = sys.argv[1]
    AAbar_file = sys.argv[2]
    output_dir = sys.argv[3]

    os.makedirs(output_dir, exist_ok=True)

    tissue = extract_tissue(A_file)

    # ------------------------------------------------------------
    # Load TSVs
    # ------------------------------------------------------------
    A = pd.read_csv(A_file, sep="\t")
    AAbar = pd.read_csv(AAbar_file, sep="\t")

    # ------------------------------------------------------------
    # Compute 1 - |cigar| / |length|
    # ------------------------------------------------------------
    A_ratio = 1 - (A["cigar_A"].astype(float) / A["length_A"].astype(float))
    AAbar_ratio = 1 - (AAbar["cigar_AAbar"].astype(float) / AAbar["length_AAbar"].astype(float))

    # ------------------------------------------------------------
    # Plot outputs
    # ------------------------------------------------------------
    out_A = os.path.join(output_dir, f"{tissue}_A_hist.pdf")
    out_AAbar = os.path.join(output_dir, f"{tissue}_AAbar_hist.pdf")

    plot_hist(A_ratio, f"{tissue} A", out_A, "seagreen", tissue)
    plot_hist(AAbar_ratio, f"{tissue} AAbar", out_AAbar, "steelblue", tissue)


if __name__ == "__main__":
    main()
