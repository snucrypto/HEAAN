/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Ring.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/lip.h>
#include <NTL/sp_arith.h>
#include <NTL/tools.h>
#include <NTL/vector.h>
#include <NTL/ZZVec.h>
#include <NTL/lip.h>

#include "EvaluatorUtils.h"
#include "BootContext.h"


Ring::Ring(long logN, long logQ, double sigma, long h) : logN(logN), logQ(logQ), sigma(sigma), h(h), multiplier(logN, logQ) {
	logNh = logN - 1;
	N = 1 << logN;
	logM = logN + 1;
	M = N << 1;
	Nh = N >> 1;

	logQQ = 2 * logQ;
	Q = power2_ZZ(logQ);
	QQ = power2_ZZ(logQQ);

	qpows = new ZZ[logQQ + 1];
	qpows[0] = ZZ(1);
	for (long i = 1; i < logQQ + 1; ++i) {
		qpows[i] = qpows[i - 1] << 1;
	}
	rotGroup = new long[N / 2];
	long fivePows = 1;
	for (long i = 0; i < N / 2; ++i) {
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


	taylorCoeffsMap.insert(
			pair<string, double*>(LOGARITHM,
					new double[11] { 0, 1, -0.5, 1. / 3, -1. / 4, 1. / 5, -1.
							/ 6, 1. / 7, -1. / 8, 1. / 9, -1. / 10 }));
	taylorCoeffsMap.insert(
			pair<string, double*>(EXPONENT,
					new double[11] { 1, 1, 0.5, 1. / 6, 1. / 24, 1. / 120, 1.
							/ 720, 1. / 5040, 1. / 40320, 1. / 362880, 1.
							/ 3628800 }));
	taylorCoeffsMap.insert(
			pair<string, double*>(SIGMOID,
					new double[11] { 1. / 2, 1. / 4, 0, -1. / 48, 0, 1. / 480,
							0, -17. / 80640, 0, 31. / 1451520, 0 }));

}

void Ring::arrayBitReverse(complex<double>* vals, long n) {
	for (long i = 1, j = 0; i < n; ++i) {
		long bit = n >> 1;
		for (; j >= bit; bit >>= 1) {
			j -= bit;
		}
		j += bit;
		if (i < j) {
			swap(vals[i], vals[j]);
		}
	}
}

void Ring::EMB(complex<double>* vals, long n) {
	arrayBitReverse(vals, n);
	for (long len = 2; len <= n; len <<= 1) {
		for (long i = 0; i < n; i += len) {
			long lenh = len >> 1;
			long lenq = len << 2;
			long gap = M / lenq;
			for (long j = 0; j < lenh; ++j) {
				long idx = ((rotGroup[j] % lenq)) * gap;
				complex<double> u = vals[i + j];
				complex<double> v = vals[i + j + lenh];
				v *= ksiPows[idx];
				vals[i + j] = u + v;
				vals[i + j + lenh] = u - v;
			}
		}
	}
}

void Ring::EMBInvLazy(complex<double>* vals, long n) {
	for (long len = n; len >= 1; len >>= 1) {
		for (long i = 0; i < n; i += len) {
			long lenh = len >> 1;
			long lenq = len << 2;
			long gap = M / lenq;
			for (long j = 0; j < lenh; ++j) {
				long idx = (lenq - (rotGroup[j] % lenq)) * gap;
				complex<double> u = vals[i + j] + vals[i + j + lenh];
				complex<double> v = vals[i + j] - vals[i + j + lenh];
				v *= ksiPows[idx];
				vals[i + j] = u;
				vals[i + j + lenh] = v;
			}
		}
	}
	arrayBitReverse(vals, n);
}

void Ring::EMBInv(complex<double>* vals, long n) {
	EMBInvLazy(vals, n);
	for (long i = 0; i < n; ++i) {
		vals[i] /= n;
	}
}

void Ring::encode(ZZ* mx, double* vals, long slots, long logp) {
	complex<double>* uvals = new complex<double> [slots];
	long i, jdx, idx;
	for (i = 0; i < slots; ++i) {
		uvals[i].real(vals[i]);
	}

	long gap = Nh / slots;

	EMBInv(uvals, slots);

	for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
		mx[idx] = EvaluatorUtils::scaleUpToZZ(uvals[i].real(), logp);
		mx[jdx] = EvaluatorUtils::scaleUpToZZ(uvals[i].imag(), logp);
	}
	delete[] uvals;
}

void Ring::encode(ZZ* mx, complex<double>* vals, long slots, long logp) {
	complex<double>* uvals = new complex<double> [slots];
	long i, jdx, idx;
	copy(vals, vals + slots, uvals);

	long gap = Nh / slots;
	EMBInv(uvals, slots);
	for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
		mx[idx] = EvaluatorUtils::scaleUpToZZ(uvals[i].real(), logp);
		mx[jdx] = EvaluatorUtils::scaleUpToZZ(uvals[i].imag(), logp);
	}
	delete[] uvals;
}

void Ring::decode(ZZ* mx, complex<double>* vals, long slots, long logp, long logq) {
	ZZ q = qpows[logq];
	long gap = Nh / slots;
	ZZ tmp;

	for (long i = 0, idx = 0; i < slots; ++i, idx += gap) {
		rem(tmp, mx[idx], q);
		if (NumBits(tmp) == logq)
			tmp -= q;
		vals[i].real(EvaluatorUtils::scaleDownToReal(tmp, logp));

		rem(tmp, mx[idx + Nh], q);
		if (NumBits(tmp) == logq)
			tmp -= q;
		vals[i].imag(EvaluatorUtils::scaleDownToReal(tmp, logp));
	}
	EMB(vals, slots);
}

void Ring::addBootContext(long logSlots, long logp) {
	if (bootContextMap.find(logSlots) == bootContextMap.end()) {
		long slots = 1 << logSlots;
		long dslots = slots << 1;
		long logk = logSlots >> 1;

		long k = 1 << logk;
		long i, pos, ki, jdx, idx, deg;
		long gap = Nh >> logSlots;

		long np;

		uint64_t** rpvec = new uint64_t*[slots];
		uint64_t** rpvecInv = new uint64_t*[slots];
		uint64_t* rp1;
		uint64_t* rp2;

		long* bndvec = new long[slots];
		long* bndvecInv = new long[slots];
		long bnd1;
		long bnd2;

		ZZ* pvec = new ZZ[N];
		complex<double>* pvals = new complex<double> [dslots];

		double c = 0.25 / M_PI;

		if (logSlots < logNh) {
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
						deg = ((M - rotGroup[i + pos - slots]) * i * gap) % M;
						pvals[i] = ksiPows[deg];
						pvals[i + slots].real(-pvals[i].imag());
						pvals[i + slots].imag(pvals[i].real());
					}
					EvaluatorUtils::rightRotateAndEqual(pvals, dslots, ki);
					EMBInv(pvals, dslots);
					for (i = 0, jdx = Nh, idx = 0; i < dslots; ++i, jdx += dgap, idx += dgap) {
						pvec[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
						pvec[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
					}
					bndvec[pos] = maxBits(pvec, N);
					np = ceil((bndvec[pos] + logQ + 2 * logN + 2)/59.0);
					rpvec[pos] = toNTT(pvec, np);
					for (i = 0; i < N; ++i) {
						pvec[i] = ZZ::zero();
					}
				}
			}

			for (i = 0; i < slots; ++i) {
				pvals[i] = 0.0;
				pvals[i + slots].real(0);
				pvals[i + slots].imag(-c);
			}
			EMBInv(pvals, dslots);
			for (i = 0, jdx = Nh, idx = 0; i < dslots; ++i, jdx += dgap, idx += dgap) {
				pvec[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
				pvec[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
			}
			bnd1 = maxBits(pvec, N);
			np = ceil((bnd1 + logQ + 2 * logN + 2)/59.0);
			rp1 = toNTT(pvec, np);
			for (i = 0; i < N; ++i) {
				pvec[i] = ZZ::zero();
			}

			for (i = 0; i < slots; ++i) {
				pvals[i] = c;
				pvals[i + slots] = 0;
			}

			EMBInv(pvals, dslots);
			for (i = 0, jdx = Nh, idx = 0; i < dslots; ++i, jdx += dgap, idx += dgap) {
				pvec[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
				pvec[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
			}
			bnd2 = maxBits(pvec, N);
			np = ceil((bnd2 + logQ + 2 * logN + 2)/59.0);
			rp2 = toNTT(pvec, np);
			for (i = 0; i < N; ++i) {
				pvec[i] = ZZ::zero();
			}

		} else {
			for (ki = 0; ki < slots; ki += k) {
				for (pos = ki; pos < ki + k; ++pos) {
					for (i = 0; i < slots - pos; ++i) {
						deg = ((M - rotGroup[i + pos]) * i * gap) % M;
						pvals[i] = ksiPows[deg];
					}
					for (i = slots - pos; i < slots; ++i) {
						deg = ((M - rotGroup[i + pos - slots]) * i * gap) % M;
						pvals[i] = ksiPows[deg];
					}
					EvaluatorUtils::rightRotateAndEqual(pvals, slots, ki);
					EMBInv(pvals, slots);
					for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
						pvec[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
						pvec[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
					}
					bndvec[pos] = maxBits(pvec, N);
					np = ceil((bndvec[pos] + logQ + 2 * logN + 2)/59.0);
					rpvec[pos] = toNTT(pvec, np);
					for (i = 0; i < N; ++i) {
						pvec[i] = ZZ::zero();
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
				EMBInv(pvals, slots);
				for (i = 0, jdx = Nh, idx = 0; i < slots;++i, jdx += gap, idx += gap) {
					pvec[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(), logp);
					pvec[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(), logp);
				}
				bndvecInv[pos] = maxBits(pvec, N);
				np = ceil((bndvecInv[pos] + logQ + 2 * logN + 2)/59.0);
				rpvecInv[pos] = toNTT(pvec, np);
				for (i = 0; i < N; ++i) {
					pvec[i] = ZZ::zero();
				}
			}
		}
		delete[] pvals;
		delete[] pvec;
		bootContextMap.insert(pair<long, BootContext>(logSlots,BootContext(rpvec, rpvecInv, rp1, rp2, bndvec, bndvecInv, bnd1, bnd2, logp)));
	}
}


//----------------------------------------------------------------------------------
//   MULTIPLICATION
//----------------------------------------------------------------------------------


long Ring::maxBits(const ZZ* f, long n) {
   long i, m;
   m = 0;

   for (i = 0; i < n; i++) {
      m = max(m, NumBits(f[i]));
   }
   return m;
}

uint64_t* Ring::toNTT(ZZ* x, long np) {
	return multiplier.toNTT(x, np);
}

void Ring::addNTTAndEqual(uint64_t* ra, uint64_t* rb, long np) {
	multiplier.addNTTAndEqual(ra, rb, np);
}

void Ring::mult(ZZ* x, ZZ* a, ZZ* b, long np, ZZ& q) {
	multiplier.mult(x, a, b, np, q);
}

void Ring::multNTT(ZZ* x, ZZ* a, uint64_t* rb, long np, ZZ& q) {
	multiplier.multNTT(x, a, rb, np, q);
}

void Ring::multDNTT(ZZ* x, uint64_t* ra, uint64_t* rb, long np, ZZ& q) {
	multiplier.multDNTT(x, ra, rb, np, q);
}

void Ring::multAndEqual(ZZ* a, ZZ* b, long np, ZZ& q) {
	multiplier.multAndEqual(a, b, np, q);
}

void Ring::multNTTAndEqual(ZZ* a, uint64_t* rb, long np, ZZ& q) {
	multiplier.multNTTAndEqual(a, rb, np, q);
}

void Ring::square(ZZ* x, ZZ* a, long np, ZZ& q) {
	multiplier.square(x, a, np, q);
}

void Ring::squareNTT(ZZ* x, uint64_t* ra, long np, ZZ& q) {
	multiplier.squareNTT(x, ra, np, q);
}

void Ring::squareAndEqual(ZZ* a, long np, ZZ& q) {
	multiplier.squareAndEqual(a, np, q);
}


//----------------------------------------------------------------------------------
//   OTHER
//----------------------------------------------------------------------------------


void Ring::mod(ZZ* res, ZZ* p, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		res[i] = p[i] % mod;
	}
}

void Ring::modAndEqual(ZZ* p, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		p[i] %= mod;
	}
}

void Ring::negate(ZZ* res, ZZ* p) {
	for (long i = 0; i < N; ++i) {
		res[i] = -p[i];
	}
}

void Ring::negateAndEqual(ZZ* p) {
	for (long i = 0; i < N; ++i) {
		p[i] = -p[i];
	}
}

void Ring::add(ZZ* res, ZZ* p1, ZZ* p2, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		AddMod(res[i], p1[i], p2[i], mod);
	}
}

void Ring::addAndEqual(ZZ* p1, ZZ* p2, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		AddMod(p1[i], p1[i], p2[i], mod);
	}
}

void Ring::sub(ZZ* res, ZZ* p1, ZZ* p2, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		AddMod(res[i], p1[i], -p2[i], mod);
	}
}

void Ring::subAndEqual(ZZ* p1, ZZ* p2, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		AddMod(p1[i], p1[i], -p2[i], mod);
	}
}

void Ring::subAndEqual2(ZZ* p1, ZZ* p2, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		AddMod(p2[i], p1[i], -p2[i], mod);
	}
}

void Ring::multByMonomial(ZZ* res, ZZ* p, long monomialDeg) {
	long shift = monomialDeg % M;
	if (shift == 0) {
		for (long i = 0; i < N; ++i) {
			res[i] = p[i];
		}
	} else {
		ZZ* tmpx = new ZZ[N];
		if (shift < N) {
			for (long i = 0; i < N; ++i) {
				tmpx[i] = p[i];
			}
		} else {
			for (long i = 0; i < N; ++i) {
				tmpx[i] = -p[i];
			}
		}

		shift %= N;

		for (long i = 0; i < shift; ++i) {
			res[i] = -tmpx[N - shift + i];
		}

		for (long i = shift; i < N; ++i) {
			res[i] = tmpx[i - shift];
		}
		delete[] tmpx;
	}
}

void Ring::multByMonomialAndEqual(ZZ* p, long monomialDeg) {
	long shift = monomialDeg % M;
	if (shift == 0) {
		return;
	}
	ZZ* tmpx = new ZZ[N];
	if (shift < N) {
		for (long i = 0; i < N; ++i) {
			tmpx[i] = p[i];
		}
	} else {
		for (long i = 0; i < N; ++i) {
			tmpx[i] = -p[i];
		}
	}

	shift %= N;

	for (long i = 0; i < shift; ++i) {
		p[i] = -tmpx[N - shift + i];
	}

	for (long i = shift; i < N; ++i) {
		p[i] = tmpx[i - shift];
	}

	delete[] tmpx;
}

void Ring::multByConst(ZZ* res, ZZ* p, ZZ& cnst, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		MulMod(res[i], p[i], cnst, mod);
	}
}

void Ring::multByConstAndEqual(ZZ* p, ZZ& cnst, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		MulMod(p[i], p[i], cnst, mod);
	}
}

void Ring::leftShift(ZZ* res, ZZ* p, long bits, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		res[i] = p[i] << bits;
		res[i] %= mod;
	}
}

void Ring::leftShiftAndEqual(ZZ* p, long bits, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		p[i] <<= bits;
		p[i] %= mod;
	}
}

void Ring::doubleAndEqual(ZZ* p, ZZ& mod) {
	for (long i = 0; i < N; ++i) {
		p[i] <<= 1;
		p[i] %= mod;
	}
}

void Ring::rightShift(ZZ* res, ZZ* p, long bits) {
	for (long i = 0; i < N; ++i) {
		res[i] = p[i] >> bits;
	}
}

void Ring::rightShiftAndEqual(ZZ* p, long bits) {
	for (long i = 0; i < N; ++i) {
		p[i] >>= bits;
	}
}


//----------------------------------------------------------------------------------
//   ROTATION & CONJUGATION
//----------------------------------------------------------------------------------


void Ring::leftRotate(ZZ* res, ZZ* p, long r) {
	long pow = rotGroup[r];
	for (long i = 0; i < N; ++i) {
		long ipow = i * pow;
		long shift = ipow % M;
		if (shift < N) {
			res[shift] = p[i];
		} else {
			res[shift - N] = -p[i];
		}
	}
}

void Ring::conjugate(ZZ* res, ZZ* p) {
	res[0] = p[0];
	for (long i = 1; i < N; ++i) {
		res[i] = -p[N - i];
	}
}


//----------------------------------------------------------------------------------
//   SAMPLING
//----------------------------------------------------------------------------------


void Ring::sampleGauss(ZZ* res) {
	static double Pi = 4.0 * atan(1.0);
	static long bignum = 0xfffffff;

	for (long i = 0; i < N; i+=2) {
		double r1 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double r2 = (1 + RandomBnd(bignum)) / ((double)bignum + 1);
		double theta=2 * Pi * r1;
		double rr= sqrt(-2.0 * log(r2)) * sigma;

		res[i] = (long) floor(rr * cos(theta) + 0.5);
		res[i + 1] = (long) floor(rr * sin(theta) + 0.5);
	}
}

void Ring::sampleHWT(ZZ* res) {
	long idx = 0;
	ZZ tmp = RandomBits_ZZ(h);
	while(idx < h) {
		long i = RandomBits_long(logN);
		if(res[i] == 0) {
			res[i] = (bit(tmp, idx) == 0) ? ZZ(1) : ZZ(-1);
			idx++;
		}
	}
}

void Ring::sampleZO(ZZ* res) {
	ZZ tmp = RandomBits_ZZ(M);
	for (long i = 0; i < N; ++i) {
		res[i] = (bit(tmp, 2 * i) == 0) ? ZZ(0) : (bit(tmp, 2 * i + 1) == 0) ? ZZ(1) : ZZ(-1);
	}
}

void Ring::sampleUniform2(ZZ* res, long bits) {
	for (long i = 0; i < N; i++) {
		res[i] = RandomBits_ZZ(bits);
	}
}


//----------------------------------------------------------------------------------
//   DFT
//----------------------------------------------------------------------------------


void Ring::DFT(complex<double>* vals, long n) {
	arrayBitReverse(vals, n);
	for (long len = 2; len <= n; len <<= 1) {
		long gap = M / len;
		long lenh = len >> 1;
		for (long i = 0; i < n; i += len) {
			for (long j = 0; j < lenh; ++j) {
				long idx = j * gap;
				complex<double> u = vals[i + j];
				complex<double> v = vals[i + j + lenh];
				v *= ksiPows[idx];
				vals[i + j] = u + v;
				vals[i + j + lenh] = u - v;
			}
		}
	}
}

void Ring::IDFTLazy(complex<double>* vals, long n) {
	arrayBitReverse(vals, n);
	for (long len = 2; len <= n; len <<= 1) {
		long gap = M / len;
		long lenh = len >> 1;
		for (long i = 0; i < n; i += len) {
			for (long j = 0; j < lenh; ++j) {
				long idx = (len - j) * gap;
				complex<double> u = vals[i + j];
				complex<double> v = vals[i + j + lenh];
				v *= ksiPows[idx];
				vals[i + j] = u + v;
				vals[i + j + lenh] = u - v;
			}
		}
	}
}

void Ring::IDFT(complex<double>* vals, long n) {
	IDFTLazy(vals, n);
	for (long i = 0; i < n; ++i) {
		vals[i] /= n;
	}
}
