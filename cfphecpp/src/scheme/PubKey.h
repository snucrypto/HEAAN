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

	vector<ZZX> aStars;
	vector<ZZX> bStars;

	PubKey(Params& params, SecKey& secretKey);
};

#endif /* SCHEME_PUBKEY_H_ */
