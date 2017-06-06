#ifndef SGD_SGD_H_
#define SGD_SGD_H_

#include "../scheme/Cipher.h"
#include "../scheme/Scheme.h"
#include "../scheme/SchemeAlgo.h"

using namespace std;
using namespace NTL;

class SGD {
public:
	Scheme scheme;
	SchemeAlgo algo;

	Cipher* grad(Cipher& ycipher, Cipher*& xcipher, Cipher*& wcipher, const long& dim, const long& sampledim);

	void operation(Cipher& res, Cipher& sig, Cipher& xcipher, const long& sampledim);

	SGD(Scheme& scheme, SchemeAlgo& algo) : scheme(scheme), algo(algo) {}
};

#endif /* SGD_SGD_H_ */
