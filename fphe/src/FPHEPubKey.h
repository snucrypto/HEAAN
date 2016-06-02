#ifndef FPHEPUBKEY_H_
#define FPHEPUBKEY_H_

#include <NTL/ZZX.h>
#include <vector>

#include "FPHEParams.h"
#include "FPHESecKey.h"

using namespace std;
using namespace NTL;


class FPHEPubKey {
public:
	vector<  vector<ZZ>  > A0fft;
	vector<  vector<ZZ>  > A1fft;

	vector<ZZ> c1Starfft;
	vector<ZZ> c0Starfft;

	FPHEPubKey(FPHEParams& params, FPHESecKey& secretKey);
	virtual ~FPHEPubKey();
};

#endif /* FPHEPUBKEY_H_ */
