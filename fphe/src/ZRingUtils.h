#ifndef ZRINGUTILS_H_
#define ZRINGUTILS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <vector>

using namespace std;
using namespace NTL;

class ZRingUtils {
	public:
		static void addRing(vector<ZZ>& res, vector<ZZ>& fft1, vector<ZZ>& fft2, const ZZ& mod, const long& phim);
		static void subRing(vector<ZZ>& res, vector<ZZ>& fft1, vector<ZZ>& fft2, const ZZ& mod, const long& phim);
		static void mulRing(vector<ZZ>& res, vector<ZZ>& fft1, vector<ZZ>& fft2, const ZZ& mod, const long& phim);

		static void addConstantRing(vector<ZZ>& res, vector<ZZ>& fft, const ZZ& cnst, const ZZ& mod, const long& phim);
		static void mulByConstantRing(vector<ZZ>& res, vector<ZZ>& fft, const ZZ& cnst, const ZZ& mod, const long& phim);
		static void divByConstantRing(vector<ZZ>& res, vector<ZZ>& fft, const ZZ& cnst, const ZZ& mod, const long& phim);

		static void leftShiftRing(vector<ZZ>& res, vector<ZZ>& fft, const long& cnst, const ZZ& mod, const long& phim);
		static void rightShiftRing(vector<ZZ>& res, vector<ZZ>& fft, const long& cnst, const ZZ& mod, const long& phim);

		static void sampleGaussian(vector<ZZ>& res, const long deg, double stdev);
		static void sampleUniform(vector<ZZ>& res, const ZZ& B, const long deg);

		static void factorize(vector<long> &factors, const long &N);
		static long primitiveRoot(const long& p);

		static long mobius(long n);
		static ZZX Cyclotomic(long N);

		static void convertfft(vector<ZZ>& res, const vector<ZZ>& poly, const vector<vector<ZZ> > &fftMatrix, const ZZ& mod, const long& phim);
		static void convertfftInv(vector<ZZ>& res, const vector<ZZ>& fft, const vector<vector<ZZ> > &fftInvMatrix, const ZZ& mod, const long& phim);

};

void toString(vector<ZZ> v);

#endif /* ZRINGUTILS_H_ */
