#ifndef SCHEME_PUBKEY_H_
#define SCHEME_PUBKEY_H_

#include <NTL/ZZX.h>
#include <vector>

#include "Params.h"
#include "SecKey.h"

using namespace std;
using namespace NTL;

class PubKey {
public:
	ZZX a;
	ZZX b;

	ZZX aStar;
	ZZX bStar;

	PubKey(Params& params, SecKey& secretKey);
};

#endif /* SCHEME_PUBKEY_H_ */
