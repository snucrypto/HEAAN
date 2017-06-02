/*
 * SGD.cpp
 *
 *  Created on: May 10, 2017
 *      Author: kimandrik
 */

#include "SGD.h"

// 5 levels;
Cipher* SGD::grad(Cipher& ycipher, Cipher*& xcipher, Cipher*& wcipher, const long& dim, const long& sampledim) {
	long bits = 1;
	Cipher ip = scheme.mult(xcipher[0], wcipher[0]);
	for (long i = 1; i < dim; ++i) {
		Cipher tmp = scheme.mult(xcipher[i], wcipher[i]);
		scheme.addAndEqual(ip, tmp);
	}
	scheme.modSwitchAndEqual(ip);

	Cipher tmp = scheme.modEmbed(ycipher, ip.level);
	scheme.multModSwitchAndEqual(ip, tmp);
	scheme.leftShiftAndEqual(ip, bits);
	Cipher sig =  algo.function(ip, SIGMOID1, 7);

	tmp = scheme.modEmbed(ycipher, sig.level);
	scheme.multModSwitchAndEqual(sig, tmp);

	Cipher* res = new Cipher[dim];
	for (long i = 0; i < dim; ++i) {
		res[i] = scheme.modEmbed(xcipher[i], sig.level);
		scheme.multModSwitchAndEqual(res[i], sig);
		algo.slotsumAndEqual(res[i], sampledim);
	}
	return res;
}
