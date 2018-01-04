#include "Ciphertext.h"

void Ciphertext::Write(string filename) {
	ofstream myfile;
	myfile.open("Ciphertext" + filename + ".txt");
	myfile << "Ciphertext Information" << endl;
	myfile << "filename = " << filename << endl;
	myfile << deg(ax) << endl;
	myfile << deg(bx) << endl;
	myfile << q << endl;
	myfile << logq << endl;
	myfile << slots << endl;
	if(isComplex) {
		myfile << "true" << endl;
	} else {
		myfile << "false" << endl;
	}
	for(long i = 0; i < deg(ax) + 1; i++) {
		myfile << ax[i] << endl;
	}
	for(long i = 0; i < deg(bx) + 1; i++) {
		myfile << bx[i] << endl;
	}
	myfile.close();
}

void Ciphertext::Read(string filename) {
	ifstream myfile("Ciphertext" + filename + ".txt");
	if(myfile.is_open()) {
		// kill previous memory
		ax.kill();
		bx.kill();
		// start reading
		long temp;
		string line;
		// pass first two lines
		getline(myfile, line);
		getline(myfile, line);

		// read 3rd line and get degree of ax
		getline(myfile, line);
		temp = atol(line.c_str());
		ax.SetLength(temp + 1);

		// read 4th line and get degree of bx
		getline(myfile, line);
		temp = atol(line.c_str());
		bx.SetLength(temp + 1);

		// read 5th line and get q
		getline(myfile, line);
		q = conv<ZZ>(line.c_str());

		// read 6th line and get logq
		getline(myfile, line);
		temp = atol(line.c_str());
		logq = temp;

		// read 7th line and get slots
		getline(myfile, line);
		temp = atol(line.c_str());
		slots = temp;

		// read 8th line and get isComplex
		getline(myfile, line);
		isComplex = line == "true" ? true : false;

		// read other lines and get ax and bx
		for(long i = 0; i < deg(ax) + 1; i++) {
			getline(myfile, line);
			ax[i] = conv<ZZ>(line.c_str());
		}
		for(long i = 0; i < deg(bx) + 1; i++) {
			getline(myfile, line);
			bx[i] = conv<ZZ>(line.c_str());
		}
		myfile.close();
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}
