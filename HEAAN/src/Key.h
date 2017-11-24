#ifndef HEAAN_KEY_H_
#define HEAAN_KEY_H_

#include <NTL/ZZX.h>

using namespace NTL;

class Key {
public:

	ZZX ax;
	ZZX bx;

	Key(ZZX ax = ZZX::zero(), ZZX bx = ZZX::zero()) : ax(ax), bx(bx) {}

};

#endif
