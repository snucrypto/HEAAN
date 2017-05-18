#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

#include <iostream>

using namespace std;

class TestScheme {
public:

	//-----------------------------------------

	static void testEncodeBatch(long logN, long logl, long logp, long L, long logSlots);

	//-----------------------------------------

	static void testPowerOf2(long logN, long logl, long logp, long L, long logPowDegree);
	static void testPowerOf2Extended(long logN, long logl, long logp, long L, long logPowDegree);

	//-----------------------------------------

	static void testPower(long logN, long logl, long logp, long L, long powDegree);
	static void testPowerExtended(long logN, long logl, long logp, long L, long  powDegree);

	//-----------------------------------------

	static void testProd2(long logN, long logl, long logp, long L, long logProdDegree);
	static void testProd2Extended(long logN, long logl, long logp, long L, long logProdDegree);

	//-----------------------------------------

	static void testInverse(long logN, long logl, long logp, long L, long invSteps);
	static void testInverseExtended(long logN, long logl, long logp, long L, long invSteps);

	//-----------------------------------------

	static void testExponentBatch(long logN, long logl, long logp, long L, long expSteps, long logSlots);
	static void testExponentSimpleBatch(long logN, long logl, long logp, long L, long expSteps, long logSlots);
	static void testExponentExtended(long logN, long logl, long logp, long L, long expSteps);

	//-----------------------------------------

	static void testSigmoidBatch(long logN, long logl, long logp, long L, long sigmoidSteps, long logSlots);
	static void testSigmoidSimpleBatch(long logN, long logl, long logp, long L, long sigmoidSteps, long logSlots);
	static void testSigmoidExtended(long logN, long logl, long logp, long L, long sigmoidSteps);

	//-----------------------------------------

	static void testFFT(long logN, long logl, long logp, long L, long logFFTdim, long FFTdeg);
	static void testFFTsimple(long logN, long logl, long logp, long L, long logFFTdim, long FFTdeg);
};

#endif /* TEST_TESTSCHEME_H_ */
