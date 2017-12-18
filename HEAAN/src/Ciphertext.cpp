#include "Ciphertext.h"

void Ciphertext::Write(int ciphertextID)
{
	ofstream myfile;
	myfile.open("Ciphertext" + to_string(ciphertextID) + ".txt");
	myfile << "Ciphertext Information" << endl;
	myfile << "ID = " << ciphertextID << endl;
	myfile << deg(ax) << endl;
	myfile << deg(bx) << endl;
	myfile << this->q << endl;
	myfile << this->logq << endl;
	myfile << this->slots << endl;
	if(isComplex) {
		myfile << "true" << endl;
	}
	else {
		myfile << "false" << endl;
	}
	for(int i = 0; i < deg(ax) + 1; i++) {
		myfile << ax[i] << endl;
	}
	for(int i = 0; i < deg(bx) + 1; i++) {
		myfile << bx[i] << endl;
	}
	myfile.close();
}

void Ciphertext::Read(int ciphertextID)
{
	ifstream myfile("Ciphertext" + to_string(ciphertextID) + ".txt");
	if(myfile.is_open()) {
		// kill previous memory
		this->ax.kill();
		this->bx.kill();
		// start reading
		int temp;
		string line;
		// pass first two lines
		getline(myfile, line); getline(myfile, line);
		// read 3rd line and get degree of ax
		getline(myfile, line);
		temp = atoi(line.c_str());
		this->ax.SetLength(temp + 1);
		// read 4th line and get degree of bx
		getline(myfile, line);
		temp = atoi(line.c_str());
		this->bx.SetLength(temp + 1);
		// read 5th line and get q
		getline(myfile, line);
		this->q = stringToNumber(line);
		// read 6th line and get logq
		getline(myfile, line);
		temp = atoi(line.c_str());
		this->logq = temp;
		// read 7th line and get slots
		getline(myfile, line);
		temp = atoi(line.c_str());
		this->slots = temp;
		// read 8th line and get isComplex
		getline(myfile, line);
		if(line == "true") {
			this->isComplex = 1;
		}
		else {
			this->isComplex = 0;
		}
		// read other lines and get ax and bx
		for(long i = 0; i < deg(ax) + 1; i++) {
			getline(myfile, line);
			ax[i] = stringToNumber(line);
		}
		for(long i = 0; i < deg(bx) + 1; i++) {
			getline(myfile, line);
			bx[i] = stringToNumber(line);
		}
		myfile.close();
	}
	else {
		throw std::invalid_argument("Unable to open file");
	}
}
