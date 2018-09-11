/*
 * Copyright (c) by CryptoLab inc.
 * This program is licensed under a
 * Creative Commons Attribution-NonCommercial 3.0 Unported License.
 * You should have received a copy of the license along with this
 * work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
 */
#ifndef HEAAN_RINGMULTIPLIER_H_
#define HEAAN_RINGMULTIPLIER_H_

#include <cstdint>
#include <vector>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class RingMultiplier {
public:

	long logN;
	long N;

	uint64_t* pVec;
	uint64_t* prVec;
	long* pTwok;
	uint64_t* pInvVec;
	uint64_t** scaledRootPows;
	uint64_t** scaledRootInvPows;
	uint64_t* scaledNInv;
	_ntl_general_rem_one_struct** red_ss_array;
	mulmod_precon_t** coeffpinv_array;
	ZZ* pProd;
	ZZ* pProdh;
	ZZ** pHat;
	uint64_t** pHatInvModp;

	RingMultiplier(long logN, long logQ);

	bool primeTest(uint64_t p);

	void NTT(uint64_t* a, long index);
	void INTT(uint64_t* a, long index);

	uint64_t* toNTT(ZZ* x, long np);

	void addNTTAndEqual(uint64_t* ra, uint64_t* rb, long np);

	void reconstruct(ZZ* x, uint64_t* rx, long np, ZZ& mod);

	void mult(ZZ* x, ZZ* a, ZZ* b, long np, ZZ& mod);

	void multNTT(ZZ* x, ZZ* a, uint64_t* rb, long np, ZZ& mod);

	void multDNTT(ZZ* x, uint64_t* ra, uint64_t* rb, long np, ZZ& mod);

	void multAndEqual(ZZ* a, ZZ* b, long np, ZZ& mod);

	void multNTTAndEqual(ZZ* a, uint64_t* rb, long np, ZZ& mod);

	void square(ZZ* x, ZZ* a, long np, ZZ& mod);

	void squareNTT(ZZ* x, uint64_t* ra, long np, ZZ& mod);

	void squareAndEqual(ZZ* a, long np, ZZ& mod);

	void mulMod(uint64_t& r, uint64_t a, uint64_t b, uint64_t p);

	void mulModBarrett(uint64_t& r, uint64_t a, uint64_t b, uint64_t p, uint64_t pr, long twok);

	uint64_t invMod(uint64_t x, uint64_t p);

	uint64_t powMod(uint64_t x, uint64_t y, uint64_t p);

	uint64_t inv(uint64_t x);

	uint64_t pow(uint64_t x, uint64_t y);

	uint32_t bitReverse(uint32_t x);

	void findPrimeFactors(vector<uint64_t> &s, uint64_t number);

	uint64_t findPrimitiveRoot(uint64_t m);

	uint64_t findMthRootOfUnity(uint64_t M, uint64_t p);

};

#endif /* RINGMULTIPLIER_H_ */
