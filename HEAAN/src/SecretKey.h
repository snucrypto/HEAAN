#ifndef HEAAN_SECRETKEY_H_
#define HEAAN_SECRETKEY_H_

#include <NTL/ZZX.h>

#include <fstream>

//#include "Functions.h"
#include "Params.h"
#include "NumUtils.h"


using namespace std;
using namespace NTL;

class SecretKey {
public:

	ZZX sx; ///< secret key

	SecretKey(Params& params);

	void Write(int SecretKeyID);

	void Read(int SecretKeyID);
};

#endif
