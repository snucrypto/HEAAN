#ifndef SCHEME_CIPHER_H_
#define SCHEME_CIPHER_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Cipher {
public:

	ZZX c0;
	ZZX c1;

	long slots;
	long level;

	//-----------------------------------------

	Cipher(ZZX c0 = ZZX::zero(), ZZX c1 = ZZX::zero(), long slots = 1, long level = 1) : c0(c0), c1(c1), slots(slots) , level(level) {}
};

#endif /* SCHEME_CIPHER_H_ */
