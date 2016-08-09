#ifndef SCHEME_SECKEY_H_
#define SCHEME_SECKEY_H_

#include <NTL/ZZX.h>

#include "../scheme/PolyParams.h"

using namespace NTL;

class PolySecKey {
public:

	ZZX s;

	PolySecKey(PolyParams& params);
	virtual ~PolySecKey();
};

#endif /* SCHEME_SECKEY_H_ */
