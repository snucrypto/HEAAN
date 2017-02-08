#include "Ring2Utils.h"

#include <cassert>
#include <cmath>

void Ring2Utils::add(ZZX& res, ZZX& p1, ZZX& p2, const long& degree) {
	ZZX add;
	ZZ c;
	add.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p1, i) + coeff(p2, i);
		SetCoeff(add, i, c);
	}
	add.normalize();
	res = add;
}

void Ring2Utils::add(CZZX& res, CZZX& p1, CZZX& p2, const long& degree) {
	add(res.rx, p1.rx, p2.rx, degree);
	add(res.ix, p1.rx, p2.rx, degree);
}

void Ring2Utils::add(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree) {
	ZZX add;
	ZZ c;
	add.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p1, i) + coeff(p2, i);
		truncate(c, logMod);
		SetCoeff(add, i, c);
	}
	add.normalize();
	res = add;
}

void Ring2Utils::add(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree) {
	add(res.rx, p1.rx, p2.rx, logMod, degree);
	add(res.ix, p1.ix, p2.ix, logMod, degree);
}

void Ring2Utils::sub(ZZX& res, ZZX& p1, ZZX& p2, const long& degree) {
	ZZX sub;
	ZZ c;
	sub.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p1, i) - coeff(p2, i);
		SetCoeff(sub, i, c);
	}
	sub.normalize();
	res = sub;
}

void Ring2Utils::sub(CZZX& res, CZZX& p1, CZZX& p2, const long& degree) {
	sub(res.rx, p1.rx, p2.rx, degree);
	sub(res.ix, p1.ix, p2.ix, degree);
}

void Ring2Utils::sub(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree) {
	ZZX sub;
	ZZ c;
	sub.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p1, i) - coeff(p2, i);
		truncate(c, logMod);
		SetCoeff(sub, i, c);
	}
	sub.normalize();
	res = sub;
}

void Ring2Utils::sub(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree) {
	sub(res.rx, p1.rx, p2.rx, logMod, degree);
	sub(res.ix, p1.ix, p2.ix, logMod, degree);
}

void Ring2Utils::mult(ZZX& res, ZZX& p1, ZZX& p2, const long& degree) {
	ZZ tmp;
	res.SetLength(degree);
	ZZX p = p1 * p2;

	for (long i = 0; i < degree; ++i) {
		tmp = coeff(p, i) - coeff(p, i + degree);
		SetCoeff(res, i, tmp);
	}
	res.normalize();
}

void Ring2Utils::mult(CZZX& res, CZZX& p1, CZZX& p2, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p1.rx, p1.ix, degree);
	mult(tmp2, tmp1, p2.rx, degree);
	add(tmp3, p2.rx, p2.ix, degree);
	mult(tmp4, tmp3, p1.ix, degree);

	sub(tmp1, p2.ix, p2.rx, degree);
	mult(tmp3, tmp1, p1.rx, degree);

	sub(res.rx, tmp2, tmp4, degree);
	add(res.ix, tmp2, tmp3, degree);
}

void Ring2Utils::mult(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree) {
	ZZ tmp;
	res.SetLength(degree);
	ZZX p = p1 * p2;

	for (long i = 0; i < deg(p); ++i) {
		tmp = coeff(p, i);
		truncate(tmp, logMod);
		SetCoeff(p, i, tmp);
	}

	p.normalize();

	for (long i = 0; i < degree; ++i) {
		tmp = coeff(p, i) - coeff(p, i + degree);
		truncate(tmp, logMod);
		SetCoeff(res, i, tmp);
	}
	res.normalize();
}

void Ring2Utils::mult(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p1.rx, p1.ix, logMod, degree);
	mult(tmp2, tmp1, p2.rx, logMod, degree);
	add(tmp3, p2.rx, p2.ix, logMod, degree);
	mult(tmp4, tmp3, p1.ix, logMod, degree);

	sub(tmp1, p2.ix, p2.rx, logMod, degree);
	mult(tmp3, tmp1, p1.rx, logMod, degree);

	sub(res.rx, tmp2, tmp4, logMod, degree);
	add(res.ix, tmp2, tmp3, logMod, degree);
}

void Ring2Utils::mult(CZZX& res, CZZX& p1, ZZX& p2, const long& logMod, const long& degree) {
	mult(res.rx, p1.rx, p2, logMod, degree);
	mult(res.ix, p1.ix, p2, logMod, degree);
}

void Ring2Utils::square(CZZX& res, CZZX& p, const long& logMod, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p.rx, p.ix, logMod, degree);
	sub(tmp2, p.rx, p.ix, logMod, degree);
	mult(res.rx, tmp1, tmp2, logMod, degree);
	mult(tmp2, p.rx, p.ix, logMod, degree);
	leftShift(res.ix, tmp2, 1, logMod, degree);
}

void Ring2Utils::mulMonomial(ZZX& res, ZZX& p, const long& monomialDeg, const long& degree) {
	long shift = monomialDeg % (2 * degree);
	ZZX tmpx = (shift < degree) ? p : -p;
	shift %= degree;
	long i, idx = 0;

	for (i = 0; i < shift; ++i) {
		SetCoeff(res, idx++, -coeff(p, degree - shift + i));
	}

	for (i = shift; i < degree; ++i) {
		SetCoeff(res, idx++, coeff(p, i - shift));
	}


}

void Ring2Utils::mulMonomial(CZZX& res, CZZX& p, const long& monomialDeg, const long& degree) {
	long shift = monomialDeg % (2 * degree);
	CZZX tmpx = (shift < degree) ? p : -p;
	shift %= degree;
	long i;
	CZZ tmp;
	for (i = 0; i < shift; ++i) {
		tmp = -coeff(tmpx, degree - shift + i);
		SetCoeff(res, i, tmp);
	}

	for (i = shift; i < degree; ++i) {
		tmp = coeff(tmpx,i - shift);
		SetCoeff(res, i, tmp);
	}


}

void Ring2Utils::mulCnst(ZZX& res, ZZX& p, const long& cnst, const long& degree) {
	ZZX mul;
	ZZ c;
	mul.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) * cnst;
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}

void Ring2Utils::mulCnst(CZZX& res, CZZX& p, const long& cnst, const long& degree) {
	CZZX mul;
	CZZ c;
	mul.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) * cnst;
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}

void Ring2Utils::mulCnst(ZZX& res, ZZX& p, const ZZ& cnst, const long& degree) {
	ZZX mul;
	ZZ c;
	mul.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) * cnst;
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}

void Ring2Utils::mulCnst(CZZX& res, CZZX& p, const CZZ& cnst, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p.rx, p.ix, degree);
	mulCnst(tmp2, tmp1, cnst.r, degree);

	ZZ tmp = cnst.r + cnst.i;
	mulCnst(tmp4, p.ix, tmp, degree);

	tmp = cnst.i - cnst.r;
	mulCnst(tmp3, p.rx, tmp, degree);

	sub(res.rx, tmp2, tmp4, degree);
	add(res.ix, tmp2, tmp3, degree);
}

void Ring2Utils::mulCnst(ZZX& res, ZZX& p, const ZZ& cnst, const long& logMod, const long& degree) {
	ZZX mul;
	ZZ c;
	mul.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) * cnst;
		truncate(c, logMod);
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}

void Ring2Utils::mulCnst(CZZX& res, CZZX& p, const CZZ& cnst, const long& logMod, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p.rx, p.ix, logMod, degree);
	mulCnst(tmp2, tmp1, cnst.r, logMod, degree);

	ZZ tmp = cnst.r + cnst.i;
	mulCnst(tmp4, p.ix, tmp, logMod, degree);

	tmp = cnst.i - cnst.r;
	mulCnst(tmp3, p.rx, tmp, logMod, degree);

	sub(res.rx, tmp2, tmp4, logMod, degree);
	add(res.ix, tmp2, tmp3, logMod, degree);
}

void Ring2Utils::leftShift(ZZX& res, ZZX& p, const long& bits, const long& logMod, const long& degree) {
	ZZX mul;
	ZZ c;
	mul.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) << bits;
		truncate(c, logMod);
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}

void Ring2Utils::leftShift(CZZX& res, CZZX& p, const long& bits, const long& logMod, const long& degree) {
	leftShift(res.rx, p.rx, bits, logMod, degree);
	leftShift(res.ix, p.ix, bits, logMod, degree);
}

void Ring2Utils::rightShift(ZZX& res, ZZX& p, const long& bits, const long& logMod, const long& degree) {
	ZZX mul;
	ZZ c;
	mul.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) >> bits;
		truncate(c, logMod);
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}

void Ring2Utils::rightShift(CZZX& res, CZZX& p, const long& bits, const long& logMod, const long& degree) {
	rightShift(res.rx, p.rx, bits, logMod, degree);
	rightShift(res.ix, p.ix, bits, logMod, degree);
}

void Ring2Utils::truncate(ZZ& c, const long& logMod) {
	c = sign(c) * trunc_ZZ(c, logMod);
}

void Ring2Utils::truncate(CZZ& c, const long& logMod) {
	truncate(c.r, logMod);
	truncate(c.i, logMod);
}

