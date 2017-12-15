#ifndef HEAAN_STRINGUTILS_H_
#define HEAAN_STRINGUTILS_H_

#include "Common.h"
#include "CZZ.h"

class StringUtils {
public:

	/**
	 * prints in console array
	 * @param[in] vals: long array
	 * @param[in] size: array size
	 */
	static void show(long* vals, long size);

	/**
	 * prints in console array
	 * @param[in] vals: CZZ array
	 * @param[in] size: array size
	 */
	static void show(CZZ* vals, long size);

	/**
	 * prints in console val1, val2 and (val1-val2)
	 * @param[in] val1: CZZ value
	 * @param[in] val2: CZZ value
	 * @param[in] prefix: string prefix
	 */
	static void showcompare(CZZ& val1, CZZ& val2, string prefix);

	/**
	 * prints in console pairwise val1[i], val2[i] and (val1[i]-val2[i])
	 * @param[in] vals1: CZZ array
	 * @param[in] vals2: CZZ array
	 * @param[in] size: array size
	 * @param[in] prefix: string prefix
	 */
	static void showcompare(CZZ* vals1, CZZ* vals2, long size, string prefix);

	/**
	 * prints in console pairwise val1[i], val2 and (val1[i]-val2)
	 * @param[in] vals1: CZZ array
	 * @param[in] val2: CZZ value
	 * @param[in] size: array size
	 * @param[in] prefix: string prefix
	 */
	static void showcompare(CZZ* vals1, CZZ& val2, long size, string prefix);

	/**
	 * prints in console pairwise val1, val2[i] and (val1-val2[i])
	 * @param[in] val1: CZZ value
	 * @param[in] vals2: CZZ array
	 * @param[in] size: array size
	 * @param[in] prefix: string prefix
	 */
	static void showcompare(CZZ& val1, CZZ* vals2, long size, string prefix);

};

#endif
