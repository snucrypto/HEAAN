/*
 * SGD.cpp
 *
 *  Created on: May 10, 2017
 *      Author: kimandrik
 */

#include "SGD.h"

#include <NTL/BasicThreadPool.h>

#include <thread>
// 5 levels;
Cipher* SGD::grad(Cipher& ycipher, Cipher*& xcipher, Cipher*& wcipher, const long& dim, const long& sampledim) {
	Cipher* res = new Cipher[dim];
	NTL_EXEC_RANGE(dim, first, last);
	for (long i = first; i < last; ++i) {
		res[i] = scheme.mult(xcipher[i], wcipher[i]);
	}
	NTL_EXEC_RANGE_END;

	Cipher ip = algo.sum(res, dim);
	scheme.modSwitchOneAndEqual(ip);

	Cipher tmp = scheme.modEmbed(ycipher, ip.level);
	scheme.multModSwitchOneAndEqual(ip, tmp);
	scheme.doubleAndEqual(ip);
	Cipher sig =  algo.function(ip, SIGMOID1, 7);

	tmp = scheme.modEmbed(ycipher, sig.level);
	scheme.multModSwitchOneAndEqual(sig, tmp);

	NTL_EXEC_RANGE(dim, first, last);
	for (long i = first; i < last; ++i) {
		res[i] = scheme.modEmbed(xcipher[i], sig.level);
		scheme.multModSwitchOneAndEqual(res[i], sig);
		algo.slotsumAndEqual(res[i], sampledim);

	}
	NTL_EXEC_RANGE_END;

	return res;
}

