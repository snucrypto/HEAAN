#include "Ciphertext.h"

ZZ stringToNumber(string str)
{
	ZZ number = conv<ZZ>(str[0] - '0');
    long len = str.length();
    for(long i = 1; i < len; i++)
    {
        number *= 10;
        number += conv<ZZ>(str[i] - '0');
    }
    return number;
}

void Ciphertext::Write(int ciphertextID)
{
	ofstream myfile;
	myfile.open("Ciphertext" + to_string(ciphertextID) + ".txt");
	myfile << "Ciphertext Information" << endl;
	myfile << "ID = " << ciphertextID << endl;
	myfile << deg(ax) << endl;
	myfile << this->logq << endl;
	myfile << this->slots << endl;
	if(isComplex) {
		myfile << "true" << endl;
	}
	else {
		myfile << "false" << endl;
	}
	myfile << "ax, bx = " << endl;
	for(int i = 0; i < deg(ax); i++) {
		myfile << ax[i] << endl;
	}
	for(int i = 0; i < deg(ax); i++) {
		myfile << bx[i] << endl;
	}
	myfile.close();
}

void Ciphertext::Read(int ciphertextID)
{
	ifstream myfile("Ciphertext" + to_string(ciphertextID) + ".txt");
	if(myfile.is_open())
	{
		int counter = 1;
		int data;
		string line;
		while(getline(myfile, line))
		{
			if(counter == 3)
			{
				data = atoi(line.c_str());
				this->ax.SetLength(data + 1);
				this->bx.SetLength(data + 1);
			}
			if(counter == 4)
			{
				data = atoi(line.c_str());
				this->logq = int(data);
			}
			if(counter == 5)
			{
				data = atoi(line.c_str());
				this->slots = int(data);
			}
			if(counter == 6)
			{
				if(line == "true")
				{
					this->isComplex = 1;
				}
				else if(line == "false")
				{
					this->isComplex = 0;
				}
			}
			if(counter > 8 && counter < 8 + deg(ax) + 1)
			{
				ax[counter - 9] = stringToNumber(line);
			}
			if(counter > 8 + deg(ax) + 1)
			{
				bx[counter - 9 - deg(ax) - 1] = stringToNumber(line);
			}
			counter++;
		}
		myfile.close();
	}
	else
	{
		throw std::invalid_argument("Unable to open file");
	}
}
