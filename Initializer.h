#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <string>
#include "Compiler.h"
#include "Machine.h"
#include "Memory.h"
#include "Types.h"

class Initializer {
public:
	//read and remember parameters
	Initializer(string config);

	//initialize machine, memory
	void init() const;

	//getters
	CompileMode getCompMode();

private:

	//helper reader methodes
	double readNumber(string & line);

	//config
	Time ta, tm, te, tw;
	unsigned memoryNumber;
	CompileMode mod;
};

//inline
inline CompileMode Initializer::getCompMode() {
	return mod;
}

#endif