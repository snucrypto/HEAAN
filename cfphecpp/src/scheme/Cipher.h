#ifndef SCHEME_CIPHER_H_
#define SCHEME_CIPHER_H_

#include "../czz/CZZX.h"

using namespace std;
using namespace NTL;

class Cipher {
public:
	CZZX c0;
	CZZX c1;
	long level;
	ZZ B;
	ZZ nu;

	Cipher(CZZX c0, CZZX c1, long level, ZZ B, ZZ nu) : c0(c0), c1(c1), level(level), B(B), nu(nu) {}
};

#endif /* SCHEME_CIPHER_H_ */
