#include "NumUtils.h"

#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <cassert>
#include <cmath>

#include "CZZ.h"

void NumUtils::sampleGauss(ZZX& res, const long& size, const double& stdev) {
	static double const Pi = 4.0 * atan(1.0);
	static long const bignum = 0xfffffff;
	res.SetLength(size);

	for (long i = 0; i < size; i+=2) {
		double r1 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double r2 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double theta=2 * Pi * r1;
		double rr= sqrt(-2.0 * log(r2)) * stdev;
		assert(rr < 8 * stdev); // sanity-check, no more than 8 standard deviations
		// Generate two Gaussians RV's, rounded to integers
		long x1 = (long) floor(rr * cos(theta) + 0.5);
		SetCoeff(res, i, x1);
		if(i + 1 < size) {
			long x2 = (long) floor(rr * sin(theta) + 0.5);
			SetCoeff(res, i + 1, x2);
		}
	}
}

void NumUtils::sampleHWT(ZZX& res, const long& size, const long& h) {
	res.SetLength(size);
	long idx = 0;
	ZZ tmp = RandomBits_ZZ(h);
	while(idx < h) {
		long i = RandomBnd(size);
		if(res.rep[i] == 0) {
			res.rep[i] = (bit(tmp, idx) == 0) ? ZZ(1) : ZZ(-1);
			idx++;
		}
	}
}

void NumUtils::sampleZO(ZZX& res, const long& size) {
	res.SetLength(size);
	ZZ tmp = RandomBits_ZZ(2 * size);
	for (long i = 0; i < size; ++i) {
		res.rep[i] = (bit(tmp, 2 * i) == 0) ? ZZ(0) : (bit(tmp, 2 * i + 1) == 0) ? ZZ(1) : ZZ(-1);
	}
}

void NumUtils::sampleBinary(ZZX& res, const long& size, const long& h) {
	res.SetLength(size);
	long idx = 0;
	while(idx < h) {
		long i = RandomBnd(size);
		if(res.rep[i] == 0) {
			res.rep[i] = ZZ(1);
			idx++;
		}
	}
}

void NumUtils::sampleBinary(ZZX& res, const long& size) {
	res.SetLength(size);
	ZZ tmp = RandomBits_ZZ(size);
	for (long i = 0; i < size; ++i) {
		res.rep[i] = (bit(tmp, i) == 0) ? ZZ(0) : ZZ(1);
	}
}

void NumUtils::sampleUniform2(ZZX& res, const long& size, const long& logBnd) {
	res.SetLength(size);
	for (long i = 0; i < size; i++) {
		res.rep[i] = RandomBits_ZZ(logBnd);
	}
}

void NumUtils::fftRaw(CZZ*& vals, const long& size, SchemeAux& aux, const bool& isForward) {
	for (long i = 1, j = 0; i < size; ++i) {
		long bit = size >> 1;
		for (; j >= bit; bit>>=1) {
			j -= bit;
		}
		j += bit;
		if(i < j) {
			swap(vals[i], vals[j]);
		}
	}
	if(isForward) {
		for (long len = 2; len <= size; len <<= 1) {
			long MoverLen = aux.M / len;
			for (long i = 0; i < size; i += len) {
				for (long j = 0; j < len / 2; ++j) {
					CZZ u = vals[i + j];
					CZZ v = vals[i + j + len / 2];
					RR tmp1 = to_RR(v.r) * (aux.ksiPowsr[j * MoverLen] + aux.ksiPowsi[j * MoverLen]);
					RR tmpr = tmp1 - to_RR(v.r + v.i) * aux.ksiPowsi[j * MoverLen];
					RR tmpi = tmp1 + to_RR(v.i - v.r) * aux.ksiPowsr[j * MoverLen];
					v.r = to_ZZ(tmpr);
					v.i = to_ZZ(tmpi);
					vals[i + j] = u + v;
					vals[i + j + len / 2] = u - v;
				}
			}
		}
	} else {
		for (long len = 2; len <= size; len <<= 1) {
			long MoverLen = aux.M / len;
			for (long i = 0; i < size; i += len) {
				for (long j = 0; j < len / 2; ++j) {
					CZZ u = vals[i + j];
					CZZ v = vals[i + j + len / 2];
					RR tmp1 = to_RR(v.r) * (aux.ksiPowsr[(len - j) * MoverLen] + aux.ksiPowsi[(len - j) * MoverLen]);
					RR tmpr = tmp1 - to_RR(v.r + v.i) * aux.ksiPowsi[(len - j) * MoverLen];
					RR tmpi = tmp1 + to_RR(v.i - v.r) * aux.ksiPowsr[(len - j) * MoverLen];
					v.r = to_ZZ(tmpr);
					v.i = to_ZZ(tmpi);
					vals[i + j] = u + v;
					vals[i + j + len / 2] = u - v;
				}
			}
		}
	}
}

void NumUtils::fft(CZZ*& vals, const long& size, SchemeAux& aux) {
	fftRaw(vals, size, aux, true);
}

void NumUtils::fftInv(CZZ*& vals, const long& size, SchemeAux& aux) {
	fftRaw(vals, size, aux, false);
	long logSize = log2(size);
	for (long i = 0; i < size; ++i) {
		vals[i] >>= logSize;
	}
}

void NumUtils::fftInvLazy(CZZ*& vals, const long& size, SchemeAux& aux) {
	fftRaw(vals, size, aux, false);
}

void NumUtils::fftSpecial(CZZ*& vals, const long& size, SchemeAux& aux) {
	for (int i = 1, j = 0; i < size; ++i) {
		long bit = size >> 1;
		for (; j>=bit; bit>>=1) {
			j -= bit;
		}
		j += bit;
		if(i < j) {
			swap(vals[i], vals[j]);
		}
	}
	for (long len = 2; len <= size; len <<= 1) {
		long loglen = log2(len);
		long Mover2Len = aux.M / len / 2;
		for (long i = 0; i < size; i += len) {
			for (long j = 0; j < len / 2; ++j) {
				CZZ u = vals[i + j];
				CZZ v = vals[i + j + len / 2];
				RR tmp1 = to_RR(v.r) * (aux.ksiPowsr[(2 * j + 1) * Mover2Len] + aux.ksiPowsi[(2 * j + 1) * Mover2Len]);
				RR tmpr = tmp1 - to_RR(v.r + v.i) * aux.ksiPowsi[(2 * j + 1) * Mover2Len];
				RR tmpi = tmp1 + to_RR(v.i - v.r) * aux.ksiPowsr[(2 * j + 1) * Mover2Len];
				v.r = to_ZZ(tmpr);
				v.i = to_ZZ(tmpi);
				vals[i + j] = u + v;
				vals[i + j + len / 2] = u - v;
			}
		}
	}
}

void NumUtils::fftSpecialInv(CZZ*& vals, const long& size, SchemeAux& aux) {
	fftRaw(vals, size, aux, false);
	long logsize = log2(size);
	long Mover2size = aux.M / size / 2;
	for (long i = 0; i < size; ++i) {
		RR tmp1 = to_RR(vals[i].r) * (aux.ksiPowsr[(2 * size - i) * Mover2size] + aux.ksiPowsi[(2 * size - i) * Mover2size]);
		RR tmpr = tmp1 - to_RR(vals[i].r + vals[i].i) * aux.ksiPowsi[(2 * size - i) * Mover2size];
		RR tmpi = tmp1 + to_RR(vals[i].i - vals[i].r) * aux.ksiPowsr[(2 * size - i) * Mover2size];
		vals[i].r = to_ZZ(tmpr);
		vals[i].i = to_ZZ(tmpi);
		vals[i] >>= logsize;
	}
}
