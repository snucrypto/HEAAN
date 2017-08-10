#include "EvaluatorUtils.h"

#include <cmath>
#include <cstdlib>

void EvaluatorUtils::evaluateDoubleVal(double& xr, double& xi, CZZ& x, const long& bits) {
	RR xxr = to_RR(x.r);
	xxr.e -= bits;
	xr = to_double(xxr);

	RR xxi = to_RR(x.i);
	xxi.e -= bits;
	xi = to_double(xxi);
}


void EvaluatorUtils::evaluateDoubleVals(double*& xvecr, double*& xveci, CZZ*& xvec, const long size, const long& bits) {
	for (long i = 0; i < size; ++i) {
		RR xxvecr = to_RR(xvec[i].r);
		xxvecr.e -= bits;
		xvecr[i] = to_double(xxvecr);
		RR xxveci = to_RR(xvec[i].i);
		xxveci.e -= bits;
		xveci[i] = to_double(xxveci);
	}
}

ZZ EvaluatorUtils::evaluateVal(const double& x, const long& bits) {
	return evaluateVal(to_RR(x), bits);
}

ZZ EvaluatorUtils::evaluateVal(const RR& x, const long& bits) {
	RR xp = MakeRR(x.x, x.e + bits);
	return RoundToZZ(xp);
}

CZZ EvaluatorUtils::evaluateVal(const double& xr, const double& xi, const long& bits) {
	return evaluateVal(to_RR(xr), to_RR(xi), bits);
}

CZZ EvaluatorUtils::evaluateVal(const RR& xr, const RR& xi, const long& bits) {
	RR xrp = MakeRR(xr.x, xr.e + bits);
	RR xip = MakeRR(xi.x, xi.e + bits);
	return CZZ(RoundToZZ(xrp), RoundToZZ(xip));
}

void EvaluatorUtils::evaluateRRVal(RR& xr, RR& xi, CZZ& x, const long& bits) {
	xr = to_RR(x.r);
	xr.e -= bits;
	xi = to_RR(x.i);
	xr.e -= bits;
}

CZZ EvaluatorUtils::evaluateRandomVal(const long& bits) {
	return CZZ(RandomBits_ZZ(bits), RandomBits_ZZ(bits));
}

CZZ EvaluatorUtils::evaluateRandomCircleVal(const long& bits) {
	RR angle = random_RR();
	RR mr = cos(angle * 2 * Pi);
	RR mi = sin(angle * 2 * Pi);
	return evaluateVal(mr, mi, bits);
}

CZZ* EvaluatorUtils::evaluateRandomVals(const long& size, const long& bits) {
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; i++) {
		res[i] = CZZ(RandomBits_ZZ(bits), RandomBits_ZZ(bits));
	}
	return res;
}

CZZ EvaluatorUtils::evaluatePow(const double& xr, const double& xi, const long& degree, const long& bits) {
	long logDegree = log2(degree);
	long po2Degree = 1 << logDegree;
	CZZ res = evaluatePow2(xr, xi, logDegree, bits);
	long remDegree = degree - po2Degree;
	if(remDegree > 0) {
		CZZ tmp = evaluatePow(xr, xi, remDegree, bits);
		res *= tmp;
		res >>= bits;
	}
	return res;
}

CZZ EvaluatorUtils::evaluatePow(const RR& xr, const RR& xi, const long& degree, const long& bits) {
	long logDegree = log2(degree);
	long po2Degree = 1 << logDegree;
	CZZ res = evaluatePow2(xr, xi, logDegree, bits);
	long remDegree = degree - po2Degree;
	if(remDegree > 0) {
		CZZ tmp = evaluatePow(xr, xi, remDegree, bits);
		res *= tmp;
		res >>= bits;
	}
	return res;
}

CZZ EvaluatorUtils::evaluatePow2(const double& xr, const double& xi, const long& logDegree, const long& bits) {
	return evaluatePow2(to_RR(xr), to_RR(xi), logDegree, bits);
}

CZZ EvaluatorUtils::evaluatePow2(const RR& xr, const RR& xi, const long& logDegree, const long& bits) {
	CZZ res = evaluateVal(xr, xi, bits);
	for (int i = 0; i < logDegree; ++i) {
		res *= res;
		res >>= bits;
	}
	return res;
}

CZZ* EvaluatorUtils::evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& bits) {
	return  evaluatePowvec(to_RR(xr), to_RR(xi), degree, bits);
}

CZZ* EvaluatorUtils::evaluatePowvec(const RR& xr, const RR& xi, const long& degree, const long& bits) {
	CZZ* res = new CZZ[degree];
	CZZ m = evaluateVal(xr, xi, bits);
	res[0] = m;
	for (long i = 0; i < degree - 1; ++i) {
		res[i + 1] = (res[i] * m) >> bits;
	}
	return res;
}

CZZ* EvaluatorUtils::evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& bits) {
	return evaluatePow2vec(to_RR(xr), to_RR(xi), logDegree, bits);
}

CZZ* EvaluatorUtils::evaluatePow2vec(const RR& xr, const RR& xi, const long& logDegree, const long& bits) {
	CZZ* res = new CZZ[logDegree + 1];
	CZZ m = evaluateVal(xr, xi, bits);
	res[0] = m;
	for (long i = 0; i < logDegree; ++i) {
		res[i + 1] = (res[i] * res[i]) >> bits;
	}
	return res;
}

CZZ EvaluatorUtils::evaluateInverse(const double& xr, const double& xi, const long& bits) {
	return evaluateInverse(to_RR(xr), to_RR(xi), bits);
}

CZZ EvaluatorUtils::evaluateInverse(const RR& xr, const RR& xi, const long& bits) {
	RR xinvr = xr / (xr * xr + xi * xi);
	RR xinvi = -xi / (xr * xr + xi * xi);

	return evaluateVal(xinvr, xinvi, bits);
}

CZZ EvaluatorUtils::evaluateLogarithm(const double& xr, const double& xi, const long& bits) {
	double xlogr = log(xr * xr + xi * xi) / 2;
	double xlogi = atan(xi / xr);

	return evaluateVal(xlogr, xlogi, bits);
}

CZZ EvaluatorUtils::evaluateExponent(const double& xr, const double& xi, const long& bits) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	return evaluateVal(xexpr, xexpi, bits);
}

CZZ EvaluatorUtils::evaluateExponent(const RR& xr, const RR& xi, const long& bits) {
	RR xrexp = exp(xr);
	RR xexpr = xrexp * cos(xi);
	RR xexpi = xrexp * sin(xi);

	return evaluateVal(xexpr, xexpi, bits);
}

CZZ EvaluatorUtils::evaluateSigmoid(const double& xr, const double& xi, const long& bits) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	double xsigmoidr = (xexpr * (xexpr + 1) + (xexpi * xexpi)) / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));
	double xsigmoidi = xexpi / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));

	return evaluateVal(xsigmoidr, xsigmoidi, bits);
}

CZZ EvaluatorUtils::evaluateSigmoid(const RR& xr, const RR& xi, const long& bits) {
	RR xrexp = exp(xr);
	RR xexpr = xrexp * cos(xi);
	RR xexpi = xrexp * sin(xi);

	RR xsigmoidr = (xexpr * (xexpr + 1) + (xexpi * xexpi)) / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));
	RR xsigmoidi = xexpi / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));

	return evaluateVal(xsigmoidr, xsigmoidi, bits);
}

void EvaluatorUtils::leftShiftAndEqual(CZZ*& vals, const long& size, const long& bits) {
	for (long i = 0; i < size; ++i) {
		vals[i] <<= bits;
	}
}

void EvaluatorUtils::leftRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize) {
	long remrotSize = rotSize % size;
	CZZ* tmp = new CZZ[size];
	for (long i = 0; i < size - remrotSize; ++i) {
		tmp[i] = vals[i + remrotSize];
	}
	for (long i = size - remrotSize; i < size; ++i) {
		tmp[i] = vals[i + remrotSize - size];
	}
	vals = tmp;
}

void EvaluatorUtils::rightRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize) {
	long remrotSize = rotSize % size;
	CZZ* tmp = new CZZ[size];
	for (long i = 0; i < size - remrotSize; ++i) {
		tmp[i + remrotSize] = vals[i];
	}
	for (long i = size - remrotSize; i < size; ++i) {
		tmp[i + remrotSize - size] = vals[i];
	}
	vals = tmp;
}
