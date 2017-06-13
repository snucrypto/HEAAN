#include "TestSGD.h"

#include <NTL/ZZ.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include <NTL/BasicThreadPool.h>

#include "../czz/CZZ.h"
#include "../scheme/Cipher.h"
#include "../scheme/Params.h"
#include "../scheme/PubKey.h"
#include "../scheme/Scheme.h"
#include "../scheme/SchemeAlgo.h"
#include "../scheme/SecKey.h"
#include "../sgd/SGD.h"
#include "../utils/StringUtils.h"
#include "../utils/TimeUtils.h"

using namespace NTL;

//-----------------------------------------

void TestSGD::testSGD(long logN, long logl, long logp, long L) {
	cout << "!!! START TEST SGD !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, secretKey, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	SGD sgd(scheme, algo);
	//-----------------------------------------
	SetNumThreads(8);
	string filename = "data.txt";
	vector<vector<long>> xdata;
	vector<long> ydata;

	long dim = 0; 		// dimension of x
	long sampledim = 0;	// number of samples

	ifstream openFile(filename.data());
	if(openFile.is_open()) {
		string line;
		getline(openFile, line);

		for(long i = 0; i < line.length(); ++i) if(line[i] == ',' ) dim++;

		while(getline(openFile, line)){
			if(line.length() != 2 * dim + 1 ) {
				cout << "Error: data format" << endl;
				break;
			}
			if(line[0] == '0') {
				ydata.push_back(-1);
			} else if(line[0] == '1') {
				ydata.push_back(1);
			} else {
				cout << "Error: data value" << endl;
				break;
			}
			vector<long> vecline;
			for(long i = 2; i < 2 * dim + 1; i += 2) {
				if(line[i] == '0') {
					vecline.push_back(0);
				} else if(line[i] == '1') {
					vecline.push_back(1);
				} else{
					cout << "Error: data value" << endl;
					break;
				}
			}
			xdata.push_back(vecline);
			sampledim++;
		}
		openFile.close();
	} else {
		cout << "Error: cannot read file" << endl;
	}
	cout << "Data dimension: " << dim << ", Number of samples: " << sampledim << "\n\n";

	long tmpsampledim = (1 << (long)log2(sampledim));
	long truesampledim = tmpsampledim < sampledim ? tmpsampledim << 1 : tmpsampledim;

	long bits = 15;
	long xbits = 7;

	CZZ** xsample = new CZZ*[dim];
	CZZ* ysample = new CZZ[truesampledim];
	CZZ* widx = new CZZ[dim];

	for (long i = 0; i < dim; ++i) {
		xsample[i] = new CZZ[truesampledim];
		widx[i] = CZZ();
	}

	for (long j = 0; j < sampledim; ++j) {
		for (long i = 0; i < dim; ++i) {
			xsample[i][j] = xdata[i][j] == 0 ? CZZ() : CZZ((params.p >> xbits));
		}
		ysample[j] = ydata[j] == 0 ? CZZ() : CZZ(params.p);
	}
	for (long j = sampledim; j < truesampledim; ++j) {
		for (long i = 0; i < dim; ++i) {
			xsample[i][j] = CZZ();
		}
		ysample[j] = CZZ();
	}

	Cipher* xcipher = new Cipher[dim];
	for (long i = 0; i < dim; ++i) {
		xcipher[i] = scheme.encryptFull(xsample[i], truesampledim);
	}
	Cipher ycipher = scheme.encryptFull(ysample, truesampledim);
	Cipher* wcipher = scheme.encryptFullSingleArray(widx, dim);

	long itter = 10;
	for (long k = 0; k < itter; ++k) {
		timeutils.start("SGD");
		Cipher* cgrad = sgd.grad(ycipher, xcipher, wcipher, dim, truesampledim);
		timeutils.stop("SGD");

		NTL_EXEC_RANGE(dim, first, last);
		for (long i = first; i < last; ++i) {
			scheme.leftShiftAndEqual(cgrad[i], bits);
			scheme.modSwitchOneAndEqual(cgrad[i]);
			scheme.modEmbedAndEqual(wcipher[i], cgrad[i].level);
			scheme.addAndEqual(wcipher[i], cgrad[i]);
		}
		NTL_EXEC_RANGE_END;

		CZZ* dwidx = scheme.decryptFullSingleArray(wcipher, dim);
		StringUtils::showcompare(widx, dwidx, 2, "sgd");
		widx = dwidx;
		wcipher = scheme.encryptFullSingleArray(widx, dim);
	}

	cout << "!!! END TEST SGD !!!" << endl;
}
