This directory contains a set of files that allow you to get the core and features 
of a variability model codified into a BDD using the BuDDy BDD package.

The BuDDy package is a package written in C for the manipulation of binary decision 
diagrams (BDDs), and it can be found on the server http://sourceforge.net/projects/buddy/.

Our algorithm is written in C and it uses buddy for computing core and dead features
form variability model.

INSTALLATION

To use our algorithm you will need to install BuDDy BDD package in your system. Follow 
the instructions in this section to install BuDDy BDD package on your system.

 1. Download buddy-2.4.tar.gz (766,5 kB).
 2. Unzip "gunzip buddy-2.4.tar.gz".
 3. Untar "tar xvf buddy-2.4.tar".
 4. "./configure"
 5. "make"
 6. "make install" 

DOCUMENTATION

Directory buddy-2.4/doc contains documentation for the BuDDy package in postscript(tm) 
format. The recommended starting point is bddnotes.ps, where an introduction to binary 
decision diagrams is shown. In addition, you can find a full description of the buddy 
package in buddy.ps.

Test

Follow the instructions in this section to test our algorithm.

 1. Download the code hperez30-CoreAndDeadFeatures-51bbbae.tar (2,1 kB).
 2. Unzip "gunzip hperez30-CoreAndDeadFeatures-51bbbae.tar.gz".
 3. Untar "tar xvf hperez30-CoreAndDeadFeatures-51bbbae.tar".
 
 The following directory estructures are created.
    "./configure"
    "make"
    "make install" 
 
 g++ -O3 -std=c++0x test.c core.c -o test -lbdd -lgmp -I ../buddy-2.4/src -I ./  
