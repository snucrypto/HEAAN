#include "Ciphertext.h"

void Ciphertext::Write(int ciphertextID)
{
	ofstream out;
	out.open("Ciphertext" + to_string(ciphertextID) + ".txt");
	out << "Ciphertext Information" << endl;
	out << "ID = " << ciphertextID << endl;
	out << "degree = " << deg(ax) << endl;
	out << "logq = " << this->logq << endl;
	out << "slots = " << this->slots << endl;
	out << "isComplex = ";
	if(isComplex) {
		out << "true" << endl;
	}
	else {
		out << "false" << endl;
	}
	out << "ax = " << endl;
	for(int i = 0; i < deg(ax); i++) {
		out << ax[i] << endl;
	}
	out << "bx = " << endl;
	for(int i = 0; i < deg(ax); i++) {
		out << bx[i] << endl;
	}
}
