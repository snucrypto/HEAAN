/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "SerializationUtils.h"

void SerializationUtils::writeCiphertext(Ciphertext& cipher, string path) {
	ofstream myfile;
	myfile.open(path);
	myfile << "Ciphertext" << endl;
	myfile << cipher.N << endl;
	myfile << cipher.logp << endl;
	myfile << cipher.logq << endl;
	myfile << cipher.n << endl;
	for(long i = 0; i < cipher.N; i++) {
		myfile << cipher.ax[i] << endl;
	}
	for(long i = 0; i < cipher.N; i++) {
		myfile << cipher.bx[i] << endl;
	}
	myfile.close();
}

Ciphertext SerializationUtils::readCiphertext(string path) {
	ifstream myfile(path);
	if(myfile.is_open()) {
		string line;
		getline(myfile, line);
		getline(myfile, line);
		long N = atol(line.c_str());
		getline(myfile, line);
		long logp = atol(line.c_str());
		getline(myfile, line);
		long logq = atol(line.c_str());
		getline(myfile, line);
		long n = atol(line.c_str());
		
		ZZ* ax = new ZZ[N];
		ZZ* bx = new ZZ[N];
		for(long i = 0; i < N; i++) {
			getline(myfile, line);
			ax[i] = conv<ZZ>(line.c_str());
		}
		for(long i = 0; i < N; i++) {
			getline(myfile, line);
			bx[i] = conv<ZZ>(line.c_str());
		}
		myfile.close();
		return Ciphertext(ax, bx, logp, logq, N, n);
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}

void SerializationUtils::writePlaintext(Plaintext& message, string path) {
	ofstream myfile;
	myfile.open(path);
	myfile << "Plaintext" << endl;
	myfile << message.N << endl;
	myfile << message.logp << endl;
	myfile << message.logq << endl;
	myfile << message.n << endl;
	for(long i = 0; i < message.N; i++) {
		myfile << message.mx[i] << endl;
	}
	myfile.close();
}

Plaintext SerializationUtils::readPlaintext(string path) {
	ifstream myfile(path);
	if(myfile.is_open()) {

		string line;
		getline(myfile, line);
		getline(myfile, line);
		long N = atol(line.c_str());
		getline(myfile, line);
		long logp = atol(line.c_str());
		getline(myfile, line);
		long logq = atol(line.c_str());
		getline(myfile, line);
		long slots = atol(line.c_str());
		getline(myfile, line);
		bool isComplex = atoi(line.c_str());

		ZZ* mx = new ZZ[N];
		for(long i = 0; i < N; i++) {
			getline(myfile, line);
			mx[i] = conv<ZZ>(line.c_str());
		}
		myfile.close();
		return Plaintext(mx, logp, logq, slots, isComplex);
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}

void SerializationUtils::writeKey(Key* key, string path) {
	fstream fout;
	fout.open(path, ios::binary|ios::out);
	long N = key -> N;
	long np = key -> np;
	fout.write(reinterpret_cast<char*>(&N), sizeof(long));
	fout.write(reinterpret_cast<char*>(&np), sizeof(long));
	fout.write(reinterpret_cast<char*>(key->rax), N * np * sizeof(uint64_t));
	fout.write(reinterpret_cast<char*>(key->rbx), N * np * sizeof(uint64_t));
	fout.close();
}

Key* SerializationUtils::readKey(string path) {
	long N, np;
	fstream fin;
	fin.open(path, ios::binary|ios::in);
	fin.read(reinterpret_cast<char*>(&N), sizeof(long));
	fin.read(reinterpret_cast<char*>(&np), sizeof(long));
	uint64_t* rax = new uint64_t[N*np];
	fin.read(reinterpret_cast<char*>(rax), N*np*sizeof(uint64_t));
	uint64_t* rbx = new uint64_t[N*np];
	fin.read(reinterpret_cast<char*>(rbx), N*np*sizeof(uint64_t));
	fin.close();
	return new Key(rax, rbx, N, np);
}

