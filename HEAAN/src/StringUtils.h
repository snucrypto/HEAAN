#ifndef UTILS_STRINGUTILS_H_
#define UTILS_STRINGUTILS_H_

#include <iostream>

#include "CZZ.h"

class StringUtils {
public:

	//-----------------------------------------

	/**
	 * prints in console array
	 * @param[in] array
	 * @param[in] size of array
	 */
	static void show(long*& vals, long size);

	/**
	 * prints in console array
	 * @param[in] array
	 * @param[in] size of array
	 */
	static void show(CZZ*& vals, long size);

	//-----------------------------------------

	/**
	 * prints in console val1, val2 and (val1-val2)
	 * @param[in] CZZ val
	 * @param[in] CZZ val
	 * @param[in] string prefix
	 */
	static void showcompare(CZZ& val1, CZZ& val2, string prefix);

	/**
	 * prints in console pairwise val1[i], val2[i] and (val1[i]-val2[i])
	 * @param[in] array of CZZ val
	 * @param[in] array of CZZ val
	 * @param[in] string prefix
	 */
	static void showcompare(CZZ*& vals1, CZZ*& vals2, long size, string prefix);

	/**
	 * prints in console pairwise val1[i], val2 and (val1[i]-val2)
	 * @param[in] array of CZZ val
	 * @param[in] CZZ val
	 * @param[in] string prefix
	 */
	static void showcompare(CZZ*& vals1, CZZ& val2, long size, string prefix);

	/**
	 * prints in console pairwise val1, val2[i] and (val1-val2[i])
	 * @param[in] CZZ val
	 * @param[in] array of CZZ val
	 * @param[in] string prefix
	 */
	static void showcompare(CZZ& val1, CZZ*& vals2, long size, string prefix);

	//-----------------------------------------

};

#endif /* UTILS_STRINGUTILS_H_ */
