#include "FPHEParams.h"

#include <NTL/ZZ.h>
#include <sstream>
#include <string>


FPHEParams::FPHEParams(long lambda, bool isGauss) : lambda(lambda), isGauss(isGauss) {
	long i, j, pLong;

	levels = 6;
	tau = 7;
	stdev = 3;

	m = 2048;
	phim = 1024;
	p = ZZ(40961);
	conv(pLong, p);
	pRoot = ZZ(2);
	RandomBits(B, 3);

	vector<long> coprime;
	for (i = 0; i < m; ++i) {
		if(GCD(i, m) == 1) coprime.push_back(i);
	}

	for (i = 1; i <= levels; ++i) {
		ZZ q = power(p, i);
		qi.push_back(q);
	}

	for (i = 1; i <= levels; ++i) {
		ZZ Pq = power(qi[i-1], 3);
		Pqi.push_back(Pq);
	}

	qL = qi[levels-1];
	PqL = Pqi[levels-1];
	P = qL * qL;

	ZZ pp = PowerMod(pRoot, p-1, qi[1]);
	if(IsOne(pp)) {
		qRoot = pRoot + p;
	} else {
		qRoot = pRoot;
	}
	ZZ primDeg = qi[levels-2] * (p-1) / m;
	ZZ alpha = PowerMod(qRoot, primDeg, qL);

	for (i = 0; i < phim; ++i) {
		long di = coprime[i];
		ZZ beta = PowerMod(alpha, di, qL);
		vector<ZZ> fftRow;
		for (j = 0; j < phim; ++j) {
			fftRow.push_back(PowerMod(beta, j, qL));
		}
		fft.push_back(fftRow);
	}



	ZZ phimZ = ZZ(phim);
	ZZ phimInv;

	InvMod(phimInv, phimZ, qL);

	ZZ alphaInv;
	InvMod(alphaInv, alpha, qL);

	for (i = 0; i < phim; ++i) {
		ZZ betaInv = PowerMod(alphaInv, i, qL);
		vector<ZZ> fftInvRow;
		for (j = 0; j < phim; ++j) {
			long dj = coprime[j];
			ZZ resInv = (PowerMod(betaInv, dj, qL) * phimInv) % qL;
			fftInvRow.push_back(resInv);
		}
		fftInv.push_back(fftInvRow);
	}
}

ZZ& FPHEParams::getModulo(long level) {
	return qi[levels - level];
}

FPHEParams::~FPHEParams() {
	// TODO Auto-generated destructor stub
}

string FPHEParams::toString() {
	stringstream ss;

	ss << "Params: ";
	ss << "lambda=";
	ss << lambda;
	ss << ", p=";
	ss << p;
	ss << ", qL=";
	ss << qL;
	ss << ", levels=";
	ss << levels;
	ss << ", tau=";
	ss << tau;
	ss << ", m=";
	ss << m;
	ss << ", phim=";
	ss << phim;
	ss << ", isGauss=";
	ss << isGauss;
	ss << ", stdev=";
	ss << stdev;
	ss << ", B=";
	ss << B;

	return ss.str();
}
