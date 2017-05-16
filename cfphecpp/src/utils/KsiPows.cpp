#include "KsiPows.h"

#include <cmath>

KsiPows::KsiPows() {
	logp = 0;
}

void KsiPows::setLogp(long logp) {
	this->logp = logp;
}

void KsiPows::precompute(long logSize) {
	long i, j;
	double angle;
	long idx = pows.size();

	if(logp < 31) {
		long p = (1 << logp);
		for (i = idx; i < logSize; ++i) {
			long ipow = (1 << i);
			vector<CZZ> temp;

			for (j = 0; j < ipow; ++j) {
				angle = 2.0 * M_PI * j / ipow;
				ZZ rx = to_ZZ(cos(angle) * p);
				ZZ ix = to_ZZ(sin(angle) * p);
				CZZ x(rx, ix);
				temp.push_back(x);
			}
			temp.push_back(temp[0]);
			pows.push_back(temp);
		}
	} else {
		long tmp = (1 << 30);
		for (i = idx; i < logSize; ++i) {
			long ipow = (1 << i);
			vector<CZZ> temp;

			for (j = 0; j < ipow; ++j) {
				angle = 2.0 * M_PI * j / ipow;
				ZZ rx = to_ZZ((long)(cos(angle) * tmp));
				rx <<= (logp - 30);
				ZZ ix = to_ZZ((long)(sin(angle) * tmp));
				ix <<= (logp - 30);
				CZZ x(rx, ix);
				temp.push_back(x);
			}
			temp.push_back(temp[0]);
			pows.push_back(temp);
		}
	}
}

KsiPows::~KsiPows() {
	// TODO Auto-generated destructor stub
}
