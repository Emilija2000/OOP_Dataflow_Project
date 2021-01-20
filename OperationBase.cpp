#include "OperationBase.h"

OperationBase::OperationBase(Time Ta, Time Tm, Time Te, Time Tw) {
	for (int i = OperationType::FIRST + 1; i != OperationType::LAST; i++)
		this->delayTimes.push_back(0);
	
	this->delayTimes[ADD] = Ta;
	this->delayTimes[MULTIPLY] = Tm;
	this->delayTimes[EXPONENT] = Te;
	this->delayTimes[ASSIGNMENT] = Tw;
}

OperationType OperationBase::symbolToOperation(const string & operation) {
	OperationType op;
	if (operation == "+") op = ADD;
	else if (operation == "*") op = MULTIPLY;
	else if (operation == "^") op = EXPONENT;
	else if (operation == "=") op = ASSIGNMENT;
	else throw NonexistentOperation();
	return op;
}

const string OperationBase::getOperationSymbol(OperationType operationId) {
	string symbol;
	switch (operationId) {
	case ADD:
		symbol = "+";
		break;
	case MULTIPLY:
		symbol = "*";
		break;
	case EXPONENT:
		symbol = "^";
		break;
	case ASSIGNMENT:
		symbol = "=";
		break;
	default:
		throw NonexistentOperation();
	}
	return symbol;
}

Operation * OperationBase::getOp(OperationType op) {
	Operation* operation;
	switch (op) {
	case ADD:
		operation = new Addition(getDelay(op));
		break;
	case MULTIPLY:
		operation = new Multiplication(getDelay(op));
		break;
	case EXPONENT:
		operation = new Exponent(getDelay(op));
		break;
	case ASSIGNMENT:
		operation = new Assignment(getDelay(op));
		break;
	default:
		throw NonexistentOperation();
	}
	return operation;
}

Operation * OperationBase::getOp(const string & opSymbol) {
	OperationType op = symbolToOperation(opSymbol);
	return getOp(op);
}

Time OperationBase::getDelay(OperationType op) {
	Time delay = delayTimes[op];
	return delay;
}
