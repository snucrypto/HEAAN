#ifndef ZRINGUTILS_H_
#define ZRINGUTILS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

using namespace NTL;

class ZRingUtils {
	public:
		static void addRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi);
		static void subRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi);
		static void mulRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi);
		static void mulByConstantRing(ZZX& res, ZZX& poly, ZZ& cnst, const ZZ& mod, const ZZX& phi);
		static void bitPoly(ZZX& res, ZZX& poly, long i);
		static void sampleGaussian(ZZX& res, long deg, double stdev);
		static void sampleUniform(ZZX& res, ZZ& B, long deg);
		static long mobius(long n);
		static ZZX Cyclotomic(long N);
};

#endif /* ZRINGUTILS_H_ */
