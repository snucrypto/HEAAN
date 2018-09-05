/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_RING2X_H_
#define HEAAN_RING2X_H_

#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <complex>
#include <map>
#include "BootContext.h"
#include "RingMultiplier.h"

using namespace std;
using namespace NTL;

static string LOGARITHM = "Logarithm"; ///< log(x)
static string EXPONENT  = "Exponent"; ///< exp(x)
static string SIGMOID   = "Sigmoid"; ///< sigmoid(x) = exp(x) / (1 + exp(x))

static RR Pi = ComputePi_RR();

class Ring {

public:

	long logN;
	long logM;
	long logNh;

	long N;
	long M;
	long Nh;

	long logQ; ///< log of Q
	double sigma; ///< standard deviation for Gaussian distribution
	long h; ///< parameter for HWT distribution

	long logQQ; ///< log of PQ

	ZZ Q; ///< Q corresponds to the highest modulus
	ZZ QQ; ///< PQ = Q * Q

	ZZ* qpows;

	long* rotGroup; ///< auxiliary information about rotation group indexes for batch encoding
	complex<double>* ksiPows; ///< storing ksi pows for fft calculation

	map<string, double*> taylorCoeffsMap;

	map<long, BootContext> bootContextMap;

	RingMultiplier multiplier;

	Ring(long logN, long logQ, double sigma = 3.2, long h = 64);


	//----------------------------------------------------------------------------------
	//   Encode & Decode
	//----------------------------------------------------------------------------------


	void arrayBitReverse(complex<double>* vals, long size);

	void EMB(complex<double>* vals, long size);

	void EMBInvLazy(complex<double>* vals, long size);

	void EMBInv(complex<double>* vals, long size);

	void encode(ZZ* mx, double* vals, long slots, long logp);

	void encode(ZZ* mx, complex<double>* vals, long slots, long logp);

	void decode(ZZ* mx, complex<double>* vals, long slots, long logp, long logq);


	//----------------------------------------------------------------------------------
	//   CONTEXT
	//----------------------------------------------------------------------------------


	void addBootContext(long logSlots, long logp);


	//----------------------------------------------------------------------------------
	//   MULTIPLICATION
	//----------------------------------------------------------------------------------

	long maxBits(const ZZ* f, long n);

	uint64_t* toNTT(ZZ* x, long np);

	void addNTTAndEqual(uint64_t* ra, uint64_t* rb, long np);

	void mult(ZZ* x, ZZ* a, ZZ* b, long np, ZZ& q);

	void multNTT(ZZ* x, ZZ* a, uint64_t* rb, long np, ZZ& q);

	void multDNTT(ZZ* x, uint64_t* a, uint64_t* rb, long np, ZZ& q);

	void multAndEqual(ZZ* a, ZZ* b, long np, ZZ& q);

	void multNTTAndEqual(ZZ* a, uint64_t* rb, long np, ZZ& q);

	void square(ZZ* x, ZZ* a, long np, ZZ& q);

	void squareNTT(ZZ* x, uint64_t* ra, long np, ZZ& q);

	void squareAndEqual(ZZ* a, long np, ZZ& q);


	//----------------------------------------------------------------------------------
	//   OTHER
	//----------------------------------------------------------------------------------


	void mod(ZZ* res, ZZ* p, ZZ& mod);

	void modAndEqual(ZZ* p, ZZ& mod);

	void negate(ZZ* res, ZZ* p);

	void negateAndEqual(ZZ* p);

	void add(ZZ* res, ZZ* p1, ZZ* p2, ZZ& mod);

	void addAndEqual(ZZ* p1, ZZ* p2, ZZ& mod);

	void sub(ZZ* res, ZZ* p1, ZZ* p2, ZZ& mod);

	void subAndEqual(ZZ* p1, ZZ* p2, ZZ& mod);

	void subAndEqual2(ZZ* p1, ZZ* p2, ZZ& mod);

	void multByMonomial(ZZ* res, ZZ* p, long mDeg);

	void multByMonomialAndEqual(ZZ* p, long mDeg);

	void multByConst(ZZ* res, ZZ* p, ZZ& cnst, ZZ& mod);

	void multByConstAndEqual(ZZ* p, ZZ& cnst, ZZ& mod);

	void leftShift(ZZ* res, ZZ* p, long bits, ZZ& mod);

	void leftShiftAndEqual(ZZ* p, long bits, ZZ& mod);

	void doubleAndEqual(ZZ* p, ZZ& mod);

	void rightShift(ZZ* res, ZZ* p, long bits);

	void rightShiftAndEqual(ZZ* p, long bits);


	//----------------------------------------------------------------------------------
	//   ROTATION & CONJUGATION
	//----------------------------------------------------------------------------------


	void leftRotate(ZZ* res, ZZ* p, long r);

	void conjugate(ZZ* res, ZZ* p);


	//----------------------------------------------------------------------------------
	//   SAMPLING
	//----------------------------------------------------------------------------------


	void sampleGauss(ZZ* res);

	void sampleHWT(ZZ* res);

	void sampleZO(ZZ* res);

	void sampleUniform2(ZZ* res, long bits);


	//----------------------------------------------------------------------------------
	//   DFT
	//----------------------------------------------------------------------------------


	void DFT(complex<double>* vals, long n);

	void IDFTLazy(complex<double>* vals, long n);

	void IDFT(complex<double>* vals, long n);

};

#endif
