# HEAAN
HEAAN is software library that implements homomorphic encryption (HE) that supports fixed point arithmetics.
This library supports approximate operations between rational numbers.
The approximate error depends on some parameters and almost same with floating point operation errors.
The scheme in this library is on the paper "Homomorphic Encryption for Arithmetic of Approximate Numbers" (https://eprint.iacr.org/2016/421.pdf).

## Version
v1.0 Implementation of Original HEAAN scheme

V1.1 Implementation of Original HEAAN scheme with bootstrapping (https://eprint.iacr.org/2018/153.pdf)

V2.1 Faster Implementation of HEAAN scheme

## Dependency
This library is written by c++ and using NTL library (http://www.shoup.net/ntl/).

## How to use this library?
### 1. Build a static library/Running test functions
You can make a static library by typing "make all" in the /lib directory. After successful compilation you can find a static library libHEAAN.a in the /lib directory.

After you build libHEAAN.a, you can run a test program in the /run directory. In run.cpp, you need uncomment tests you need and type "make" in the /run directory. This command will run exe file "HEAAN".

We checked the program was working well on Ubuntu 16.04.2 LTS. You need to install NTL (with GMP), pThread, libraries.

## License
Copyright (c) by CryptoLab inc.
This program is licensed under a
Creative Commons Attribution-NonCommercial 3.0 Unported License.
You should have received a copy of the license along with this
work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.

## Example
```c++
#include "HEAAN.h"

using namespace std;
using namespace NTL;

int main() {

  // Parameters //
  long logN = 15;
  long logQ = 353;
  long logp = 30; ///< Larger logp will give you more correct result (smaller computation noise)
  long slots = 1024; ///< This should be power of two
  long numThread = 8;
	
  // Construct and Generate Public Keys //
  TimeUtils timeutils;
  Ring ring(logN, logQ);
  SecretKey secretKey(ring);
  Scheme scheme(secretKey, ring);
  scheme.addLeftRotKeys(secretKey); ///< When you need left rotation for the vectorized message
  scheme.addRightRotKeys(secretKey); ///< When you need right rotation for the vectorized message
  
  // Make Random Array of Complex //
  complex<double>* mvec1 = EvaluatorUtils::randomComplexArray(slots);
  complex<double>* mvec2 = EvaluatorUtils::randomComplexArray(slots);
  
  // Encrypt Two Arry of Complex //
  Ciphertext cipher1 = scheme.encrypt(mvec1, slots, logp, logQ);
  Ciphertext cipher2 = scheme.encrypt(mvec2, slots, logp, logQ);
  
  // Addition //
  Ciphertext cipherAdd = scheme.add(cipher1, cipher2);
  
  // Multiplication And Rescale //
  Ciphertext cipherMult = scheme.mult(cipher1, cipher2);
  Ciphertext cipherMultAfterReScale = scheme.reScaleBy(cipherMult, logp);
  
  // Rotation //
  long idx = 1;
  Ciphertext cipherRot = scheme.leftRotate(cipher1, idx);
  
  // Decrypt //
  complex<double>* dvec1 = scheme.decrypt(secretKey, cipher1);
  complex<double>* dvec2 = scheme.decrypt(secretKey, cipher2);
  
  return 0;

}
  
```
