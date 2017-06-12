#ifndef SCHEME_SECKEY_H_
#define SCHEME_SECKEY_H_

#include <NTL/ZZX.h>

#include "Params.h"

using namespace std;
using namespace NTL;

class SecKey {
public:

	ZZX sx;

	//-----------------------------------------

	SecKey(Params& params);

	string toString();
};

#endif /* SCHEME_SECKEY_H_ */
