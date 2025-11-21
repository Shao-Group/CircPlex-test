# Introduction

This repository contains data and scripts for reproducing the results of the circular RNA assembler CircPlex. The official CircPlex repository is available [here](https://github.com/Shao-Group/CircPlex).

# Dependency

Reproducing the results of CircPlex requires C++11 and python3.

# Data

The `data` folder contains integrated information from CircPlex experiments. There are two subdirectories `data/testis` and `data/brain` that contain information from the testis and brain sample respectively.

`data/{tissue}/{tissue}_features_A_combined.tsv` - Details of circRNAs sseqeunces predicted by CircPlex from rolling circular nanopore long reads.

`data/{tissue}/{tissue}_features_AAbar_combined.tsv` - Details of seqeunces containing circRNA and its partial reverse complement from rolling circular nanopore long reads.

`data/{tissue}/{tissue}_bsj_A.tsv` - Details of back-splicing junctions from circRNAs predicted by CircPlex.

`data/{tissue}/{tissue}_bsj_A_0.1.tsv` - Details of back-splicing junctions from circRNAs predicted by CircPlex that have absolute over-prediction ratio ≤ 1.

`data/{tissue}/{tissue}_bsj_cluster_A.tsv` - Details of clustered back-splicing junctions from circRNAs predicted by CircPlex.

`data/{tissue}/{tissue}_bsj_cluster_A_0.1.tsv` - Details of clustered back-splicing junctions from circRNAs predicted by CircPlex that have absolute over-prediction ratio ≤ 1.

# Scripts

The `scripts` directory contain scripts to reproduce the results from the CircPlex experiments.

`scripts/plot.py` - To generate over-prediction ratio distributions of circRNA seqeunces from CircPlex and seqeunces containing circRNA and its partial reverse complement.

`scripts/compare.cpp` - To compare overlaps of BSJs predicted by CircPlex and isoCirc with circBase.

# Plot Generation

For generating plots, use th following command:
```
python3 scripts/plot.py data/{tissue}/{tissue}_features_A_combined.tsv data/{tissue}/{tissue}_features_AAbar_combined.tsv <output-driectory>
```
Running the above command will generate two PDF files `{tissue}_A_hist.pdf` and `{tissue}_AAbar_hist.pdf` inside `<output-driectory>` 

# Comparing Overlaps

The cpp file `scripts/compare.cpp` takes as input a BSJ file from the `/data/{tissue}` folder, the isocirc annotaion (isocirc.out) file for the tissue and circBase annotation (bed) file. It outputs a file `{prefix}_venn_counts.txt` in the specified output directory that summarizes the overlap counts between CircPlex, isoCirc and circBase. The `{prefix}` will be automiatically generated based on the input BSJ file.

## Install isoCirc

## Download circBase Annotation

To generate isoCirc annotations, download the corresponding long reads data for human testis and brain sampples from GEO accession number: GSE141693. Then run the tool isoCirc on the testis and brain reads to get respective isoCirc annotations.

```
g++ -O3 scripts/comapre.cpp -o scripts/compare
```
```
scripts/compare /data/{tissue}/{tissue}_{bsj/cluster}_{A/A_0.1}.tsv <tissue-isocirc-annotation (isocirc.out file)> <circBase-bed-file> <output-directory>
```

