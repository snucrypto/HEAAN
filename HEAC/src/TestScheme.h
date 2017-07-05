#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

class TestScheme {
public:

	//-----------------------------------------

	/**
	 * Testing encoding and decoding timing of the ciphertext
	 * c(m_1, ..., m_slots)
	 * number of levels switched: 0
	 * Tested on: 13, 2, 30, 4, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] log of number of slots
	 */
	static void testEncodeBatch(long logN, long logl, long logp, long L, long logSlots);

	//-----------------------------------------

	/**
	 * Testing conjugation timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(conjugate(m_1), ...,conjugate(m_slots))
	 * number of levels switched: 0
	 * Tested on: 13, 2, 30, 4, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] log of number of slots
	 */
	static void testConjugateBatch(long logN, long logl, long logp, long L, long logSlots);

	/**
	 * Testing left rotation timing in the ciphertext
	 * number of levels switched: 0
	 * c(m_1, ..., m_slots) -> c(m_(rotslots+1), m_(rotslots+2), ... m_(rotslots-1))
	 * Tested on: 13, 2, 30, 4, 2, 5
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] log of rotation number
	 * @param[in] log of number of slots
	 */
	static void testLeftRotate2Batch(long logN, long logl, long logp, long L, long rotlogSlots, long logSlots);

	/**
	 * Testing left rotation timing in the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_(rotslots+1), m_(rotslots+2), ... m_(rotslots-1))
	 * number of levels switched: 0
	 * Tested on: 13, 2, 30, 4, 17, 5
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] rotation number
	 * @param[in] log of number of slots
	 */
	static void testLeftRotateBatch(long logN, long logl, long logp, long L, long rotSlots, long logSlots);

	/**
	 * Testing slot summation timing in the ciphertext
	 * c(m_1, ..., m_slots) -> c(sum(m_i), sum(m_i), ..., sum(m_i))
	 * number of levels switched: 0
	 * Tested on: 13, 2, 30, 4, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] log of number of slots
	 */
	static void testSlotsSum(long logN, long logl, long logp, long L, long logSlots);

	//-----------------------------------------


	/**
	 * Testing power of 2 timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_1^degree/p^{degree-1}, ..., m_slots^degree/p^{degree-1})
	 * number of levels switched: logDegree
	 * Tested on: 13, 2, 30, 5, 4, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] log of power degree
	 * @param[in] log of number of slots
	 */
	static void testPowerOf2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots);

	//-----------------------------------------

	/**
	 * Testing power timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_1^degree/p^{degree-1}, ..., m_slots^degree/p^{degree-1})
	 * number of levels switched: ceil(log(degree))
	 * Tested on: 15, 4, 56, 11, 1023, 4
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] power degree
	 * @param[in] log of number of slots
	 */

	static void testPowerBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Testing product timing of ciphertexts
	 * array of c_i(m_1, ..., m_slots) -> c(prod_i(m_1), ..., prod_i(m_slots))
	 * number of levels switched: logDegree
	 * Tested on: 13, 4, 30, 5, 4, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] log of number of ciphertexts
	 * @param[in] log of number of slots
	 */

	static void testProd2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots);

	//-----------------------------------------


	/**
	 * Testing inverse timing of ciphertext
	 * c(m_1, ..., m_slots) -> c(p^2/m_1, ..., p^2/m_slots)
	 * number of levels switched: invSteps
	 * Tested on: 14, 3, 32, 10, 8, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] number of steps used in method for calculating inverse
	 * @param[in] log of number of slots
	 */
	static void testInverseBatch(long logN, long logl, long logp, long L, long invSteps, long logSlots);

	//-----------------------------------------

	/**
	 * Testing logarithm timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(log(m_1), ..., log(m_slots))
	 * number of levels switched: ceil(log(degree))
	 * Tested on: 13, 4, 35, 6, 10, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] degree of Taylor series approximation
	 * @param[in] log of number of slots
	 */
	static void testLogarithmBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Testing exponent timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(exp(m_1/p) * p, ..., exp(m_slots/p) * p)
	 * number of levels switched: ceil(log(degree))
	 * Tested on: 13, 4, 35, 6, 10, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] degree of Taylor series approximation
	 * @param[in] log of number of slots
	 */
	static void testExponentBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	/**
	 * Suggested: 13, 37, 35, 4, 8, 3
	 */
	/**
	 * Testing exponent timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(exp(m_1/p) * p, ..., exp(m_slots/p) * p)
	 * number of levels switched: ceil(log(degree)) - 1
	 * Tested on: 13, 37, 35, 4, 8, 3
	 * @param[in] logN input parameter for Params class
	 * @param[in] logl input parameter for Params class
	 * @param[in] logp input parameter for Params class
	 * @param[in] L input parameter for Params class
	 * @param[in] degree of Taylor series approximation
	 * @param[in] log of number of slots
	 */
	static void testExponentBatchLazy(long logN, long logl, long logp, long L, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Suggested: 13, 4, 35, 6, 10, 3
	 */
	static void testSigmoidBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	/**
	 * Suggested: 13, 37, 35, 4, 8, 3
	 */
	static void testSigmoidBatchLazy(long logN, long logl, long logp, long L, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Suggested: 13, 7, 50, 4, 4, 3
	 */
	static void testFFTBatch(long logN, long logl, long logp, long L, long logfftdim, long logSlots);

	/**
	 * Suggested: 13, 51, 50, 2, 4
	 */
	static void testFFTLazy(long logN, long logl, long logp, long L, long logfftdim);

	//-----------------------------------------
};

#endif /* TEST_TESTSCHEME_H_ */
