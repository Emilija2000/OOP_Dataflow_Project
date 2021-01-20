#ifndef OPERATION_BASE_H
#define OPERATION_BASE_H

#include <vector>
#include"ArithmeticOperation.h"

class OperationBase {
public:
	//creating operation database
	OperationBase(Time Ta, Time Tm, Time Te, Time Tw);

	//translating - symbols, enum
	static OperationType symbolToOperation(const string & operation);
	static const string getOperationSymbol(OperationType operationId);

	//return value is new operation
	Operation* getOp(OperationType op);
	Operation* getOp(const string& opSymbol);
private:
	Time getDelay(OperationType op);

	vector<Time> delayTimes;
};

#endif