# SC-sample
SC-sample is a computer simulation tool for modeling spatial coalescent with recombination process, which generates ARGs directly consistent with the given sample. It attempts to uniformly select ARG from the sample-consistent ARG space. The algorithm is modified version of SC. 

sc-sample is implemented based on Macs-0.4e source code, thanks Gary.Chen to provide support.
sc-sample is used to generate ARG which is consistent with given sample. 
Below command will generate 100 ARGs matching given sample :
```
./sc-sample 20 1000 -r 0.015 -h 1000 -T -f example/sample_10_10.0 -i 100 1>stdout 2>stderr
```

**Below are README of Macs 0.4e**



# Introduction

MaCS is a simulator of the coalescent process that simulates geneologies spatially across chromosomes as a Markovian process.  The algorithm is similar to the Wiuf and Hein algorithm (Wiuf and Hein, 1999) in that an ancestral recombination graph (ARG) is stored in memory.  Where the algorithm deviates from the Wiuf and Hein are the following:

1) Recombination events occur only on the local geneology at the current position on the sequence instead of anywhere on the ARG, but can coalesce to any lineage on the ARG including those not on the local geneology (i.e. a non-ancestral edge)
2) Waiting times (i.e. the distance between recombinations on the sequence) are calculated from exponential draws with intensity based on the local geneology's branch length instead of the ARG length
3) The algorithm is n-th order Markovian where n is based on a parameter the user enters.  This makes the algorithm more general than one like FastCoal which is 1st order. Higher values provide a better approximation to the coalescent.

These changes make the algorithm substantially more efficient than the Wiuf and Hein with little loss in accuracy.

MaCS also supports all the demographic history semantics of MS.  Typing ./macs with no arguments at the command line lists the usage parameters.  Most command line arguments are the same as those in ms.

This document briefly summarizes how one compiles and runs MaCS.

Requirements:
g++
C++ boost development library (http://www.boost.org)
- If one has yum package manager, this is a matter of simply typing (under root):
yum install boost-devel

Compilation:
There are two executables.  MaCS is the simulator itself.  msformatter takes in generated data from the simulator and generates output that is compatible with that from Hudson's ms.

To compile everything:
```
make all
```

Compiling the simulator:
```
make macs
```

Compiling the MS formatter:
```
make msformatter
```

For moderate sample sizes and sequence lengths,  a typical command line would like like:
```
./macs 100 1e6 -T -t .001 -r .001 -h 1e2 -R inputfiles/hotspot.txt -F inputfiles/ascertainment.txt 0 2>trees.txt | ./msformatter > haplotypes.txt
```

The program is designed to send run-time information such as tree statistics to STDERR and simulated output to STDOUT.  This allows one to pipe desired output to other programs that may process data differently (e.g. a database loader) such as msformatter. 

The command line arguments above says:
Simulate 100 sequences on a region 1e6 basepairs long.  The per base pair mutation and recombination rate scaled at 4N is .001.  The h parameter approximates the Markovian order by instructing the program to include all geneologies from the current point to 1e2 base pairs to the left if one considers simulation proceeding from the left end to the right end of the entire sequence.  Any branches unique to the geneology that is beyond 1e2 base pairs is pruned from the ARG. -T tells MACS to output the local trees in Newick format similar to MS output.

The option -R instructs the program to read in a variable recombination file.  The first line of the file hotspot.txt says that from position 0 to .3 (unit scaled on entire sequence to be simulated), the cM to Mb ratio is 0.57. If there are any coordinates not covered by ranges in the file, the ratio defaults to 1.  
The option -F instructs the program to read in a SNP ascertainment file.  Entering 1 after the filename instructs the program to assume any derived allele frequency (DAF) > .5 to have a DAF of 1-DAF.  This might be useful for scenarios where one is interested only in the minor allele frequency and the identity of the derived allele is unknown.  The flag 0 disables this behavior.  The first line of the file ascertainment.txt instructs the program to completely filter out all SNPs with DAF of range 0 to 0.01.  The second line says to filter SNPs with DAF from 0.01 to 0.05 to the point where SNPs in this DAF range comprise 1% of the SNPs output.

For very large sample sizes and/or sequence lengths, we recommend storing output in a text file to postprocess later (e.g. import into a database)
 ```
./macs 10000 1e9 -t .001 -r .001 -h 1e2 -F inputfiles/ascertainment.txt 0 2>trees.txt 1> sites.txt
```
For comments or questions, please contact me at gary.k.chen@usc.edu

# Citation
If you use ```SC-sample``` in your research, please cite

Wang Y, Zhou Y, Li L, Chen X, Liu Y, Ma ZM, Xu S. A new method for modeling coalescent processes with recombination. BMC Bioinformatics. 2014 Aug 11;15(1):273. doi: 10.1186/1471-2105-15-273. PMID: 25113665; PMCID: PMC4137079.

(Link: https://bmcbioinformatics.biomedcentral.com/articles/10.1186/1471-2105-15-273)
