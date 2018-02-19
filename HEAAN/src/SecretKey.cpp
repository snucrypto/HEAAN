#include "SecretKey.h"

SecretKey::SecretKey(long logN, long h) {
	long N = 1 << logN;
	NumUtils::sampleHWT(sx, N, h);
}
