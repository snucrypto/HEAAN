#ifndef SCHEME_SCHEMEALGO_H_
#define SCHEME_SCHEMEALGO_H_

#include <iostream>

#include "Cipher.h"
#include "Scheme.h"

class SchemeAlgo {
public:
	Scheme& scheme;

	//-----------------------------------------

	SchemeAlgo(Scheme& scheme) : scheme(scheme) {};

	//-----------------------------------------

	Cipher powerOf2(Cipher& cipher, const long& logDegree);
	Cipher* powerOf2Extended(Cipher& cipher, const long& logDegree);

	//-----------------------------------------

	Cipher power(Cipher& cipher, const long& degree);
	Cipher* powerExtended(Cipher& cipher, const long& degree);

	//-----------------------------------------

	Cipher prod2(Cipher*& ciphers, const long& logDegree);

	//-----------------------------------------

	Cipher sum(Cipher*& ciphers, const long& size);

	//-----------------------------------------

	Cipher* multVec(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);
	void multAndEqualVec(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);
	Cipher* multAndModSwitchVec(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);
	void multModSwitchAndEqualVec(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);

	//-----------------------------------------

	Cipher innerProd(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);

	//-----------------------------------------

	Cipher inverse(Cipher& cipher, const long& steps);
	Cipher* inverseExtended(Cipher& cipher, const long& steps);

	//-----------------------------------------

	Cipher function(Cipher& cipher, string& funcName, const long& degree);
	Cipher functionLazy(Cipher& cipher, string& funcName, const long& degree);
	Cipher* functionExtended(Cipher& cipher, string& funcName, const long& degree);

	//-----------------------------------------

	Cipher* fftRaw(Cipher*& ciphers, const long& size, const bool& isForward);
	Cipher* fft(Cipher*& ciphers, const long& size);
	Cipher* fftInv(Cipher*& ciphers, const long& size);
	Cipher* fftInvLazy(Cipher*& ciphers, const long& size);

	void butOperation(Cipher& res1, Cipher& res2, Cipher& y1, Cipher& y2, long shift);
	void rescale(Cipher& c, long& bits);

	//-----------------------------------------

	Cipher slotsum(Cipher& cipher, const long& slots);
	void slotsumAndEqual(Cipher& cipher, const long& slots);

	//-----------------------------------------

};

#endif /* SCHEME_SCHEMEALGO_H_ */
