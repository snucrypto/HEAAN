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
	ZZ temp;
	res.SetLength(size);
	long i;
	for (i = 0; i < size; ++i) {
		RandomBits(temp, 2);
		if(temp == 0) {
			res.rep[i] = ZZ(1);
		} else if (temp == 1) {
			res.rep[i] = ZZ(-1);
		} else {
			res.rep[i] = ZZ(0);
		}
	}
	res.normalize();
}

void NumUtils::sampleUniform2(ZZX& res, const long& size, const long& logBnd) {
	ZZ tmp;
	res.SetLength(size);
	for (long i = 0; i < size; i++) {
		RandomBits(tmp, logBnd);
		res.rep[i] = tmp;
	}
	res.normalize();
}

CZZ* NumUtils::sampleGauss(const long& size, const double& stdev) {
	static double const Pi = 4.0 * atan(1.0); // Pi=3.1415..
	static long const bignum = 0xfffffff;
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; i++) {
		double r1 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double r2 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double theta=2 * Pi * r1;
		double rr= sqrt(-2.0 * log(r2)) * stdev;

		assert(rr < 8 * stdev); // sanity-check, no more than 8 standard deviations

		long xr = (long) floor(rr * cos(theta) + 0.5);
		long xi = (long) floor(rr * sin(theta) + 0.5);
		res[i] = CZZ(xr, xi);
	}
	return res;
}

CZZ* NumUtils::sampleZO(const long& size) {
	CZZ* res = new CZZ[size];
	ZZ temp;
	for (long i = 0; i < size; ++i) {
		RandomBits(temp, 2);
		res[i] = temp == 0 ? CZZ(1, 0) : temp == 1 ? CZZ(-1, 0) : CZZ(0,0);
	}
	return res;
}


CZZ* NumUtils::sampleUniform2(const long& size, const long& logBnd) {
	CZZ* res = new CZZ[size];
	ZZ tmpr, tmpi;
	for (long i = 0; i < size; i++) {
		RandomBits(tmpr, logBnd);
		RandomBits(tmpi, logBnd);
		res[i] = CZZ(tmpr, tmpi);
	}
	return res;
}

CZZ* NumUtils::fftRaw(CZZ*& vals, const long& size, KsiPows& ksiPows, const bool& isForward) {
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
	CZZ* y1 = fftRaw(sub1, sizeh, ksiPows, isForward);
	CZZ* y2 = fftRaw(sub2, sizeh, ksiPows, isForward);
	if (isForward) {
		for (long i = 0; i < sizeh; ++i) {
			y2[i] *= ksiPows.pows[logsize][i];
			y2[i] >>= ksiPows.logp;
		}
	} else {
		for (long i = 0; i < sizeh; ++i) {
			y2[i] *= ksiPows.pows[logsize][size - i];
			y2[i] >>= ksiPows.logp;
		}
	}
	CZZ* res = new CZZ[size];
	for (long i = 0; i < sizeh; ++i) {
		res[i] = y1[i] + y2[i];
		res[sizeh + i] = y1[i] - y2[i];
	}
	return res;
}

CZZ* NumUtils::fft(CZZ*& vals, const long& size, KsiPows& ksiPows) {
	return fftRaw(vals, size, ksiPows, true);
}

CZZ* NumUtils::fftInv(CZZ*& vals, const long& size, KsiPows& ksiPows) {
	CZZ* fftInv = fftRaw(vals, size, ksiPows, false);
	long logSize = log2(size);
	for (long i = 0; i < size; ++i) {
		fftInv[i] >>= logSize;
	}
	return fftInv;
}

CZZ* NumUtils::fftInvLazy(CZZ*& vals, const long& size, KsiPows& ksiPows) {
	return fftRaw(vals, size, ksiPows, false);
}

CZZ* NumUtils::fftSpecial(CZZ*& vals, const long& size, KsiPows& ksiPows) {
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

	CZZ* y1 = fftSpecial(sub1, sizeh, ksiPows);
	CZZ* y2 = fftSpecial(sub2, sizeh, ksiPows);

	for (long i = 0; i < sizeh; ++i) {
		y2[i] *= ksiPows.pows[logsize + 1][2 * i + 1];
		y2[i] >>= ksiPows.logp;
	}

	CZZ* res = new CZZ[size];
	for (long i = 0; i < sizeh; ++i) {
		res[i] = y1[i] + y2[i];
		res[sizeh + i] = y1[i] - y2[i];
	}

	return res;
}

CZZ* NumUtils::fftSpecialInv(CZZ*& vals, const long& size, KsiPows& ksiPows) {
	CZZ* fftInv = fftRaw(vals, size, ksiPows, false);
	long logsize = log2(size);
	for (long i = 0; i < size; ++i) {
		fftInv[i] *= ksiPows.pows[logsize + 1][2 * size - i];
		fftInv[i] >>= (ksiPows.logp + logsize);
	}
	return fftInv;
}

CZZ* NumUtils::fftFull(CZZ*& vals1, CZZ*& vals2, const long& size, KsiPows& ksiPows) {
	CZZ* mfft1 = fft(vals1, size, ksiPows);
	CZZ* mfft2 = fft(vals2, size, ksiPows);

	for (long i = 0; i < size; ++i) {
		mfft1[i] *= mfft2[i];
		mfft1[i] >>= ksiPows.logp;
	}
	return fftInv(mfft1, size, ksiPows);
}


CZZ* NumUtils::fftFullLazy(CZZ*& vals1, CZZ*& vals2, const long& size, KsiPows& ksiPows) {
	CZZ* mfft1 = fft(vals1, size, ksiPows);
	CZZ* mfft2 = fft(vals2, size, ksiPows);

	for (long i = 0; i < size; ++i) {
		mfft1[i] *= mfft2[i];
		mfft1[i] >>= ksiPows.logp;
	}
	return fftInvLazy(mfft1, size, ksiPows);
}

vector<CZZ> NumUtils::bitReverse(vector<CZZ>& vals) {
	vector<CZZ> res;
	vector<CZZ> tmp;
	long valsSize = vals.size();
	long logValsSize = log2(valsSize);
	res.push_back(vals[0]);
	for (long i = 0; i < logValsSize; ++i) {
		long powi = (1 << i);
		long powni = (1 << (logValsSize - i));
		long powniHalf = (1 << (logValsSize - i - 1));
		for (long j = 0; j < powi; ++j) {
			tmp.push_back(vals[j * powni + powniHalf]);
		}
		tmp = bitReverse(tmp);
		for (long j = 0; j < tmp.size(); ++j) {
			res.push_back(tmp[j]);
		}
		tmp.clear();
	}
	return res;
}

vector<CZZ> NumUtils::fftButterfly(vector<CZZ>& vals, KsiPows& ksiPows) {
	vector<CZZ> fft = vals;
	long valsSize = vals.size();
	CZZ as, at, w;
	long logValsSize = log2(valsSize);
	for (long i = 0; i < logValsSize; ++i) {
		long powi = (1 << i);
		long logPowni = logValsSize - i;
		long powniHalf = (1 << (logPowni - 1));
		long powni = (1 << logPowni);
		for (long j = 0; j < powi; ++j) {
			for (long k = 0; k < powniHalf; ++k) {
				long s = j * powni + k;
				long t = s + powniHalf;
				as = fft[s] + fft[t];
				at = ((ksiPows.pows[logPowni][k] * (fft[s] - fft[t])) >> ksiPows.logp);
				fft[s] = as;
				fft[t] = at;
			}
		}
	}

	return bitReverse(fft);
}

vector<CZZ> NumUtils::fftButterflyInv(vector<CZZ>& vals, KsiPows& ksiPows) {
	vector<CZZ> fftInv = vals;
	long valsSize = vals.size();
	CZZ as, at;
	long logValsSize = log2(valsSize);
	for (long i = 0; i < logValsSize; ++i) {
		long powi = (1 << i);
		long logPowni = logValsSize - i;
		long pownih = (1 << (logPowni - 1));
		long powni = (1 << logPowni);
		for (long j = 0; j < powi; ++j) {
			for (long k = 0; k < pownih; ++k) {
				long s = j * powni + k;
				long t = s + pownih;
				as = fftInv[s] + fftInv[t];
				at = ((ksiPows.pows[logPowni][powni - k] * (fftInv[s] - fftInv[t])) >> ksiPows.logp);
				fftInv[s] = as;
				fftInv[t] = at;
			}
		}
	}
	for (long i = 0; i < valsSize; ++i) {
		fftInv[i] >>= logValsSize;
	}
	return bitReverse(fftInv);
}
