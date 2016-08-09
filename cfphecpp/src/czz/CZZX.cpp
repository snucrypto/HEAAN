#include "CZZX.h"

CZZX CZZX::operator+(const CZZX& o) {
	ZZX resr = rx + o.rx;
	ZZX resi = ix + o.ix;
	CZZX res(resr, resi);
	return res;
}

CZZX CZZX::operator-(const CZZX& o) {
	ZZX resr = rx - o.rx;
	ZZX resi = ix - o.ix;
	CZZX res(resr, resi);
	return res;
}

CZZX CZZX::operator *(const CZZX& o) {
	ZZX resr = rx * o.rx - ix * o.ix;
	ZZX resi = rx * o.ix + ix * o.rx;
	CZZX res(resr, resi);
	return res;
}

void CZZX::operator+=(const CZZX& o) {
	rx += o.rx;
	ix += o.ix;
}

void CZZX::operator-=(const CZZX& o) {
	rx -= o.rx;
	ix -= o.ix;
}

void CZZX::operator*=(const CZZX& o) {
	ZZX tr = rx * o.rx - ix * o.ix;
	ix = rx * o.ix + ix * o.rx;
	rx = tr;
}

CZZX CZZX::operator >>(const long& s) {
	return CZZX();
}

CZZX CZZX::operator <<(const long& s) {
	return CZZX();
}

void CZZX::SetMaxLength(long d) {

}

void CZZX::SetLength(long d) {

}

void CZZX::normalize() {

}

CZZ coeff(CZZX& cx, long s) {
	ZZ r = NTL::coeff(cx.rx, s);
	ZZ i = NTL::coeff(cx.ix, s);
	CZZ res(r, i);
	return res;
}

void SetCoeff(CZZX& cx, long s, CZZ& c) {
	NTL::SetCoeff(cx.rx, s, c.r);
	NTL::SetCoeff(cx.ix, s, c.i);
}

void GetCoeff(CZZ& c, CZZX& cx, long s) {

}

void mul(CZZX& res, const CZZX& cx1, const CZZX& cx2) {

}

long deg(CZZX& cx) {
	return 0;
}
