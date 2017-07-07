# fpHE
fpHE is software library that implements homomorphic encryption (HE) that supports fixed point arithmetics.
This library supports approximate operations between rational numbers.
The approximate error depends on some parameters and almost same with floating point operation errors.
The scheme in this library is on the paper "Homomorphic Encryption for Arithmetic of Approximate Numbers" (https://eprint.iacr.org/2016/421.pdf).

This library does not contain bootstrapping yet, and we plan to add bootstrapping in next version.

This library is written by c++ and using NTL library (http://www.shoup.net/ntl/).

## How to use this library?
### 1. Build a static library
TBA

### 2. Use portable version
TBA

### 3. Running test functions
You can make a sample test program with test/HEAC.cpp by typing ./run in the main directory. We checked the sample program was working well on Ubuntu 16.04.2 LTS. You need to install NTL, GMP, pThread, and libbsd libraries before starting 'run'. After successful compilation, you can see 'a.out' file in test directory.
