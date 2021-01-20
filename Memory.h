#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <set>
#include "Machine.h"

//Remember all variables. Singleton DP.
class Memory {
public:
	//instance setter and getter
	static Memory& setInstance(unsigned n);
	static Memory& getInstance();

	//allowing another input at the same time
	bool isFree() const;
	friend void Machine::endVariableInput();
	friend void Machine::startVariableInput();

	//read and write from/into memory
	void set(string varName, double val);
	double get(string varName) const;

protected:
	Memory(unsigned n);
	Memory();
	~Memory();

private:
	
	std::set<pair<std::string, double>> variables;

	unsigned paralelInputs;
	unsigned currentInputs;
	static bool initialized;
};

#endif