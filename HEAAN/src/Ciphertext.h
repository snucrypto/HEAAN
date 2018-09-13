/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_CIPHERTEXT_H_
#define HEAAN_CIPHERTEXT_H_

#include <NTL/ZZ.h>

#include <fstream>

using namespace std;
using namespace NTL;

/**
 * Ciphertext is an RLWE instance (ax, bx = mx + ex - ax * sx) in the ring Z_q[X] / (X^N + 1)
 */
class Ciphertext {
public:

	ZZ* ax; ///< a(x) - part of RLWE instance
	ZZ* bx; ///< b(x) - part of RLWE instance

	long logp; ///< number of message quantized bits
	long logq; ///< number of modulus bits

	long N; ///< degree of RLWE
	long n; ///< number of slots

	Ciphertext(ZZ* ax = NULL, ZZ* bx = NULL, long logp = 0, long logq = 0, long N = 0, long n = 0);

	Ciphertext(const Ciphertext& o);

	Ciphertext& operator=(const Ciphertext &o);

	virtual ~Ciphertext();
	
	void kill();

};

#endif
