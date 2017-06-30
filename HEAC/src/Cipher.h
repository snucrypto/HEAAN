#ifndef SCHEME_CIPHER_H_
#define SCHEME_CIPHER_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Cipher {
public:

	ZZX bx;
	ZZX ax;

	long slots; ///< number of slots
	long level; ///< level of ciphertext

	//-----------------------------------------

	/**
	 * Ciphertext = (bx = mx + ex - ax * sx, ax) for secret key sx and error ex
	 * @param[in] slots: number of slots
	 * @param[in] level: ciphertext level
	 */
	Cipher(ZZX bx = ZZX::zero(), ZZX ax = ZZX::zero(), long slots = 1, long level = 1) : bx(bx), ax(ax), slots(slots) , level(level) {}

	//-----------------------------------------

	string toString();
};

#endif /* SCHEME_CIPHER_H_ */
