/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "SerializationUtils.h"

void SerializationUtils::writeCiphertext(Ciphertext* cipher, string path) {
	fstream fout;
	fout.open(path, ios::binary|ios::out);
	long N = cipher -> N;
	long n = cipher->n;
	long logp = cipher->logp;
	long logq = cipher->logq;
	fout.write(reinterpret_cast<char*>(&N), sizeof(long));
	fout.write(reinterpret_cast<char*>(&n), sizeof(long));
	fout.write(reinterpret_cast<char*>(&logp), sizeof(long));
	fout.write(reinterpret_cast<char*>(&logq), sizeof(long));

	long np = ceil(((double)logq + 1)/8);
	ZZ q = conv<ZZ>(1) << logq;
	unsigned char* bytes = new unsigned char[np];
	for (long i = 0; i < N; ++i) {
		cipher->ax[i] %= q;
		BytesFromZZ(bytes, cipher->ax[i], np);
		fout.write(reinterpret_cast<char*>(bytes), np);
	}
	for (long i = 0; i < N; ++i) {
		cipher->bx[i] %= q;
		BytesFromZZ(bytes, cipher->bx[i], np);
		fout.write(reinterpret_cast<char*>(bytes), np);
	}
	fout.close();
}

Ciphertext* SerializationUtils::readCiphertext(string path) {
	long N, n, logp, logq;
	fstream fin;
	fin.open(path, ios::binary|ios::in);
	fin.read(reinterpret_cast<char*>(&N), sizeof(long));
	fin.read(reinterpret_cast<char*>(&n), sizeof(long));
	fin.read(reinterpret_cast<char*>(&logp), sizeof(long));
	fin.read(reinterpret_cast<char*>(&logq), sizeof(long));

	long np = ceil(((double)logq + 1)/8);
	unsigned char* bytes = new unsigned char[np];

	ZZ* ax = new ZZ[N];
	for (long i = 0; i < N; ++i) {
		fin.read(reinterpret_cast<char*>(bytes), np);
		ZZFromBytes(ax[i], bytes, np);
	}
	ZZ* bx = new ZZ[N];
	for (long i = 0; i < N; ++i) {
		fin.read(reinterpret_cast<char*>(bytes), np);
		ZZFromBytes(bx[i], bytes, np);
	}
	fin.close();
	return new Ciphertext(ax, bx, logp, logq, N, n);
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

