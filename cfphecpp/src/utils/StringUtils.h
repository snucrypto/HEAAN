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
	static void show(vector<CZZ>& vals);
	static void showcompare(vector<CZZ>& vals1, vector<CZZ>& vals2, string prefix);

};

#endif /* UTILS_STRINGUTILS_H_ */
