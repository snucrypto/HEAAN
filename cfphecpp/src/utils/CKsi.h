#ifndef UTILS_CKSI_H_
#define UTILS_CKSI_H_

#include <vector>

#include "../czz/CZZ.h"
#include "NTL/ZZ.h"

using namespace std;
using namespace NTL;

class CKsi {
public:

	long logp;
	vector<vector<CZZ>> pows;

	CKsi();
	void setLogp(long logp);

	void precompute(long logSize);
	virtual ~CKsi();
};

#endif /* UTILS_CKSI_H_ */
