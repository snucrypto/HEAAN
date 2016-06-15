#ifndef POLY_FPHESECKEY_H_
#define POLY_FPHESECKEY_H_

#include <NTL/ZZX.h>

#include "poly/FPHEParams.h"

using namespace NTL;

class FPHESecKey {
public:

	ZZX s;

	FPHESecKey(FPHEParams& params);
	virtual ~FPHESecKey();
};

#endif /* POLY_FPHESECKEY_H_ */
