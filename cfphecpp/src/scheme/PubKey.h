#ifndef SCHEME_PUBKEY_H_
#define SCHEME_PUBKEY_H_

#include <NTL/ZZX.h>
#include <vector>

#include "../scheme/PolyParams.h"
#include "../scheme/PolySecKey.h"

using namespace std;
using namespace NTL;


class PolyPubKey {
public:
	vector<ZZX> A0;
	vector<ZZX> A1;

	ZZX aStar;
	ZZX bStar;

	PolyPubKey(PolyParams& params, PolySecKey& secretKey);
	virtual ~PolyPubKey();
};

#endif /* SCHEME_PUBKEY_H_ */
