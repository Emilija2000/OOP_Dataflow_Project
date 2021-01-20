#include "Exceptions.h"
#include "Memory.h"

bool Memory::initialized = false;

//required set of parameters
Memory & Memory::setInstance(unsigned n) {
	static Memory instance(n); //instance created just the first time
	initialized = true;
	return instance;
}

//getter of singleton instance
Memory & Memory::getInstance() {
	if (!initialized)
		throw Uninitialized();
	return setInstance(0);
}

//if another variable can be written at the same time return true
bool Memory::isFree() const {
	if (currentInputs < paralelInputs)
		return true;
	return false;
}

void Memory::set(string varName, double val) {
	//checking if the same variable already exists
	for (auto it : variables) {
		if (it.first == varName)
			throw RepeatedName();
	}
	//new input
	pair<string, double> newVar;
	newVar.first = varName;
	newVar.second = val;
	variables.insert(newVar);
}

double Memory::get(string varName) const {
	bool found = false;
	for (auto it : variables) {
		if (it.first == varName) {
			found = true;
			return it.second;
		}
	}
	if (!found)
		throw VarNotAvailableException();
}

//private constructor
Memory::Memory(unsigned n): paralelInputs(n), currentInputs(0) {}

Memory::Memory() {
	variables.clear();
}

Memory::~Memory() {
	variables.clear();
}
