#include "EvaluatorUtils.h"

#include <cmath>
#include <cstdlib>

CZZ EvaluatorUtils::evaluateVal(const double& xr, const double& xi, const long& logp) {
	return evaluateVal(to_RR(xr), to_RR(xi), logp);
}

void EvaluatorUtils::evaluateDoubleVal(double& xr, double& xi, CZZ& x, const long& logp) {
	RR xxr = to_RR(x.r);
	xxr.e -= logp;
	xr = to_double(xxr);

	RR xxi = to_RR(x.i);
	xxi.e -= logp;
	xi = to_double(xxi);
}


void EvaluatorUtils::evaluateDoubleVals(double*& xvecr, double*& xveci, CZZ*& xvec, const long size, const long& logp) {
	for (long i = 0; i < size; ++i) {
		RR xxvecr = to_RR(xvec[i].r);
		xxvecr.e -= logp;
		xvecr[i] = to_double(xxvecr);
		RR xxveci = to_RR(xvec[i].i);
		xxveci.e -= logp;
		xveci[i] = to_double(xxveci);
	}
}

CZZ EvaluatorUtils::evaluateVal(const RR& xr, const RR& xi, const long& logp) {
	RR xrp = MakeRR(xr.x, xr.e + logp);
	RR xip = MakeRR(xi.x, xi.e + logp);
	return CZZ(RoundToZZ(xrp), RoundToZZ(xip));
}

void EvaluatorUtils::evaluateRRVal(RR& xr, RR& xi, CZZ& x, const long& logp) {
	xr = to_RR(x.r);
	xr.e -= logp;
	xi = to_RR(x.i);
	xr.e -= logp;
}

CZZ EvaluatorUtils::evaluateRandomVal(const long& logp) {
	return CZZ(RandomBits_ZZ(logp), RandomBits_ZZ(logp));
}

CZZ EvaluatorUtils::evaluateRandomCircleVal(const long& logp) {
	RR angle = random_RR();
	RR mr = cos(angle * 2 * Pi);
	RR mi = sin(angle * 2 * Pi);
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

CZZ EvaluatorUtils::evaluatePow(const RR& xr, const RR& xi, const long& degree, const long& logp) {
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
	return evaluatePow2(to_RR(xr), to_RR(xi), logDegree, logp);
}

CZZ EvaluatorUtils::evaluatePow2(const RR& xr, const RR& xi, const long& logDegree, const long& logp) {
	CZZ res = evaluateVal(xr, xi, logp);
	for (int i = 0; i < logDegree; ++i) {
		res *= res;
		res >>= logp;
	}
	return res;
}

CZZ* EvaluatorUtils::evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& logp) {
	return  evaluatePowvec(to_RR(xr), to_RR(xi), degree, logp);
}

CZZ* EvaluatorUtils::evaluatePowvec(const RR& xr, const RR& xi, const long& degree, const long& logp) {
	CZZ* res = new CZZ[degree];
	CZZ m = evaluateVal(xr, xi, logp);
	res[0] = m;
	for (long i = 0; i < degree - 1; ++i) {
		res[i + 1] = (res[i] * m) >> logp;
	}
	return res;
}

CZZ* EvaluatorUtils::evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& logp) {
	return evaluatePow2vec(to_RR(xr), to_RR(xi), logDegree, logp);
}

CZZ* EvaluatorUtils::evaluatePow2vec(const RR& xr, const RR& xi, const long& logDegree, const long& logp) {
	CZZ* res = new CZZ[logDegree + 1];
	CZZ m = evaluateVal(xr, xi, logp);
	res[0] = m;
	for (long i = 0; i < logDegree; ++i) {
		res[i + 1] = (res[i] * res[i]) >> logp;
	}
	return res;
}

CZZ EvaluatorUtils::evaluateInverse(const double& xr, const double& xi, const long& logp) {
	return evaluateInverse(to_RR(xr), to_RR(xi), logp);
}

CZZ EvaluatorUtils::evaluateInverse(const RR& xr, const RR& xi, const long& logp) {
	RR xinvr = xr / (xr * xr + xi * xi);
	RR xinvi = -xi / (xr * xr + xi * xi);

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

CZZ EvaluatorUtils::evaluateExponent(const RR& xr, const RR& xi, const long& logp) {
	RR xrexp = exp(xr);
	RR xexpr = xrexp * cos(xi);
	RR xexpi = xrexp * sin(xi);

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

CZZ EvaluatorUtils::evaluateSigmoid(const RR& xr, const RR& xi, const long& logp) {
	RR xrexp = exp(xr);
	RR xexpr = xrexp * cos(xi);
	RR xexpi = xrexp * sin(xi);

	RR xsigmoidr = (xexpr * (xexpr + 1) + (xexpi * xexpi)) / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));
	RR xsigmoidi = xexpi / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));

	return evaluateVal(xsigmoidr, xsigmoidi, logp);
}

void EvaluatorUtils::leftShift(CZZ*& vals, const long& size, const long& logp) {
	for (long i = 0; i < size; ++i) {
		vals[i] <<= logp;
	}
}

void EvaluatorUtils::leftRotate(CZZ*& vals, const long& size, const long& shift) {
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
