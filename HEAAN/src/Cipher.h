#ifndef SCHEME_CIPHER_H_
#define SCHEME_CIPHER_H_

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

	long slots; ///< number of slots
	long level; ///< level of ciphertext

	//-----------------------------------------

	/**
	 * Ciphertext = (bx = mx + ex - ax * sx, ax) for secret key sx and error ex
	 * @param[in] slots: number of slots
	 * @param[in] level: ciphertext level
	 */
	Cipher(ZZX ax = ZZX::zero(), ZZX bx = ZZX::zero(), long slots = 1, long level = 1) : ax(ax), bx(bx), slots(slots) , level(level) {}

	//-----------------------------------------

	string toString();
};

#endif /* SCHEME_CIPHER_H_ */
