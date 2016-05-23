#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <string>

#include "FPHECipher.h"
#include "FPHEParams.h"
#include "FPHEPubKey.h"
#include "FPHEScheme.h"
#include "FPHESecKey.h"
#include "TimeUtils.h"

using namespace std;

int main() {

	TimeUtils timeutils;

	cout << "!!!HELLO FPHE!!!" << endl; // prints !!!Hello World!!!

	long lambda = 10;

	cout << "------------------" << endl;

	timeutils.start("GenParams");
	FPHEParams params(lambda, false);
	timeutils.stop("GenParams");

	cout << "------------------" << endl;

	timeutils.start("GenSecKey");
	FPHESecKey secretKey(params);
	timeutils.stop("GenSecKey");

	cout << "------------------" << endl;

	timeutils.start("GenPubKey");
	FPHEPubKey publicKey(params, secretKey);
	timeutils.stop("GenPubKey");

	cout << "------------------" << endl;

	timeutils.start("GenScheme");
	FPHEScheme scheme(params, secretKey, publicKey);
	timeutils.stop("GenScheme");

	cout << "------------------" << endl;

//	cout << "sk: " << secretKey.s << endl;
//	cout << params.toString() << endl;

	cout << "------------------" << endl;

	ZZ m = ZZ(779790);
	ZZ mm = m + m;
	ZZ m2 = m * m;
	ZZ ms = m2 / params.p;
	ZZ me = m2;
	ZZ ms2 = ms * ms;
	ZZ mss = ms2 / params.p;
	ZZ mse = ms2;

	cout << "------------------" << endl;

	cout << " m:  " << m << endl;
	cout << " mm: " << mm << endl;
	cout << " m2: " << m2 << endl;
	cout << " ms: " << ms << endl;
	cout << " me: " << me << endl;
	cout << " ms2: " << ms2 << endl;
	cout << " mss: " << mss << endl;
	cout << " mse: " << mse << endl;

	cout << "------------------" << endl;

	timeutils.start("Encrypt c");
	FPHECipher c = scheme.encrypt(m);
	timeutils.stop("Encrypt c");

	cout << "------------------" << endl;

	timeutils.start("Add cc");
	FPHECipher cc = scheme.add(c, c);
	timeutils.stop("Add cc");

	cout << "------------------" << endl;

	timeutils.start("Mul c2");
	FPHECipher c2 = scheme.mul(c, c);
	timeutils.stop("Mul c2");

	cout << "------------------" << endl;

	timeutils.start("MS cs");
	FPHECipher cs = scheme.modSwitch(c2, 2);
	timeutils.stop("MS cs");

	cout << "------------------" << endl;

	timeutils.start("ME ce");
	FPHECipher ce = scheme.modEmbed(c2, 2);
	timeutils.stop("ME ce");

	cout << "------------------" << endl;

	timeutils.start("Mul cs2");
	FPHECipher cs2 = scheme.mul(cs, cs);
	timeutils.stop("Mul cs2");

	cout << "------------------" << endl;

	timeutils.start("MS css");
	FPHECipher css = scheme.modSwitch(cs2, 3);
	timeutils.stop("MS css");

	cout << "------------------" << endl;

	timeutils.start("ME cse");
	FPHECipher cse = scheme.modEmbed(cs2, 3);
	timeutils.stop("ME cse");

	cout << "------------------" << endl;

//	cout << "c:  " << c.toString() << endl;
//	cout << "cc: " << cc.toString() << endl;
//	cout << "c2: " << c2.toString() << endl;
//	cout << "cs: " << cs.toString() << endl;
//	cout << "ce: " << ce.toString() << endl;
//	cout << "cs2: " << cs2.toString() << endl;
//	cout << "css: " << css.toString() << endl;
//	cout << "cse: " << cse.toString() << endl;

	cout << "------------------" << endl;

	timeutils.start("Decrypt c");
	ZZ d = scheme.decrypt(c);
	timeutils.stop("Decrypt c");

	cout << "------------------" << endl;

	ZZ dd = scheme.decrypt(cc);
	ZZ d2 = scheme.decrypt(c2);
	ZZ ds = scheme.decrypt(cs);
	ZZ de = scheme.decrypt(ce);
	ZZ ds2 = scheme.decrypt(cs2);
	ZZ dss = scheme.decrypt(css);
	ZZ dse = scheme.decrypt(cse);

	cout << "------------------" << endl;

	cout << "qL: " << params.qL << endl;
	cout << "p: " << params.p << endl;
	cout << "d:  " << d << endl;
	cout << "dd: " << dd << endl;
	cout << "d2: " << d2 << endl;
	cout << "ds: " << ds << endl;
	cout << "de: " << de << endl;
	cout << "ds2: " << ds2 << endl;
	cout << "dss: " << dss << endl;
	cout << "dse: " << dse << endl;

	cout << "------------------" << endl;

	ZZ e = m - d;
	ZZ ee = mm - dd;
	ZZ e2 = m2 - d2;
	ZZ es = ms - ds;
	ZZ eE = me - de;
	ZZ es2 = ms2 - ds2;
	ZZ ess = mss - dss;
	ZZ ese = mse - dse;

	cout << "------------------" << endl;
	cout << "m:  " << m << endl;
	cout << "d:  " << d << endl;
	cout << "e:  " << e << endl;
	cout << "------------------" << endl;
	cout << "mm: " << mm << endl;
	cout << "dd: " << dd << endl;
	cout << "ee: " << ee << endl;
	cout << "------------------" << endl;
	cout << "m2: " << m2 << endl;
	cout << "d2: " << d2 << endl;
	cout << "e2: " << e2 << endl;
	cout << "------------------" << endl;
	cout << "ms: " << ms << endl;
	cout << "ds: " << ds << endl;
	cout << "es: " << es << endl;
	cout << "------------------" << endl;
	cout << "me: " << me << endl;
	cout << "de: " << de << endl;
	cout << "eE: " << eE << endl;
	cout << "------------------" << endl;
	cout << "ms2: " << ms2 << endl;
	cout << "ds2: " << ds2 << endl;
	cout << "es2: " << es2 << endl;
	cout << "------------------" << endl;
	cout << "mss: " << mss << endl;
	cout << "dss: " << dss << endl;
	cout << "ess: " << ess << endl;
	cout << "------------------" << endl;
	cout << "mse: " << mse << endl;
	cout << "dse: " << dse << endl;
	cout << "ese: " << ese << endl;
	cout << "------------------" << endl;

	cout << "!!!BYE FPHE!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
