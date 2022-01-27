/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_RING_H_
#define HEAAN_RING_H_

#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <complex>
#include <map>
#include "BootContext.h"
#include "RingMultiplier.h"

namespace heaan {

static NTL::RR Pi = NTL::ComputePi_RR();

class Ring {

public:

	NTL::ZZ* qpows;
	long* rotGroup;
	std::complex<double>* ksiPows;
	std::map<long, BootContext*> bootContextMap;
	RingMultiplier multiplier;

	Ring();


	//----------------------------------------------------------------------------------
	//   Encode & Decode
	//----------------------------------------------------------------------------------


	void arrayBitReverse(std::complex<double>* vals, long size);

	void EMB(std::complex<double>* vals, long size);

	void EMBInvLazy(std::complex<double>* vals, long size);

	void EMBInv(std::complex<double>* vals, long size);

	void encode(NTL::ZZ* mx, double* vals, long slots, long logp);

	void encode(NTL::ZZ* mx, std::complex<double>* vals, long slots, long logp);

	void decode(NTL::ZZ* mx, std::complex<double>* vals, long slots, long logp, long logq);


	//----------------------------------------------------------------------------------
	//   CONTEXT
	//----------------------------------------------------------------------------------


	void addBootContext(long logSlots, long logp);


	//----------------------------------------------------------------------------------
	//   MULTIPLICATION
	//----------------------------------------------------------------------------------

	long maxBits(const NTL::ZZ* f, long n);

	void CRT(uint64_t* rx, NTL::ZZ* x, const long np);

	void addNTTAndEqual(uint64_t* ra, uint64_t* rb, const long np);

	void mult(NTL::ZZ* x, NTL::ZZ* a, NTL::ZZ* b, long np, const NTL::ZZ& q);

	void multNTT(NTL::ZZ* x, NTL::ZZ* a, uint64_t* rb, long np, const NTL::ZZ& q);

	void multDNTT(NTL::ZZ* x, uint64_t* a, uint64_t* rb, long np, const NTL::ZZ& q);

	void multAndEqual(NTL::ZZ* a, NTL::ZZ* b, long np, const NTL::ZZ& q);

	void multNTTAndEqual(NTL::ZZ* a, uint64_t* rb, long np, const NTL::ZZ& q);

	void square(NTL::ZZ* x, NTL::ZZ* a, long np, const NTL::ZZ& q);

	void squareNTT(NTL::ZZ* x, uint64_t* ra, long np, const NTL::ZZ& q);

	void squareAndEqual(NTL::ZZ* a, long np, const NTL::ZZ& q);


	//----------------------------------------------------------------------------------
	//   OTHER
	//----------------------------------------------------------------------------------


	void mod(NTL::ZZ* res, NTL::ZZ* p, const NTL::ZZ& QQ);

	void modAndEqual(NTL::ZZ* p, const NTL::ZZ& QQ);

	void negate(NTL::ZZ* res, NTL::ZZ* p);

	void negateAndEqual(NTL::ZZ* p);

	void add(NTL::ZZ* res, NTL::ZZ* p1, NTL::ZZ* p2, const NTL::ZZ& QQ);

	void addAndEqual(NTL::ZZ* p1, NTL::ZZ* p2, const NTL::ZZ& QQ);

	void sub(NTL::ZZ* res, NTL::ZZ* p1, NTL::ZZ* p2, const NTL::ZZ& QQ);

	void subAndEqual(NTL::ZZ* p1, NTL::ZZ* p2, const NTL::ZZ& QQ);

	void subAndEqual2(NTL::ZZ* p1, NTL::ZZ* p2, const NTL::ZZ& QQ);

	void multByMonomial(NTL::ZZ* res, NTL::ZZ* p, long mDeg);

	void multByMonomialAndEqual(NTL::ZZ* p, long mDeg);

	void multByConst(NTL::ZZ* res, NTL::ZZ* p, NTL::ZZ& cnst, const NTL::ZZ& QQ);

	void multByConstAndEqual(NTL::ZZ* p, NTL::ZZ& cnst, const NTL::ZZ& QQ);

	void leftShift(NTL::ZZ* res, NTL::ZZ* p, const long bits, const NTL::ZZ& QQ);

	void leftShiftAndEqual(NTL::ZZ* p, const long bits, const NTL::ZZ& QQ);

	void doubleAndEqual(NTL::ZZ* p, const NTL::ZZ& QQ);

	void rightShift(NTL::ZZ* res, NTL::ZZ* p, long bits);

	void rightShiftAndEqual(NTL::ZZ* p, long bits);


	//----------------------------------------------------------------------------------
	//   ROTATION & CONJUGATION
	//----------------------------------------------------------------------------------


	void leftRotate(NTL::ZZ* res, NTL::ZZ* p, long r);

	void conjugate(NTL::ZZ* res, NTL::ZZ* p);


	//----------------------------------------------------------------------------------
	//   SAMPLING
	//----------------------------------------------------------------------------------


	void subFromGaussAndEqual(NTL::ZZ* res, const NTL::ZZ& q);

	void subFromGaussAndEqual(NTL::ZZ* res, const NTL::ZZ& q, double _sigma);

	void addGaussAndEqual(NTL::ZZ* res, const NTL::ZZ& q);

	void addGaussAndEqual(NTL::ZZ* res, const NTL::ZZ& q, double _sigma);

	void sampleHWT(NTL::ZZ* res);

	void sampleZO(NTL::ZZ* res);

	void sampleUniform2(NTL::ZZ* res, long bits);


	//----------------------------------------------------------------------------------
	//   DFT
	//----------------------------------------------------------------------------------


	void DFT(std::complex<double>* vals, long n);

	void IDFTLazy(std::complex<double>* vals, long n);

	void IDFT(std::complex<double>* vals, long n);

};

}  // namespace heaan

#endif
