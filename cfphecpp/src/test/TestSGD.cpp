#include "TestSGD.h"

#include <NTL/ZZ.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

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
	string filePath = "data.txt";

	long dim = 0; 		// dimension of x
	long sampledim = 0;	// number of samples

	ifstream openFile(filePath.data());
	vector< vector<int> > data_X;
	vector<int> data_Y;

	if( openFile.is_open() ){
		string line;

		// counting dimension
		getline(openFile, line);
		for(int i = 0; i < line.length(); ++i){
			if( line[i] == ',' ) dim++;
		}

		// read and store data
		while(getline(openFile, line)){
			if( line.length() != 2 * dim + 1 ){ cout << "Error: data format" << endl; break; }

			// read y
			if( line[0] == '0' ) data_Y.push_back(-1);
			else if( line[0] == '1' ) data_Y.push_back(1);
			else{ cout << "Error: data value" << endl; break; }

			// read x
			vector<int> vec_line;
			for(int i = 2; i < 2 * dim + 1; i += 2){
				if( line[i] == '0' ) vec_line.push_back(0);
				else if( line[i] == '1' ) vec_line.push_back(1);
				else{ cout << "Error: data value" << endl; break; }
			}
			data_X.push_back(vec_line);
			sampledim++;
		}
		openFile.close();
	}
	else{ cout << "Error: cannot read file" << endl; }
	cout << "Data dimension: " << dim << ", Number of samples: " << sampledim << "\n\n";

	long tmpsampledim = (1 << (long)log2(sampledim));
	long truesampledim = tmpsampledim < sampledim ? tmpsampledim << 1 : tmpsampledim;

	CZZ** xsample = new CZZ*[dim];
	CZZ* ysample = new CZZ[truesampledim];
	CZZ* widx = new CZZ[dim];

	for (long i = 0; i < dim; ++i) {
		xsample[i] = new CZZ[truesampledim];
		widx[i] = CZZ();
	}

	for (long j = 0; j < sampledim; ++j) {
		for (long i = 0; i < dim; ++i) {
			xsample[i][j] = data_X[i][j] == 0 ? CZZ() : CZZ(params.p);
		}
		ysample[j] = data_Y[j] == 0 ? CZZ() : CZZ(params.p);
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
	long bits = 20;
	for (long k = 0; k < itter; ++k) {
		timeutils.start("SGD");
		Cipher* grad = sgd.grad(ycipher, xcipher, wcipher, dim, truesampledim);
		timeutils.stop("SGD");

		for (long i = 0; i < dim; ++i) {
			scheme.leftShiftAndEqual(grad[i], bits);
			scheme.modSwitchAndEqual(grad[i]);
			scheme.modEmbedAndEqual(wcipher[i], grad[i].level);
			scheme.addAndEqual(wcipher[i], grad[i]);
		}

		CZZ* dwidx = scheme.decryptFullSingleArray(wcipher, dim);
		StringUtils::showcompare(widx, dwidx, 2, "sgd");
		widx = dwidx;
		wcipher = scheme.encryptFullSingleArray(widx, dim);
	}

	cout << "!!! END TEST SGD !!!" << endl;
}
