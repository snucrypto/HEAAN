#include "CKsi.h"
#include <cmath>

CKsi::CKsi() {
	logp = 0;
}

void CKsi::setLogp(long logp) {
	this->logp = logp;
}

void CKsi::precompute(long logSize) {
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
			pows.push_back(temp);
		}
	} else {
		long tmp = (1 << 31);
		for (i = idx; i < logSize; ++i) {
			long ipow = (1 << i);
			vector<CZZ> temp;

			for (j = 0; j < ipow; ++j) {
				angle = 2.0 * M_PI * j / ipow;
				ZZ rx = to_ZZ((long)(cos(angle) * tmp));
				rx <<= (logp-30);
				ZZ ix = to_ZZ((long)(sin(angle) * tmp));
				ix <<= (logp-30);
				CZZ x(rx, ix);
				temp.push_back(x);
			}
			pows.push_back(temp);
		}
	}
}

CKsi::~CKsi() {
	// TODO Auto-generated destructor stub
}

