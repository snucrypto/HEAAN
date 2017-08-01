#ifndef SCHEME_PUBKEY_H_
#define SCHEME_PUBKEY_H_

#include <NTL/ZZX.h>

#include "SecKey.h"

using namespace std;
using namespace NTL;

class PubKey {
public:

	ZZX ax; ///<< information for symmetric encryption
	ZZX bx; ///<< information for symmetric encryption

	ZZX axStar; ///<< auxiliary information for multiplication
	ZZX bxStar; ///<< auxiliary information for multiplication

	ZZX axConj; ///<< auxiliary information for conjugation
	ZZX bxConj; ///<< auxiliary information for conjugation

	ZZX* axLeftRot; ///< auxiliary information for rotation
	ZZX* bxLeftRot; ///< auxiliary information for rotation

	ZZX* axRightRot; ///< auxiliary information for rotation
	ZZX* bxRightRot; ///< auxiliary information for rotation

	//-----------------------------------------

	PubKey(Params& params, SecKey& secretKey);

	//-----------------------------------------

};

#endif /* SCHEME_PUBKEY_H_ */
