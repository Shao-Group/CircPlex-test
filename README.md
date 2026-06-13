# Introduction

This repository contains data and scripts for reproducing the results of the circular RNA assembler CircPlex. The official CircPlex repository is available [here](https://github.com/Shao-Group/CircPlex).

# Dependency

Reproducing the results of CircPlex requires C++11 and python3.

# Data

The `data` folder contains integrated information from CircPlex experiments. There are two subdirectories `data/EquiRep` and `data/TideHunter` that contain folders `brain` and `testis` with information from the testis and brain sample respectively.

`data/{tool}/{tissue}/{tissue}_features_A_combined.tsv` - Details of circRNAs sseqeunces predicted by CircPlex from rolling circular nanopore long reads.

`data/{tool}/{tissue}/{tissue}_features_AAbar_combined.tsv` - Details of seqeunces containing circRNA and its partial reverse complement from rolling circular nanopore long reads.

`data/{tool}/{tissue}/{tissue}_bsj_A.tsv` - Details of back-splicing junctions from circRNAs predicted by CircPlex.

`data/{tool}/{tissue}/{tissue}_bsj_A_0.1.tsv` - Details of back-splicing junctions from circRNAs predicted by CircPlex that have absolute over-prediction ratio ≤ 1.

`data/{tool}/{tissue}/{tissue}_bsj_cluster_A.tsv` - Details of clustered back-splicing junctions from circRNAs predicted by CircPlex.

`data/{tool}/{tissue}/{tissue}_bsj_cluster_A_0.1.tsv` - Details of clustered back-splicing junctions from circRNAs predicted by CircPlex that have absolute over-prediction ratio ≤ 1.

# Scripts

The `scripts` directory contain scripts to reproduce the results from the CircPlex experiments.

`scripts/plot.py` - To generate over-prediction ratio distributions of circRNA seqeunces from CircPlex and seqeunces containing circRNA and its partial reverse complement.

`scripts/compare.cpp` - To compare overlaps of BSJs predicted by CircPlex and isoCirc with circBase.

# Plot Generation

For generating plots, run the following command.
```
python3 scripts/plot.py data/{tool}/{tissue}/{tissue}_features_A_combined.tsv data/{tool}/{tissue}/{tissue}_features_AAbar_combined.tsv <output-driectory>
```
Output files `{tissue}_A_hist.pdf` and `{tissue}_AAbar_hist.pdf` will be generated inside `<output-directory>`. 

# Overlap Comparison

The cpp file `scripts/compare.cpp` takes as input a BSJ file from the `/data/{tool}/{tissue}` folder, the isocirc annotaion (isocirc.out) file for the tissue and circBase annotation (bed) file. An output file `{prefix}_venn_counts.txt` is generated in the specified output directory that summarizes the overlap counts between CircPlex, isoCirc and circBase. The `{prefix}` will be automiatically generated based on the input BSJ file.

To generate isoCirc annotations, download the corresponding long reads data for human testis and brain sampples from GEO accession number: GSE141693. Then run the tool isoCirc on the testis and brain reads to get respective isoCirc annotations. Note that the isoCirc annotations used in CircPlex evaluation are filtered to remove duplicate BSJs and partial circRNAs.

## Install isoCirc

To install isoCirc [(license)](https://github.com/Xinglab/isoCirc/blob/main/LICENSE), visit (https://github.com/Xinglab/isoCirc).

## Download circBase Annotation

The circBase annotation bed file can be downloaded from [here](https://www.circbase.org/cgi-bin/downloads.cgi).

## Usage
Compile the `scripts/compare.cpp` file.

```
g++ -O3 scripts/comapre.cpp -o scripts/compare
```

Run the follwoing command.
```
scripts/compare /data/{tool}/{tissue}/{tissue}_{bsj/cluster}_{A/A_0.1}.tsv <tissue-isocirc-annotation (filtered isocirc.out file)> <circBase-bed-file> <output-directory>
```

Output file `{tissue}_{bsj/cluster}_{A/A_0.1}_venn_counts.txt` will be generated inside `<output-directory>` with the overlap information.

