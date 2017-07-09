#ifndef SCHEME_SECKEY_H_
#define SCHEME_SECKEY_H_

#include <NTL/ZZX.h>

#include "Params.h"

using namespace std;
using namespace NTL;

class SecKey {
public:

	ZZX sx; ///< secret key

	//-----------------------------------------

	SecKey(Params& params);

	//-----------------------------------------

};

#endif /* SCHEME_SECKEY_H_ */
