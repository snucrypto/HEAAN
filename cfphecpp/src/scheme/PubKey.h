#ifndef SCHEME_PUBKEY_H_
#define SCHEME_PUBKEY_H_

#include <NTL/ZZX.h>

#include "SecKey.h"

using namespace std;
using namespace NTL;

class PubKey {
public:

	ZZX ax;
	ZZX bx;

	ZZX axStar;
	ZZX bxStar;

	ZZX* axKeySwitch;
	ZZX* bxKeySwitch;

	//-----------------------------------------

	PubKey(Params& params, SecKey& secretKey);

	string toString();
};

#endif /* SCHEME_PUBKEY_H_ */
