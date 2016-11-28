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
	void powerOf2(vector<Cipher>& c2k, Cipher& c, long& deg);
	void prodOfSame(vector<Cipher>& c2k, Cipher& c, long& deg);
	void inverse(vector<Cipher>& c2k, vector<Cipher>& v2k, Cipher& c, long& k);

	vector<Cipher> fft(vector<Cipher>& ciphers, CKsi& cksi);
	vector<Cipher> fftInv(vector<Cipher>& ciphers, CKsi& cksi);
	vector<Cipher> fftNew(vector<Cipher>& ciphers, CKsi& cksi, ZZ& factor, long bnd);
	vector<Cipher> fftInvNew(vector<Cipher>& ciphers, CKsi& cksi, ZZ& factor, long bnd);
};

#endif /* SCHEME_SCHEMEALGO_H_ */
