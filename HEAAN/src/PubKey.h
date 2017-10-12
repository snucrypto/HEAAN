#ifndef SCHEME_PUBKEY_H_
#define SCHEME_PUBKEY_H_

#include <NTL/ZZX.h>
#include <map>

#include "RLWE.h"
#include "SecKey.h"

using namespace std;
using namespace NTL;

class PubKey {
public:

	map<long, RLWE> keyMap;

	map<long, RLWE> leftRotKeyMap;

	//-----------------------------------------

	PubKey(Params& params, SecKey& secretKey);

	void addEncKey(Params& params, SecKey& secretKey);
	void addConjKey(Params& params, SecKey& secretKey);
	void addMultKey(Params& params, SecKey& secretKey);

	void addLeftRotKey(Params& params, SecKey& secretKey, long rot);
	void addLeftRotKeys(Params& params, SecKey& secretKey);
	void addRightRotKeys(Params& params, SecKey& secretKey);

	//-----------------------------------------

};

static long ENCRYPTION = 0;
static long MULTIPLICATION  = 1;
static long CONJUGATION = 2;


#endif /* SCHEME_PUBKEY_H_ */
