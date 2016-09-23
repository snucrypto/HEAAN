#include "Ksi.h"

Ksi::Ksi(long n, long logpow) : n(n), logpow(logpow) {
	double angle;
	long i;
	long pow = (1 << logpow);
	for (i = 0; i < n; ++i) {
		angle = 2.0 * M_PI * i / n;

		double rx = cos(angle) * pow;
		double ix = sin(angle) * pow;

		powr.push_back(to_ZZ(rx));
		powi.push_back(to_ZZ(ix));
	}
}

