#include "CKsi.h"
#include <cmath>

CKsi::CKsi() {
	logp = 0;
	p = 1;
}

void CKsi::setLogp(long logp) {
	this->logp = logp;
	p = (1 << logp);
}

void CKsi::precompute(long logSize) {
	long i, j;
	double angle;
	long idx = pows.size();

	for (i = idx; i < logSize; ++i) {
		long ipow = (1 << i);
		vector<CZZ> temp;

		for (j = 0; j < ipow; ++j) {
			angle = 2.0 * M_PI * j / ipow;

			double rx = cos(angle) * p;
			double ix = sin(angle) * p;
			CZZ x(to_ZZ(rx), to_ZZ(ix));
			temp.push_back(x);
		}
		pows.push_back(temp);
	}
}

CKsi::~CKsi() {
	// TODO Auto-generated destructor stub
}

