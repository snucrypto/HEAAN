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
	rx.SetMaxLength(d);
	ix.SetMaxLength(d);
}

void CZZX::SetLength(long d) {
	rx.SetLength(d);
	ix.SetLength(d);
}

void CZZX::normalize() {
	rx.normalize();
	ix.normalize();
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
	GetCoeff(c.r, cx.rx, s);
	GetCoeff(c.i, cx.ix, s);
}

void mul(CZZX& res, const CZZX& cx1, const CZZX& cx2) {
	res.rx = cx1.rx * cx2.rx - cx1.ix * cx2.ix;
	res.ix = cx1.rx * cx2.ix + cx1.ix * cx2.rx;
}

long deg(CZZX& cx) {
	return deg(cx.rx);
}
