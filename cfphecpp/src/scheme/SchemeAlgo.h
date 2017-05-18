#ifndef SCHEME_SCHEMEALGO_H_
#define SCHEME_SCHEMEALGO_H_

#include "Cipher.h"
#include "Scheme.h"

#include "../utils/KsiPows.h"
#include "../utils/TimeUtils.h"

class SchemeAlgo {
public:
	Scheme& scheme;

	SchemeAlgo(Scheme& scheme) : scheme(scheme) {};

	//-----------------------------------------

	Cipher powerOf2(Cipher& cipher, const long& logDegree);
	void powerOf2Extended(vector<Cipher>& res, Cipher& cipher, const long& logDegree);

	//-----------------------------------------

	Cipher power(Cipher& cipher, const long& degree);
	void powerExtended(vector<Cipher>& res, Cipher& cipher, const long& degree);

	//-----------------------------------------

	Cipher prod2(vector<Cipher>& ciphers, const long& logDegree);
	void prod2Extended(vector<vector<Cipher>>& res, vector<Cipher>& ciphers, const long& logDegree);

	//-----------------------------------------

	Cipher inverse(Cipher& cipher, const long& steps);
	void inverseExtended(vector<Cipher>& res, Cipher& cipher, const long& steps);

	//-----------------------------------------

	Cipher function(Cipher& cipher, string& funcName, const long& degree);
	Cipher functionLazy(Cipher& cipher, string& funcName, const long& degree);
	void functionExtended(vector<Cipher>& res, Cipher& cipher, string& funcName, const long& degree);

	//-----------------------------------------

	vector<Cipher> fftRaw(vector<Cipher>& ciphers, const bool& isForward);
	vector<Cipher> fft(vector<Cipher>& ciphers);
	vector<Cipher> fftInv(vector<Cipher>& ciphers);
	vector<Cipher> fftInvLazy(vector<Cipher>& ciphers);

	//-----------------------------------------

};

#endif /* SCHEME_SCHEMEALGO_H_ */
