#ifndef SCHEME_CIPHER_H_
#define SCHEME_CIPHER_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Cipher {
public:

	ZZX b;
	ZZX a;

	long slots;
	long level;

	//-----------------------------------------

	Cipher(ZZX b = ZZX::zero(), ZZX a = ZZX::zero(), long slots = 1, long level = 1) : b(b), a(a), slots(slots) , level(level) {}
};

#endif /* SCHEME_CIPHER_H_ */
