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

	Cipher powerOf2(Cipher& c, const long& logDegree);
	void powerOf2Extended(vector<Cipher>& res, Cipher& c, const long& logDegree);

	//-----------------------------------------

	void powerExtended(vector<Cipher>& res, Cipher& c, const long& degree);

	//-----------------------------------------

	Cipher prod2(vector<Cipher>& cs, const long& logDegree);
	void prod2Extended(vector<vector<Cipher>>& res, vector<Cipher>& cs, const long& logDegree);

	//-----------------------------------------

	Cipher inverse(Cipher& c, const long& steps);
	void inverseExtended(vector<Cipher>& res, vector<Cipher>& v2k, Cipher& c, const long& steps);

	//-----------------------------------------

	Cipher function(Cipher& c, string& funcName, const long& degree);
	void functionExtended(vector<Cipher>& res, Cipher& c, string& funcName, const long& degree);

	//-----------------------------------------

	vector<Cipher> fftRaw(vector<Cipher>& ciphers, const bool& isForward);
	vector<Cipher> fft(vector<Cipher>& ciphers);
	vector<Cipher> fftInv(vector<Cipher>& ciphers);

	//-----------------------------------------

};

#endif /* SCHEME_SCHEMEALGO_H_ */
