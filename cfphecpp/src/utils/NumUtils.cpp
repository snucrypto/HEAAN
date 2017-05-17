#include "NumUtils.h"

#include <cassert>
#include <cmath>

void NumUtils::sampleGauss(vector<CZZ>& res, const long& dim, const double& stdev) {
	static double const Pi = 4.0 * atan(1.0); // Pi=3.1415..
	static long const bignum = 0xfffffff;

	res.clear();
	// Uses the Box-Muller method to get two Normal(0,stdev^2) variables
	for (long i = 0; i < dim; i++) {
		double r1 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double r2 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double theta=2 * Pi * r1;
		double rr= sqrt(-2.0 * log(r2)) * stdev;

		assert(rr < 8 * stdev); // sanity-check, no more than 8 standard deviations

		// Generate two Gaussians RV's, rounded to integers
		long xr = (long) floor(rr * cos(theta) + 0.5);
		long xi = (long) floor(rr * sin(theta) + 0.5);
		res.push_back(CZZ(xr, xi));
	}
}

void NumUtils::sampleGauss(ZZX& res, const long& dim, const double& stdev) {
	static double const Pi = 4.0 * atan(1.0); // Pi=3.1415..
	static long const bignum = 0xfffffff;
	res.SetLength(dim);
	// Uses the Box-Muller method to get two Normal(0,stdev^2) variables
	for (long i = 0; i < dim; i+=2) {
		double r1 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double r2 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double theta=2 * Pi * r1;
		double rr= sqrt(-2.0 * log(r2)) * stdev;

		assert(rr < 8 * stdev); // sanity-check, no more than 8 standard deviations

		// Generate two Gaussians RV's, rounded to integers
		long x1 = (long) floor(rr * cos(theta) + 0.5);
		SetCoeff(res, i, x1);
		if(i + 1 < dim) {
			long x2 = (long) floor(rr * sin(theta) + 0.5);
			SetCoeff(res, i + 1, x2);
		}

	}
}

void NumUtils::sampleZO(vector<CZZ>& res, const long& dim) {
	ZZ temp;
	long i;
	for (i = 0; i < dim; ++i) {
		RandomBits(temp, 2);
		if(temp == 0) {
			res.push_back(CZZ(1, 0));
		} else if (temp == 1) {
			res.push_back(CZZ(-1, 0));
		} else {
			res.push_back(CZZ(0,0));
		}
	}
}

void NumUtils::sampleZO(ZZX& res, const long& dim) {
	ZZ temp;
	res.SetLength(dim);
	long i;
	for (i = 0; i < dim; ++i) {
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

void NumUtils::sampleUniform2(vector<CZZ>& res, const long& dim, const long& logBnd) {
	ZZ tmpr, tmpi;
	for (long i = 0; i < dim; i++) {
		RandomBits(tmpr, logBnd);
		RandomBits(tmpi, logBnd);
		res.push_back(CZZ(tmpr, tmpi));
	}
}

void NumUtils::sampleUniform2(ZZX& res, const long& dim, const long& logBnd) {
	ZZ tmp;
	res.SetLength(dim);
	for (long i = 0; i < dim; i++) {
		RandomBits(tmp, logBnd);
		res.rep[i] = tmp;
	}
	res.normalize();
}

vector<CZZ> NumUtils::doubleConjugate(vector<CZZ>& vals) {
	vector<CZZ> res;
	long vsize = vals.size();
	for (long i = 0; i < vsize; ++i) {
		res.push_back(vals[i]);
	}
	for (long i = 0; i < vsize; ++i) {
		res.push_back(vals[vsize - i - 1].conjugate());
	}
	return res;
}

vector<CZZ> NumUtils::fftRaw(vector<CZZ>& vals, KsiPows& ksiPows, const bool& isForward) {
	long valsSize = vals.size();
	if (valsSize == 1) {
		return vals;
	}
	long logValsSize = log2(valsSize);
	long Nh = valsSize >> 1;
	vector<CZZ> res, tmp, sub1, sub2;
	for (long i = 0; i < valsSize; i = i + 2) {
		sub1.push_back(vals[i]);
		sub2.push_back(vals[i + 1]);
	}
	vector<CZZ> y1 = fftRaw(sub1, ksiPows, isForward);
	vector<CZZ> y2 = fftRaw(sub2, ksiPows, isForward);
	if (isForward) {
		for (long i = 0; i < Nh; ++i) {
			y2[i] *= ksiPows.pows[logValsSize][i];
			y2[i] >>= ksiPows.logp;
		}
	} else {
		for (long i = 0; i < Nh; ++i) {
			y2[i] *= ksiPows.pows[logValsSize][valsSize - i];
			y2[i] >>= ksiPows.logp;
		}
	}
	for (long i = 0; i < Nh; ++i) {
		CZZ sum = y1[i] + y2[i];
		CZZ diff = y1[i] - y2[i];
		res.push_back(sum);
		tmp.push_back(diff);
	}
	for (long i = 0; i < Nh; ++i) {
		res.push_back(tmp[i]);
	}
	return res;
}

vector<CZZ> NumUtils::fft(vector<CZZ>& vals, KsiPows& ksiPows) {
	return fftRaw(vals, ksiPows, true);
}

vector<CZZ> NumUtils::fftInv(vector<CZZ>& vals, KsiPows& ksiPows) {
	vector<CZZ> fftInv = fftRaw(vals, ksiPows, false);
	long fftInvSize = fftInv.size();
	long logFftInvSize = log2(fftInvSize);
	for (long i = 0; i < fftInvSize; ++i) {
		fftInv[i] >>= logFftInvSize;
	}
	return fftInv;
}

vector<CZZ> NumUtils::fftSpecial(vector<CZZ>& vals, KsiPows& ksiPows) {
	long valsSize = vals.size();
	if(valsSize == 1) {
		return vals;
	}

	long i;
	long logValsSize = log2(valsSize);
	long valsSizeHalf = valsSize >> 1;

	vector<CZZ> res, tmp, sub1, sub2;

	for (i = 0; i < valsSize; i = i+2) {
		sub1.push_back(vals[i]);
		sub2.push_back(vals[i+1]);
	}

	vector<CZZ> y1 = fftSpecial(sub1, ksiPows);
	vector<CZZ> y2 = fftSpecial(sub2, ksiPows);
	for (i = 0; i < valsSizeHalf; ++i) {
		y2[i] *= ksiPows.pows[logValsSize + 1][2 * i + 1];
		y2[i] >>= ksiPows.logp;
	}

	for (i = 0; i < valsSizeHalf; ++i) {
		CZZ sum = y1[i] + y2[i];
		CZZ diff = y1[i] - y2[i];
		res.push_back(sum);
		tmp.push_back(diff);
	}
	for (i = 0; i < valsSizeHalf; ++i) {
		res.push_back(tmp[i]);
	}

	return res;
}

vector<CZZ> NumUtils::fftSpecialInv(vector<CZZ>& vals, KsiPows& ksiPows) {
	vector<CZZ> fftInv = fftRaw(vals, ksiPows, false);
	long fftInvSize = fftInv.size();
	long logFftInvSize = log2(fftInvSize);
	for (long i = 0; i < fftInvSize; ++i) {
		fftInv[i] *= ksiPows.pows[logFftInvSize + 1][2 * fftInvSize - i];
		fftInv[i] >>= (ksiPows.logp + logFftInvSize);
	}
	return fftInv;
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
