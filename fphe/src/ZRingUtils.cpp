#include "ZRingUtils.h"

#include <cassert>
#include <cmath>

using namespace std;
using namespace NTL;

void ZRingUtils::addRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi) {
	long i;
	ZZX add;
	ZZ c;
	add.SetLength(deg(phi));
	for (i = 0; i < deg(phi); ++i) {
		c = (coeff(poly1, i) + coeff(poly2, i)) % mod;
		SetCoeff(add, i, c);
	}
	add.normalize();
	res = add;
}

void ZRingUtils::subRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi) {
	long i;
	ZZX sub;
	ZZ c;
	sub.SetLength(deg(phi));
	for (i = 0; i < deg(phi); ++i) {
		c = (coeff(poly1, i) - coeff(poly2, i)) % mod;
		SetCoeff(sub, i, c);
	}
	sub.normalize();
	res = sub;
}

void ZRingUtils::mulRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi) {
	long i;
	ZZ c;
	ZZX poly;
	mul(poly, poly1, poly2);

	for (i = 0; i < deg(poly); ++i) {
		c = coeff(poly, i) % mod;
		SetCoeff(poly, i, c);
	}

	PseudoRem(res, poly, phi);
	//res = poly % phi;

	for (i = 0; i < deg(phi); ++i) {
		c = coeff(res, i) % mod;
		SetCoeff(res, i, c);
	}
	res.normalize();
}

void ZRingUtils::mulByConstantRing(ZZX& res, ZZX& poly, const ZZ& cnst, const ZZ& mod, const ZZX& phi) {
	long i;
	ZZX mul;
	ZZ c;
	mul.SetLength(deg(phi));
	for (i = 0; i < deg(phi); ++i) {
		c = (coeff(poly, i) * cnst) % mod;
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}

void ZRingUtils::rightShiftRing(ZZX& res, ZZX& poly, const long& bits, const ZZ& mod, const ZZX& phi) {
	long i;
	ZZX mul;
	ZZ c;
	mul.SetLength(deg(phi));
	for (i = 0; i < deg(phi); ++i) {
		c = (coeff(poly, i) >> bits) % mod;
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}

void ZRingUtils::leftShiftRing(ZZX& res, ZZX& poly, const long& bits, const ZZ& mod, const ZZX& phi) {
	long i;
	ZZX mul;
	ZZ c;
	mul.SetLength(deg(phi));
	for (i = 0; i < deg(phi); ++i) {
		c = (coeff(poly, i) << bits) % mod;
		SetCoeff(mul, i, c);
	}
	mul.normalize();
	res = mul;
}


void ZRingUtils::bitPoly(ZZX& res, ZZX& poly, long i) {
	ZZX bitPoly;
	ZZ c;
	bitPoly.SetLength(deg(poly) + 1);
	long j;
	for (j = 0; j < deg(poly) + 1; ++j) {
		c = bit(coeff(poly, j), i);
		SetCoeff(bitPoly, j, c);
	}
	bitPoly.normalize();
	res = bitPoly;
}

void ZRingUtils::wordPoly(ZZX& res, ZZX& poly, long i) {
	ZZX wordPoly;
	long c;
	wordPoly.SetLength(deg(poly) + 1);
	long j;
	for (j = 0; j < deg(poly) + 1; ++j) {
		c = bit(coeff(poly, j), i);
		SetCoeff(wordPoly, j, c);
	}
	wordPoly.normalize();
	res = wordPoly;
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

void ZRingUtils::sampleGaussian(ZZX &res, long d, double stdev) {
  static double const Pi=4.0*atan(1.0); // Pi=3.1415..
  static long const bignum = 0xfffffff;
  // THREADS: C++11 guarantees these are initialized only once

  if (d<=0) d=deg(res)+1; if (d<=0) return;
  res.SetMaxLength(d); // allocate space for degree-(n-1) polynomial
  for (long i=0; i<d; i++) SetCoeff(res, i, ZZ::zero());

  // Uses the Box-Muller method to get two Normal(0,stdev^2) variables
  for (long i=0; i<d; i+=2) {
    double r1 = (1+RandomBnd(bignum))/((double)bignum+1);
    double r2 = (1+RandomBnd(bignum))/((double)bignum+1);
    double theta=2*Pi*r1;
    double rr= sqrt(-2.0*log(r2))*stdev;

    assert(rr < 8*stdev); // sanity-check, no more than 8 standard deviations

    // Generate two Gaussians RV's, rounded to integers
    long x = (long) floor(rr*cos(theta) +0.5);
    SetCoeff(res, i, x);
    if (i+1 < d) {
      x = (long) floor(rr*sin(theta) +0.5);
      SetCoeff(res, i+1, x);
    }
  }
  res.normalize(); // need to call this after we work on the coeffs
}

void ZRingUtils::sampleUniform(ZZX& res, ZZ& B, long d) {

	if (d<=0) d=deg(res)+1; if (d<=0) return;
	if (B <= 0) {
		clear(res);
		return;
	}

	res.SetMaxLength(d); // allocate space for degree-(n-1) polynomial

	ZZ UB, tmp;

	UB =  2*B + 1;

	for (long i = 0; i < d; i++) {
		RandomBnd(tmp, UB);
		tmp -= B;
		SetCoeff(res, i, tmp);
	}
	res.normalize();
}
