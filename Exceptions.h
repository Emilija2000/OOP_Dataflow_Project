#ifndef DATAFLOW_EXCEPTIONS_H
#define DATAFLOW_EXCEPTIONS_H

#include <exception>
using namespace std;

class IncorrectArguments: public exception{
public:
	const char* what() const override {
		return "Invalid arguments in the main function";
	}
};

class CompilerError : public exception {
public:
	const char* what() const override {
		return "Error: the compiler could not perform the requested action";
	}
};

class BadInput : public exception {
public:
	const char* what() const override {
		return "Error: input file is not well formated";
	}
};

class Mismatched : public exception {
public:
	const char* what() const override {
		return "Error: mismatched operator in input file";
	}
};

class MismatchedBrackets : public exception {
public:
	const char* what() const override {
		return "Error: mismatched brackets in input file";
	}
};

class Uninitialized : public exception {
public:
	const char* what() const override {
		return "Error: forgotten initialization of an object";
	}
};

class NonexistentOperation : public exception {
public:
	const char* what() const override {
		return "Error: tried to find an operation that does not exist";
	}
};

class RepeatedName : public exception {
public:
	const char* what() const override {
		return "Error: two variables (tokens) cannot have a same name";
	}
};

class NoFreePorts : public exception {
public:
	const char* what() const override {
		return "Error: operation cannot receve more input tokens - all input ports are full";
	}
};

class NotANumber : public exception {
public:
	const char* what() const override {
		return "Error: number expected";
	}
};

class VarNotAvailableException : public exception {
public:
	const char* what() const override {
		return "Error: variable with that name does not exist";
	}
};

class MemoryInputNotPossible : public exception {
public:
	const char* what() const override {
		return "Error: too many variables in memory at the same time";
	}
};

#endif