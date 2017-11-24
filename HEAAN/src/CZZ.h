#ifndef HEAAN_CZZ_H_
#define HEAAN_CZZ_H_

#include <NTL/ZZ.h>

#include "Common.h"


using namespace std;
using namespace NTL;

class CZZ {
public:
	ZZ r;
	ZZ i;

	CZZ(long r, long i) : r(ZZ(r)), i(ZZ(i)) {};

	CZZ(ZZ r = ZZ::zero(), ZZ i = ZZ::zero()) : r(r), i(i) {};

	CZZ(const CZZ& o) : r(o.r), i(o.i) {};

	CZZ operator-();
	CZZ operator+(const CZZ& o);
	CZZ operator+(const ZZ& m);
	void operator+=(const CZZ& o);

	CZZ operator-(const CZZ& o);
	CZZ operator-(const ZZ& m);
	void operator-=(const CZZ& o);

	CZZ operator *(const CZZ& o);
	CZZ operator *(const ZZ& o);
	CZZ operator *(const long& o);
	void operator *=(const CZZ& o);

	CZZ operator /(const ZZ& o);
	CZZ operator /(const long& o);
	void operator /=(const long& o);

	CZZ operator %(const ZZ& o);
	void operator %=(const ZZ& o);

	CZZ operator <<(const long& s);
	void operator <<=(const long& s);
	CZZ operator >>(const long& s);
	void operator >>=(const long& s);

	CZZ sqr();
	void sqrThis();
	ZZ norm();
	CZZ conjugate();

	string toString();
};

#endif
