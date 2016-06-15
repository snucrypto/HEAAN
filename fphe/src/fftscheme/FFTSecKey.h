#ifndef FPHEFFTSECKEY_H_
#define FPHEFFTSECKEY_H_

#include <NTL/ZZX.h>

#include "FFTParams.h"

using namespace std;
using namespace NTL;

class FFTSecKey {
public:

	vector<ZZ> s;
	vector<ZZ> sfft;

	FFTSecKey(FFTParams& params);
	virtual ~FFTSecKey();
};

#endif /* FPHEFFTSECKEY_H_ */
