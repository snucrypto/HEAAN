#ifndef FPHEFFTSECKEY_H_
#define FPHEFFTSECKEY_H_

#include <NTL/ZZX.h>

#include "FPHEFFTParams.h"

using namespace std;
using namespace NTL;

class FPHEFFTSecKey {
public:

	vector<ZZ> s;
	vector<ZZ> sfft;

	FPHEFFTSecKey(FPHEFFTParams& params);
	virtual ~FPHEFFTSecKey();
};

#endif /* FPHEFFTSECKEY_H_ */
