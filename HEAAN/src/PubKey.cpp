#include "PubKey.h"

#include "NumUtils.h"
#include "Params.h"
#include "Ring2Utils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) {
	addEncKey(params, secretKey);
	addMultKey(params, secretKey);
}

void PubKey::addEncKey(Params& params, SecKey& secretKey) {
	ZZX ex, ax, bx;

	NumUtils::sampleUniform2(ax, params.N, params.logq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::mult(bx, secretKey.sx, ax, params.q, params.N);
	Ring2Utils::sub(bx, ex, bx, params.q, params.N);

	keyMap.insert(pair<long, RLWE>(ENCRYPTION, RLWE(ax, bx)));
}

void PubKey::addMultKey(Params& params, SecKey& secretKey) {
	ZZX ex, ax, bx, sxsx;

	Ring2Utils::mult(sxsx, secretKey.sx, secretKey.sx, params.q, params.N);
	Ring2Utils::leftShiftAndEqual(sxsx, params.logq, params.qq, params.N);
	NumUtils::sampleUniform2(ax, params.N, params.logqq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::addAndEqual(ex, sxsx, params.qq, params.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, params.qq, params.N);
	Ring2Utils::sub(bx, ex, bx, params.qq, params.N);

	keyMap.insert(pair<long, RLWE>(MULTIPLICATION, RLWE(ax, bx)));
}

void PubKey::addConjKey(Params& params, SecKey& secretKey) {
	ZZX ex, ax, bx, sxconj;
	Ring2Utils::conjugate(sxconj, secretKey.sx, params.N);
	Ring2Utils::leftShiftAndEqual(sxconj, params.logq, params.qq, params.N);
	NumUtils::sampleUniform2(ax, params.N, params.logqq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::addAndEqual(ex, sxconj, params.qq, params.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, params.qq, params.N);
	Ring2Utils::sub(bx, ex, bx, params.qq, params.N);

	keyMap.insert(pair<long, RLWE>(CONJUGATION, RLWE(ax, bx)));
}

void PubKey::addLeftRotKey(Params& params, SecKey& secretKey, long rot) {
	ZZX ex, sxsx, ax, bx, spow;
	Ring2Utils::inpower(spow, secretKey.sx, params.rotGroup[rot], params.q, params.N);
	Ring2Utils::leftShiftAndEqual(spow, params.logq, params.qq, params.N);
	NumUtils::sampleUniform2(ax, params.N, params.logqq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::addAndEqual(ex, spow, params.qq, params.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, params.qq, params.N);
	Ring2Utils::sub(bx, ex, bx, params.qq, params.N);
	leftRotKeyMap.insert(pair<long, RLWE>(rot, RLWE(ax, bx)));
}

void PubKey::addLeftRotKeys(Params& params, SecKey& secretKey) {
	for (long i = 0; i < params.logN - 1; ++i) {
		long idx = 1 << i;
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end()) {
			addLeftRotKey(params, secretKey, idx);
		}
	}
}

void PubKey::addRightRotKeys(Params& params, SecKey& secretKey) {
	for (long i = 0; i < params.logN - 1; ++i) {
		long idx = params.N/2 - (1 << i);
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end()) {
			addLeftRotKey(params, secretKey, idx);
		}
	}
}

void PubKey::addBootKeys(Params& params, SecKey& secretKey, long size) {
	long sizeh = size/2;
	long k = 1 << sizeh;
	long m = 1 << (size - sizeh);

	for (long i = 1; i < k; ++i) {
		if(leftRotKeyMap.find(i) == leftRotKeyMap.end()) {
			addLeftRotKey(params, secretKey, i);
		}
	}
	for (long i = 1; i < m; ++i) {
		long idx = i * k;
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end()) {
			addLeftRotKey(params, secretKey, idx);
		}
	}
}

void PubKey::addSortKeys(Params& params, SecKey& secretKey, long size) {
	for (long i = 0; i < size; ++i) {
		if(leftRotKeyMap.find(i) == leftRotKeyMap.end()) {
			addLeftRotKey(params, secretKey, i);
		}
	}
}

