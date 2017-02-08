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

void Ring2Utils::addNew(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long& degree) {
	ZZX add;
	ZZ c;
	add.SetLength(degree);
	p1.SetLength(degree);
	p2.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		AddMod(add.rep[i], p1.rep[i], p2.rep[i], mod);
	}
	res = add;

}

void Ring2Utils::addNew(CZZX& res, CZZX& p1, CZZX& p2, ZZ& mod, const long& degree) {
	addNew(res.rx, p1.rx, p2.rx, mod, degree);
	addNew(res.ix, p1.ix, p2.ix, mod, degree);
}


void Ring2Utils::addAndEqual(ZZX& p1, ZZX& p2, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p1, i) + coeff(p2, i);
		SetCoeff(p1, i, c);
	}
}

void Ring2Utils::addAndEqual(CZZX& p1, CZZX& p2, const long& degree) {
	addAndEqual(p1.rx, p2.rx, degree);
	addAndEqual(p1.rx, p2.rx, degree);
}

void Ring2Utils::addAndEqual(ZZX& p1, ZZX& p2, const long& logMod, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p1, i) + coeff(p2, i);
		truncate(c, logMod);
		SetCoeff(p1, i, c);
	}
}

void Ring2Utils::addAndEqual(CZZX& p1, CZZX& p2, const long& logMod, const long& degree) {
	addAndEqual(p1.rx, p2.rx, logMod, degree);
	addAndEqual(p1.ix, p2.ix, logMod, degree);
}

void Ring2Utils::addAndEqualNew(ZZX& p1, ZZX& p2, ZZ& mod, const long& degree) {
	ZZ c;
	p1.SetLength(degree);
	p2.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		AddMod(p1.rep[i], p1.rep[i], p2.rep[i], mod);
	}
}

void Ring2Utils::addAndEqualNew(CZZX& p1, CZZX& p2, ZZ& mod, const long& degree) {
	addAndEqualNew(p1.rx, p2.rx, mod, degree);
	addAndEqualNew(p1.ix, p2.ix, mod, degree);
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

void Ring2Utils::subNew(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long& degree) {
	ZZX sub;
	ZZ c;
	sub.SetLength(degree);
	p1.SetLength(degree);
	p2.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		AddMod(sub.rep[i], p1.rep[i], -p2.rep[i], mod);
	}
	res = sub;
}

void Ring2Utils::subNew(CZZX& res, CZZX& p1, CZZX& p2, ZZ& mod, const long& degree) {
	subNew(res.rx, p1.rx, p2.rx, mod, degree);
	subNew(res.ix, p1.ix, p2.ix, mod, degree);
}

void Ring2Utils::subAndEqual(ZZX& p1, ZZX& p2, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p1, i) - coeff(p2, i);
		SetCoeff(p1, i, c);
	}
}


void Ring2Utils::subAndEqual(CZZX& p1, CZZX& p2, const long& degree) {
	subAndEqual(p1.rx, p2.rx, degree);
	subAndEqual(p1.ix, p2.ix, degree);
}

void Ring2Utils::subAndEqual(ZZX& p1, ZZX& p2, const long& logMod, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p1, i) - coeff(p2, i);
		truncate(c, logMod);
		SetCoeff(p1, i, c);
	}
}

void Ring2Utils::subAndEqual(CZZX& p1, CZZX& p2, const long& logMod, const long& degree) {
	subAndEqual(p1.rx, p2.rx, logMod, degree);
	subAndEqual(p1.ix, p2.ix, logMod, degree);
}

void Ring2Utils::subAndEqualNew(ZZX& p1, ZZX& p2, ZZ& mod, const long& degree) {
	ZZ c;
	p1.SetLength(degree);
	p2.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		AddMod(p1.rep[i], p1.rep[i], -p2.rep[i], mod);
	}
}

void Ring2Utils::subAndEqualNew(CZZX& p1, CZZX& p2, ZZ& mod, const long& degree) {
	subAndEqualNew(p1.rx, p2.rx, mod, degree);
	subAndEqualNew(p1.ix, p2.ix, mod, degree);
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

void Ring2Utils::multAndEqual(ZZX& p1, ZZX& p2, const long& degree) {
	ZZ tmp;
	ZZX p = p1 * p2;

	for (long i = 0; i < degree; ++i) {
		tmp = coeff(p, i) - coeff(p, i + degree);
		SetCoeff(p1, i, tmp);
	}
}

void Ring2Utils::multAndEqual(CZZX& p1, CZZX& p2, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p1.rx, p1.ix, degree);
	mult(tmp2, tmp1, p2.rx, degree);
	add(tmp3, p2.rx, p2.ix, degree);
	mult(tmp4, tmp3, p1.ix, degree);

	sub(tmp1, p2.ix, p2.rx, degree);
	multAndEqual(tmp1, p1.rx, degree);

	sub(p1.rx, tmp2, tmp4, degree);
	add(p1.ix, tmp2, tmp1, degree);
}

void Ring2Utils::multAndEqual(ZZX& p1, ZZX& p2, const long& logMod, const long& degree) {
	ZZ tmp;
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
		SetCoeff(p1, i, tmp);
	}
}

void Ring2Utils::multAndEqual(CZZX& p1, CZZX& p2, const long& logMod, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p1.rx, p1.ix, logMod, degree);
	mult(tmp2, tmp1, p2.rx, logMod, degree);
	add(tmp3, p2.rx, p2.ix, logMod, degree);
	mult(tmp4, tmp3, p1.ix, logMod, degree);

	sub(tmp1, p2.ix, p2.rx, logMod, degree);
	multAndEqual(tmp1, p1.rx, logMod, degree);

	sub(p1.rx, tmp2, tmp4, logMod, degree);
	add(p2.ix, tmp2, tmp1, logMod, degree);
}

void Ring2Utils::multAndEqual(CZZX& p1, ZZX& p2, const long& logMod, const long& degree) {
	multAndEqual(p1.rx, p2, logMod, degree);
	multAndEqual(p1.ix, p2, logMod, degree);
}

void Ring2Utils::square(CZZX& res, CZZX& p, const long& logMod, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p.rx, p.ix, logMod, degree);
	sub(tmp2, p.rx, p.ix, logMod, degree);
	mult(res.rx, tmp1, tmp2, logMod, degree);
	mult(tmp2, p.rx, p.ix, logMod, degree);
	leftShift(res.ix, tmp2, 1, logMod, degree);
}

void Ring2Utils::squareAndEqual(CZZX& p, const long& logMod, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p.rx, p.ix, logMod, degree);
	sub(tmp2, p.rx, p.ix, logMod, degree);
	mult(p.rx, tmp1, tmp2, logMod, degree);
	mult(tmp2, p.rx, p.ix, logMod, degree);
	leftShift(p.ix, tmp2, 1, logMod, degree);
}

void Ring2Utils::mulMonomial(ZZX& res, ZZX& p, const long& monomialDeg, const long& degree) {
	long shift = monomialDeg % (2 * degree);
	if(shift == 0) {
		res = p;
	}
	ZZX tmpx = (shift < degree) ? p : -p;
	shift %= degree;

	ZZ tmp;
	for (long i = 0; i < shift; ++i) {
		tmp = -coeff(tmpx, degree - shift + i);
		SetCoeff(res, i, tmp);
	}

	for (long i = shift; i < degree; ++i) {
		tmp = coeff(tmpx, i - shift);
		SetCoeff(res, i, tmp);
	}
}

void Ring2Utils::mulMonomial(CZZX& res, CZZX& p, const long& monomialDeg, const long& degree) {
	long shift = monomialDeg % (2 * degree);
	if(shift == 0) {
		res = p;
	}
	CZZX tmpx = (shift < degree) ? p : -p;
	shift %= degree;
	CZZ tmp;
	for (long i = 0; i < shift; ++i) {
		tmp = -coeff(tmpx, degree - shift + i);
		SetCoeff(res, i, tmp);
	}

	for (long i = shift; i < degree; ++i) {
		tmp = coeff(tmpx,i - shift);
		SetCoeff(res, i, tmp);
	}
}

void Ring2Utils::mulMonomialAndEqual(ZZX& p, const long& monomialDeg, const long& degree) {
	long shift = monomialDeg % (2 * degree);
	if(shift == 0) {
		return;
	}
	ZZX tmpx = (shift < degree) ? p : -p;
	shift %= degree;
	ZZ tmp;
	for (long i = 0; i < shift; ++i) {
		tmp = -coeff(tmpx, degree - shift + i);
		SetCoeff(p, i, tmp);
	}

	for (long i = shift; i < degree; ++i) {
		tmp = coeff(tmpx, i - shift);
		SetCoeff(p, i, tmp);
	}
}

void Ring2Utils::mulMonomialAndEqual(CZZX& p, const long& monomialDeg, const long& degree) {
	mulMonomialAndEqual(p.rx, monomialDeg, degree);
	mulMonomialAndEqual(p.ix, monomialDeg, degree);
}

void Ring2Utils::mulMonomialAndEqualNew(ZZX& p, const long& monomialDeg, const long& degree) {
	long shift = monomialDeg % (2 * degree);
	if(shift == 0) {
		return;
	}
	ZZX tmpx = (shift < degree) ? p : -p;
	shift %= degree;
	p.SetLength(degree);
	tmpx.SetLength(degree);
	for (long i = 0; i < shift; ++i) {
		p.rep[i] = -tmpx.rep[degree - shift + i];
	}

	for (long i = shift; i < degree; ++i) {
		p.rep[i] = tmpx.rep[i - shift];
	}
}

void Ring2Utils::mulMonomialAndEqualNew(CZZX& p, const long& monomialDeg, const long& degree) {
	mulMonomialAndEqualNew(p.rx, monomialDeg, degree);
	mulMonomialAndEqualNew(p.ix, monomialDeg, degree);
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

void Ring2Utils::mulCnstAndEqual(ZZX& p, const long& cnst, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) * cnst;
		SetCoeff(p, i, c);
	}
}

void Ring2Utils::mulCnstAndEqual(CZZX& p, const long& cnst, const long& degree) {
	CZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) * cnst;
		SetCoeff(p, i, c);
	}
}

void Ring2Utils::mulCnstAndEqual(ZZX& p, const ZZ& cnst, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) * cnst;
		SetCoeff(p, i, c);
	}
}

void Ring2Utils::mulCnstAndEqual(CZZX& p, const CZZ& cnst, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p.rx, p.ix, degree);
	mulCnst(tmp2, tmp1, cnst.r, degree);

	ZZ tmp = cnst.r + cnst.i;
	mulCnst(tmp4, p.ix, tmp, degree);

	tmp = cnst.i - cnst.r;
	mulCnst(tmp3, p.rx, tmp, degree);

	sub(p.rx, tmp2, tmp4, degree);
	add(p.ix, tmp2, tmp3, degree);
}

void Ring2Utils::mulCnstAndEqual(ZZX& p, const ZZ& cnst, const long& logMod, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) * cnst;
		truncate(c, logMod);
		SetCoeff(p, i, c);
	}
}

void Ring2Utils::mulCnstAndEqual(CZZX& p, const CZZ& cnst, const long& logMod, const long& degree) {
	ZZX tmp1, tmp2, tmp3, tmp4;

	add(tmp1, p.rx, p.ix, logMod, degree);
	mulCnst(tmp2, tmp1, cnst.r, logMod, degree);

	ZZ tmp = cnst.r + cnst.i;
	mulCnst(tmp4, p.ix, tmp, logMod, degree);

	tmp = cnst.i - cnst.r;
	mulCnst(tmp3, p.rx, tmp, logMod, degree);

	sub(p.rx, tmp2, tmp4, logMod, degree);
	add(p.ix, tmp2, tmp3, logMod, degree);
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

void Ring2Utils::leftShiftAndEqual(ZZX& p, const long& bits, const long& logMod, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) << bits;
		truncate(c, logMod);
		SetCoeff(p, i, c);
	}
}

void Ring2Utils::leftShiftAndEqual(CZZX& p, const long& bits, const long& logMod, const long& degree) {
	leftShiftAndEqual(p.rx, bits, logMod, degree);
	leftShiftAndEqual(p.ix, bits, logMod, degree);
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

void Ring2Utils::rightShiftAndEqual(ZZX& p, const long& bits, const long& logMod, const long& degree) {
	ZZ c;
	for (long i = 0; i < degree; ++i) {
		c = coeff(p, i) >> bits;
		truncate(c, logMod);
		SetCoeff(p, i, c);
	}
}

void Ring2Utils::rightShiftAndEqual(CZZX& p, const long& bits, const long& logMod, const long& degree) {
	rightShiftAndEqual(p.rx, bits, logMod, degree);
	rightShiftAndEqual(p.ix, bits, logMod, degree);
}

void Ring2Utils::truncate(ZZ& c, const long& logMod) {
	c = sign(c) * trunc_ZZ(c, logMod);
}

void Ring2Utils::truncate(CZZ& c, const long& logMod) {
	truncate(c.r, logMod);
	truncate(c.i, logMod);
}

