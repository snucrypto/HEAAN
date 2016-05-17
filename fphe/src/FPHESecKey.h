#ifndef FPHESECKEY_H_
#define FPHESECKEY_H_

#include <NTL/ZZX.h>

#include "FPHEParams.h"

using namespace NTL;

class FPHESecKey {
public:

	ZZX s;

	FPHESecKey(FPHEParams& params);
	virtual ~FPHESecKey();
};

#endif /* FPHESECKEY_H_ */
