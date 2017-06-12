#ifndef SCHEME_CIPHER_H_
#define SCHEME_CIPHER_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Cipher {
public:


	ZZX bx;
	ZZX ax;

	long slots;
	long level;

	//-----------------------------------------

	/**
	 * Ciphertext bx = mx + ex - ax * sx
	 * slots: batch number of slots
	 * level: ciphertext level
	 */
	Cipher(ZZX bx = ZZX::zero(), ZZX ax = ZZX::zero(), long slots = 1, long level = 1) : bx(bx), ax(ax), slots(slots) , level(level) {}

	string toString();
};

#endif /* SCHEME_CIPHER_H_ */
