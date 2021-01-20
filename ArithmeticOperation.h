#ifndef ARITHMETHIC_OPERATION_H
#define ARITHMETHIC_OPERATION_H

#include "Operation.h"

class Addition : public Operation {
public:
	Addition(double delayTime) : Operation(ADD, 2, delayTime) {}
protected:
	virtual Token* specificOperation() override;
};

class Multiplication : public Operation {
public:
	Multiplication(double delayTime) : Operation(MULTIPLY, 2, delayTime) {}
protected:
	virtual Token* specificOperation() override;
};

class Exponent : public Operation {
public:
	Exponent(double delayTime) : Operation(EXPONENT, 2, delayTime) {}
protected:
	virtual Token* specificOperation() override;
};

class Assignment : public Operation {
public:
	Assignment(double delayTime) : Operation(ASSIGNMENT, 2, delayTime) {}
	virtual bool executionAllowed() const override;
protected:
	virtual bool portsFull() const override;
	virtual Token* specificOperation() override;
};

#endif