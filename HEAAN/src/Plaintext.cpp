/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Plaintext.h"

Plaintext::Plaintext(ZZ* mx, long logp, long logq, long N, long n) : mx(mx), logp(logp), logq(logq), N(N), n(n) {

}

Plaintext::Plaintext(const Plaintext& o) : mx(o.mx), logp(o.logp), logq(o.logq), N(o.N), n(o.n) {
	mx = new ZZ[N];
	for (long i = 0; i < N; ++i) {
		mx[i] = o.mx[i];
	}
}

Plaintext& Plaintext::operator=(const Plaintext& o) {
	if(this == &o) return *this; // handling of self assignment, thanks for your advice, arul.
	delete[] mx; // freeing previously used memory
	logp = o.logp;
	logq = o.logq;
	N = o.N;
	n = o.n;
	mx = new ZZ[N];
	for (long i = 0; i < N; ++i) {
		mx[i] = o.mx[i];
	}
	return *this;
}

Plaintext::~Plaintext() {
	if (mx) delete[] mx;
}
