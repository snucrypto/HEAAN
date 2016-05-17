#include <iostream>

#include "FPHEParams.h"
#include "FPHEPubKey.h"
#include "FPHEScheme.h"
#include "FPHESecKey.h"
#include "FPHECipher.h"

using namespace std;

int main() {

	cout << "!!!HELLO FPHE!!!" << endl; // prints !!!Hello World!!!

	long lambda = 10;
	FPHEParams params(lambda, false);
	FPHESecKey secretKey(params);
	FPHEPubKey publicKey(params, secretKey);
	FPHEScheme scheme(params, secretKey, publicKey);

	cout << "------------------" << endl;
	cout << "sk: " << secretKey.s << endl;
	cout << params.toString() << endl;
	cout << "------------------" << endl;


	ZZ m = ZZ(857465976520445);
	ZZ mm = m + m;
	ZZ m2 = m * m;
	ZZ ms = m2 / params.p;
	ZZ me = m2;
	ZZ ms2 = ms * ms;
	ZZ mss = ms2 / params.p;
	ZZ mse = ms2;

	cout << "------------------" << endl;
	cout << "m:  " << m << endl;
	cout << "mm: " << mm << endl;
	cout << "m2: " << m2 << endl;
	cout << "ms: " << ms << endl;
	cout << "me: " << me << endl;
	cout << "ms2: " << ms2 << endl;
	cout << "mss: " << mss << endl;
	cout << "mse: " << mse << endl;
	cout << "------------------" << endl;

	FPHECipher c = scheme.encrypt(m);
	FPHECipher cc = scheme.add(c, c);
	FPHECipher c2 = scheme.mul(c, c);
	FPHECipher cs = scheme.modSwitch(c2, 2);
	FPHECipher ce = scheme.modEmbed(c2, 2);
	FPHECipher cs2 = scheme.mul(cs, cs);
	FPHECipher css = scheme.modSwitch(cs2, 3);
	FPHECipher cse = scheme.modEmbed(cs2, 3);

	cout << "------------------" << endl;
	cout << "c:  " << c.toString() << endl;
	cout << "cc: " << cc.toString() << endl;
	cout << "c2: " << c2.toString() << endl;
	cout << "cs: " << cs.toString() << endl;
	cout << "ce: " << ce.toString() << endl;
	cout << "cs2: " << cs2.toString() << endl;
	cout << "css: " << css.toString() << endl;
	cout << "cse: " << cse.toString() << endl;
	cout << "------------------" << endl;

	ZZ d = scheme.decrypt(c);
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
