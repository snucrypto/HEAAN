#include "ZRingUtils.h"

#include <cassert>
#include <cmath>
#include <vector>

using namespace std;
using namespace NTL;

void toString(vector<ZZ> v) {
	cout << "[";
	for (long i = 0; i < v.size(); ++i) {
		cout << v[i] << ", ";
	}
	cout << "]" << endl;
}

void ZRingUtils::addRing(vector<ZZ>& res, vector<ZZ>& fft1, vector<ZZ>& fft2, const ZZ& mod, const long& phim) {
	long i;
	vector<ZZ> add;
	for (i = 0; i < phim; ++i) {
		ZZ c = (fft1[i] + fft2[i]) % mod;
		add.push_back(c);
	}
	res = add;
}

void ZRingUtils::addConstantRing(vector<ZZ>& res, vector<ZZ>& fft1, const ZZ& cnst, const ZZ& mod, const long& phim) {
	long i;
	vector<ZZ> add;
	for (i = 0; i < phim; ++i) {
		ZZ c = (fft1[i] + cnst) % mod;
		add.push_back(c);
	}
	res = add;
}

void ZRingUtils::subRing(vector<ZZ>& res, vector<ZZ>& fft1, vector<ZZ>& fft2, const ZZ& mod, const long& phim) {
	long i;
	vector<ZZ> sub;
	for (i = 0; i < phim; ++i) {
		ZZ c = (fft1[i] - fft2[i]) % mod;
		sub.push_back(c);
	}
	res = sub;
}

void ZRingUtils::mulRing(vector<ZZ>& res, vector<ZZ>& fft1, vector<ZZ>& fft2, const ZZ& mod, const long& phim) {
	long i;
	vector<ZZ> mul;
	for (i = 0; i < phim; ++i) {
		ZZ c = (fft1[i] * fft2[i]) % mod;
		mul.push_back(c);
	}
	res = mul;
}

void ZRingUtils::mulByConstantRing(vector<ZZ>& res, vector<ZZ>& fft, const ZZ& cnst, const ZZ& mod, const long& phim) {
	long i;
	vector<ZZ> mul;
	for (i = 0; i < phim; ++i) {
		ZZ c = (fft[i] * cnst) % mod;
		mul.push_back(c);
	}
	res = mul;
}

void ZRingUtils::divByConstantRing(vector<ZZ>& res, vector<ZZ>& fft, const ZZ& cnst, const ZZ& mod, const long& phim) {
	long i;
	vector<ZZ> mul;
	for (i = 0; i < phim; ++i) {
		ZZ c = fft[i] / cnst;
		mul.push_back(c);
	}
	res = mul;
}

void ZRingUtils::rightShiftRing(vector<ZZ>& res, vector<ZZ>& fft, const long& bits, const ZZ& mod, const long& phim) {
	long i;
	vector<ZZ> mul;
	for (i = 0; i < phim; ++i) {
		ZZ c = (fft[i] >> bits) % mod;
		mul.push_back(c);
	}
	res = mul;
}

void ZRingUtils::leftShiftRing(vector<ZZ>& res, vector<ZZ>& poly, const long& bits, const ZZ& mod, const long& phim) {
	long i;
	vector<ZZ> mul;
	for (i = 0; i < phim; ++i) {
		ZZ c = (poly[i] << bits) % mod;
		mul.push_back(c);
	}
	res = mul;
}

void ZRingUtils::factorize(vector<long> &factors, const long &N) {
	factors.resize(0);
	if(N<2) return;
	PrimeSeq s;
	long n = N;
	while (true) {
		if(ProbPrime(n)) {
			factors.push_back(n);
			return;
		}
		long p = s.next();
		if((n%p) == 0) {
			factors.push_back(p);
			do { n /= p; } while ((n%p)==0);
		}
		if (n == 1) return;
	}
}

long ZRingUtils::primitiveRoot(const long& p) {
	long i;

	long phi = p-1;
	vector<long> facts;
	PrimeSeq s;

	bool flag;
	long g = 1;
	factorize(facts, phi);

	while(!flag) {
		g++;
		flag = true;
		for (i = 0; i < facts.size(); i++) {
			long f = facts[i];
			long div = phi / f;
			if(PowerMod(f, div, p) == 1) {
				flag = false;
				break;
			}
		}
	}
	return g;
}

long ZRingUtils::mobius(long n) {
  long p,e,arity=0;
  PrimeSeq s;
  while (n!=1)
    { p=s.next();
      e=0;
      while ((n%p==0)) { n=n/p; e++; }
      if (e>1) { return 0; }
      if (e!=0) { arity^=1; }
    }
  if (arity==0) { return 1; }
  return -1;
}

ZZX ZRingUtils::Cyclotomic(long N) {
  ZZX Num,Den,G,F;
  set(Num); set(Den);
  long m,d;
  for (d=1; d<=N; d++)
    { if ((N%d)==0)
         { clear(G);
           SetCoeff(G,N/d,1); SetCoeff(G,0,-1);
           m=ZRingUtils::mobius(d);
           if (m==1)       { Num*=G; }
           else if (m==-1) { Den*=G; }
         }
    }
  F=Num/Den;
  return F;
}

void ZRingUtils::sampleGaussian(vector<ZZ> &res, long d, double stdev) {
	static double const Pi=4.0*atan(1.0); // Pi=3.1415..
	static long const bignum = 0xfffffff;

	res.resize(0);

	for (long i=0; i<d; i+=2) {
		double r1 = (1+RandomBnd(bignum))/((double)bignum+1);
		double r2 = (1+RandomBnd(bignum))/((double)bignum+1);
		double theta=2*Pi*r1;
		double rr= sqrt(-2.0*log(r2))*stdev;

		assert(rr < 8*stdev); // sanity-check, no more than 8 standard deviations

		// Generate two Gaussians RV's, rounded to integers
		long x = (long) floor(rr*cos(theta) +0.5);
		res.push_back(ZZ(x));
		if (i+1 < d) {
		  x = (long) floor(rr*sin(theta) +0.5);
		  res.push_back(ZZ(x));
		}
	}
}

void ZRingUtils::sampleUniform(vector<ZZ>& res, const ZZ& B, const long d) {
	res.resize(0);
	ZZ UB;
	UB =  2*B + 1;
	for (long i = 0; i < d; i++) {
		ZZ tmp;
		RandomBnd(tmp, UB);
		tmp -= B;
		res.push_back(tmp);
	}
}

void ZRingUtils::convertfft(vector<ZZ>& res, const vector<ZZ>& poly, const vector<  vector<ZZ>  >& fftMatrix, const ZZ& mod, const long& phim) {
	res.resize(0);
	long i, j;
	for (i = 0; i < phim; ++i) {
		ZZ f;
		vector<ZZ> fftRowi = fftMatrix[i];
		for (j = 0; j < phim; ++j) {
			f += poly[j] * fftRowi[j];
			f %= mod;
		}
		res.push_back(f);
	}
}

void ZRingUtils::convertfftInv(vector<ZZ>& res, const vector<ZZ>& fft, const vector<  vector<ZZ>  >& fftInvMatrix, const ZZ& mod, const long& phim) {
	res.resize(0);
	long i, j;
	for (i = 0; i < phim; ++i) {
		ZZ f;
		vector<ZZ> fftRowi = fftInvMatrix[i];
		for (j = 0; j < phim; ++j) {
			f += fft[j] * fftRowi[j];
			f %= mod;
		}
		res.push_back(f);
	}
}





