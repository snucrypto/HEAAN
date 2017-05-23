#ifndef UTILS_KSIPOWS_H_
#define UTILS_KSIPOWS_H_

#include "../czz/CZZ.h"

using namespace std;
using namespace NTL;

class KsiPows {
public:

	long logp;
	CZZ** pows;

	void setLogp(long logp);
	void precompute(long logSize);

	//-----------------------------------------

	KsiPows();
};

#endif /* UTILS_KSIPOWS_H_ */
