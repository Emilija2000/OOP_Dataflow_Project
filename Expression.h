#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Event.h"
#include "ArithmeticOperation.h"
#include "Token.h"

class Expression {
public:
	Expression(ID id, Token* leftOp, Token* rightOp, Token* result, Operation* pok);
	~Expression();

	ID getId();
	//methods for result
	Token* getResult();
	string getResultName();
	void setResultValue(double val);
	//methods for operands
	Token* getLeftOp();
	Token* getRightOp();
	string getLeftOpName();
	string getRightOpName();
	void setLeftValue(double val);
	void setRightValue(double val);

	Operation* getOp();

private:
	Token* leftOp, *rightOp, *result;
	Operation * myOperation;
	ID id;
};

#endif