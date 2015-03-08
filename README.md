# WHAM

We have implemented a Weighted Histogram Analysis Method (WHAM) that is:

1. <b>Generalized</b> to handle data generated from:
    *  <b>N-dimensional</b> Umbrella Sampling (US)
    *  Temperature Replica Exchange (T-REX)
    *  <b>N-dimensional</b> Hamiltonian Replica Exchange (H-REX)
    *  <b>N-dimensional</b> Hamiltonian Mapping (i.e., Hamiltonian re-weighting)
    *  A combination of any of the above methods
2. Uses energies as input which avoids false assumptions (i.e., K vs. K/2 force constants)
3. Written in C++ and is both speed and memory efficient
4. Does <b>not</b> require binning (i.e., binless) and is, therefore, theoretically more accurate than all other binning-based methods
5. Can generate <b>N-dimensional</b> potentials-of-mean-force (PMFs) using sparse matrices

#Installation

    make clean
    make install

The executable can be found in /path/to/WHAM/bin/wham

#Usage

    Usage:   wham [-options] <metadatafile>
    Options: [-bins rcoor1[:rcoor2[:rcoor3]]]
             [-iter value] [-tol value | -Ftol value]
             [-temp T1[:T2...[[:TN[:Ttarget]]]] | -temp T1=TN=[incr[=Ttarget]]]
             [-exp n:T1:T2]
             [-fguess file | -fval file]

#Bugs and Comments

Please send any comments, bug reports, and suggestions to 
    
    Sean M. Law, seanmylaw@gmail.com
    
# Reference

If you use this code, please cite:

Law S. M., L. S. Ahlstrom, A. Panahi, C. L. Brooks III. <b>Hamiltonian Mapping Revisited: Calibrating Minimalist Models to Capture Molecular Recognition by Intrinsically Disordered Proteins</b>, 2014, <i>J. Phys. Chem. Lett.</i> 5(19):3441-3444.

[Link](http://pubs.acs.org/doi/abs/10.1021/jz501811k)

# Acknowledgements

This work greatly benefited from valuable scientific discussions with Dr. [Michael Feig](http://feig.bch.msu.edu) and Dr. [Bing Zhang](https://www.linkedin.com/pub/bin-zhang/70/410/50b)
