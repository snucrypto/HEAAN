#ifndef POLYSCHEME_ZRINGUTILS_H_
#define POLYSCHEME_ZRINGUTILS_H_

#include <NTL/ZZ.h>
#include "../czz/CZZ.h"
#include "../czz/CZZX.h"

using namespace NTL;

class CPolyRingUtils {
	public:
		static void addPolyRing(CZZX& res, CZZX& poly1, CZZX& poly2, const ZZ& mod, const long& deg);
		static void subPolyRing(CZZX& res, CZZX& poly1, CZZX& poly2, const ZZ& mod, const long& deg);
		static void mulPolyRing(CZZX& res, CZZX& poly1, CZZX& poly2, const ZZ& mod, const long& deg);

		static void mulPolyByConstantRing(CZZX& res, CZZX& poly, const CZZ& cnst, const ZZ& mod, const long& deg);

		static void leftShiftPolyRing(CZZX& res, CZZX& poly, const long& cnst, const ZZ& mod, const long& deg);
		static void rightShiftPolyRing(CZZX& res, CZZX& poly, const long& cnst, const ZZ& mod, const long& deg);

		static void sampleGaussian(ZZX& res, long deg, double stdev);
		static void sampleGaussian(CZZX& res, long deg, double stdev);
		static void sampleUniform(ZZX& res, ZZ& B, long deg);
		static void sampleUniform(CZZX& res, ZZ& B, long deg);

		static long mobius(long n);
		static ZZX Cyclotomic(long N);

		static void addPolyRing2(CZZX& res, CZZX& poly1, CZZX& poly2, const long& logMod, const long& deg);
		static void subPolyRing2(CZZX& res, CZZX& poly1, CZZX& poly2, const long& logMod, const long& deg);
		static void mulPolyRing2(CZZX& res, CZZX& poly1, CZZX& poly2, const long& logMod, const long& deg);

		static void mulPolyByConstantRing2(CZZX& res, CZZX& poly, const CZZ& cnst, const long& logMod, const long& deg);

		static void leftShiftPolyRing2(CZZX& res, CZZX& poly, const long& cnst, const long& logMod, const long& deg);
		static void rightShiftPolyRing2(CZZX& res, CZZX& poly, const long& cnst, const long& logMod, const long& deg);

		static void sampleUniform2(ZZX& res, long& logB, long deg);
		static void sampleUniform2(CZZX& res, long& logB, long deg);
		static void truncate(CZZ& c, const long& logMod);
};

#endif /* POLYSCHEME_ZRINGUTILS_H_ */
