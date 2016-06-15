#include "PolyScheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>

#include "../utils/PolyRingUtils.h"
#include "PolyCipher.h"

using namespace std;
using namespace NTL;

PolyCipher PolyScheme::encrypt(ZZ& m) {
	ZZ bits;
	RandomBits(bits, params.tau);
	ZZX c0;
	ZZX c1;

	long i;
	for (i = 0; i < params.tau; ++i) {
		if(bit(bits, i)) {
			ZZX lwe0 = publicKey.A0[i];
			ZZX lwe1 = publicKey.A1[i];

			PolyRingUtils::addPolyRing(c0, c0, lwe0, params.qL, params.phi);
			PolyRingUtils::addPolyRing(c1, c1, lwe1, params.qL, params.phi);
		}
	}
	SetCoeff(c0, 0, (coeff(c0, 0) + m) % params.qL);
	c0.normalize();
	PolyCipher cipher(c0, c1, 1);
	return cipher;
}

ZZ PolyScheme::decrypt(PolyCipher& cipher) {

	ZZ& qi = getModulo(cipher.level);

	ZZX m;
	PolyRingUtils::mulPolyRing(m, secretKey.s, cipher.c1, qi, params.phi);
	PolyRingUtils::addPolyRing(m, m, cipher.c0, qi, params.phi);
	ZZ c;
	GetCoeff(c, m, 0);
	if(c < 0) {c += qi;}
	return c;
}

PolyCipher PolyScheme::add(PolyCipher& cipher1, PolyCipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	ZZX c0;
	ZZX c1;
	PolyRingUtils::addPolyRing(c0, cipher1.c0, cipher2.c0, qi, params.phi);
	PolyRingUtils::addPolyRing(c1, cipher1.c1, cipher2.c1, qi, params.phi);
	PolyCipher res(c0, c1, cipher1.level);
	return res;
}

void PolyScheme::addAndEqual(PolyCipher& cipher1, PolyCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	PolyRingUtils::addPolyRing(cipher1.c0, cipher1.c0, cipher2.c0, qi, params.phi);
	PolyRingUtils::addPolyRing(cipher1.c1, cipher1.c1, cipher2.c1, qi, params.phi);
}

PolyCipher PolyScheme::sub(PolyCipher& cipher1, PolyCipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	ZZX c0;
	ZZX c1;
	PolyRingUtils::subPolyRing(c0, cipher1.c0, cipher2.c0, qi, params.phi);
	PolyRingUtils::subPolyRing(c1, cipher1.c1, cipher2.c1, qi, params.phi);
	PolyCipher res(c0, c1, cipher1.level);
	return res;
}

void PolyScheme::subAndEqual(PolyCipher& cipher1, PolyCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	PolyRingUtils::subPolyRing(cipher1.c0, cipher1.c0, cipher2.c0, qi, params.phi);
	PolyRingUtils::subPolyRing(cipher1.c1, cipher1.c1, cipher2.c1, qi, params.phi);
}

PolyCipher PolyScheme::mul(PolyCipher& cipher1, PolyCipher& cipher2) {
	long i, j, idx;
	ZZ qi = getModulo(cipher1.level);
	ZZ Pqi = getPqModulo(cipher1.level);

	ZZX cc00;
	ZZX cc01;
	ZZX cc10;
	ZZX cc11;

	ZZX mulC1;
	ZZX mulC0;
	PolyRingUtils::mulPolyRing(cc00, cipher1.c0, cipher2.c0, qi, params.phi);
	PolyRingUtils::mulPolyRing(cc01, cipher1.c0, cipher2.c1, qi, params.phi);
	PolyRingUtils::mulPolyRing(cc10, cipher1.c1, cipher2.c0, qi, params.phi);
	PolyRingUtils::mulPolyRing(cc11, cipher1.c1, cipher2.c1, qi, params.phi);

	PolyRingUtils::mulPolyRing(mulC1, publicKey.aStar, cc11, Pqi, params.phi);
	PolyRingUtils::mulPolyRing(mulC0, publicKey.bStar, cc11, Pqi, params.phi);

	PolyRingUtils::rightShiftPolyRing(mulC1, mulC1, params.Pbits, qi, params.phi);
	PolyRingUtils::rightShiftPolyRing(mulC0, mulC0, params.Pbits, qi, params.phi);

	PolyRingUtils::addPolyRing(mulC1, mulC1, cc10, qi, params.phi);
	PolyRingUtils::addPolyRing(mulC1, mulC1, cc01, qi, params.phi);
	PolyRingUtils::addPolyRing(mulC0, mulC0, cc00, qi, params.phi);

	PolyCipher cipher(mulC0, mulC1, cipher1.level);
	return cipher;
}

void PolyScheme::mulAndEqual(PolyCipher& cipher1, PolyCipher& cipher2) {
	long i, j, idx;
	ZZ qi = getModulo(cipher1.level);
	ZZ Pqi = getPqModulo(cipher1.level);

	ZZX cc00;
	ZZX cc01;
	ZZX cc10;
	ZZX cc11;

	ZZX mulC1;
	ZZX mulC0;

	PolyRingUtils::mulPolyRing(cc00, cipher1.c0, cipher2.c0, qi, params.phi);
	PolyRingUtils::mulPolyRing(cc01, cipher1.c0, cipher2.c1, qi, params.phi);
	PolyRingUtils::mulPolyRing(cc10, cipher1.c1, cipher2.c0, qi, params.phi);
	PolyRingUtils::mulPolyRing(cc11, cipher1.c1, cipher2.c1, qi, params.phi);

	PolyRingUtils::mulPolyRing(mulC1, publicKey.aStar, cc11, Pqi, params.phi);
	PolyRingUtils::mulPolyRing(mulC0, publicKey.bStar, cc11, Pqi, params.phi);

	PolyRingUtils::rightShiftPolyRing(mulC1, mulC1, params.Pbits, qi, params.phi);
	PolyRingUtils::rightShiftPolyRing(mulC0, mulC0, params.Pbits, qi, params.phi);

	PolyRingUtils::addPolyRing(mulC1, mulC1, cc10, qi, params.phi);
	PolyRingUtils::addPolyRing(mulC1, mulC1, cc01, qi, params.phi);
	PolyRingUtils::addPolyRing(mulC0, mulC0, cc00, qi, params.phi);

	cipher1.c0 = mulC0;
	cipher1.c1 = mulC1;
}

PolyCipher PolyScheme::addConstant(PolyCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	ZZX c0 = cipher.c0;
	ZZX c1 = cipher.c1;
	SetCoeff(c1, 0, (coeff(cipher.c1,0) + cnst) % qi);
	c0.normalize();
	PolyCipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

PolyCipher PolyScheme::mulByConstant(PolyCipher& cipher, ZZ& cnst) {
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
	PolyCipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

void PolyScheme::addConstantAndEqual(PolyCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);
	long i;
	SetCoeff(cipher.c0, 0, (coeff(cipher.c0, 0) * cnst) % qi);
	cipher.c0.normalize();
}

void PolyScheme::mulByConstantAndEqual(PolyCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(cipher.c0, i, (coeff(cipher.c0,i) * cnst) % qi);
		SetCoeff(cipher.c1, i, (coeff(cipher.c1,i) * cnst) % qi);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
}

PolyCipher PolyScheme::modSwitch(PolyCipher& cipher, long newLevel) {
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
	PolyCipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void PolyScheme::modSwitchAndEqual(PolyCipher& cipher, long newLevel) {
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

PolyCipher PolyScheme::modEmbed(PolyCipher& cipher, long newLevel) {
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
	PolyCipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void PolyScheme::modEmbedAndEqual(PolyCipher& cipher, long newLevel) {
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

ZZ& PolyScheme::getModulo(long level) {
	return params.qi[params.levels - level];
}

ZZ& PolyScheme::getPqModulo(long level) {
	return params.Pqi[params.levels - level];
}
