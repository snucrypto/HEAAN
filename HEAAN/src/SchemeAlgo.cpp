#include "SchemeAlgo.h"


Ciphertext* SchemeAlgo::encryptSingleArray(CZZ*& vals, long size) {
	Ciphertext* res = new Ciphertext[size];
	for (long i = 0; i < size; ++i) {
		res[i] = scheme.encryptSingle(vals[i], scheme.context.logQ);
	}
	return res;
}

CZZ* SchemeAlgo::decryptSingleArray(SecretKey& secretKey, Ciphertext*& ciphers, long size) {
	CZZ* res = new CZZ[size];
	for (int i = 0; i < size; ++i) {
		res[i] = scheme.decryptSingle(secretKey, ciphers[i]);
	}
	return res;
}

Ciphertext SchemeAlgo::powerOf2(Ciphertext& cipher, const long logp, const long logDegree) {
	Ciphertext res = cipher;
	for (long i = 0; i < logDegree; ++i) {
		scheme.squareAndEqual(res);
		scheme.reScaleByAndEqual(res, logp);
	}
	return res;
}

Ciphertext* SchemeAlgo::powerOf2Extended(Ciphertext& cipher, const long logp, const long logDegree) {
	Ciphertext* res = new Ciphertext[logDegree + 1];
	res[0] = cipher;
	for (long i = 1; i < logDegree + 1; ++i) {
		res[i] = scheme.square(res[i-1]);
		scheme.reScaleByAndEqual(res[i], logp);
	}
	return res;
}

//-----------------------------------------

Ciphertext SchemeAlgo::power(Ciphertext& cipher, const long logp, const long degree) {
	long logDegree = log2((double)degree);
	long po2Degree = 1 << logDegree;

	Ciphertext res = powerOf2(cipher, logp, logDegree);
	long remDegree = degree - po2Degree;
	if(remDegree > 0) {
		Ciphertext tmp = power(cipher, logp, remDegree);
		long bitsDown = tmp.logq - res.logq;
		scheme.modDownByAndEqual(tmp, bitsDown);
		scheme.multAndEqual(res, tmp);
		scheme.reScaleByAndEqual(res, logp);
	}
	return res;
}

Ciphertext* SchemeAlgo::powerExtended(Ciphertext& cipher, const long logp, const long degree) {
	Ciphertext* res = new Ciphertext[degree];
	long logDegree = log2((double)degree);
	Ciphertext* cpows = powerOf2Extended(cipher, logp, logDegree);
	long idx = 0;
	for (long i = 0; i < logDegree; ++i) {
		long powi = (1 << i);
		res[idx++] = cpows[i];
		for (int j = 0; j < powi-1; ++j) {
			long bitsDown = res[j].logq - cpows[i].logq;
			res[idx] = scheme.modDownBy(res[j], bitsDown);
			scheme.multAndEqual(res[idx], cpows[i]);
			scheme.reScaleByAndEqual(res[idx++], logp);
		}
	}
	res[idx++] = cpows[logDegree];
	long degree2 = (1 << logDegree);
	for (int i = 0; i < (degree - degree2); ++i) {
		long bitsDown = res[i].logq - cpows[logDegree].logq;
		res[idx] = scheme.modDownBy(res[i], bitsDown);
		scheme.multAndEqual(res[idx], cpows[logDegree]);
		scheme.reScaleByAndEqual(res[idx++], logp);
	}
	return res;
}

//-----------------------------------------

Ciphertext SchemeAlgo::prodOfPo2(Ciphertext*& ciphers, const long logp, const long logDegree) {
	Ciphertext* res = ciphers;
	for (long i = logDegree - 1; i >= 0; --i) {
		long powih = (1 << i);
		Ciphertext* tmp = new Ciphertext[powih];
		NTL_EXEC_RANGE(powih, first, last);
		for (long j = first; j < last; ++j) {
			tmp[j] = scheme.mult(res[2 * j], res[2 * j + 1]);
			scheme.reScaleByAndEqual(tmp[j], logp);
		}
		NTL_EXEC_RANGE_END;
		res = tmp;
	}
	return res[0];
}

Ciphertext SchemeAlgo::prod(Ciphertext*& ciphers, const long logp, const long degree) {
	long logDegree = log2((double)degree) + 1;
	long idx = 0;
	bool isinit = false;
	Ciphertext res;
	for (long i = 0; i < logDegree; ++i) {
		if(bit(degree, i)) {
			long powi = (1 << i);
			Ciphertext* tmp = new Ciphertext[powi];
			for (long j = 0; j < powi; ++j) {
				tmp[j] = ciphers[idx + j];
			}
			Ciphertext iprod = prodOfPo2(tmp, logp, i);
			if(isinit) {
				long bitsDown = res.logq - iprod.logq;
				scheme.modDownByAndEqual(res, bitsDown);
				scheme.multAndEqual(res, iprod);
				scheme.reScaleByAndEqual(res, logp);
			} else {
				res = iprod;
				isinit = true;
			}
			idx += powi;
		}
	}
	return res;
}

Ciphertext SchemeAlgo::sum(Ciphertext*& ciphers, const long size) {
	Ciphertext res = ciphers[0];
	for (long i = 1; i < size; ++i) {
		scheme.addAndEqual(res, ciphers[i]);
	}
	return res;
}

Ciphertext SchemeAlgo::distance(Ciphertext& cipher1, Ciphertext& cipher2, const long logp) {
	Ciphertext cres = scheme.sub(cipher1, cipher2);
	scheme.squareAndEqual(cres);
	scheme.reScaleByAndEqual(cres, logp);
	partialSlotsSumAndEqual(cres, cres.slots);
	return cres;
}

Ciphertext* SchemeAlgo::multVec(Ciphertext*& ciphers1, Ciphertext*& ciphers2, const long size) {
	Ciphertext* res = new Ciphertext[size];
	NTL_EXEC_RANGE(size, first, last);
	for (long i = first; i < last; ++i) {
		res[i] = scheme.mult(ciphers1[i], ciphers2[i]);
	}
	NTL_EXEC_RANGE_END;
	return res;
}

void SchemeAlgo::multAndEqualVec(Ciphertext*& ciphers1, Ciphertext*& ciphers2, const long size) {
	NTL_EXEC_RANGE(size, first, last);
	for (long i = first; i < last; ++i) {
		scheme.multAndEqual(ciphers1[i], ciphers2[i]);
	}
	NTL_EXEC_RANGE_END;
}


Ciphertext* SchemeAlgo::multAndModSwitchVec(Ciphertext*& ciphers1, Ciphertext*& ciphers2, const long precisionBits, const long size) {
	Ciphertext* res = new Ciphertext[size];
	NTL_EXEC_RANGE(size, first, last);
	for (long i = first; i < last; ++i) {
		res[i] = scheme.mult(ciphers1[i], ciphers2[i]);
		scheme.reScaleByAndEqual(res[i], precisionBits);
	}
	NTL_EXEC_RANGE_END;
	return res;
}

void SchemeAlgo::multModSwitchAndEqualVec(Ciphertext*& ciphers1, Ciphertext*& ciphers2, const long precisionBits, const long size) {
	NTL_EXEC_RANGE(size, first, last);
	for (long i = first; i < last; ++i) {
		scheme.multAndEqual(ciphers1[i], ciphers2[i]);
		scheme.reScaleByAndEqual(ciphers1[i], precisionBits);
	}
	NTL_EXEC_RANGE_END;
}

Ciphertext SchemeAlgo::innerProd(Ciphertext*& ciphers1, Ciphertext*& ciphers2, const long logp, const long size) {
	Ciphertext cip = scheme.mult(ciphers1[size-1], ciphers2[size-1]);

	NTL_EXEC_RANGE(size-1, first, last);
	for (long i = first; i < last; ++i) {
		Ciphertext cprodi = scheme.mult(ciphers1[i], ciphers2[i]);
		scheme.addAndEqual(cip, cprodi);
	}
	NTL_EXEC_RANGE_END;

	scheme.reScaleByAndEqual(cip, logp);
	return cip;
}

Ciphertext SchemeAlgo::partialSlotsSum(Ciphertext& cipher, const long slots) {
	Ciphertext res = cipher;
	for (long i = 1; i < slots; i <<= 1) {
		Ciphertext rot = scheme.leftRotateFast(cipher, i);
		scheme.addAndEqual(res, rot);
	}
	return res;
}

void SchemeAlgo::partialSlotsSumAndEqual(Ciphertext& cipher, const long slots) {
	for (long i = 1; i < slots; i <<= 1) {
		Ciphertext rot = scheme.leftRotateFast(cipher, i);
		scheme.addAndEqual(cipher, rot);
	}
}

//-----------------------------------------

Ciphertext SchemeAlgo::inverse(Ciphertext& cipher, const long logp, const long steps) {
	ZZ precision = power2_ZZ(logp);
	Ciphertext cpow = cipher;
	Ciphertext tmp = scheme.addConst(cipher, precision);
	scheme.modDownByAndEqual(tmp, logp);
	Ciphertext res = tmp;

	for (long i = 1; i < steps; ++i) {
		scheme.squareAndEqual(cpow);
		scheme.reScaleByAndEqual(cpow, logp);
		tmp = cpow;
		scheme.addConstAndEqual(tmp, precision);
		scheme.multAndEqual(tmp, res);
		scheme.reScaleByAndEqual(tmp, logp);
		res = tmp;
	}
	return res;
}

Ciphertext* SchemeAlgo::inverseExtended(Ciphertext& cipher, const long logp, const long steps) {
	ZZ precision = power2_ZZ(logp);

	Ciphertext* res = new Ciphertext[steps];
	Ciphertext cpow = cipher;
	Ciphertext tmp = scheme.addConst(cipher, precision);
	scheme.modDownByAndEqual(tmp, logp);
	res[0] = tmp;

	for (long i = 1; i < steps; ++i) {
		scheme.squareAndEqual(cpow);
		scheme.reScaleByAndEqual(cpow, logp);
		tmp = cpow;
		scheme.addConstAndEqual(tmp, precision);
		scheme.multAndEqual(tmp, res[i - 1]);
		scheme.reScaleByAndEqual(tmp, logp);
		res[i] = tmp;
	}
	return res;
}

//-----------------------------------------

Ciphertext SchemeAlgo::function(Ciphertext& cipher, string& funcName, const long logp, const long degree) {
	Ciphertext* cpows = powerExtended(cipher, logp, degree);

	long dlogp = 2 * logp;

	double* coeffs = scheme.context.taylorCoeffsMap.at(funcName);

	ZZ tmp = EvaluatorUtils::evalZZ(coeffs[1], logp);
	Ciphertext res = scheme.multByConst(cpows[0], tmp);

	tmp = EvaluatorUtils::evalZZ(coeffs[0], dlogp);
	scheme.addConstAndEqual(res, tmp);

	for (int i = 1; i < degree; ++i) {
		if(abs(coeffs[i + 1]) > 1e-27) {
			tmp = EvaluatorUtils::evalZZ(coeffs[i + 1], logp);
			Ciphertext aixi = scheme.multByConst(cpows[i], tmp);
			long bitsDown = res.logq - aixi.logq;
			scheme.modDownByAndEqual(res, bitsDown);
			scheme.addAndEqual(res, aixi);
		}
	}
	scheme.reScaleByAndEqual(res, logp);
	return res;
}

Ciphertext SchemeAlgo::functionLazy(Ciphertext& cipher, string& funcName, const long logp, const long degree) {
	Ciphertext* cpows = powerExtended(cipher, logp, degree);

	long dlogp = 2 * logp;

	double* coeffs = scheme.context.taylorCoeffsMap.at(funcName);

	ZZ tmp = EvaluatorUtils::evalZZ(coeffs[1], logp);
	Ciphertext res = scheme.multByConst(cpows[0], tmp);

	tmp = EvaluatorUtils::evalZZ(coeffs[0], dlogp);
	scheme.addConstAndEqual(res, tmp);

	for (int i = 1; i < degree; ++i) {
		if(abs(coeffs[i + 1]) > 1e-27) {
			tmp = EvaluatorUtils::evalZZ(coeffs[i + 1], logp);
			Ciphertext aixi = scheme.multByConst(cpows[i], tmp);
			long bitsDown = res.logq - aixi.logq;
			scheme.modDownByAndEqual(res, bitsDown);
			scheme.addAndEqual(res, aixi);
		}
	}
	return res;
}

Ciphertext* SchemeAlgo::functionExtended(Ciphertext& cipher, string& funcName, const long logp, const long degree) {
	Ciphertext* cpows = powerExtended(cipher, logp, degree);

	long dlogp = 2 * logp;
	double* coeffs = scheme.context.taylorCoeffsMap.at(funcName);

	ZZ tmp = EvaluatorUtils::evalZZ(coeffs[1], logp);
	Ciphertext aixi = scheme.multByConst(cpows[0], tmp);

	tmp = EvaluatorUtils::evalZZ(coeffs[0], dlogp);
	scheme.addConstAndEqual(aixi, tmp);

	Ciphertext* res = new Ciphertext[degree];
	res[0] = aixi;
	for (long i = 1; i < degree; ++i) {
		if(abs(coeffs[i + 1]) > 1e-27) {
			tmp = EvaluatorUtils::evalZZ(coeffs[i + 1], logp);
			aixi = scheme.multByConst(cpows[i], tmp);
			long bitsDown = res[i - 1].logq - aixi.logq;
			Ciphertext ctmp = scheme.modDownBy(res[i - 1], bitsDown);
			scheme.addAndEqual(aixi, ctmp);
			res[i] = aixi;
		} else {
			res[i] = res[i - 1];
		}
	}
	NTL_EXEC_RANGE(degree, first, last);
	for (long i = first; i < last; ++i) {
		scheme.reScaleByAndEqual(res[i], logp);
	}
	NTL_EXEC_RANGE_END;
	return res;
}

void SchemeAlgo::fftRaw(Ciphertext*& ciphers, const long size, const bool isForward) {
	for (long i = 1, j = 0; i < size; ++i) {
		long bit = size >> 1;
		for (; j >= bit; bit>>=1) {
			j -= bit;
		}
		j += bit;
		if(i < j) {
			swap(ciphers[i], ciphers[j]);
		}
	}

	for (long len = 2; len <= size; len <<= 1) {
		long shift = isForward ? ((scheme.context.N / len) << 1) : ((scheme.context.N - scheme.context.N / len) << 1);
		for (long i = 0; i < size; i += len) {
			NTL_EXEC_RANGE(len / 2, first, last);
			for (long j = first; j < last; ++j) {
				Ciphertext u = ciphers[i + j];
				scheme.multByMonomialAndEqual(ciphers[i + j + len / 2], shift * j);
				scheme.addAndEqual(ciphers[i + j], ciphers[i + j + len / 2]);
				scheme.subAndEqual2(u, ciphers[i + j + len / 2]);
			}
			NTL_EXEC_RANGE_END;
		}
	}
}

void SchemeAlgo::fft(Ciphertext*& ciphers, const long size) {
	fftRaw(ciphers, size, true);
}

void SchemeAlgo::fftInv(Ciphertext*& ciphers, const long size) {
	fftRaw(ciphers, size, false);
	long logsize = log2((double)size);

	NTL_EXEC_RANGE(size, first, last);
	for (long i = first; i < last; ++i) {
		scheme.reScaleByAndEqual(ciphers[i], logsize);
	}
	NTL_EXEC_RANGE_END;
}

void SchemeAlgo::fftInvLazy(Ciphertext*& ciphers, const long size) {
	return fftRaw(ciphers, size, false);
}
