#include "FPHEScheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>

#include "FPHECipher.h"
#include "ZRingUtils.h"

using namespace std;
using namespace NTL;

FPHECipher FPHEScheme::encrypt(ZZ& m) {
	ZZ bits;
	RandomBits(bits, params.tau);
	ZZX c0;
	ZZX c1;

	long i;
	for (i = 0; i < params.tau; ++i) {
		if(bit(bits, i)) {
			ZZX lwe0 = publicKey.A0[i];
			ZZX lwe1 = publicKey.A1[i];

			ZRingUtils::addRing(c0, c0, lwe0, params.qL, params.phi);
			ZRingUtils::addRing(c1, c1, lwe1, params.qL, params.phi);
		}
	}
	SetCoeff(c0, 0, (coeff(c0, 0) + m) % params.qL);
	c0.normalize();
	FPHECipher cipher(c0, c1, 1);
	return cipher;
}

ZZ FPHEScheme::decrypt(FPHECipher& cipher) {

	ZZ& qi = getModulo(cipher.level);

	ZZX m;
	ZRingUtils::mulRing(m, secretKey.s, cipher.c1, qi, params.phi);
	ZRingUtils::addRing(m, m, cipher.c0, qi, params.phi);
	ZZ c;
	GetCoeff(c, m, 0);
	if(c < 0) {c += qi;}
	return c;
}

FPHECipher FPHEScheme::add(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	ZZX c0;
	ZZX c1;
	ZRingUtils::addRing(c0, cipher1.c0, cipher2.c0, qi, params.phi);
	ZRingUtils::addRing(c1, cipher1.c1, cipher2.c1, qi, params.phi);
	FPHECipher res(c0, c1, cipher1.level);
	return res;
}

void FPHEScheme::addAndEqual(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZRingUtils::addRing(cipher1.c0, cipher1.c0, cipher2.c0, qi, params.phi);
	ZRingUtils::addRing(cipher1.c1, cipher1.c1, cipher2.c1, qi, params.phi);
}

FPHECipher FPHEScheme::sub(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	ZZX c0;
	ZZX c1;
	ZRingUtils::subRing(c0, cipher1.c0, cipher2.c0, qi, params.phi);
	ZRingUtils::subRing(c1, cipher1.c1, cipher2.c1, qi, params.phi);
	FPHECipher res(c0, c1, cipher1.level);
	return res;
}

void FPHEScheme::subAndEqual(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZRingUtils::subRing(cipher1.c0, cipher1.c0, cipher2.c0, qi, params.phi);
	ZRingUtils::subRing(cipher1.c1, cipher1.c1, cipher2.c1, qi, params.phi);
}

FPHECipher FPHEScheme::mul(FPHECipher& cipher1, FPHECipher& cipher2) {
	long i, j, idx;
	ZZ qi = getModulo(cipher1.level);

	ZZX multC0;
	ZZX multC1;
	ZZX temp;
	ZZX bPoly;
	ZZX cc[4];
	ZRingUtils::mulRing(cc[0], cipher1.c0, cipher2.c0, qi, params.phi);
	ZRingUtils::mulRing(cc[1], cipher1.c0, cipher2.c1, qi, params.phi);
	ZRingUtils::mulRing(cc[2], cipher1.c1, cipher2.c0, qi, params.phi);
	ZRingUtils::mulRing(cc[3], cipher1.c1, cipher2.c1, qi, params.phi);

	long logQi = params.logQi[params.levels - cipher1.level];

	idx = 0;
 	for (j = 0; j < logQi; ++j) {
 		for (i = 0; i < 4; ++i) {
			ZRingUtils::bitPoly(bPoly, cc[i], j);
			ZZX lwe0 = publicKey.AL0[idx];
			ZZX lwe1 = publicKey.AL1[idx];

			ZRingUtils::mulRing(temp, bPoly, lwe0, qi, params.phi);
			ZRingUtils::addRing(multC0, multC0, temp, qi, params.phi);

			ZRingUtils::mulRing(temp, bPoly, lwe1, qi, params.phi);
			ZRingUtils::addRing(multC1, multC1, temp, qi, params.phi);
			idx++;
		}
	}

	FPHECipher cipher(multC0, multC1, cipher1.level);
	return cipher;
}

void FPHEScheme::mulAndEqual(FPHECipher& cipher1, FPHECipher& cipher2) {
	long i, j, idx;
	ZZ qi = getModulo(cipher1.level);

	ZZX multC0;
	ZZX multC1;
	ZZX temp;
	ZZX bPoly;
	ZZX cc;


	long logQi = params.logQi[params.levels - cipher1.level];

	idx = 0;
	for (i = 0; i < 4; ++i) {
		if(i == 0) {
			ZRingUtils::mulRing(cc, cipher1.c0, cipher2.c0, qi, params.phi);
		} else if(i == 1) {
			ZRingUtils::mulRing(cc, cipher1.c0, cipher2.c1, qi, params.phi);
		} else if(i == 2) {
			ZRingUtils::mulRing(cc, cipher1.c1, cipher2.c0, qi, params.phi);
		} else {
			ZRingUtils::mulRing(cc, cipher1.c1, cipher2.c1, qi, params.phi);
		}

		for (j = 0; j < logQi; ++j) {
			ZRingUtils::bitPoly(bPoly, cc, j);
			ZZX lwe0 = publicKey.AL0[idx];
			ZZX lwe1 = publicKey.AL1[idx];

			ZRingUtils::mulRing(temp, bPoly, lwe0, qi, params.phi);
			ZRingUtils::addRing(multC0, multC0, temp, qi, params.phi);

			ZRingUtils::mulRing(temp, bPoly, lwe1, qi, params.phi);
			ZRingUtils::addRing(multC1, multC1, temp, qi, params.phi);
			idx++;
		}
	}
	cipher1.c0 = multC0;
	cipher1.c1 = multC1;
}

FPHECipher FPHEScheme::addConstant(FPHECipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	ZZX c0 = cipher.c0;
	ZZX c1 = cipher.c1;
	SetCoeff(c1, 0, (coeff(cipher.c1,0) + cnst) % qi);
	c0.normalize();
	FPHECipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

FPHECipher FPHEScheme::mulByConstant(FPHECipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	ZZX c0;
	ZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(c0, i, (coeff(cipher.c0,i) * cnst) % qi);
		SetCoeff(c1, i, (coeff(cipher.c1,i) * cnst) % qi);
	}
	c0.normalize();
	c1.normalize();
	FPHECipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

void FPHEScheme::addConstantAndEqual(FPHECipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);
	long i;
	SetCoeff(cipher.c0, 0, (coeff(cipher.c0, 0) * cnst) % qi);
	cipher.c0.normalize();
}

void FPHEScheme::mulByConstantAndEqual(FPHECipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(cipher.c0, i, (coeff(cipher.c0,i) * cnst) % qi);
		SetCoeff(cipher.c1, i, (coeff(cipher.c1,i) * cnst) % qi);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
}

FPHECipher FPHEScheme::modSwitch(FPHECipher& cipher, long newLevel) {
	ZZ& divFactor = params.qi[newLevel-cipher.level-1];

	ZZX c0;
	ZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(c0, i, coeff(cipher.c0,i) / divFactor);
		SetCoeff(c1, i, coeff(cipher.c1,i) / divFactor);
	}
	c0.normalize();
	c1.normalize();
	FPHECipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void FPHEScheme::modSwitchAndEqual(FPHECipher& cipher, long newLevel) {
	ZZ& divFactor = params.qi[newLevel-cipher.level-1];

	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(cipher.c0, i, coeff(cipher.c0,i) / divFactor);
		SetCoeff(cipher.c1, i, coeff(cipher.c1,i) / divFactor);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;
}

FPHECipher FPHEScheme::modEmbed(FPHECipher& cipher, long newLevel) {
	ZZ& newQi = getModulo(newLevel);

	ZZX c0;
	ZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(c0, i, coeff(cipher.c0,i) % newQi);
		SetCoeff(c1, i, coeff(cipher.c1,i) % newQi);
	}
	c0.normalize();
	c1.normalize();
	FPHECipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void FPHEScheme::modEmbedAndEqual(FPHECipher& cipher, long newLevel) {
	ZZ& newQi = getModulo(newLevel);
	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(cipher.c0, i, coeff(cipher.c0,i) % newQi);
		SetCoeff(cipher.c1, i, coeff(cipher.c1,i) % newQi);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;
}

ZZ& FPHEScheme::getModulo(long level) {
	return params.qi[params.levels - level];
}
