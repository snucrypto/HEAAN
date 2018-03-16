/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_KEY_H_
#define HEAAN_KEY_H_

#include <NTL/ZZX.h>

#include "Common.h"

using namespace NTL;
using namespace std;

/**
 * Key is an RLWE instance (ax, bx = mx + ex - ax * sx) in ring Z_q[X] / (X^N + 1);
 */
class Key {
public:

	ZZX ax;
	ZZX bx;

	Key(ZZX ax = ZZX::zero(), ZZX bx = ZZX::zero()) : ax(ax), bx(bx) {}

};

#endif
