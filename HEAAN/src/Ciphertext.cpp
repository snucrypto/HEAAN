/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Ciphertext.h"

#include <NTL/tools.h>
#include "Common.h"

Ciphertext::Ciphertext(ZZ* ax, ZZ* bx, long logp, long logq, long N, long n) :
		ax(ax), bx(bx), logp(logp), logq(logq), N(N), n(n) {

}

Ciphertext::Ciphertext(const Ciphertext& o) : logp(o.logp), logq(o.logq), N(o.N), n(o.n) {
	ax = new ZZ[N];
	bx = new ZZ[N];
	for (long i = 0; i < N; ++i) {
		ax[i] = o.ax[i];
		bx[i] = o.bx[i];
	}
}

Ciphertext& Ciphertext::operator=(const Ciphertext& o) {
	if(this == &o) return *this;
	delete[] ax;
	delete[] bx;
	logp = o.logp;
	logq = o.logq;
	N = o.N;
	n = o.n;
	ax = new ZZ[N];
	bx = new ZZ[N];
	for (long i = 0; i < N; ++i) {
		ax[i] = o.ax[i];
		bx[i] = o.bx[i];
	}
	return *this;
}

Ciphertext::~Ciphertext() {
	if(ax != NULL) delete[] ax;
	if(bx != NULL) delete[] bx;
}

void Ciphertext::kill() {
	if(ax != NULL) {
		for(long i = 0; i < N; i++) {
			ax[i].kill();
		}
	}
	if(bx != NULL) {
		for(long i = 0; i < N; i++) {
			bx[i].kill();
		}
	}
}
