#ifndef FPHESECKEY_H_
#define FPHESECKEY_H_

#include <NTL/ZZX.h>

#include "FPHEParams.h"

using namespace std;
using namespace NTL;

class FPHESecKey {
public:

	vector<ZZ> s;
	vector<ZZ> sfft;

	FPHESecKey(FPHEParams& params);
	virtual ~FPHESecKey();
};

#endif /* FPHESECKEY_H_ */
