/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_KEY_H_
#define HEAAN_KEY_H_

#include <NTL/ZZ.h>

using namespace NTL;

/**
 * Key is an RLWE instance (ax, bx = kx + ex - ax * sx) in the ring Z_q[X] / (X^N + 1)
 */
class Key {
public:

	long N;
	long np;

	uint64_t* rax;
	uint64_t* rbx;

	Key(uint64_t* rax, uint64_t* rbx, long N, long np);

	virtual ~Key();
};

#endif
