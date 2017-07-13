#ifndef UTILS_CZZX_H_
#define UTILS_CZZX_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

#include "CZZ.h"

using namespace std;
using namespace NTL;

class CZZX {
public:

	ZZX rx;
	ZZX ix;

	CZZX(ZZX rx = ZZX::zero(), ZZX ix = ZZX::zero()) : rx(rx), ix(ix) {};

	CZZX operator+(const CZZX& o);
	void operator+=(const CZZX& o);

	CZZX operator-(const CZZX& o);
	void operator-=(const CZZX& o);
	CZZX operator -();

	CZZX operator *(const CZZX& o);
	void operator *=(const CZZX& o);

	CZZX operator >>(const long& s);
	void operator >>=(const long& s);

	CZZX operator <<(const long& s);
	void operator <<=(const long& s);

	CZZX sqr();
	void sqrThis();
	void SetMaxLength(long d);
	void SetLength(long d);
	void normalize();

	string toString();
};

CZZ coeff(CZZX& cx, long s);
void SetCoeff(CZZX& cx, long s, CZZ& c);
void GetCoeff(CZZ& c, CZZX& cx, long s);
long deg(CZZX& cx);

#endif /* UTILS_CZZX_H_ */
