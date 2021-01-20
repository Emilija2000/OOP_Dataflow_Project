#include "Expression.h"

Expression::Expression(ID id, Token* leftOp, Token* rightOp, Token* result, Operation* pok) :
	id(id), leftOp(leftOp), rightOp(rightOp), result(result), myOperation(pok) {}

Expression::~Expression() {
	delete leftOp;
	delete rightOp;
	delete result;
	delete myOperation;
}

ID Expression::getId() {
	return this->id;
}

Token * Expression::getResult() {
	return result;
}

string Expression::getResultName() {
	return this->result->getName();
}



void Expression::setResultValue(double val) {
	this->result->setValue(val);
}

Token * Expression::getLeftOp() {
	return this->leftOp;
}

Token * Expression::getRightOp() {
	return this->rightOp;
}

string Expression::getLeftOpName() {
	return leftOp->getName();
}

string Expression::getRightOpName() {
	return rightOp->getName();
}

void Expression::setLeftValue(double val) {
	this->leftOp->setValue(val);
}

void Expression::setRightValue(double val) {
	this->rightOp->setValue(val);
}

Operation * Expression::getOp() {
	return myOperation;
}
