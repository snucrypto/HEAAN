#include "EvaluatorUtils.h"

#include <cmath>
#include <cstdlib>

CZZ EvaluatorUtils::evaluateVal(const double& xr, const double& xi, const long& logp) {
	RR xrp = to_RR(xr);
	RR xip = to_RR(xi);
	xrp.e += logp;
	xip.e += logp;
	return CZZ(RoundToZZ(xrp), RoundToZZ(xip));
}

CZZ EvaluatorUtils::evaluateRandomVal(const long& logp) {
	return CZZ(RandomBits_ZZ(logp), RandomBits_ZZ(logp));
}

CZZ EvaluatorUtils::evaluateRandomCircleVal(const long& logp) {
	double angle = (double)arc4random() / RAND_MAX;
	double mr = cos(angle * 2 * Pi);
	double mi = sin(angle * 2 * Pi);
	return evaluateVal(mr, mi, logp);
}

CZZ* EvaluatorUtils::evaluateRandomVals(const long& size, const long& logp) {
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; i++) {
		res[i] = CZZ(RandomBits_ZZ(logp), RandomBits_ZZ(logp));
	}
	return res;
}

CZZ EvaluatorUtils::evaluatePow(const double& xr, const double& xi, const long& degree, const long& logp) {
	long logDegree = log2(degree);
	long po2Degree = 1 << logDegree;
	CZZ res = evaluatePow2(xr, xi, logDegree, logp);
	long remDegree = degree - po2Degree;
	if(remDegree > 0) {
		CZZ tmp = evaluatePow(xr, xi, remDegree, logp);
		res *= tmp;
		res >>= logp;
	}
	return res;
}

CZZ EvaluatorUtils::evaluatePow2(const double& xr, const double& xi, const long& logDegree, const long& logp) {
	CZZ res = evaluateVal(xr, xi, logp);
	for (int i = 0; i < logDegree; ++i) {
		res = (res * res) >> logp;
	}
	return res;
}

CZZ* EvaluatorUtils::evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& logp) {
	CZZ* res = new CZZ[degree];
	CZZ m = evaluateVal(xr, xi, logp);
	res[0] = m;
	for (long i = 0; i < degree - 1; ++i) {
		res[i + 1] = (res[i] * m) >> logp;
	}
	return res;
}

CZZ* EvaluatorUtils::evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& logp) {
	CZZ* res = new CZZ[logDegree + 1];
	CZZ m = evaluateVal(xr, xi, logp);
	res[0] = m;
	for (long i = 0; i < logDegree; ++i) {
		res[i + 1] = (res[i] * res[i]) >> logp;
	}
	return res;
}

CZZ EvaluatorUtils::evaluateInverse(const double& xr, const double& xi, const long& logp) {
	double xinvr = xr / (xr * xr + xi * xi);
	double xinvi = -xi / (xr * xr + xi * xi);

	return evaluateVal(xinvr, xinvi, logp);
}

CZZ EvaluatorUtils::evaluateLogarithm(const double& xr, const double& xi, const long& logp) {
	double xlogr = log(xr * xr + xi * xi) / 2;
	double xlogi = atan(xi / xr);

	return evaluateVal(xlogr, xlogi, logp);
}

CZZ EvaluatorUtils::evaluateExponent(const double& xr, const double& xi, const long& logp) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	return evaluateVal(xexpr, xexpi, logp);
}

CZZ EvaluatorUtils::evaluateSigmoid(const double& xr, const double& xi, const long& logp) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	double xsigmoidr = (xexpr * (xexpr + 1) + (xexpi * xexpi)) / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));
	double xsigmoidi = xexpi / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));

	return evaluateVal(xsigmoidr, xsigmoidi, logp);
}

void EvaluatorUtils::leftShift(CZZ*& vals, const long& size, const long& logp) {
	for (long i = 0; i < size; ++i) {
		vals[i] <<= logp;
	}
}

void EvaluatorUtils::idxShift(CZZ*& vals, const long& size, const long& shift) {
	long remshift = shift % size;
	CZZ* tmp = new CZZ[size];
	for (long i = 0; i < size - remshift; ++i) {
		tmp[i] = vals[i + remshift];
	}
	for (long i = size - remshift; i < size; ++i) {
		tmp[i] = vals[i + remshift - size];
	}
	vals = tmp;
}
