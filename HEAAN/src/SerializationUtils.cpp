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
	myfile << deg(cipher.ax) << endl;
	myfile << deg(cipher.bx) << endl;
	myfile << cipher.logp << endl;
	myfile << cipher.logq << endl;
	myfile << cipher.slots << endl;
	myfile << cipher.isComplex << endl;
	for(long i = 0; i < deg(cipher.ax) + 1; i++) {
		myfile << cipher.ax[i] << endl;
	}
	for(long i = 0; i < deg(cipher.bx) + 1; i++) {
		myfile << cipher.bx[i] << endl;
	}
	myfile.close();
}

Ciphertext SerializationUtils::readCiphertext(string path) {
	ifstream myfile(path);
	if(myfile.is_open()) {
		ZZX ax, bx;
		long temp;
		string line;
		getline(myfile, line);
		getline(myfile, line);
		temp = atol(line.c_str());
		ax.SetLength(temp + 1);
		getline(myfile, line);
		temp = atol(line.c_str());
		bx.SetLength(temp + 1);
		getline(myfile, line);
		long logp = atol(line.c_str());
		getline(myfile, line);
		long logq = atol(line.c_str());
		getline(myfile, line);
		long slots = atol(line.c_str());
		getline(myfile, line);
		bool isComplex = atoi(line.c_str());

		for(long i = 0; i < deg(ax) + 1; i++) {
			getline(myfile, line);
			ax[i] = conv<ZZ>(line.c_str());
		}
		for(long i = 0; i < deg(bx) + 1; i++) {
			getline(myfile, line);
			bx[i] = conv<ZZ>(line.c_str());
		}
		myfile.close();
		return Ciphertext(ax, bx, logp, logq, slots, isComplex);
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}

void SerializationUtils::writePlaintext(Plaintext& message, string path) {
	ofstream myfile;
	myfile.open(path);
	myfile << "Plaintext" << endl;
	myfile << deg(message.mx) << endl;
	myfile << message.logp << endl;
	myfile << message.logq << endl;
	myfile << message.slots << endl;
	myfile << message.isComplex << endl;
	for(long i = 0; i < deg(message.mx) + 1; i++) {
		myfile << message.mx[i] << endl;
	}
	myfile.close();
}

Plaintext SerializationUtils::readPlaintext(string path) {
	ifstream myfile(path);
	if(myfile.is_open()) {
		ZZX mx;
		long temp;
		string line;
		getline(myfile, line);
		getline(myfile, line);
		temp = atol(line.c_str());
		mx.SetLength(temp + 1);
		getline(myfile, line);
		long logp = atol(line.c_str());
		getline(myfile, line);
		long logq = atol(line.c_str());
		getline(myfile, line);
		long slots = atol(line.c_str());
		getline(myfile, line);
		bool isComplex = atoi(line.c_str());

		for(long i = 0; i < deg(mx) + 1; i++) {
			getline(myfile, line);
			mx[i] = conv<ZZ>(line.c_str());
		}
		myfile.close();
		return Plaintext(mx, logp, logq, slots, isComplex);
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}

void SerializationUtils::writeContext(Context& context, string path) {
	ofstream myfile;
	myfile.open(path);
	myfile << "Context" << endl;
	myfile << context.logN << endl;
	myfile << context.logQ << endl;
	myfile << context.sigma << endl;
	myfile << context.h << endl;
	myfile << "BootContext" << endl;
	myfile << context.bootContextMap.size() << endl;
	for (auto const& element : context.bootContextMap) {
		myfile << element.first << endl;
		myfile << element.second.logp << endl;
	}
	myfile.close();
}

Context SerializationUtils::readContext(string path) {
	ifstream myfile(path);
	if(myfile.is_open()) {
		string line;
		//Context
		getline(myfile, line);
		//logN
		getline(myfile, line);
		long logN = atol(line.c_str());
		//logQ
		getline(myfile, line);
		long logQ = atol(line.c_str());
		//sigma
		getline(myfile, line);
		double sigma = atof(line.c_str());
		//h
		getline(myfile, line);
		long h = atol(line.c_str());
		Context context(logN, logQ, sigma, h);

		getline(myfile, line);
		getline(myfile, line);
		long bootsize = atol(line.c_str());
		for (long i = 0; i < bootsize; ++i) {
			getline(myfile, line);
			long logslots = atol(line.c_str());
			getline(myfile, line);
			long logp = atol(line.c_str());
			context.addBootContext(logslots, logp);
		}
		return context;
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}

void SerializationUtils::writeSchemeKeys(Scheme& scheme, string path) {
	ofstream myfile;
	myfile.open(path);
	myfile << "Keys" << endl;
	myfile << scheme.keyMap.size() << endl;
	for (auto const& element : scheme.keyMap) {
		myfile << element.first << endl;
		myfile << deg(element.second.ax) << endl;
		myfile << deg(element.second.bx) << endl;
		for (long i = 0; i < deg(element.second.ax) + 1; ++i) {
			myfile << element.second.ax[i] << endl;
		}
		for (long i = 0; i < deg(element.second.bx) + 1; ++i) {
			myfile << element.second.bx[i] << endl;
		}
	}
	myfile << "Left Rotation Keys" << endl;
	myfile << scheme.leftRotKeyMap.size() << endl;
	for (auto const& element : scheme.leftRotKeyMap) {
		myfile << element.first << endl;
		myfile << deg(element.second.ax) << endl;
		myfile << deg(element.second.bx) << endl;
		for (long i = 0; i < deg(element.second.ax) + 1; ++i) {
			myfile << element.second.ax[i] << endl;
		}
		for (long i = 0; i < deg(element.second.bx) + 1; ++i) {
			myfile << element.second.bx[i] << endl;
		}
	}
	myfile.close();
}

void SerializationUtils::readSchemeKeys(Scheme& scheme, string path) {
	ifstream myfile(path);
	if(myfile.is_open()) {
		string line;
		//Keys
		getline(myfile, line);
		//Num of Keys
		getline(myfile, line);
		long keyNum = atol(line.c_str());

		for (long i = 0; i < keyNum; ++i) {
			getline(myfile, line);
			long keyID = atol(line.c_str());
			ZZX ax, bx;

			getline(myfile, line);
			long axdeg = atol(line.c_str());
			ax.SetLength(axdeg + 1);

			getline(myfile, line);
			long bxdeg = atol(line.c_str());
			bx.SetLength(bxdeg + 1);
			for(long j = 0; j < axdeg + 1; j++) {
				getline(myfile, line);
				ax[j] = conv<ZZ>(line.c_str());
			}
			for(long j = 0; j < bxdeg + 1; j++) {
				getline(myfile, line);
				bx[j] = conv<ZZ>(line.c_str());
			}
			scheme.keyMap.insert(pair<long, Key>(keyID, Key(ax, bx)));
		}

		getline(myfile, line);

		getline(myfile, line);
		long leftRotKeyNum = atol(line.c_str());

		for (long i = 0; i < leftRotKeyNum; ++i) {
			getline(myfile, line);
			long keyID = atol(line.c_str());
			ZZX ax, bx;
			getline(myfile, line);
			long axdeg = atol(line.c_str());
			ax.SetLength(axdeg + 1);

			getline(myfile, line);
			long bxdeg = atol(line.c_str());
			bx.SetLength(bxdeg + 1);

			for(long j = 0; j < axdeg + 1; j++) {
				getline(myfile, line);
				ax[j] = conv<ZZ>(line.c_str());
			}
			for(long j = 0; j < bxdeg + 1; j++) {
				getline(myfile, line);
				bx[j] = conv<ZZ>(line.c_str());
			}
			scheme.leftRotKeyMap.insert(pair<long, Key>(keyID, Key(ax, bx)));
		}
		cout << scheme.context.N << endl;
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}

void SerializationUtils::writeSecretKey(SecretKey& secretKey, string path) {
	ofstream myfile;
	myfile.open(path);
	myfile << "SecretKey" << endl;
	myfile << deg(secretKey.sx) << endl;
	for(long i = 0; i < deg(secretKey.sx) + 1; i++) {
		myfile << secretKey.sx[i] << endl;
	}
	myfile.close();
}

SecretKey SerializationUtils::readSecretKey(string path) {
	ifstream myfile(path);
	if(myfile.is_open()) {
		ZZX sx;
		long temp;
		string line;
		getline(myfile, line);
		getline(myfile, line);
		temp = atol(line.c_str());
		sx.SetLength(temp + 1);
		for(long i = 0; i < deg(sx) + 1; i++) {
			getline(myfile, line);
			sx[i] = conv<ZZ>(line.c_str());
		}
		myfile.close();
		return SecretKey(sx);
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}

void SerializationUtils::writeKey(Key& key, string path) {
	ofstream myfile;
	myfile.open(path);
	myfile << "Key" << endl;
	myfile << deg(key.ax) << endl;
	myfile << deg(key.bx) << endl;
	for(long i = 0; i < deg(key.ax) + 1; i++) {
		myfile << key.ax[i] << endl;
	}
	for(long i = 0; i < deg(key.bx) + 1; i++) {
		myfile << key.bx[i] << endl;
	}
	myfile.close();
}

Key SerializationUtils::readKey(string path) {
	ifstream myfile(path);
	if(myfile.is_open()) {
		ZZX ax, bx;
		long temp;
		string line;
		getline(myfile, line);
		getline(myfile, line);
		temp = atol(line.c_str());
		ax.SetLength(temp + 1);
		getline(myfile, line);
		temp = atol(line.c_str());
		bx.SetLength(temp + 1);
		for(long i = 0; i < deg(ax) + 1; i++) {
			getline(myfile, line);
			ax[i] = conv<ZZ>(line.c_str());
		}
		for(long i = 0; i < deg(bx) + 1; i++) {
			getline(myfile, line);
			bx[i] = conv<ZZ>(line.c_str());
		}
		myfile.close();
		return Key(ax, bx);
	} else {
		throw std::invalid_argument("Unable to open file");
	}
}
