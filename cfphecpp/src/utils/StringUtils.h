/*
 * StringUtils.h
 *
 *  Created on: May 12, 2017
 *      Author: kimandrik
 */

#ifndef UTILS_STRINGUTILS_H_
#define UTILS_STRINGUTILS_H_

#include <vector>

#include "../czz/CZZ.h"

class StringUtils {
public:
	static void show(long*& vals, long size);
	static void show(CZZ*& vals, long size);

	//-----------------------------------------

	static void showcompare(CZZ& val1, CZZ& val2, string prefix);
	static void showcompare(CZZ*& vals1, CZZ*& vals2, long size, string prefix);
	static void showcompare(CZZ*& vals1, CZZ& val2, long size, string prefix);
	static void showcompare(CZZ& val1, CZZ*& vals2, long size, string prefix);

};

#endif /* UTILS_STRINGUTILS_H_ */
