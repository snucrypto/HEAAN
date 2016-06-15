#ifndef POLY_FPHEPUBKEY_H_
#define POLY_FPHEPUBKEY_H_

#include <NTL/ZZX.h>
#include <vector>

#include "poly/FPHEParams.h"
#include "poly/FPHESecKey.h"

using namespace std;
using namespace NTL;


class FPHEPubKey {
public:
	vector<ZZX> A0;
	vector<ZZX> A1;

	ZZX aStar;
	ZZX bStar;

	FPHEPubKey(FPHEParams& params, FPHESecKey& secretKey);
	virtual ~FPHEPubKey();
};

#endif /* POLY_FPHEPUBKEY_H_ */
