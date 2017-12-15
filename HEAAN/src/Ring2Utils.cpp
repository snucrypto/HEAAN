#include "Ring2Utils.h"


void Ring2Utils::mod(ZZX& res, ZZX& p, ZZ& mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		res.rep[i] = p.rep[i] % mod;
	}
}

void Ring2Utils::modAndEqual(ZZX& p, ZZ& mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		p.rep[i] %= mod;
	}
}

void Ring2Utils::add(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		AddMod(res.rep[i], p1.rep[i], p2.rep[i], mod);
	}
}

ZZX Ring2Utils::add(ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	ZZX res;
	add(res, p1, p2, mod, degree);
	return res;
}

void Ring2Utils::addAndEqual(ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		AddMod(p1.rep[i], p1.rep[i], p2.rep[i], mod);
	}
}

void Ring2Utils::sub(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		AddMod(res.rep[i], p1.rep[i], -p2.rep[i], mod);
	}
}

ZZX Ring2Utils::sub(ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	ZZX res;
	sub(res, p1, p2, mod, degree);
	return res;
}

void Ring2Utils::subAndEqual(ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		AddMod(p1.rep[i], p1.rep[i], -p2.rep[i], mod);
	}
}

void Ring2Utils::subAndEqual2(ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		AddMod(p2.rep[i], p1.rep[i], -p2.rep[i], mod);
	}
}

void Ring2Utils::mult(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	res.SetLength(degree);
	ZZX p;
	mul(p, p1, p2);
	p.SetLength(2 * degree);
	for (long i = 0; i < degree; ++i) {
		p.rep[i] %= mod;
		p.rep[i + degree] %= mod;
		SubMod(res.rep[i], p.rep[i], p.rep[i + degree], mod);
	}
}

ZZX Ring2Utils::mult(ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	ZZX res;
	mult(res, p1, p2, mod, degree);
	return res;
}

void Ring2Utils::multAndEqual(ZZX& p1, ZZX& p2, ZZ& mod, const long degree) {
	ZZX p;
	mul(p, p1, p2);
	p.SetLength(2 * degree);

	for (long i = 0; i < degree; ++i) {
		p.rep[i] %= mod;
		p.rep[i + degree] %= mod;
		SubMod(p1.rep[i], p.rep[i], p.rep[i + degree], mod);
	}
}

void Ring2Utils::square(ZZX& res, ZZX& p, ZZ& mod, const long degree) {
	res.SetLength(degree);
	ZZX pp;
	sqr(pp, p);
	pp.SetLength(2 * degree);

	for (long i = 0; i < degree; ++i) {
		pp.rep[i] %= mod;
		pp.rep[i + degree] %= mod;
		SubMod(res.rep[i], pp.rep[i], pp.rep[i + degree], mod);
	}
}

ZZX Ring2Utils::square(ZZX& p, ZZ& mod, const long degree) {
	ZZX res;
	square(res, p, mod, degree);
	return res;
}

void Ring2Utils::squareAndEqual(ZZX& p, ZZ& mod, const long degree) {
	ZZX pp;
	sqr(pp, p);
	pp.SetLength(2 * degree);

	for (long i = 0; i < degree; ++i) {
		pp.rep[i] %= mod;
		pp.rep[i + degree] %= mod;
		SubMod(p.rep[i], pp.rep[i], pp.rep[i + degree], mod);
	}
}

void Ring2Utils::multByMonomial(ZZX& res, ZZX& p, const long monomialDeg, const long degree) {
	long shift = monomialDeg % (2 * degree);
	if(shift == 0) {
		res = p;
	} else {
		ZZX tmpx;
		tmpx.SetLength(degree);
		tmpx = (shift < degree) ? p : -p;
		shift %= degree;

		res.SetLength(degree);

		for (long i = 0; i < shift; ++i) {
			res.rep[i] = -tmpx.rep[degree - shift + i];
		}

		for (long i = shift; i < degree; ++i) {
			res.rep[i] = tmpx.rep[i - shift];
		}
	}
}

ZZX Ring2Utils::multByMonomial(ZZX& p, const long monomialDeg, const long degree) {
	ZZX res;
	multByMonomial(res, p, monomialDeg, degree);
	return res;
}

void Ring2Utils::multByMonomialAndEqual(ZZX& p, const long monomialDeg, const long degree) {
	long shift = monomialDeg % (2 * degree);
	if(shift == 0) {
		return;
	}
	ZZX tmpx;
	tmpx.SetLength(degree);
	tmpx = (shift < degree) ? p : -p;
	shift %= degree;
	for (long i = 0; i < shift; ++i) {
		p.rep[i] = -tmpx.rep[degree - shift + i];
	}

	for (long i = shift; i < degree; ++i) {
		p.rep[i] = tmpx.rep[i - shift];
	}
}

void Ring2Utils::multByConst(ZZX& res, ZZX& p, const ZZ& cnst, ZZ& mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		MulMod(res.rep[i], p.rep[i], cnst, mod);
	}
}

ZZX Ring2Utils::multByConst(ZZX& p, const ZZ& cnst, ZZ& mod, const long degree) {
	ZZX res;
	multByConst(res, p, cnst, mod, degree);
	return res;
}

void Ring2Utils::multByConstAndEqual(ZZX& p, const ZZ& cnst, ZZ& mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		MulMod(p.rep[i], p.rep[i], cnst, mod);
	}
}

void Ring2Utils::leftShift(ZZX& res, ZZX& p, const long bits, ZZ& mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		res.rep[i] = p.rep[i] << bits;
		res.rep[i] %= mod;
	}
}

void Ring2Utils::leftShiftAndEqual(ZZX& p, const long bits, ZZ& mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		p.rep[i] <<= bits;
		p.rep[i] %= mod;
	}
}

void Ring2Utils::doubleAndEqual(ZZX& p, ZZ& mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		p.rep[i] <<= 1;
		p.rep[i] %= mod;
	}
}

void Ring2Utils::rightShift(ZZX& res, ZZX& p, const long bits, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		res.rep[i] = p.rep[i] >> bits;
	}
}

void Ring2Utils::rightShiftAndEqual(ZZX& p, const long bits, const long degree) {
	for (long i = 0; i < degree; ++i) {
		p.rep[i] >>= bits;
	}
}

void Ring2Utils::conjugate(ZZX& res, ZZX& p, const long degree) {
	res.SetLength(degree);
	res.rep[0] = p.rep[0];
	for (long i = 1; i < degree; ++i) {
		res.rep[i] = -p.rep[degree - i];
	}
}

void Ring2Utils::conjugateAndEqual(ZZX& p, const long degree) {
	for (long i = 0; i < degree / 2; ++i) {
		ZZ tmp = p.rep[i];
		p.rep[i] = p.rep[degree - i];
		p.rep[degree - i] = tmp;
	}
	p.rep[degree / 2] = -p.rep[degree / 2];
}

void Ring2Utils::inpower(ZZX& res, ZZX& p, const long pow, ZZ& mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		long ipow = i * pow;
		long shift = ipow % (2 * degree);
		if(shift < degree) {
			AddMod(res.rep[shift % degree], res.rep[shift % degree], p.rep[i], mod);
		} else {
			AddMod(res.rep[shift % degree], res.rep[shift % degree], -p.rep[i], mod);
		}
	}
}

ZZX Ring2Utils::inpower(ZZX& p, const long pow, ZZ& mod, const long degree) {
	ZZX res;
	inpower(res, p, pow, mod, degree);
	return res;
}

ZZX* Ring2Utils::bitDecomposition(ZZX& p, const long logMod, const long degree) {
	ZZX* res = new ZZX[logMod];
	for (long i = 0; i < logMod; ++i) {
		res[i].SetLength(degree);
	}

	for (long j = 0; j < degree; ++j) {
		ZZ coeff = p.rep[j];
		for (long i = 0; i < logMod; ++i) {
			res[i].rep[j] = bit(coeff, j);
		}
	}
	return res;
}

ZZX Ring2Utils::innerProduct(ZZX* pvec1, ZZX* pvec2, const long size, ZZ& mod, const long degree) {
	ZZX res = mult(pvec1[0], pvec2[0], mod, degree);
	for (long i = 1; i < size; ++i) {
		ZZX termi = mult(pvec1[i], pvec2[i], mod, degree);
		addAndEqual(res, termi, mod, degree);
	}
	return res;
}
