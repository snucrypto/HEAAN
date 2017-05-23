#include "KsiPows.h"

#include <cmath>

KsiPows::KsiPows() : pows() {
	logp = 0;
}

void KsiPows::setLogp(long logp) {
	this->logp = logp;
}

void KsiPows::precompute(long logSize) {
	pows = new CZZ*[logSize];
	if(logp < 31) {
		long p = (1 << logp);
		for (long i = 0; i < logSize; ++i) {
			long ipow = (1 << i);
			CZZ* temp = new CZZ[ipow + 1];

			for (long j = 0; j < ipow; ++j) {
				double angle = 2.0 * M_PI * j / ipow;
				temp[j] = CZZ(to_ZZ(cos(angle) * p), to_ZZ(sin(angle) * p));
			}
			temp[ipow] = temp[0];
			pows[i] = temp;
		}
	} else {
		long tmp = (1 << 30);
		for (long i = 0; i < logSize; ++i) {
			long ipow = (1 << i);
			CZZ* temp = new CZZ[ipow + 1];
			for (long j = 0; j < ipow; ++j) {
				double angle = 2.0 * M_PI * j / ipow;
				ZZ rx = to_ZZ((long)(cos(angle) * tmp)) << (logp - 30);
				ZZ ix = to_ZZ((long)(sin(angle) * tmp)) << (logp - 30);
				temp[j] = CZZ(rx, ix);
			}
			temp[ipow] = temp[0];
			pows[i] = temp;
		}
	}
}
