#ifndef POLYSCHEME_POLYSECKEY_H_
#define POLYSCHEME_POLYSECKEY_H_

#include <NTL/ZZX.h>

#include "PolyParams.h"

using namespace NTL;

class PolySecKey {
public:

	ZZX s;

	PolySecKey(PolyParams& params);
	virtual ~PolySecKey();
};

#endif /* POLYSCHEME_POLYSECKEY_H_ */
