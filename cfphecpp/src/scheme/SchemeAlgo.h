#ifndef SCHEME_SCHEMEALGO_H_
#define SCHEME_SCHEMEALGO_H_

#include "Cipher.h"
#include "Scheme.h"

#include "../utils/CKsi.h"
#include "../utils/TimeUtils.h"

class SchemeAlgo {
public:
	Scheme& scheme;
	TimeUtils& timeutils;

	SchemeAlgo(Scheme& scheme, TimeUtils& timeutils) : scheme(scheme), timeutils(timeutils) {};
	void powerOf2(vector<Cipher>& c2k, Cipher& c, const long& deg);
	void prodOfSame(vector<Cipher>& c2k, Cipher& c, const long& deg);
	void inverse(vector<Cipher>& c2k, vector<Cipher>& v2k, Cipher& c, const long& k);

	vector<Cipher> fft(vector<Cipher>& ciphers, CKsi& cksi, const long& is);
};

#endif /* SCHEME_SCHEMEALGO_H_ */
