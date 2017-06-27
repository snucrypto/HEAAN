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

void NumUtils::sampleZO(ZZX& res, const long& size) {
	res.SetLength(size);
	for (long i = 0; i < size; ++i) {
		ZZ temp = RandomBits_ZZ(2);
		res.rep[i] = (temp == 0) ? ZZ(1) : (temp == 1) ? ZZ(-1) : ZZ(0);
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

CZZ* NumUtils::fftRaw(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp, const bool& isForward) {
	if (size == 1) {
		return vals;
	}
	long logsize = log2(size);
	long sizeh = size >> 1;
	CZZ* sub1 = new CZZ[sizeh];
	CZZ* sub2 = new CZZ[sizeh];
	for (long i = 0; i < sizeh; ++i) {
		sub1[i] = vals[2 * i];
		sub2[i] = vals[2 * i + 1];
	}
	//-----------------------------------------
	CZZ* y1 = fftRaw(sub1, sizeh, ksiPows, logp, isForward);
	CZZ* y2 = fftRaw(sub2, sizeh, ksiPows, logp, isForward);
	//-----------------------------------------
	if (isForward) {
		for (long i = 0; i < sizeh; ++i) {
			y2[i] *= ksiPows[logsize][i];
			y2[i] >>= logp;
		}
	} else {
		for (long i = 0; i < sizeh; ++i) {
			y2[i] *= ksiPows[logsize][size - i];
			y2[i] >>= logp;
		}
	}
	CZZ* res = new CZZ[size];
	for (long i = 0; i < sizeh; ++i) {
		res[i] = y1[i] + y2[i];
		res[sizeh + i] = y1[i] - y2[i];
	}
	return res;
}

CZZ* NumUtils::fft(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp) {
	return fftRaw(vals, size, ksiPows, logp, true);
}

CZZ* NumUtils::fftInv(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp) {
	CZZ* fftInv = fftRaw(vals, size, ksiPows, logp, false);
	long logSize = log2(size);
	for (long i = 0; i < size; ++i) {
		fftInv[i] >>= logSize;
	}
	return fftInv;
}

CZZ* NumUtils::fftInvLazy(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp) {
	return fftRaw(vals, size, ksiPows, logp, false);
}

CZZ* NumUtils::fftSpecial(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp) {
	if(size == 1) {
		return vals;
	}

	long logsize = log2(size);
	long sizeh = size >> 1;

	CZZ* sub1 = new CZZ[sizeh];
	CZZ* sub2 = new CZZ[sizeh];

	for (long i = 0; i < sizeh; ++i) {
		sub1[i] = vals[2 * i];
		sub2[i] = vals[2 * i + 1];
	}

	CZZ* y1 = fftSpecial(sub1, sizeh, ksiPows, logp);
	CZZ* y2 = fftSpecial(sub2, sizeh, ksiPows, logp);

	CZZ* res = new CZZ[size];
	for (long i = 0; i < sizeh; ++i) {
		y2[i] *= ksiPows[logsize + 1][2 * i + 1];
		y2[i] >>= logp;
		res[i] = y1[i] + y2[i];
		res[sizeh + i] = y1[i] - y2[i];
	}

	return res;
}

CZZ* NumUtils::fftSpecialInv(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp) {
	CZZ* fftInv = fftRaw(vals, size, ksiPows, logp, false);
	long logsize = log2(size);
	for (long i = 0; i < size; ++i) {
		fftInv[i] *= ksiPows[logsize + 1][2 * size - i];
		fftInv[i] >>= (logp + logsize);
	}
	return fftInv;
}

CZZ* NumUtils::fftFull(CZZ*& vals1, CZZ*& vals2, const long& size, CZZ**& ksiPows, const long& logp) {
	CZZ* mfft1 = fft(vals1, size, ksiPows, logp);
	CZZ* mfft2 = fft(vals2, size, ksiPows, logp);

	for (long i = 0; i < size; ++i) {
		mfft1[i] *= mfft2[i];
		mfft1[i] >>= logp;
	}
	return fftInv(mfft1, size, ksiPows, logp);
}


CZZ* NumUtils::fftFullLazy(CZZ*& vals1, CZZ*& vals2, const long& size, CZZ**& ksiPows, const long& logp) {
	CZZ* mfft1 = fft(vals1, size, ksiPows, logp);
	CZZ* mfft2 = fft(vals2, size, ksiPows, logp);

	for (long i = 0; i < size; ++i) {
		mfft1[i] *= mfft2[i];
		mfft1[i] >>= logp;
	}
	return fftInvLazy(mfft1, size, ksiPows, logp);
}
