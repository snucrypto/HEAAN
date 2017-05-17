#include "EvaluatorUtils.h"


CZZ EvaluatorUtils::evaluateVal(const double& xr, const double& xi, const long& logp) {
	if(logp < 31) {
		long p = 1 << logp;
		ZZ pxexpr = to_ZZ(xr * p);
		ZZ pxexpi = to_ZZ(xi * p);
		return CZZ(pxexpr, pxexpi);
	} else {
		long tmp = (1 << 30);
		ZZ pxexpr = to_ZZ(xr * tmp) << (logp - 30);
		ZZ pxexpi = to_ZZ(xi * tmp) << (logp - 30);
		return CZZ(pxexpr, pxexpi);
	}
}

vector<CZZ> EvaluatorUtils::evaluateRandomVals(const long& size, const long& logp) {
	vector<CZZ> res;
	for (long i = 0; i < size; ++i) {
		double mr = (double)rand() / RAND_MAX;
		double mi = (double)rand() / RAND_MAX;
		CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
		res.push_back(m);
	}
	return res;
}

vector<CZZ> EvaluatorUtils::evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& logp) {

}

vector<CZZ> EvaluatorUtils::evaluateProd2vec(const double& xr, const double& xi, const long& logDegree, const long& logp) {

}

vector<CZZ> EvaluatorUtils::evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& logp) {

}

CZZ EvaluatorUtils::evaluateInverse(const double& xr, const double& xi, const long& logp) {

}

CZZ EvaluatorUtils::evaluateExponent(const double& xr, const double& xi, const long& logp) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	if(logp < 31) {
		long p = 1 << logp;
		ZZ pxexpr = to_ZZ(xexpr * p);
		ZZ pxexpi = to_ZZ(xexpi * p);
		return CZZ(pxexpr, pxexpi);
	} else {
		long tmp = (1 << 30);
		ZZ pxexpr = to_ZZ(xexpr * tmp) << (logp - 30);
		ZZ pxexpi = to_ZZ(xexpi * tmp) << (logp - 30);
		return CZZ(pxexpr, pxexpi);
	}
}

CZZ EvaluatorUtils::evaluateSigmoid(const double& xr, const double& xi, const long& logp) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	double xsigmoidr = (xexpr * (xexpr + 1) + (xexpi * xexpi)) / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));
	double xsigmoidi = xexpi / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));

	if(logp < 31) {
		long p = (1 << logp);
		ZZ pxsigmoidr = to_ZZ(xsigmoidr * p);
		ZZ pxsigmoidi = to_ZZ(xsigmoidi * p);
		return CZZ(pxsigmoidr, pxsigmoidi);
	} else {
		long tmp = (1 << 30);

		ZZ pxsigmoidr = to_ZZ(xsigmoidr * tmp) << (logp - 30);
		ZZ pxsigmoidi = to_ZZ(xsigmoidi * tmp) << (logp - 30);
		return CZZ(pxsigmoidr, pxsigmoidi);
	}
}
