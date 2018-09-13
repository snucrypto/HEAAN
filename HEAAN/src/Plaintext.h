/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_PLAINTEXT_H_
#define HEAAN_PLAINTEXT_H_

#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class Plaintext {
public:

	ZZ* mx;
	long N;

	long logp;
	long logq;

	long n; ///< number of slots in ciphertext

	//-----------------------------------------

	Plaintext(ZZ* mx = NULL, long logp = 0, long logq = 0, long N = 1, long n = 1);

	Plaintext(const Plaintext& o);

	Plaintext& operator=(const Plaintext &o);

	virtual ~Plaintext();
};

#endif
