# WHAM

A <b>generalized</b> Weighted Histogram Analysis Method

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

[http://pubs.acs.org/doi/abs/10.1021/jz501811k]
