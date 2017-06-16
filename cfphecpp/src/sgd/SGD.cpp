#include "SGD.h"

#include <NTL/BasicThreadPool.h>

#include <thread>
Cipher* SGD::grad(Cipher& ycipher, Cipher*& xcipher, Cipher*& wcipher, const long& dim, const long& sampledim) {
	Cipher ip = algo.innerProd(xcipher, wcipher, dim); // 1 level

	Cipher tmp = scheme.modEmbed(ycipher, ip.level);
	scheme.multModSwitchOneAndEqual(ip, tmp); // 1 level
	scheme.doubleAndEqual(ip);
	Cipher sig =  algo.function(ip, SIGMOID1, 7); // 3 levels

	tmp = scheme.modEmbed(ycipher, sig.level);
	scheme.multModSwitchOneAndEqual(sig, tmp); // 1 level

	Cipher* res = new Cipher[dim];
	NTL_EXEC_RANGE(dim, first, last);
	for (long i = first; i < last; ++i) {
		res[i] = scheme.modEmbed(xcipher[i], sig.level);
		scheme.multModSwitchOneAndEqual(res[i], sig); // 1 level
		algo.slotsumAndEqual(res[i], sampledim);
	}
	NTL_EXEC_RANGE_END;

	return res; // 7 levels total
}

