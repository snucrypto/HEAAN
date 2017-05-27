/*
 * SGD.cpp
 *
 *  Created on: May 10, 2017
 *      Author: kimandrik
 */

#include "SGD.h"

// 5 levels;
Cipher* SGD::grad(Cipher& ysample, Cipher*& xsample, Cipher* widx, const long& idxsize, const long& samplesize) {
	Cipher ip = scheme.mult(xsample[0], widx[0]);
	for (long i = 1; i < idxsize; ++i) {
		Cipher tmp = scheme.mult(xsample[i], widx[i]);
		scheme.addAndEqual(ip, tmp);
	}
	scheme.modSwitchAndEqual(ip);
	Cipher tmp = scheme.modEmbed(ysample, ip.level);
	scheme.multModSwitchAndEqual(ip, tmp);

	Cipher sigy =  algo.function(ip, SIGMOID1, 7);
	scheme.modEmbedAndEqual(ip, sigy.level);
	scheme.multModSwitchAndEqual(sigy, ip);
	Cipher* res = new Cipher[idxsize];
	for (long i = 0; i < idxsize; ++i) {
		Cipher tmp = scheme.modEmbed(xsample[i], sigy.level);
		res[i] = scheme.multAndModSwitch(sigy, tmp);
		algo.slotsum(res[i], samplesize);
	}
	return res;
}


