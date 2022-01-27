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
#include "Params.h"

namespace heaan {

class Ciphertext {
public:

	NTL::ZZ* ax = new NTL::ZZ[N];
	NTL::ZZ* bx = new NTL::ZZ[N];

	long logp;
	long logq;

	long n;

	Ciphertext(long logp = 0, long logq = 0, long n = 0);

	Ciphertext(const Ciphertext& o);

	void copyParams(Ciphertext& o);

	void copy(Ciphertext& o);

	void free();

	virtual ~Ciphertext();

};

}  // namespace heaan

#endif
