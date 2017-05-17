#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

#include <iostream>

using namespace std;

class TestScheme {
public:

	static void testEncode(long logN, long logp, long L, long logSlots);
	static void testOperations(long logN, long logp, long L, long logSlots);

	static void testPowerOf2(long logN, long logp, long L, long logPowDegree);
	static void testPowerOf2Extended(long logN, long logp, long L, long logPowDegree);

	static void testPower(long logN, long logp, long L, long powDegree);
	static void testPowerExtended(long logN, long logp, long L, long  powDegree);

	static void testProd2(long logN, long logp, long L, long logProdDegree);
	static void testProd2Extended(long logN, long logp, long L, long logProdDegree);

	static void testInverse(long logN, long logp, long L, long invSteps);
	static void testInverseExtended(long logN, long logp, long L, long invSteps);

	static void testExponent(long logN, long logp, long L, long expSteps, long logSlots);
	static void testExponentExtended(long logN, long logp, long L, long expSteps);

	static void testSigmoid(long logN, long logp, long L, long sigmoidSteps, long logSlots);
	static void testSigmoidExtended(long logN, long logp, long L, long sigmoidSteps);


	static void testFFT(long logN, long logp, long L, long logFFTdim, long FFTdeg);
};

#endif /* TEST_TESTSCHEME_H_ */
