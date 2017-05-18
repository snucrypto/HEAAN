#ifndef SCHEME_CIPHER_H_
#define SCHEME_CIPHER_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Cipher {
public:

	ZZX c0;
	ZZX c1;

	long logSlots;
	long level;

	//-----------------------------------------

	Cipher(ZZX c0, ZZX c1, long logSlots, long level = 1) : c0(c0), c1(c1), logSlots(logSlots) , level(level) {}
};

#endif /* SCHEME_CIPHER_H_ */
