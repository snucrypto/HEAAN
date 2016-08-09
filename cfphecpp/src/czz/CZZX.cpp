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
