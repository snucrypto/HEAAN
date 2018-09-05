/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_CONTEXT_H_
#define HEAAN_CONTEXT_H_

#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <complex>

#include "BootContext.h"
#include "Common.h"

using namespace std;
using namespace NTL;

static RR const Pi = ComputePi_RR();

static string LOGARITHM = "Logarithm"; ///< log(x)
static string EXPONENT  = "Exponent"; ///< exp(x)
static string SIGMOID   = "Sigmoid"; ///< sigmoid(x) = exp(x) / (1 + exp(x))

class Context {
public:

	long logN; ///< log of N
	long logQ; ///< log of Q
	double sigma; ///< standard deviation for Gaussian distribution
	long h; ///< parameter for HWT distribution

	long N; ///< N is a power-of-two that corresponds to the ring Z[X]/(X^N + 1)
	long Nh; ///< Nh = N/2
	long logNh; ///< logNh = logN - 1
	long M; ///< M = 2N
	long logQQ; ///< log of PQ

	ZZ Q; ///< Q corresponds to the highest modulus
	ZZ QQ; ///< PQ = Q * Q

	long* rotGroup; ///< precomputed rotation group indexes
	complex<double>* ksiPows; ///< precomputed ksi powers
	ZZ* qpowvec; ///< precomputed powers of 2

	map<string, double*> taylorCoeffsMap; ///< precomputed taylor coefficients

	map<long, BootContext> bootContextMap; ///< precomputed bootstrapping auxiliary information

	Context(long logN, long logQ, double sigma = 3.2, long h = 64);

	Context(const Context& o);

	void init(long logN, long logQ, double sigma, long h);

	virtual ~Context();


	//----------------------------------------------------------------------------------
	//   ENCODINGS & BOOTSTRAPPING
	//----------------------------------------------------------------------------------


	/**
	 * encoding of values to polynomial
	 * @param[in] vals: array of values
	 * @param[in] slots: size of array
	 * @param[in] logp: number of quantized bits
	 */
	ZZX encode(complex<double>* vals, long slots, long logp);

	/**
	 * encoding a value to polynomial
	 * @param[in] val: value
	 * @param[in] logp: number of quantized bits
	 */
	ZZX encodeSingle(complex<double> val, long logp);

	/**
	 * encoding of values to polynomial
	 * @param[in] vals: array of values
	 * @param[in] slots: size of array
	 * @param[in] logp: number of quantized bits
	 */
	ZZX encode(double* vals, long slots, long logp);

	/**
	 * encoding a value to polynomial
	 * @param[in] val: value
	 * @param[in] logp: number of quantized bits
	 */
	ZZX encodeSingle(double val, long logp);

	/**
	 * decoding values from a polynomial
	 * @param[in] mx: polynomial
	 * @param[in] slots: size of array
	 * @param[in] logp: number of quantized bits
	 * @param[in] logq: number of modulus bits
	 */
	complex<double>* decode(ZZX& mx, long slots, long logp, long logq);

	/**
	 * decoding value from a polynomial
	 * @param[in] mx: polynomial
	 * @param[in] logp: number of quantized bits
	 * @param[in] logq: number of modulus bits
	 */
	complex<double> decodeSingle(ZZX& mx, long logp, long logq, bool isComplex = true);


	/**
	 * adding information for Bootstrapping
	 * @param[in] logl: log of slots
	 * @param[in] logp: log of precision
	 */
	void addBootContext(long logSlots, long logp);


	//----------------------------------------------------------------------------------
	//   FFT & FFT INVERSE
	//----------------------------------------------------------------------------------


	/**
	 * reverse bits for fft calculations
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 */
	void bitReverse(complex<double>* vals, const long size);

	/**
	 * calculates fft in Z_q[X] / (X^N + 1)
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 */
	void fft(complex<double>* vals, const long size);

	/**
	 * calculates fft inverse lazy in Z_q[X] / (X^N + 1)
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 */
	void fftInvLazy(complex<double>* vals, const long size);

	/**
	 * calculates fft inverse in Z_q[X] / (X^N + 1)
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 */
	void fftInv(complex<double>* vals, const long size);

	/**
	 * calculates special fft in Z_q[X] / (X^N + 1) for encoding/decoding
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 */
	void fftSpecial(complex<double>* vals, const long size);

	/**
	 * calculates special fft inverse lazy in Z_q[X] / (X^N + 1) for encoding/decoding
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 */
	void fftSpecialInvLazy(complex<double>* vals, const long size);

	/**
	 * calculates special fft inverse in Z_q[X] / (X^N + 1) for encoding/decoding
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 */
	void fftSpecialInv(complex<double>* vals, const long size);

};

#endif /* CONTEXT_H_ */
