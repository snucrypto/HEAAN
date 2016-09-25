#ifndef EVAL_KSI_H_
#define EVAL_KSI_H_

#include <vector>
#include "../czz/CZZ.h"

using namespace std;
using namespace NTL;

class Ksi {
public:

	long n;
	long logpow;

	vector<CZZ> pows;

	Ksi(long n, long logpow);
};

#endif /* EVAL_KSI_H_ */
