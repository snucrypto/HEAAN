#include "CZZX.h"

#include <sstream>
#include <string>

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
	ZZX tmpProd = (rx + ix) * o.rx;

	ZZX resr = tmpProd - (o.rx + o.ix) * ix;
	ZZX resi = tmpProd + (o.ix - o.rx) * rx;
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
	ZZX tmpProd = (rx + ix) * o.rx;

	ZZX tr = tmpProd - (o.rx + o.ix) * ix;
	ix = tmpProd + (o.ix - o.rx) * rx;
	rx = tr;
}

CZZX CZZX::operator >>(const long& s) {
	ZZX resr = rx >> s;
	ZZX resi = ix >> s;
	return CZZX(resr, resi);
}

CZZX CZZX::operator <<(const long& s) {
	ZZX resr = rx << s;
	ZZX resi = ix << s;
	return CZZX(resr, resi);
}

CZZX CZZX::sqr() {
	ZZX resr = (rx + ix) * (rx - ix);
	ZZX resi = (rx << 1) * ix;
	CZZX res(resr, resi);
	return res;
}

void CZZX::sqrThis() {
	ZZX tr = (rx + ix) * (rx - ix);
	ix *= (rx << 1);
	rx = tr;
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

string CZZX::toString() {
	stringstream ss;
	ss << " [rx = ";
	ss << rx;
	ss << " ,ix = ";
	ss << ix;
	ss << " ] ";
	return ss.str();
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
