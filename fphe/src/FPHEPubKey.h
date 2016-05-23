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
	vector<ZZX> A0;
	vector<ZZX> A1;

	ZZX aStar;
	ZZX bStar;

	FPHEPubKey(FPHEParams& params, FPHESecKey& secretKey);
	virtual ~FPHEPubKey();
};

#endif /* FPHEPUBKEY_H_ */
