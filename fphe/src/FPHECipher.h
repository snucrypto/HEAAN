#ifndef FPHECIPHER_H_
#define FPHECIPHER_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class FPHECipher {
public:
	ZZX c0;
	ZZX c1;
	long level;

	FPHECipher(ZZX c0, ZZX c1, long level) : c0(c0), c1(c1), level(level) {}
	virtual ~FPHECipher() {}

	string toString();
};

#endif /* FPHECIPHER_H_ */
