#ifndef POLYSCHEME_ZRINGUTILS_H_
#define POLYSCHEME_ZRINGUTILS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include "../czz/CZZ.h"
#include "../czz/CZZX.h"

using namespace NTL;

class Ring2Utils {
	public:
		static void add(ZZX& res, ZZX& p1, ZZX& p2, const long& degree);
		static void add(CZZX& res, CZZX& p1, CZZX& p2, const long& degree);

		static void add(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void add(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree);

		static void addNew(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long& degree);
		static void addNew(CZZX& res, CZZX& p1, CZZX& p2, ZZ& mod, const long& degree);

		static void addAndEqual(ZZX& p1, ZZX& p2, const long& degree);
		static void addAndEqual(CZZX& p1, CZZX& p2, const long& degree);

		static void addAndEqual(ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void addAndEqual(CZZX& p1, CZZX& p2, const long& logMod, const long& degree);

		static void addAndEqualNew(ZZX& p1, ZZX& p2, ZZ& mod, const long& degree);
		static void addAndEqualNew(CZZX& p1, CZZX& p2, ZZ& mod, const long& degree);

		static void sub(ZZX& res, ZZX& p1, ZZX& p2, const long& degree);
		static void sub(CZZX& res, CZZX& p1, CZZX& p2, const long& degree);

		static void sub(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void sub(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree);

		static void subNew(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long& degree);
		static void subNew(CZZX& res, CZZX& p1, CZZX& p2, ZZ& mod, const long& degree);

		static void subAndEqual(ZZX& p1, ZZX& p2, const long& degree);
		static void subAndEqual(CZZX& p1, CZZX& p2, const long& degree);

		static void subAndEqual(ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void subAndEqual(CZZX& p1, CZZX& p2, const long& logMod, const long& degree);

		static void subAndEqualNew(ZZX& p1, ZZX& p2, ZZ& mod, const long& degree);
		static void subAndEqualNew(CZZX& p1, CZZX& p2, ZZ& mod, const long& degree);

		static void mult(ZZX& res, ZZX& p1, ZZX& p2, const long& degree);
		static void mult(CZZX& res, CZZX& p1, CZZX& p2, const long& degree);

		static void mult(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void mult(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree);
		static void mult(CZZX& res, CZZX& p1, ZZX& p2, const long& logMod, const long& degree);

		static void multNew(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, ZZ& mod, const long& degree);
		static void multNew(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, ZZ& mod, const long& degree);
		static void multNew(CZZX& res, CZZX& p1, ZZX& p2, const long& logMod, ZZ& mod, const long& degree);

		static void multAndEqual(ZZX& p1, ZZX& p2, const long& degree);
		static void multAndEqual(CZZX& p1, CZZX& p2, const long& degree);

		static void multAndEqual(ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void multAndEqual(CZZX& p1, CZZX& p2, const long& logMod, const long& degree);
		static void multAndEqual(CZZX& p1, ZZX& p2, const long& logMod, const long& degree);

		static void multAndEqualNew(ZZX& p1, ZZX& p2, const long& logMod, ZZ& mod, const long& degree);
		static void multAndEqualNew(CZZX& p1, CZZX& p2, const long& logMod, ZZ& mod, const long& degree);
		static void multAndEqualNew(CZZX& p1, ZZX& p2, const long& logMod, ZZ& mod, const long& degree);

		static void square(CZZX& res, CZZX& p, const long& logMod, const long& degree);
		static void squareNew(CZZX& res, CZZX& p, const long& logMod, ZZ& mod, const long& degree);

		static void squareAndEqual(CZZX& p, const long& logMod, const long& degree);
		static void squareAndEqualNew(CZZX& p, const long& logMod, ZZ& mod, const long& degree);

		static void multByMonomial(ZZX& res, ZZX& p, const long& monomialDeg, const long& degree);
		static void multByMonomial(CZZX& res, CZZX& p, const long& monomialDeg, const long& degree);

		static void multByMonomialNew(ZZX& res, ZZX& p, const long& monomialDeg, const long& degree);
		static void multByMonomialNew(CZZX& res, CZZX& p, const long& monomialDeg, const long& degree);

		static void multByMonomialAndEqual(ZZX& p, const long& monomialDeg, const long& degree);
		static void multByMonomialAndEqual(CZZX& p, const long& monomialDeg, const long& degree);

		static void multByMonomialAndEqualNew(ZZX& p, const long& monomialDeg, const long& degree);
		static void multByMonomialAndEqualNew(CZZX& p, const long& monomialDeg, const long& degree);

		static void multByConst(ZZX& res, ZZX& p, const long& cnst, const long& degree);
		static void multByConst(CZZX& res, CZZX& p, const long& cnst, const long& degree);
		static void multByConst(ZZX& res, ZZX& p, const ZZ& cnst, const long& degree);
		static void multByConst(CZZX& res, CZZX& p, const CZZ& cnst, const long& degree);

		static void multByConst(ZZX& res, ZZX& p, const ZZ& cnst, const long& logMod, const long& degree);
		static void multByConst(CZZX& res, CZZX& p, const ZZ& cnst, const long& logMod, const long& degree);
		static void multByConst(CZZX& res, CZZX& p, const CZZ& cnst, const long& logMod, const long& degree);

		static void multByConstNew(ZZX& res, ZZX& p, const ZZ& cnst, ZZ& mod, const long& degree);
		static void multByConstNew(CZZX& res, CZZX& p, const ZZ& cnst, ZZ& mod, const long& degree);
		static void multByConstNew(CZZX& res, CZZX& p, const CZZ& cnst, ZZ& mod, const long& degree);

		static void multByConstAndEqual(ZZX& p, const long& cnst, const long& degree);
		static void multByConstAndEqual(CZZX& p, const long& cnst, const long& degree);
		static void multByConstAndEqual(ZZX& p, const ZZ& cnst, const long& degree);
		static void multByConstAndEqual(CZZX& p, const CZZ& cnst, const long& degree);

		static void multByConstAndEqual(ZZX& p, const ZZ& cnst, const long& logMod, const long& degree);
		static void multByConstAndEqual(CZZX& p, const ZZ& cnst, const long& logMod, const long& degree);
		static void multByConstAndEqual(CZZX& p, const CZZ& cnst, const long& logMod, const long& degree);

		static void multByConstAndEqualNew(ZZX& p, const ZZ& cnst, ZZ& mod, const long& degree);
		static void multByConstAndEqualNew(CZZX& p, const ZZ& cnst, ZZ& mod, const long& degree);
		static void multByConstAndEqualNew(CZZX& p, const CZZ& cnst, ZZ& mod, const long& degree);

		static void leftShift(ZZX& res, ZZX& p, const long& bits, const long& logMod, const long& degree);
		static void leftShift(CZZX& res, CZZX& p, const long& bits, const long& logMod, const long& degree);

		static void leftShiftAndEqual(ZZX& p, const long& bits, const long& logMod, const long& degree);
		static void leftShiftAndEqual(CZZX& p, const long& bits, const long& logMod, const long& degree);

		static void rightShift(ZZX& res, ZZX& p, const long& bits, const long& degree);
		static void rightShift(CZZX& res, CZZX& p, const long& bits, const long& degree);

		static void rightShift(ZZX& res, ZZX& p, const long& bits, const long& logMod, const long& degree);
		static void rightShift(CZZX& res, CZZX& p, const long& bits, const long& logMod, const long& degree);

		static void rightShiftAndEqual(ZZX& p, const long& bits, const long& degree);
		static void rightShiftAndEqual(CZZX& p, const long& bits, const long& degree);

		static void rightShiftAndEqual(ZZX& p, const long& bits, const long& logMod, const long& degree);
		static void rightShiftAndEqual(CZZX& p, const long& bits, const long& logMod, const long& degree);

		static void truncate(ZZX& res, ZZX& p, const long& logMod, const long& degree);
		static void truncate(CZZX& res, CZZX& p, const long& logMod, const long& degree);

		static void truncateAndEqual(ZZX& p, const long& logMod, const long& degree);
		static void truncateAndEqual(CZZX& p, const long& logMod, const long& degree);

		//-------------------------
		static void truncate(ZZ& res, const long& logMod);
		static void truncate(CZZ& res, const long& logMod);
};

#endif /* POLYSCHEME_ZRINGUTILS_H_ */
