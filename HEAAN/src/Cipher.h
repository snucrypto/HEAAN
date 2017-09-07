#ifndef SCHEME_CIPHER_H_
#define SCHEME_CIPHER_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

/**
 * Cipher consist a pair of elements (ax, bx) of ring Z_qi[X] / (X^N + 1)
 *
 */
class Cipher {
public:

	ZZX ax;
	ZZX bx;

	ZZ mod; ///< mod in cipher
	long cbits; ///< bits in cipher
	long slots; ///< number of slots

	//-----------------------------------------

	/**
	 * Ciphertext = (bx = mx + ex - ax * sx, ax) for secret key sx and error ex
	 * @param[in] bits: bits in cipher
	 * @param[in] slots: number of slots
	 */
	Cipher(ZZX ax = ZZX::zero(), ZZX bx = ZZX::zero(), ZZ mod = ZZ::zero(), long cbits = 0, long slots = 1) : ax(ax), bx(bx), mod(mod), cbits(cbits), slots(slots) {}

	Cipher(const Cipher& o) : ax(o.ax), bx(o.bx), mod(o.mod), cbits(o.cbits), slots(o.slots) {}

};

#endif /* SCHEME_CIPHER_H_ */
