#include "EvaluatorUtils.h"

double EvaluatorUtils::evalReal(const ZZ& x, const long& logp) {
	RR xp = to_RR(x);
	xp.e -= logp;
	return to_double(xp);
}

void EvaluatorUtils::evalReal(double& res, const ZZ& x, const long& logp) {
	res = evalReal(x, logp);
}

double EvaluatorUtils::evalReal(const CZZ& x, const long& logp) {
	return evalReal(x.r, logp);
}

void EvaluatorUtils::evalReal(double& res, const CZZ& x, const long& logp) {
	res = evalReal(x.r, logp);
}

void EvaluatorUtils::evalComplex(double& resx, double& resi, const CZZ& x, const long& logp) {
	resx = evalReal(x.r, logp);
	resi = evalReal(x.i, logp);
}

void EvaluatorUtils::evalRealArray(double*& res, const ZZ*& xarray, const long& size, const long& logp) {
	for (long i = 0; i < size; ++i) {
		res[i] = evalReal(xarray[i], logp);
	}
}

double* EvaluatorUtils::evalRealArray(const ZZ*& xarray, const long& size, const long& logp) {
	double* res = new double[size];
	evalRealArray(res, xarray, size, logp);
	return res;
}

void EvaluatorUtils::evalRealArray(double*& res, const CZZ*& xarray, const long& size, const long& logp) {
	for (long i = 0; i < size; ++i) {
		res[i] = evalReal(xarray[i].r, logp);
	}
}

double* EvaluatorUtils::evalRealArray(const CZZ*& xarray, const long& size, const long& logp) {
	double* res = new double[size];
	evalRealArray(res, xarray, size, logp);
	return res;
}

void EvaluatorUtils::evalComplexArray(double*& resx, double*& resi, const CZZ*& xarray, const long& size, const long& logp) {
	for (long i = 0; i < size; ++i) {
		resx[i] = evalReal(xarray[i].r, logp);
		resi[i] = evalReal(xarray[i].i, logp);
	}
}

ZZ EvaluatorUtils::evalZZ(const double& x, const long& logp) {
	return evalZZ(to_RR(x), logp);
}

ZZ EvaluatorUtils::evalZZ(const RR& x, const long& logp) {
	RR xp = MakeRR(x.x, x.e + logp);
	return RoundToZZ(xp);
}

CZZ EvaluatorUtils::evalCZZ(const double& xr, const double& xi, const long& logp) {
	return evalCZZ(to_RR(xr), to_RR(xi), logp);
}

CZZ EvaluatorUtils::evalCZZ(const RR& xr, const RR& xi, const long& logp) {
	RR xrp = MakeRR(xr.x, xr.e + logp);
	RR xip = MakeRR(xi.x, xi.e + logp);
	return CZZ(RoundToZZ(xrp), RoundToZZ(xip));
}

CZZ EvaluatorUtils::evalCZZ0(const double& xr, const long& logp) {
	return evalCZZ0(to_RR(xr), logp);
}

CZZ EvaluatorUtils::evalCZZ0(const RR& xr, const long& logp) {
	RR xrp = MakeRR(xr.x, xr.e + logp);
	return CZZ(RoundToZZ(xrp));
}

CZZ* EvaluatorUtils::evalCZZArray(double*& xr, double*& xi, const long& size, const long& logp) {
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; ++i) {
		res[i] = evalCZZ(xr[i], xi[i], logp);
	}
	return res;
}

CZZ* EvaluatorUtils::evalCZZArray(RR*& xr, RR*& xi, const long& size, const long& logp) {
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; ++i) {
		res[i] = evalCZZ(xr[i], xi[i], logp);
	}
	return res;
}

CZZ* EvaluatorUtils::evalCZZ0Array(double*& xr, const long& size, const long& logp) {
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; ++i) {
		res[i] = evalCZZ0(xr[i], logp);
	}
	return res;
}

CZZ* EvaluatorUtils::evalCZZ0Array(RR*& xr, const long& size, const long& logp) {
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; ++i) {
		res[i] = evalCZZ0(xr[i], logp);
	}
	return res;
}

CZZ EvaluatorUtils::evalRandCZZ(const long& logp) {
	return CZZ(RandomBits_ZZ(logp), RandomBits_ZZ(logp));
}

CZZ EvaluatorUtils::evalRandCZZ0(const long& logp) {
	return CZZ(RandomBits_ZZ(logp));
}

CZZ EvaluatorUtils::evalRandCZZCircle(const long& logp) {
	RR angle = random_RR();
	RR mr = cos(angle * 2 * Pi);
	RR mi = sin(angle * 2 * Pi);
	return evalCZZ(mr, mi, logp);
}

ZZ* EvaluatorUtils::evalRandZZArray(const long& size, const long& logp) {
	ZZ* res = new ZZ[size];
	for (long i = 0; i < size; i++) {
		res[i] = RandomBits_ZZ(logp);
	}
	return res;
}

CZZ* EvaluatorUtils::evalRandCZZArray(const long& size, const long& logp) {
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; i++) {
		res[i].r = RandomBits_ZZ(logp);
		res[i].i = RandomBits_ZZ(logp);
	}
	return res;
}

CZZ* EvaluatorUtils::evalRandCZZ0Array(const long& size, const long& logp) {
	CZZ* res = new CZZ[size];
	for (long i = 0; i < size; i++) {
		res[i].r = RandomBits_ZZ(logp);
	}
	return res;
}

CZZ EvaluatorUtils::evalCZZPow(const double& xr, const double& xi, const long& degree, const long& logp) {
	long logDegree = log2(degree);
	long po2Degree = 1 << logDegree;
	CZZ res = evalCZZPow2(xr, xi, logDegree, logp);
	long remDegree = degree - po2Degree;
	if(remDegree > 0) {
		CZZ tmp = evalCZZPow(xr, xi, remDegree, logp);
		res *= tmp;
		res >>= logp;
	}
	return res;
}

CZZ EvaluatorUtils::evalCZZPow(const RR& xr, const RR& xi, const long& degree, const long& logp) {
	long logDegree = log2(degree);
	long po2Degree = 1 << logDegree;
	CZZ res = evalCZZPow2(xr, xi, logDegree, logp);
	long remDegree = degree - po2Degree;
	if(remDegree > 0) {
		CZZ tmp = evalCZZPow(xr, xi, remDegree, logp);
		res *= tmp;
		res >>= logp;
	}
	return res;
}

CZZ EvaluatorUtils::evalCZZPow2(const double& xr, const double& xi, const long& logDegree, const long& logp) {
	return evalCZZPow2(to_RR(xr), to_RR(xi), logDegree, logp);
}

CZZ EvaluatorUtils::evalCZZPow2(const RR& xr, const RR& xi, const long& logDegree, const long& logp) {
	CZZ res = evalCZZ(xr, xi, logp);
	for (int i = 0; i < logDegree; ++i) {
		res *= res;
		res >>= logp;
	}
	return res;
}

CZZ* EvaluatorUtils::evalCZZPowArray(const double& xr, const double& xi, const long& degree, const long& logp) {
	return  evalCZZPowArray(to_RR(xr), to_RR(xi), degree, logp);
}

CZZ* EvaluatorUtils::evalCZZPowArray(const RR& xr, const RR& xi, const long& degree, const long& logp) {
	CZZ* res = new CZZ[degree];
	CZZ m = evalCZZ(xr, xi, logp);
	res[0] = m;
	for (long i = 0; i < degree - 1; ++i) {
		res[i + 1] = (res[i] * m) >> logp;
	}
	return res;
}

CZZ* EvaluatorUtils::evalCZZPow2Array(const double& xr, const double& xi, const long& logDegree, const long& logp) {
	return evalCZZPow2Array(to_RR(xr), to_RR(xi), logDegree, logp);
}

CZZ* EvaluatorUtils::evalCZZPow2Array(const RR& xr, const RR& xi, const long& logDegree, const long& logp) {
	CZZ* res = new CZZ[logDegree + 1];
	CZZ m = evalCZZ(xr, xi, logp);
	res[0] = m;
	for (long i = 0; i < logDegree; ++i) {
		res[i + 1] = (res[i] * res[i]) >> logp;
	}
	return res;
}

CZZ EvaluatorUtils::evalCZZInv(const double& xr, const double& xi, const long& logp) {
	return evalCZZInv(to_RR(xr), to_RR(xi), logp);
}

CZZ EvaluatorUtils::evalCZZInv(const RR& xr, const RR& xi, const long& logp) {
	RR xinvr = xr / (xr * xr + xi * xi);
	RR xinvi = -xi / (xr * xr + xi * xi);

	return evalCZZ(xinvr, xinvi, logp);
}

CZZ EvaluatorUtils::evalCZZLog(const double& xr, const double& xi, const long& logp) {
	double xlogr = log(xr * xr + xi * xi) / 2;
	double xlogi = atan(xi / xr);

	return evalCZZ(xlogr, xlogi, logp);
}

CZZ EvaluatorUtils::evalCZZExp(const double& xr, const double& xi, const long& logp) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	return evalCZZ(xexpr, xexpi, logp);
}

CZZ EvaluatorUtils::evalCZZExp(const RR& xr, const RR& xi, const long& logp) {
	RR xrexp = exp(xr);
	RR xexpr = xrexp * cos(xi);
	RR xexpi = xrexp * sin(xi);

	return evalCZZ(xexpr, xexpi, logp);
}

CZZ EvaluatorUtils::evalCZZSigmoid(const double& xr, const double& xi, const long& logp) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	double xsigmoidr = (xexpr * (xexpr + 1) + (xexpi * xexpi)) / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));
	double xsigmoidi = xexpi / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));

	return evalCZZ(xsigmoidr, xsigmoidi, logp);
}

CZZ EvaluatorUtils::evalCZZSigmoid(const RR& xr, const RR& xi, const long& logp) {
	RR xrexp = exp(xr);
	RR xexpr = xrexp * cos(xi);
	RR xexpi = xrexp * sin(xi);

	RR xsigmoidr = (xexpr * (xexpr + 1) + (xexpi * xexpi)) / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));
	RR xsigmoidi = xexpi / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));

	return evalCZZ(xsigmoidr, xsigmoidi, logp);
}

void EvaluatorUtils::leftShiftAndEqual(CZZ*& vals, const long& size, const long& bits) {
	for (long i = 0; i < size; ++i) {
		vals[i] <<= bits;
	}
}

void EvaluatorUtils::leftRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize) {
	long remrotSize = rotSize % size;
	if(remrotSize != 0) {
		long divisor = GCD(remrotSize, size);
		long steps = size / divisor;
		for (long i = 0; i < divisor; ++i) {
			CZZ tmp = vals[i];
			long idx = i;
			for (long j = 0; j < steps - 1; ++j) {
				vals[idx] = vals[(idx + remrotSize) % size];
				idx = (idx + remrotSize) % size;
			}
			vals[idx] = tmp;
		}
	}
}

void EvaluatorUtils::rightRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize) {
	long remrotSize = rotSize % size;
	long leftremrotSize = (size - remrotSize) % size;
	leftRotateAndEqual(vals, size, leftremrotSize);
}
