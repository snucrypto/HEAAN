#ifndef SCHEME_SECKEY_H_
#define SCHEME_SECKEY_H_

#include "../czz/CZZX.h"
#include "Params.h"

class SecKey {
public:

	CZZX s;

	SecKey(Params& params);
};

#endif /* SCHEME_SECKEY_H_ */
