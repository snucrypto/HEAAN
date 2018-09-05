/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "NumUtils.h"


void NumUtils::sampleGauss(ZZX& res, const long size, const double stdev) {
	static double const Pi = 4.0 * atan(1.0);
	static long const bignum = 0xfffffff;
	res.SetLength(size);

	for (long i = 0; i < size; i+=2) {
		double r1 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double r2 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double theta = 2 * Pi * r1;
		double rr= sqrt(-2.0 * log(r2)) * stdev;
		assert(rr < 8 * stdev); // sanity-check, no more than 8 standard deviations
		// Generate two Gaussians RV's, rounded to integers
		long x1 = (long) floor(rr * cos(theta) + 0.5);
		res.rep[i] = x1;
		if(i + 1 < size) {
			long x2 = (long) floor(rr * sin(theta) + 0.5);
			res.rep[i + 1] = x2;
		}
	}
}

void NumUtils::sampleHWT(ZZX& res, const long size, const long h) {
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

void NumUtils::sampleZO(ZZX& res, const long size) {
	res.SetLength(size);
	ZZ tmp = RandomBits_ZZ(2 * size);
	for (long i = 0; i < size; ++i) {
		res.rep[i] = (bit(tmp, 2 * i) == 0) ? ZZ(0) : (bit(tmp, 2 * i + 1) == 0) ? ZZ(1) : ZZ(-1);
	}
}

void NumUtils::sampleBinary(ZZX& res, const long size, const long h) {
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

void NumUtils::sampleBinary(ZZX& res, const long size) {
	res.SetLength(size);
	ZZ tmp = RandomBits_ZZ(size);
	for (long i = 0; i < size; ++i) {
		res.rep[i] = (bit(tmp, i) == 0) ? ZZ(0) : ZZ(1);
	}
}

void NumUtils::sampleUniform2(ZZX& res, const long size, const long bits) {
	res.SetLength(size);
	for (long i = 0; i < size; i++) {
		res.rep[i] = RandomBits_ZZ(bits);
	}
}
