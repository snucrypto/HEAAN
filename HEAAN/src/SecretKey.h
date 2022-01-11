/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_SECRETKEY_H_
#define HEAAN_SECRETKEY_H_

#include <NTL/ZZ.h>

#include "Ring.h"

namespace heaan {

class SecretKey {
public:

	NTL::ZZ* sx = new NTL::ZZ[N];

	SecretKey(Ring& ring);

};

}  // namespace heaan

#endif
