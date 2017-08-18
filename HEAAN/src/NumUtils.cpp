#include "NumUtils.h"

#include <cassert>
#include <cmath>

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

void NumUtils::sampleZO(ZZX& res, const long& size, const long& h) {
	res.SetLength(size);
	long idx = 0;
	while(idx < h) {
		long i = RandomBnd(size);
		if(res.rep[i] == 0) {
			res.rep[i] = (0 == ( rand() % 2 )) ? ZZ(1) : ZZ(-1);
			idx++;
		}
	}
	res.normalize();
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
	res.normalize();
}

void NumUtils::sampleBinary(ZZX& res, const long& size) {
	res.SetLength(size);
	for (long i = 0; i < size; ++i) {
		res.rep[i] = (rand() % 2) ? ZZ(0) : ZZ(1);
	}
	res.normalize();
}

void NumUtils::sampleUniform2(ZZX& res, const long& size, const long& logBnd) {
	res.SetLength(size);
	for (long i = 0; i < size; i++) {
		res.rep[i] = RandomBits_ZZ(logBnd);
	}
	res.normalize();
}

void NumUtils::fftRaw(CZZ*& vals, const long& size, RR**& ksiPowsr, RR**& ksiPowsi, const bool& isForward) {
	for (long i = 1, j = 0; i < size; ++i) {
		long bit = size >> 1;
		for (; j >= bit; bit>>=1) {
			j -= bit;
		}
		j += bit;
		if(i < j) {
			CZZ tmp = vals[i];
			vals[i] = vals[j];
			vals[j] = tmp;
		}
	}
	if(isForward) {
		for (long len = 2; len <= size; len <<= 1) {
			long loglen = log2(len);
			for (long i = 0; i < size; i += len) {
				for (long j = 0; j < len / 2; ++j) {
					CZZ u = vals[i + j];
					CZZ v = vals[i + j + len / 2];
					RR tmp1 = to_RR(v.r) * (ksiPowsr[loglen][j] + ksiPowsi[loglen][j]);
					RR tmpr = tmp1 - to_RR(v.r + v.i) * ksiPowsi[loglen][j];
					RR tmpi = tmp1 + to_RR(v.i - v.r) * ksiPowsr[loglen][j];
					v.r = to_ZZ(tmpr);
					v.i = to_ZZ(tmpi);
					vals[i + j] = u + v;
					vals[i + j + len / 2] = u - v;
				}
			}
		}
	} else {
		for (long len = 2; len <= size; len <<= 1) {
			long loglen = log2(len);
			for (long i = 0; i < size; i += len) {
				for (long j = 0; j < len / 2; ++j) {
					CZZ u = vals[i + j];
					CZZ v = vals[i + j + len / 2];
					RR tmp1 = to_RR(v.r) * (ksiPowsr[loglen][len -j] + ksiPowsi[loglen][len - j]);
					RR tmpr = tmp1 - to_RR(v.r + v.i) * ksiPowsi[loglen][len - j];
					RR tmpi = tmp1 + to_RR(v.i - v.r) * ksiPowsr[loglen][len - j];
					v.r = to_ZZ(tmpr);
					v.i = to_ZZ(tmpi);
					vals[i + j] = u + v;
					vals[i + j + len / 2] = u - v;
				}
			}
		}
	}
}

void NumUtils::fft(CZZ*& vals, const long& size, RR**& ksiPowsr, RR**& ksiPowsi) {
	fftRaw(vals, size, ksiPowsr, ksiPowsi, true);
}

void NumUtils::fftInv(CZZ*& vals, const long& size, RR**& ksiPowsr, RR**& ksiPowsi) {
	fftRaw(vals, size, ksiPowsr, ksiPowsi, false);
	long logSize = log2(size);
	for (long i = 0; i < size; ++i) {
		vals[i] >>= logSize;
	}
}

void NumUtils::fftInvLazy(CZZ*& vals, const long& size, RR**& ksiPowsr, RR**& ksiPowsi) {
	fftRaw(vals, size, ksiPowsr, ksiPowsi, false);
}

void NumUtils::fftSpecial(CZZ*& vals, const long& size, RR**& ksiPowsr, RR**& ksiPowsi) {
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
		for (long i = 0; i < size; i += len) {
			for (long j = 0; j < len / 2; ++j) {
				CZZ u = vals[i + j];
				CZZ v = vals[i + j + len / 2];
				RR tmp1 = to_RR(v.r) * (ksiPowsr[loglen + 1][2 * j + 1] + ksiPowsi[loglen + 1][2 * j + 1]);
				RR tmpr = tmp1 - to_RR(v.r + v.i) * ksiPowsi[loglen + 1][2 * j + 1];
				RR tmpi = tmp1 + to_RR(v.i - v.r) * ksiPowsr[loglen + 1][2 * j + 1];
				v.r = to_ZZ(tmpr);
				v.i = to_ZZ(tmpi);
				vals[i + j] = u + v;
				vals[i + j + len / 2] = u - v;
			}
		}
	}
}

void NumUtils::fftSpecialInv(CZZ*& vals, const long& size, RR**& ksiPowsr, RR**& ksiPowsi) {
	fftRaw(vals, size, ksiPowsr, ksiPowsi, false);
	long logsize = log2(size);
	for (long i = 0; i < size; ++i) {
		RR tmp1 = to_RR(vals[i].r) * (ksiPowsr[logsize + 1][2 * size - i] + ksiPowsi[logsize + 1][2 * size - i]);
		RR tmpr = tmp1 - to_RR(vals[i].r + vals[i].i) * ksiPowsi[logsize + 1][2 * size - i];
		RR tmpi = tmp1 + to_RR(vals[i].i - vals[i].r) * ksiPowsr[logsize + 1][2 * size - i];
		vals[i].r = to_ZZ(tmpr);
		vals[i].i = to_ZZ(tmpi);
		vals[i] >>= logsize;
	}
}
