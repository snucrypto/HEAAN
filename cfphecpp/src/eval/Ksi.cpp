#include "Ksi.h"

#include <NTL/ZZ.h>
#include <cmath>

Ksi::Ksi(long n, long logpow) : n(n), logpow(logpow) {
	double angle;
	pow = (1 << logpow);
	for (long i = 0; i < n; ++i) {
		angle = 2.0 * M_PI * i / n;

		double rx = cos(angle) * pow;
		double ix = sin(angle) * pow;
		CZZ x(to_ZZ(rx), to_ZZ(ix));
		pows.push_back(x);
	}
}

