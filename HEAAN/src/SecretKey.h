/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
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
