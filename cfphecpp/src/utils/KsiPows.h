#ifndef UTILS_KSIPOWS_H_
#define UTILS_KSIPOWS_H_

#include <vector>

#include "../czz/CZZ.h"
#include "NTL/ZZ.h"

using namespace std;
using namespace NTL;

class KsiPows {
public:

	long logp;
	vector<vector<CZZ>> pows;

	KsiPows();
	void setLogp(long logp);

	void precompute(long logSize);
};

#endif /* UTILS_KSIPOWS_H_ */
