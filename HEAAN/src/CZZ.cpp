#include "CZZ.h"
#include <sstream>


CZZ CZZ::operator+(const CZZ& o) {
	ZZ resr = r + o.r;
	ZZ resi = i + o.i;
	CZZ res(resr, resi);
	return res;
}

CZZ CZZ::operator+(const ZZ& o) {
	ZZ resr = r + o;
	CZZ res(resr, i);
	return res;
}

void CZZ::operator+=(const CZZ& o) {
	r += o.r;
	i += o.i;
}

CZZ CZZ::operator-() {
	CZZ res(-r, -i);
	return res;
}

CZZ CZZ::operator-(const CZZ& o) {
	ZZ resr = r - o.r;
	ZZ resi = i - o.i;
	CZZ res(resr, resi);
	return res;
}

CZZ CZZ::operator-(const ZZ& o) {
	ZZ resr = r - o;
	CZZ res(resr, i);
	return res;
}

void CZZ::operator-=(const CZZ& o) {
	r -= o.r;
	i -= o.i;
}

CZZ CZZ::operator *(const CZZ& o) {
	ZZ tmpProd = (r + i) * o.r;

	ZZ resr = tmpProd - (o.r + o.i) * i;
	ZZ resi = tmpProd + (o.i - o.r) * r;
	CZZ res(resr, resi);
	return res;
}

CZZ CZZ::operator *(const ZZ& o) {
	ZZ resr = r * o;
	ZZ resi = i * o;
	CZZ res(resr, resi);
	return res;
}

CZZ CZZ::operator *(const long& o) {
	ZZ resr = r * o;
	ZZ resi = i * o;
	CZZ res(resr, resi);
	return res;
}

void CZZ::operator*=(const CZZ& o) {
	ZZ tmpProd = (r + i) * o.r;

	ZZ tr = tmpProd - (o.r + o.i) * i;
	i = tmpProd + (o.i - o.r) * r;
	r = tr;
}

CZZ CZZ::operator /(const ZZ& o) {
	ZZ resr = r / o;
	ZZ resi = i / o;
	CZZ res(resr, resi);
	return res;
}

CZZ CZZ::operator /(const long& o) {
	ZZ resr = r / o;
	ZZ resi = i / o;
	CZZ res(resr, resi);
	return res;
}

void CZZ::operator /=(const long& o) {
	r /= o;
	i /= o;
}

CZZ CZZ::operator %(const ZZ& o) {
	ZZ tr = r % o;
	ZZ to = i % o;
	return CZZ(tr, to);
}

void CZZ::operator%=(const ZZ& o) {
	r %= o;
	i %= o;
}

CZZ CZZ::operator <<(const long& s) {
	ZZ tr = r << s;
	ZZ to = i << s;
	return CZZ(tr, to);
}

void CZZ::operator <<=(const long& s) {
	r <<= s;
	i <<= s;
}

CZZ CZZ::operator >>(const long& s) {
	ZZ tr = r >> s;
	ZZ to = i >> s;
	return CZZ(tr, to);
}

void CZZ::operator >>=(const long& s) {
	r >>= s;
	i >>= s;
}

CZZ CZZ::sqr() {
	ZZ resr = (r + i) * (r - i);
	ZZ resi = (r << 1) * i;
	CZZ res(resr, resi);
	return res;
}

void CZZ::sqrThis() {
	ZZ tr = (r + i) * (r - i);
	i *= (r << 1);
	r = tr;
}

ZZ CZZ::norm() {
	return SqrRoot(r*r + i*i);
}

CZZ CZZ::conjugate() {
	CZZ res = CZZ(r, -i);
	return res;
}

string CZZ::toString() {
	stringstream ss;
	ss << " [r = " << r;
	ss << " , i = " << i;
	ss << " ] ";
	return ss.str();
}
