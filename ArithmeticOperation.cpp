#include <cmath>
#include "ArithmeticOperation.h"
#include "Exceptions.h"
#include "Memory.h"

Token * Addition::specificOperation() {
	Token* newToken = new Token("+", getResultName());
	if (!newToken)
		throw bad_alloc();
	double newValue = inputPorts[0]->getValue() + inputPorts[1]->getValue();
	newToken->setValue(newValue);
	return newToken;
}

Token * Multiplication::specificOperation() {
	Token* newToken = new Token("*", getResultName());
	if (!newToken)
		throw bad_alloc();
	double newValue = inputPorts[0]->getValue() * inputPorts[1]->getValue();
	newToken->setValue(newValue);
	return newToken;
}

Token * Exponent::specificOperation() {
	Token* newToken = new Token("^", getResultName());
	if (!newToken)
		throw bad_alloc();
	double newValue = std::pow(inputPorts[0]->getValue(),inputPorts[1]->getValue());
	newToken->setValue(newValue);
	return newToken;
}

bool Assignment::portsFull() const {
	if (inputPorts[0] == nullptr || inputPorts[1] == nullptr || !(inputPorts[1]->isCalculated())) {
		return false;
	}
	return true;
}

bool Assignment::executionAllowed() const {
	if (portsFull() && Memory::getInstance().isFree())
		return true;
	else
		return false;
}

Token * Assignment::specificOperation() {
	//calculate result
	Token* newToken = new Token("=", getResultName());
	if (!newToken)
		throw bad_alloc();
	double newValue = inputPorts[1]->getValue();
	newToken->setValue(newValue);
	//memory update - writing is done in this moment - finished
	Memory::getInstance().set(newToken->getName(), newToken->getValue());
	Machine::getInstance().endVariableInput();
	//return result
	return newToken;
}