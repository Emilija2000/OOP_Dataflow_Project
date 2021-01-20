#include <fstream>
#include "Initializer.h"

Initializer::Initializer(string config) {
	fstream inputFile(config);

	string line;
	try {
		//read numbers
		//addition
		getline(inputFile, line);
		ta = readNumber(line);
		//multiplication
		getline(inputFile, line);
		tm = readNumber(line);
		//exponent
		getline(inputFile, line);
		te = readNumber(line);
		//assignment
		getline(inputFile, line);
		tw = readNumber(line);
		//num of paralel inputs
		getline(inputFile, line);
		memoryNumber = readNumber(line);

		//compilation mode
		getline(inputFile, line);
		string help = line.substr(line.find("=")); //part after = sign
		for (int i = 0; i < help.length(); i++)
			if (help[i] == ' ' || help[i] == '=') {
				help.erase(0, 1); //remove = sign and spaces
				i--; 
			}
		if (help == "simple")
			mod = SIMPLE;
		else if (help == "advanced")
			mod = ADVANCED;
		else
			throw BadInput();
	}
	catch(exception& e) {
		throw BadInput();
	}

	inputFile.close();
}

void Initializer::init() const {
	Compiler::setInstance(ta, tm, te, tw, memoryNumber);
	Machine::setInstance(ta, tm, te, tw);
	Memory::setInstance(memoryNumber);
}

double Initializer::readNumber(string & line) {
	//iterate through line
	string::iterator it = line.begin();
	//skip other characters
	while (it != line.end() && !isdigit(*it))
		it++;
	if(it == line.end())
		throw NotANumber();

	double integer = 0, decimal = 0;

	while (it != line.end() && isdigit(*it) && (*it)!='.') {
		integer = 10 * integer + *it - '0';
		it++;
	}
	if (it!=line.end() && *it == '.') 
		it++;
	int decPoint = 1;
	while (it != line.end() && isdigit(*it)) {
		decimal = decimal * 10 + *it - '0';
		decPoint *= 10;
		it++;
	}
	decimal = decimal / decPoint;
		
	return (integer + decimal);
}