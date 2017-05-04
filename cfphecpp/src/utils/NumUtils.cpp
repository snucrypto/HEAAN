#include "NumUtils.h"

#include <cassert>
#include <cmath>

void NumUtils::sampleGauss(vector<CZZ>& res, const long& d, const double& stdev) {
	static double const Pi = 4.0 * atan(1.0); // Pi=3.1415..
	static long const bignum = 0xfffffff;

	res.clear();
	// Uses the Box-Muller method to get two Normal(0,stdev^2) variables
	for (long i = 0; i < d; i++) {
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

void NumUtils::sampleGauss(ZZX& res, const long& d, const double& stdev) {
	static double const Pi = 4.0 * atan(1.0); // Pi=3.1415..
	static long const bignum = 0xfffffff;
	res.SetMaxLength(d);
	// Uses the Box-Muller method to get two Normal(0,stdev^2) variables
	for (long i = 0; i < d; i++) {
		double r1 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double r2 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double theta=2 * Pi * r1;
		double rr= sqrt(-2.0 * log(r2)) * stdev;

		assert(rr < 8 * stdev); // sanity-check, no more than 8 standard deviations

		// Generate two Gaussians RV's, rounded to integers
		long x1 = (long) floor(rr * cos(theta) + 0.5);
		SetCoeff(res, i, x1);
		if(i + 1 < d) {
			long x2 = (long) floor(rr * sin(theta) + 0.5);
			SetCoeff(res, i + 1, x2);
		}

	}
}

void NumUtils::sampleZO(vector<CZZ>& res, const long& d) {
	ZZ temp;
	long i;
	for (i = 0; i < d; ++i) {
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

void NumUtils::sampleZO(ZZX& res, const long& d) {
	ZZ temp;
	long i;
	for (i = 0; i < d; ++i) {
		RandomBits(temp, 2);
		if(temp == 0) {
			SetCoeff(res, i, ZZ(1));
		} else if (temp == 1) {
			SetCoeff(res, i, ZZ(-1));
		} else {
			SetCoeff(res, i, ZZ(0));
		}
	}
}

void NumUtils::sampleUniform2(vector<CZZ>& res, const long& d, const long& logBnd) {
	ZZ tmpr, tmpi;
	for (long i = 0; i < d; i++) {
		RandomBits(tmpr, logBnd);
		RandomBits(tmpi, logBnd);
		res.push_back(CZZ(tmpr, tmpi));
	}
}

void NumUtils::sampleUniform2(ZZX& res, const long& d, const long& logBnd) {
	ZZ tmp;
	res.SetMaxLength(d);
	for (long i = 0; i < d; i++) {
		RandomBits(tmp, logBnd);
		res.rep[i] = tmp;
	}
	res.normalize();
}

vector<CZZ> NumUtils::fftRaw(vector<CZZ>& coeffs, CKsi& cksi, const bool& isForward) {
	long csize = coeffs.size();

	if(csize == 1) {
		return coeffs;
	}

	long i;
	long logcsize = log2(csize);
	long csizeh = csize >> 1;

	vector<CZZ> res, tmp, sub1, sub2;

	for (i = 0; i < csize; i = i+2) {
		sub1.push_back(coeffs[i]);
		sub2.push_back(coeffs[i+1]);
	}

	vector<CZZ> y1 = fftRaw(sub1, cksi, isForward);
	vector<CZZ> y2 = fftRaw(sub2, cksi, isForward);
	if(isForward) {
		for (i = 0; i < csizeh; ++i) {
			y2[i] *= cksi.pows[logcsize][i];
			y2[i] >>= cksi.logp;
		}
	} else {
		y2[0] *= cksi.pows[logcsize][0];
		y2[0] >>= cksi.logp;

		for (i = 1; i < csizeh; ++i) {
			y2[i] *= cksi.pows[logcsize][csize - i];
			y2[i] >>= cksi.logp;
		}
	}

	for (i = 0; i < csizeh; ++i) {
		CZZ sum = y1[i] + y2[i];
		CZZ diff = y1[i] - y2[i];
		res.push_back(sum);
		tmp.push_back(diff);
	}
	for (i = 0; i < csizeh; ++i) {
		res.push_back(tmp[i]);
	}

	return res;
}

vector<CZZ> NumUtils::fft(vector<CZZ>& coeffs, CKsi& cksi) {
	return fftRaw(coeffs, cksi, true);
}

vector<CZZ> NumUtils::fftInv(vector<CZZ>& coeffs, CKsi& cksi) {
	vector<CZZ> fftInv = fftRaw(coeffs, cksi, false);
	long N = fftInv.size();
	long logN = log2(N);
	for (long i = 0; i < N; ++i) {
		fftInv[i] >>= logN;
	}
	return fftInv;
}

vector<CZZ> NumUtils::doubleConjugate(vector<CZZ>& coeffs) {
	vector<CZZ> res;
	long csize = coeffs.size();
	res.push_back(CZZ(0,0));
	for (long i = 0; i < csize; ++i) {
		res.push_back(coeffs[i]);
	}
	res.push_back(CZZ(0,0));
	for (long i = 0; i < csize; ++i) {
		res.push_back(coeffs[csize - i - 1].conjugate());
	}

	return res;
}
