#ifndef HEAAN_PLAINTEXT_H_
#define HEAAN_PLAINTEXT_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Plaintext {
public:

	ZZX mx; ///< message mod X^N + 1

	ZZ q; ///< ciphertext modulus
	long logq; ///< number of bits in modulus
	long slots; ///< number of slots in ciphertext

	bool isComplex; ///< there is an option for single real value
	//-----------------------------------------

	/**
	 * Plaintext: mx
	 * @param[in] mx: ZZX polynomial
	 * @param[in] q: ciphertext modulus
	 * @param[in] logq: number of bits in modulus
	 * @param[in] slots: number of slots in message
	 */
	Plaintext(ZZX mx = ZZX::zero(), ZZ q = ZZ::zero(), long logq = 0, long slots = 1, bool isComplex = true) : mx(mx), q(q), logq(logq), slots(slots), isComplex(isComplex) {}

	Plaintext(const Plaintext& o) : mx(o.mx), q(o.q), logq(o.logq), slots(o.slots), isComplex(o.isComplex) {}
};

#endif
