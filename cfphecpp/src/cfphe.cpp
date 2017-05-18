#include "scheme/Params.h"
#include "scheme/PubKey.h"
#include "scheme/Scheme.h"
#include "scheme/SchemeAlgo.h"
#include "scheme/SecKey.h"
#include "test/TestScheme.h"
#include "utils/TimeUtils.h"

void test() {
	//----------------------------
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
	//----------------------------

}

int main() {

//	test();

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, logSlots
	 * Suggested: 13, 30, 5, 12
	 */
//	TestScheme::testEncode(13, 30, 4, 12);

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, logSlots
	 * Suggested: 13, 30, 5, 12
	 */
//	TestScheme::testOperations(13, 30, 4, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, logPowsDegree
	 * Suggested: 13, 30, 5, 4
	 * Suggested: 15, 56, 11, 10
	 */

//	TestScheme::testPowerOf2(13, 30, 5, 4);
//	TestScheme::testPowerOf2Extended(15, 56, 11, 10);

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, powDegree
	 * Suggested: 13, 30, 5, 13
	 * Suggested: 15, 56, 11, 903
	 */

//	TestScheme::testPowerExtended(13, 30, 5, 13);

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, logProdDegree
	 * Suggested: 13, 30, 5, 4
	 * Suggested: 15, 56, 11, 10
	 */
//	TestScheme::testProd2Extended(15, 56, 5, 4);

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, invSteps
	 * Suggested: 13, 25, 6, 5
	 * Suggested: 14, 32, 6, 5
	 */

//	TestScheme::testInverse(13, 25, 6, 5);
//	TestScheme::testInverse(13, 25, 6, 5);
//	TestScheme::testInverseExtended(13, 25, 6, 5);
//	TestScheme::testInverseExtended(13, 25, 6, 5);
	//-----------------------------------------

	/*
	 * Params: logN, logp, L, expSteps, logSlots
	 * Suggested: 13, 25, 6, 10, 5
	 */
//	TestScheme::testExponent(13, 25, 6, 10, 5);
//	TestScheme::testExponentSimple(13, 25, 6, 10, 5);
//	TestScheme::testExponentExtended(13, 25, 6, 10);

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, sigmoidSteps, logSlots
	 * Suggested: 13, 25, 6, 10, 5
	 */
//	TestScheme::testSigmoid(13, 25, 6, 10, 5);
//	TestScheme::testSigmoidSimple(13, 25, 6, 10, 5);
//	TestScheme::testSigmoidExtended(13, 25, 6, 10);

	//-----------------------------------------

	/*
	 * Params: logN, logp, L, logFFTdim, FFTdeg
	 * Suggested: 13, 30, 3, 4, 5
	 * Suggested: 14, 30, 3, 13, 5
	 */
	TestScheme::testFFT(14, 30, 3, 7, 10);
//	TestScheme::testFFTsimple(14, 30, 3, 13, 5);

	//-----------------------------------------

	return 0;
}
