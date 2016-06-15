#ifndef POLYSCHEME_POLYPUBKEY_H_
#define POLYSCHEME_POLYPUBKEY_H_

#include <NTL/ZZX.h>
#include <vector>

#include "PolySecKey.h"
#include "PolyParams.h"

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

#endif /* POLYSCHEME_POLYPUBKEY_H_ */
