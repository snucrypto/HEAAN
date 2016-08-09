#ifndef UTILS_CZZX_H_
#define UTILS_CZZX_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class CZZX {
public:

	ZZX rx;
	ZZX ix;

	CZZX(ZZX rx, ZZX ix) : rx(rx), ix(ix) {};
	CZZX() : rx(ZZX::zero()), ix(ZZX::zero()) {};

	CZZX operator+(const CZZX& o);
	CZZX operator-(const CZZX& o);
	CZZX operator *(const CZZX& o);
	void operator+=(const CZZX& o);
	void operator-=(const CZZX& o);
	void operator *=(const CZZX& o);

	CZZX operator >>(const long& s);
	CZZX operator <<(const long& s);

	void SetMaxLength(long d);
	void SetLength(long d);
	void normalize();

};

CZZ coeff(CZZX& cx, long s) {
	ZZ r = coeff(cx.rx, s);
	ZZ i = coeff(cx.ix, s);
	CZZ res(r, i);
	return res;
}

void SetCoeff(CZZX& cx, long s, CZZ& c) {
	SetCoeff(cx.rx, s, c.r);
	SetCoeff(cx.ix, s, c.i);
}

void GetCoeff(CZZ& c, CZZX& cx, long s) {

}

void mul(CZZX& res, const CZZX& cx1, const CZZX& cx2) {

}

long deg(CZZX& cx) {

}
#endif /* UTILS_CZZX_H_ */
