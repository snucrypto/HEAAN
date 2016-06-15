#ifndef POLYSCHEME_POLYCIPHER_H_
#define POLYSCHEME_POLYCIPHER_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class PolyCipher {
public:
	ZZX c0;
	ZZX c1;
	long level;

	PolyCipher(ZZX c0, ZZX c1, long level) : c0(c0), c1(c1), level(level) {}
	virtual ~PolyCipher() {}

	string toString();
};

#endif /* POLYSCHEME_POLYCIPHER_H_ */
