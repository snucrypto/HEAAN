/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_TESTSCHEME_H_
#define HEAAN_TESTSCHEME_H_

class TestScheme {
public:


	//----------------------------------------------------------------------------------
	//   STANDARD TESTS
	//----------------------------------------------------------------------------------
	
	/**
	 * Testing encoding and decoding timing of the ciphertext
	 * c(m_1, ..., m_slots)
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 */
	static void testEncodeBatch(long logN, long logQ, long logp, long logSlots);

	/**
	 * Testing encoding and decoding timing of the ciphertext of single value
	 * c(m_1)
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 */
	static void testEncodeSingle(long logN, long logQ, long logp);

	/**
	 * Testing encoding, decoding, add, and mult timing of the ciphertext
	 * c(m_1, ..., m_slots)
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 */
	static void testBasic(long logN, long logQ, long logp, long logSlot);

	/**
	 * Testing conjugation timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(conjugate(m_1), ...,conjugate(m_slots))
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 */
	static void testConjugateBatch(long logN, long logQ, long logp, long logSlots);

	/**
	 * Testing multiplication by i (imaginary 1) timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(i * m_1, ...,i * m_slots)
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 */
	static void testimultBatch(long logN, long logQ, long logp, long logSlots);

	/**
	 * Testing left rotation timing in the ciphertext
	 * number of modulus bits down: 0
	 * c(m_1, ..., m_slots) -> c(m_(rotslots+1), m_(rotslots+2), ... m_(rotslots-1))
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logRotSlots: log of number of rotation slots
	 * @param[in] logSlots: log of number of slots
	 * @param[in] isLeft: switching between left and right rotations
	 */
	static void testRotateByPo2Batch(long logN, long logQ, long logp, long logRotSlots, long logSlots, bool isLeft);

	/**
	 * Testing left rotation timing in the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_(rotslots+1), m_(rotslots+2), ... m_(rotslots-1))
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] rotSlots: number of rotation slots
	 * @param[in] logSlots: log of number of slots
	 * @param[in] isLeft: switching between left and right rotations
	 */
	static void testRotateBatch(long logN, long logQ, long logp, long rotSlots, long logSlots, bool isLeft);

	/**
	 * Testing slot summation timing in the ciphertext
	 * c(m_1, ..., m_slots) -> c(sum(m_i), sum(m_i), ..., sum(m_i))
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 */
	static void testSlotsSum(long logN, long logQ, long logp, long logSlots);


	//----------------------------------------------------------------------------------
	//   POWER & PRODUCT TESTS
	//----------------------------------------------------------------------------------


	/**
	 * Testing power of 2 timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_1^degree/p^{degree-1}, ..., m_slots^degree/p^{degree-1})
	 * number of modulus bits down: logDegree * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logDegree: log of degree
	 * @param[in] logSlots: log of number of slots
	 */
	static void testPowerOf2Batch(long logN, long logQ, long logp, long logDegree, long logSlots);

	/**
	 * Testing power timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_1^degree/p^{degree-1}, ..., m_slots^degree/p^{degree-1})
	 * number of modulus bits down: ceil(log(degree)) * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] degree: power degree
	 * @param[in] logSlots: log of number of slots
	 */
	static void testPowerBatch(long logN, long logQ, long logp, long degree, long logSlots);

	/**
	 * Testing product timing of ciphertexts
	 * array of c_i(m_1, ..., m_slots) -> c(prod_i(m_1), ..., prod_i(m_slots))
	 * number of modulus bits down: logDegree * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logDegree: log of number of ciphertexts
	 * @param[in] logSlots: log of number of slots
	 */
	static void testProdOfPo2Batch(long logN, long logQ, long logp, long logDegree, long logSlots);

	/**
	 * Testing product timing of ciphertexts
	 * array of c_i(m_1, ..., m_slots) -> c(prod_i(m_1), ..., prod_i(m_slots))
	 * number of modulus bits down: ceil(log(degree)) * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] degree: number of ciphertexts
	 * @param[in] logSlots: log of number of slots
	 */
	static void testProdBatch(long logN, long logQ, long logp, long degree, long logSlots);


	//----------------------------------------------------------------------------------
	//   FUNCTION TESTS
	//----------------------------------------------------------------------------------


	/**
	 * Testing inverse timing of ciphertext
	 * c(m_1, ..., m_slots) -> c(p^2/m_1, ..., p^2/m_slots)
	 * number of modulus bits down: invSteps * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] invSteps: number of inverse steps
	 * @param[in] logSlots: log of number of slots
	 */
	static void testInverseBatch(long logN, long logQ, long logp, long invSteps, long logSlots);

	/**
	 * Testing logarithm timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(log(m_1), ..., log(m_slots))
	 * number of modulus bits down: ceil(log(degree)) * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] degree: Taylor approximation polynomial degree
	 * @param[in] logSlots: log of number of slots
	 */
	static void testLogarithmBatch(long logN, long logQ, long logp, long degree, long logSlots);

	/**
	 * Testing exponent timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(exp(m_1/p) * p, ..., exp(m_slots/p) * p)
	 * number of modulus bits down: ceil(log(degree)) * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] degree: Taylor approximation polynomial degree
	 * @param[in] logSlots: log of number of slots
	 */
	static void testExponentBatch(long logN, long logQ, long logp, long degree, long logSlots);

	/**
	 * Testing exponent timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(exp(m_1/p) * p^2, ..., exp(m_slots/p) * p^2)
	 * number of modulus bits down: (ceil(log(degree)) - 1) * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] degree: Taylor approximation polynomial degree
	 * @param[in] logSlots: log of number of slots
	 */
	static void testExponentBatchLazy(long logN, long logQ, long logp, long degree, long logSlots);

	/**
	 * Testing sigmoid timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(sigmoid(m_1/p) * p, ..., sigmoid(m_slots/p) * p)
	 * number of modulus bits down: ceil(log(degree)) * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] degree: Taylor approximation polynomial degree
	 * @param[in] logSlots: log of number of slots
	 */
	static void testSigmoidBatch(long logN, long logQ, long logp, long degree, long logSlots);

	/**
	 * Testing sigmoid timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(sigmoid(m_1/p) * p^2, ..., sigmoid(m_slots/p) * p^2)
	 * number of modulus bits down: (ceil(log(degree)) - 1) * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] degree: Taylor approximation polynomial degree
	 * @param[in] logSlots: log of number of slots
	 */
	static void testSigmoidBatchLazy(long logN, long logQ, long logp, long degree, long logSlots);


	//----------------------------------------------------------------------------------
	//   FFT TESTS
	//----------------------------------------------------------------------------------


	/**
	 * Testing full fft pipeline timing of two ciphertext arrays
	 * fftinv( mult ( fft(c_1, ..., c_slots), fft(c_1, ..., c_slots) ) )
	 * number of modulus bits: 2 * logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logfftdim: log of fft dimension
	 * @param[in] logSlots: log of number of slots
	 */
	static void testFFTBatch(long logN, long logQ, long logp, long logfftdim, long logSlots);

	/**
	 * Testing full fft pipeline timing of two ciphertext arrays
	 * fftinvlazy( mult ( fft(c_1, ..., c_slots), fft(c_1, ..., c_slots) ) )
	 * number of modulus bits down: logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logfftdim: log of fft dimension
	 * @param[in] logSlots: log of number of slots
	 */
	static void testFFTBatchLazy(long logN, long logQ, long logp, long logSlots, long logfftdim);

	/**
	 * Testing full fft pipeline timing of several ciphertext arrays with multiple hadamard multipications
	 * fftinvlazy( mult ( fft(c_1, ..., c_slots), fft(c_1, ..., c_slots) ) )
	 * number of modulus bits down: logp
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 * @param[in] logfftdim: log of fft dimension
	 * @param[in] logHdim: log of number of Hadamard multiplications
	 */
	static void testFFTBatchLazyMultipleHadamard(long logN, long logQ, long logp, long logSlots, long logfftdim, long logHdim);

	/**
	 * Testing Write and Read functions for Ciphertext
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 */
	static void testWriteAndRead(long logN, long logQ, long logp, long logSlots);


	//----------------------------------------------------------------------------------
	//   BOOTSTRAPPING TESTS
	//----------------------------------------------------------------------------------


	/**
	 * Testing bootstrapping procedure
	 * number of modulus bits up: depends on parameters
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logq: log of initial modulus
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logSlots: log of number of slots
	 * @param[in] nu: auxiliary parameter, corresonds to message bits (message bits is logq - nu)
	 * @param[in] logT: auxiliary parameter, corresponds to number of iterations in removeIpart (num of iterations is logI + logT)
	 */
	static void testBootstrap(long logN, long logq, long logQ, long logSlots, long nu, long logT);

	/**
	 * Testing bootstrapping procedure for single real value
	 * number of modulus bits up: depends on parameters
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logq: log of initial modulus
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] nu: auxiliary parameter, corresonds to message bits (message bits is logq - nu)
	 * @param[in] logT: auxiliary parameter, corresponds to number of iterations in removeIpart (num of iterations is logI + logT)
	 */
	static void testBootstrapSingleReal(long logN, long logq, long logQ, long nu, long logT);

};

#endif
