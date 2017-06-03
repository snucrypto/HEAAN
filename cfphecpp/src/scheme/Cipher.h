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

	Cipher(ZZX bx = ZZX::zero(), ZZX ax = ZZX::zero(), long slots = 1, long level = 1) : bx(bx), ax(ax), slots(slots) , level(level) {}
};

#endif /* SCHEME_CIPHER_H_ */
