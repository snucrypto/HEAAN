#include "SecretKey.h"

SecretKey::SecretKey(Params& params) {
	NumUtils::sampleHWT(sx, params.N, params.h);
}

void SecretKey::Write(int SecretKeyID)
{
	ofstream myfile;
	myfile.open("SecretKey" + to_string(SecretKeyID) + ".txt");
	myfile << "SecretKey Data" << endl;
	myfile << "ID = " << SecretKeyID << endl;
	myfile << deg(sx) << endl;
	for(int i = 0; i < deg(sx) + 1; i++) {
		myfile << sx[i] << endl;
	}
	myfile.close();
}

void SecretKey::Read(int SecretKeyID)
{
	ifstream myfile("SecretKey" + to_string(SecretKeyID) + ".txt");
	if(myfile.is_open())
	{
		// kill previous memory
		sx.kill();
		// start reading
		int temp;
		string line;
		// pass first two lines
		getline(myfile, line); getline(myfile, line);
		// read 3rd line and get degree of sx
		getline(myfile, line);
		temp = atoi(line.c_str());
		this->sx.SetLength(temp + 1);
		// read other lines and get sx
		for(long i = 0; i < deg(sx) + 1; i++){
			getline(myfile, line);
			if(line == "0"){
				this->sx[i] = to_ZZ(0);
			}
			else if(line == "1"){
				this->sx[i] = to_ZZ(1);
			}
			else {
				this->sx[i] = to_ZZ(-1);
			}
		}
		myfile.close();
	}
	else
	{
		throw std::invalid_argument("Unable to open file");
	}
}