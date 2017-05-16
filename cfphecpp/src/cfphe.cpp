#include "test/TestScheme.h"

int main() {

//	----------------------------

	/*
	 * Params: logN, logp, L, slots
	 * Suggested: 13, 30, 5, 12
	 */
//	TestScheme::testEncode(13, 30, 4, 12);

//	----------------------------

	/*
	 * Params: logN, logp, L
	 * Suggested: 13, 30, 5, 12
	 */
//	TestScheme::testOperations(13, 30, 4, 8);

//	----------------------------

	/*
	 * Params: logN, logp, L, logPowsDegree
	 * Suggested: 13, 30, 5, 4
	 * Suggested: 15, 56, 11, 10
	 */
//	TestScheme::testPow(13, 30, 11, 10);

//	----------------------------

	/*
	 * Params: logN, logp, L, logProdDegree
	 * Suggested: 13, 30, 5, 4
	 * Suggested: 15, 56, 11, 10
	 */
//	TestScheme::testProd(13, 30, 11, 10);

//	----------------------------

	/*
	 * Params: logN, logp, L, invSteps
	 * Suggested: 13, 25, 6, 5
	 */
	TestScheme::testInv(13, 30, 11, 6);

//	----------------------------

	/*
	 * Params: logN, logp, L, logFFTdim, FFTdeg
	 * Suggested: 13, 30, 3, 4, 5
	 */
//	TestScheme::testFFT(13, 30, 3, 4, 5);

//	----------------------------

	return 0;
}
