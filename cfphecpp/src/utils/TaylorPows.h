#ifndef UTILS_TAYLORPOWS_H_
#define UTILS_TAYLORPOWS_H_

#include <vector>

#include "../czz/CZZ.h"
#include "NTL/ZZ.h"

using namespace std;
using namespace NTL;

class TaylorPows {
public:

	long logp;

	vector<ZZ> expPows;
	vector<double> expCoeffs;

	vector<ZZ> sigmoidPows;
	vector<double> sigmoidCoeffs;

	void setLogp(long logp);
	void precompute();

	TaylorPows();
};

#endif /* UTILS_TAYLORPOWS_H_ */
