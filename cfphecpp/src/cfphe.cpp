#include "scheme/Params.h"
#include "scheme/PubKey.h"
#include "scheme/Scheme.h"
#include "scheme/SchemeAlgo.h"
#include "scheme/SecKey.h"
#include "test/TestScheme.h"
#include "utils/TimeUtils.h"

void test() {
	//-----------------------------------------
	TimeUtils timeutils;
	long logN = 13;
	long logl = 3;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

}

int main() {

//	test();

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logSlots
	 * Suggested: 13, 2, 30, 5, 12
	 */
//	TestScheme::testEncode(13, 2, 30, 4, 12);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logPowsDegree
	 * Suggested: 13, 2, 30, 5, 4
	 * Suggested: 15, 2, 56, 11, 10
	 */

//	TestScheme::testPowerOf2(13, 30, 5, 4);
//	TestScheme::testPowerOf2Extended(15, 56, 11, 10);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, powDegree
	 * Suggested: 13, 2, 30, 5, 13
	 * Suggested: 15, 2, 56, 11, 903
	 */

//	TestScheme::testPowerExtended(13, 30, 5, 13);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logProdDegree
	 * Suggested: 13, 2, 30, 5, 4
	 * Suggested: 15, 2, 56, 11, 10
	 */
//	TestScheme::testProd2(13, 4, 30, 5, 4);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, invSteps
	 * Suggested: 13, 3, 25, 10, 8
	 * Suggested: 14, 3, 32, 10, 8
	 */

	TestScheme::testInverseBatch(13, 3, 25, 10, 8, 3);
//	TestScheme::testInverseExtended(13, 3, 25, 10, 8);
	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, expSteps, logSlots
	 * Suggested: 13, 2, 35, 5, 8, 3
	 * Suggested: 13, 37, 35, 4, 8, 3 (for simple)
	 */
//	TestScheme::testExponentBatch(13, 2, 35, 5, 8, 3);
//	TestScheme::testExponentSimpleBatch(13, 37, 35, 4, 8, 3);
//	TestScheme::testExponentExtended(13, 2, 25, 6, 10);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, sigmoidSteps, logSlots
	 * Suggested: 13, 1, 35, 5, 7, 3
	 * Suggested: 13, 36, 35, 4, 7, 3 (for simple)
	 */

//	TestScheme::testSigmoidBatch(13, 1, 35, 5, 7, 3);
//	TestScheme::testSigmoidSimpleBatch(13, 36, 35, 4, 7, 3);
//	TestScheme::testSigmoidExtended(13, 1, 35, 5, 7);

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, logFFTdim, FFTdeg
	 * Suggested: 14, 7, 50, 3, 4, 5
	 * Suggested: 14, 7, 50, 2, 4, 5 (for simple)
	 */
//	TestScheme::testFFT(14, 1, 30, 3, 13, 5);
//	TestScheme::testFFTsimple(13, 7, 50, 2, 4, 5);

	//-----------------------------------------

	return 0;
}
