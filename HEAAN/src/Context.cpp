/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Context.h"
#include "Ring2Utils.h"
#include "EvaluatorUtils.h"

#include "StringUtils.h"

Context::Context(long logN, long logQ, double sigma, long h) : logN(logN), logQ(logQ), sigma(sigma), h(h) {
	init(logN, logQ, sigma, h);
}

Context::Context(const Context& o) : logN(o.logN), logQ(o.logQ), sigma(o.sigma), h(o.h) {
	init(logN, logQ, sigma, h);
}

void Context::init(long logN, long logQ, double sigma, long h) {
	N = 1 << logN;
	Nh = N >> 1;
	logNh = logN - 1;
	M = N << 1;
	logQQ = logQ << 1;
	Q = power2_ZZ(logQ);
	QQ = power2_ZZ(logQQ);

	rotGroup = new long[Nh];
	long fivePows = 1;
	for (long i = 0; i < Nh; ++i) {
		rotGroup[i] = fivePows;
		fivePows *= 5;
		fivePows %= M;
	}

	ksiPows = new complex<double>[M + 1];
	for (long j = 0; j < M; ++j) {
		double angle = 2.0 * M_PI * j / M;
		ksiPows[j].real(cos(angle));
		ksiPows[j].imag(sin(angle));
	}

	ksiPows[M] = ksiPows[0];

	qpowvec = new ZZ[logQQ + 1];
	qpowvec[0] = ZZ(1);
	for (long i = 1; i < logQQ + 1; ++i) {
		qpowvec[i] = qpowvec[i - 1] << 1;
	}

	taylorCoeffsMap.insert(pair<string, double*>(LOGARITHM, new double[11]{0,1,-0.5,1./3,-1./4,1./5,-1./6,1./7,-1./8,1./9,-1./10}));
	taylorCoeffsMap.insert(pair<string, double*>(EXPONENT, new double[11]{1,1,0.5,1./6,1./24,1./120,1./720,1./5040, 1./40320,1./362880,1./3628800}));
	taylorCoeffsMap.insert(pair<string, double*>(SIGMOID, new double[11]{1./2,1./4,0,-1./48,0,1./480,0,-17./80640,0,31./1451520,0}));
}

Context::~Context() {
	delete[] rotGroup;
	delete[] ksiPows;
}


//----------------------------------------------------------------------------------
//   ENCODINGS & BOOTSTRAPPING
//----------------------------------------------------------------------------------


ZZX Context::encode(complex<double>* vals, long slots, long logp) {
	complex<double>* uvals = new complex<double>[slots];
	long i, jdx, idx;
	copy(vals, vals + slots, uvals);

	ZZX mx;
	mx.SetLength(N);
	long gap = Nh / slots;
	fftSpecialInv(uvals, slots);
	for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
		mx.rep[idx] = EvaluatorUtils::scaleUpToZZ(uvals[i].real(), logp);
		mx.rep[jdx] = EvaluatorUtils::scaleUpToZZ(uvals[i].imag(), logp);
	}
	delete[] uvals;
	return mx;
}

ZZX Context::encodeSingle(complex<double> val, long logp) {
	ZZX mx;
	mx.SetLength(N);
	mx.rep[0] = EvaluatorUtils::scaleUpToZZ(val.real(), logp);
	mx.rep[Nh] = EvaluatorUtils::scaleUpToZZ(val.imag(), logp);
	return mx;
}

ZZX Context::encode(double* vals, long slots, long logp) {
	complex<double>* uvals = new complex<double>[slots];
	long i, jdx, idx;
	for (i = 0; i < slots; ++i) {
		uvals[i].real(vals[i]);
	}

	ZZX mx;
	mx.SetLength(N);

	long gap = Nh / slots;

	fftSpecialInv(uvals, slots);

	for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
		mx.rep[idx] = EvaluatorUtils::scaleUpToZZ(uvals[i].real(), logp);
		mx.rep[jdx] = EvaluatorUtils::scaleUpToZZ(uvals[i].imag(), logp);
	}
	delete[] uvals;
	return mx;
}

ZZX Context::encodeSingle(double val, long logp) {
	ZZX mx;
	mx.SetLength(N);
	mx.rep[0] = EvaluatorUtils::scaleUpToZZ(val, logp);
	return mx;
}

complex<double>* Context::decode(ZZX& mx, long slots, long logp, long logq) {
	ZZ q = qpowvec[logq];
	long gap = Nh / slots;
	complex<double>* res = new complex<double>[slots];
	ZZ tmp;

	for (long i = 0, idx = 0; i < slots; ++i, idx += gap) {
		rem(tmp, mx[idx], q);
		if(NumBits(tmp) == logq) tmp -= q;
		res[i].real(EvaluatorUtils::scaleDownToReal(tmp, logp));

		rem(tmp, mx[idx + Nh], q);
		if(NumBits(tmp) == logq) tmp -= q;
		res[i].imag(EvaluatorUtils::scaleDownToReal(tmp, logp));
	}
	fftSpecial(res, slots);
	return res;
}

complex<double> Context::decodeSingle(ZZX& mx, long logp, long logq, bool isComplex) {
	ZZ q = qpowvec[logq];

	complex<double> res;
	ZZ tmp = mx.rep[0] % q;
	if(NumBits(tmp) == logq) tmp -= q;
	res.real(EvaluatorUtils::scaleDownToReal(tmp, logp));

	if(isComplex) {
		tmp = mx.rep[Nh] % q;
		if(NumBits(tmp) == logq) tmp -= q;
		res.imag(EvaluatorUtils::scaleDownToReal(tmp, logp));
	}

	return res;
}

void Context::addBootContext(long logSlots, long logp) {
	if(bootContextMap.find(logSlots) == bootContextMap.end()) {
		long slots = 1 << logSlots;
		long dslots = slots << 1;
		long logk = logSlots >> 1;

		long k = 1 << logk;
		long i, pos,  ki, jdx, idx, deg;
		long gap = Nh >> logSlots;

		ZZX* pvecInv = new ZZX[slots];
		ZZX* pvec = new ZZX[slots];

		complex<double>* pvals = new complex<double>[dslots];

		ZZX p1, p2;
		double c = 0.25/M_PI;

		if(logSlots < logNh) {
			long dgap = gap >> 1;
			for (ki = 0; ki < slots; ki += k) {
				for (pos = ki; pos < ki + k; ++pos) {
					for (i = 0; i < slots - pos; ++i) {
						deg = ((M - rotGroup[i + pos]) * i * gap) % M;
						pvals[i] = ksiPows[deg];
						pvals[i + slots].real(-pvals[i].imag());
						pvals[i + slots].imag(pvals[i].real());
					}
					for (i = slots - pos; i < slots; ++i) {
						deg =((M - rotGroup[i + pos - slots]) * i * gap) % M;
						pvals[i] = ksiPows[deg];
						pvals[i + slots].real(-pvals[i].imag());
						pvals[i + slots].imag(pvals[i].real());
					}
					EvaluatorUtils::rightRotateAndEqual(pvals, dslots, ki);
					fftSpecialInv(pvals, dslots);
					pvec[pos].SetLength(N);
					for (i = 0, jdx = Nh, idx = 0; i < dslots; ++i, jdx += dgap, idx += dgap) {
						pvec[pos].rep[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
						pvec[pos].rep[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
					}
				}
			}
			for (i = 0; i < slots; ++i) {
				pvals[i] = 0.0;
				pvals[i + slots].real(0);
				pvals[i + slots].imag(-c);
			}
			p1.SetLength(N);
			fftSpecialInv(pvals, dslots);
			for (i = 0, jdx = Nh, idx = 0; i < dslots; ++i, jdx += dgap, idx += dgap) {
				p1.rep[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
				p1.rep[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
			}

			for (i = 0; i < slots; ++i) {
				pvals[i] = c;
				pvals[i + slots] = 0;
			}

			p2.SetLength(N);
			fftSpecialInv(pvals, dslots);
			for (i = 0, jdx = Nh, idx = 0; i < dslots; ++i, jdx += dgap, idx += dgap) {
				p2.rep[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
				p2.rep[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
			}
		} else {
			for (ki = 0; ki < slots; ki += k) {
				for (pos = ki; pos < ki + k; ++pos) {
					for (i = 0; i < slots - pos; ++i) {
						deg = ((M - rotGroup[i + pos]) * i * gap) % M;
						pvals[i] = ksiPows[deg];
					}
					for (i = slots - pos; i < slots; ++i) {
						deg =((M - rotGroup[i + pos - slots]) * i * gap) % M;
						pvals[i] = ksiPows[deg];
					}
					EvaluatorUtils::rightRotateAndEqual(pvals, slots, ki);
					fftSpecialInv(pvals, slots);
					pvec[pos].SetLength(N);
					for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
						pvec[pos].rep[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
						pvec[pos].rep[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
					}
				}
			}
		}

		for (ki = 0; ki < slots; ki += k) {
			for (pos = ki; pos < ki + k; ++pos) {

				for (i = 0; i < slots - pos; ++i) {
					deg = (rotGroup[i] * (i + pos) * gap) % M;
					pvals[i] = ksiPows[deg];
				}
				for (i = slots - pos; i < slots; ++i) {
					deg = (rotGroup[i] * (i + pos - slots) * gap) % M;
					pvals[i] = ksiPows[deg];
				}
				EvaluatorUtils::rightRotateAndEqual(pvals, slots, ki);
				fftSpecialInv(pvals, slots);
				pvecInv[pos].SetLength(N);
				for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
					pvecInv[pos].rep[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
					pvecInv[pos].rep[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
				}
			}
		}

		delete[] pvals;
		bootContextMap.insert(pair<long, BootContext>(logSlots, BootContext(pvec, pvecInv, p1, p2, logp)));
	}
}

//----------------------------------------------------------------------------------
//   FFT & FFT INVERSE
//----------------------------------------------------------------------------------


void Context::bitReverse(complex<double>* vals, const long size) {
	for (long i = 1, j = 0; i < size; ++i) {
		long bit = size >> 1;
		for (; j >= bit; bit>>=1) {
			j -= bit;
		}
		j += bit;
		if(i < j) {
			swap(vals[i], vals[j]);
		}
	}
}

void Context::fft(complex<double>* vals, const long size) {
	bitReverse(vals, size);
	for (long len = 2; len <= size; len <<= 1) {
		long MoverLen = M / len;
		long lenh = len >> 1;
		for (long i = 0; i < size; i += len) {
			for (long j = 0; j < lenh; ++j) {
				long idx = j * MoverLen;
				complex<double> u = vals[i + j];
				complex<double> v = vals[i + j + lenh];
				v *= ksiPows[idx];
				vals[i + j] = u + v;
				vals[i + j + lenh] = u - v;
			}
		}
	}
}

void Context::fftInvLazy(complex<double>* vals, const long size) {
	bitReverse(vals, size);
	for (long len = 2; len <= size; len <<= 1) {
		long MoverLen = M / len;
		long lenh = len >> 1;
		for (long i = 0; i < size; i += len) {
			for (long j = 0; j < lenh; ++j) {
				long idx = (len - j) * MoverLen;
				complex<double> u = vals[i + j];
				complex<double> v = vals[i + j + lenh];
				v *= ksiPows[idx];
				vals[i + j] = u + v;
				vals[i + j + lenh] = u - v;
			}
		}
	}
}

void Context::fftInv(complex<double>* vals, const long size) {
	fftInvLazy(vals, size);
	for (long i = 0; i < size; ++i) {
		vals[i] /= size;
	}
}

void Context::fftSpecial(complex<double>* vals, const long size) {
	bitReverse(vals, size);
	for (long len = 2; len <= size; len <<= 1) {
		for (long i = 0; i < size; i += len) {
			long lenh = len >> 1;
			long lenq = len << 2;
			for (long j = 0; j < lenh; ++j) {
				long idx = ((rotGroup[j] % lenq)) * M / lenq;
				complex<double> u = vals[i + j];
				complex<double> v = vals[i + j + lenh];
				v *= ksiPows[idx];
				vals[i + j] = u + v;
				vals[i + j + lenh] = u - v;
			}
		}
	}
}

void Context::fftSpecialInvLazy(complex<double>* vals, const long size) {
	for (long len = size; len >= 1; len >>= 1) {
		for (long i = 0; i < size; i += len) {
			long lenh = len >> 1;
			long lenq = len << 2;
			for (long j = 0; j < lenh; ++j) {
				long idx = (lenq - (rotGroup[j] % lenq)) * M / lenq;
				complex<double> u = vals[i + j] + vals[i + j + lenh];
				complex<double> v = vals[i + j] - vals[i + j + lenh];
				v *= ksiPows[idx];
				vals[i + j] = u;
				vals[i + j + lenh] = v;
			}
		}
	}
	bitReverse(vals, size);
}

void Context::fftSpecialInv(complex<double>* vals, const long size) {
	fftSpecialInvLazy(vals, size);
	for (long i = 0; i < size; ++i) {
		vals[i] /= size;
	}
}
