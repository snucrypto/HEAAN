#ifndef HEAAN_SECRETKEY_H_
#define HEAAN_SECRETKEY_H_

#include <NTL/ZZX.h>

#include "NumUtils.h"

using namespace std;
using namespace NTL;


class SecretKey {
public:

	ZZX sx; ///< secret key

	SecretKey(long logN, long h = 64);

	SecretKey(ZZX sx = ZZX::zero()) : sx(sx) {};
	SecretKey(const SecretKey& o) : sx(o.sx) {};

};

#endif
