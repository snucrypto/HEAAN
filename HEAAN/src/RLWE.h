#ifndef RLWE_H_
#define RLWE_H_

#include <NTL/ZZX.h>

using namespace NTL;

class RLWE {
public:
	ZZX ax;
	ZZX bx;
	RLWE(ZZX ax = ZZX::zero(), ZZX bx = ZZX::zero()) : ax(ax), bx(bx) {}
};

#endif /* RLWE_H_ */
