This directory contains a set of files that allow you to get the core and dead features of a variability model codified into a BDD using the BuDDy BDD package.

The BuDDy package is a package for the manipulation of binary decision diagrams (BDDs), and it can be found on the server http://sourceforge.net/projects/buddy/.

Our algorithm is written in C++ and it uses buddy for computing core and dead features form variability model.

# INSTALLATION

To use our algorithm you will need to install BuDDy BDD package in your system. Follow 
the instructions in this section to install BuDDy BDD package on your system.

 1. Download buddy-2.4.tar.gz (766,5 kB).
 2. Unzip "gunzip buddy-2.4.tar.gz".
 3. Untar "tar xvf buddy-2.4.tar".
 4. "./configure"
 5. "make"
 6. "make install" 

## DOCUMENTATION

The buddy-2.4/doc directory contains documentation for the BuDDy package in postscript(tm) format. An introduction to binary decision diagrams is described in bddnotes.ps. In addition, you can find a full description of the buddy package in buddy.ps.

## TEST

Follow the instructions in this section to test our algorithm.

1. Download the code https://github.com/hperez30/CoreAndDeadFeatures/archive/refs/tags/2.0.0.zip.
2. Unzip "gunzip CoreAndDeadFeatures-2.0.0.zip".
 
 The following directories are created:    
    coreanddead/src/                the files required by our algorithm and a simple test program called test.cpp
    coreanddead/samples/            some bdd examples 
    readme.txt
 
 ## COMPILE 
 
 Follow the instructions in this section to compile test program.
    
 1. Open a terminal (Ctrl + Alt + T in ubuntu).
 2. Unzip "gunzip CoreAndDeadFeatures-2.0.0.zip".
 3. Go to "coreanddead/" directory. 
 4. Type "g++ -O3 -Wall -Wextra -std=c++0x ./src/test.cpp ./src/core.cpp ./src/utils.cpp -o ./bin/test -lbdd -I path_to_buddy/src -I . -I ./src/ -L ./lib/"

## EXECUTE

Follow the instructions in this section to run the test program.

 1. Open a terminal (Ctrl + Alt + T in ubuntu).
 2. Go to "coreanddead/" directory.
 3. Type "./bin/test path_to_bdd_sample_file option_method".
 
## CONTACT

 If you need further information, do not hesitate to contact Hector Perez-Morago (hperez@issi.uned.es).
 
 
