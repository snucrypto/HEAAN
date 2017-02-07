#ifndef SCHEME_SCHEMEALGO_H_
#define SCHEME_SCHEMEALGO_H_

#include "Cipher.h"
#include "Scheme.h"

#include "../utils/CKsi.h"
#include "../utils/TimeUtils.h"

class SchemeAlgo {
public:
	Scheme& scheme;

	SchemeAlgo(Scheme& scheme) : scheme(scheme) {};
	void powerOf2(vector<Cipher>& c2k, Cipher& c, const long& deg);
	void prod2(vector<vector<Cipher>>& cs2k, vector<Cipher>& cs, const long& deg);
	void inverse(vector<Cipher>& c2k, vector<Cipher>& v2k, Cipher& c, const long& k);

	vector<Cipher> fftRaw(vector<Cipher>& ciphers, CKsi& cksi, const bool& isForward);
	vector<Cipher> fft(vector<Cipher>& ciphers, CKsi& cksi);
	vector<Cipher> fftInv(vector<Cipher>& ciphers, CKsi& cksi);

	vector<Cipher> fftRaw2(vector<Cipher>& ciphers, const bool& isForward);
	vector<Cipher> fft2(vector<Cipher>& ciphers);
	vector<Cipher> fftInv2(vector<Cipher>& ciphers);
};

#endif /* SCHEME_SCHEMEALGO_H_ */
