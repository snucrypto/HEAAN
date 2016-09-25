#include "CZZ.h"

#include <sstream>
#include <string>

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

CZZ CZZ::operator *(const CZZ& o) {
	ZZ resr = r * o.r - i * o.i;
	ZZ resi = r * o.i + i * o.r;
	CZZ res(resr, resi);
	return res;
}

CZZ CZZ::operator *(const ZZ& o) {
	ZZ resr = r * o;
	ZZ resi = i * o;
	CZZ res(resr, resi);
	return res;
}

CZZ CZZ::operator /(const ZZ& o) {
	ZZ resr = r / o;
	ZZ resi = i / o;
	CZZ res(resr, resi);
	return res;
}

void CZZ::operator+=(const CZZ& o) {
	r += o.r;
	i += o.i;
}

void CZZ::operator-=(const CZZ& o) {
	r -= o.r;
	i -= o.i;
}

void CZZ::operator*=(const CZZ& o) {
	ZZ tr = r * o.r - i * o.i;
	i = r * o.i + i * o.r;
	r = tr;
}

CZZ CZZ::operator %(const ZZ& o) {
	ZZ tr = r % o;
	ZZ to = i % o;
	return CZZ(tr, to);
}

CZZ CZZ::operator <<(const long& s) {
	ZZ tr = r << s;
	ZZ to = i << s;
	return CZZ(tr, to);
}

CZZ CZZ::operator >>(const long& s) {
	ZZ tr = r >> s;
	ZZ to = i >> s;
	return CZZ(tr, to);
}

ZZ CZZ::norm() {
	return SqrRoot(r*r + i*i);
}

string CZZ::toString() {
	stringstream ss;
	ss << " [r = ";
	ss << r;
	ss << " , i = ";
	ss << i;
	ss << " ] ";
	return ss.str();
}
