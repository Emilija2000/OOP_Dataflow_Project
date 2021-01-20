#include "Event.h"
#include "Machine.h"
#include "Operation.h"
#include "Writer.h"

Operation::Operation(OperationType id, int numInpu, Time delay):
	id(id),numOfInputs(numInpu),delayTime(delay) {

	//possible errors
	if (numOfInputs < 0)
		numOfInputs = 0;
	if (delayTime < 0)
		delayTime = 0;

	//inputPorts alocation
	inputPorts = new const Token*[numOfInputs];
	for (int i = 0; i < numOfInputs; i++)
		inputPorts[i] = nullptr;
}

Operation::~Operation() {
	clearInputs();
	delete[] inputPorts;
}


void Operation::setInput(const Token * t) {
	int i;
	for (i = 0; i < numOfInputs; i++) {
		if (inputPorts[i] == nullptr) {
			inputPorts[i] = t;
			break;
		}
	}
	if (i == numOfInputs)
		throw NoFreePorts();
}

void Operation::clearInputs() {
	for (int i = 0; i < numOfInputs; i++)
		inputPorts[i] = nullptr;
}

void Operation::setOutputName(const string & tokenName) {
	this->resultName = tokenName;
}

//if at least one of input ports is empty, execution is not allowed
bool Operation::executionAllowed() const {
	return portsFull();
}
bool Operation::portsFull() const {
	for (int i = 0; i < numOfInputs; i++) {
		if (inputPorts[i] == nullptr || !(inputPorts[i]->isCalculated())) {
			return false;
		}
	}
	return true;
}

Token * Operation::execute() {

	//execution returns nullptr if there isn't something on every input port
	if (!portsFull())
		return nullptr;

	//function in derived class
	Token* resultToken = specificOperation();

	//clear information about this calculation
	clearInputs();
	resultName.clear();

	return resultToken;
}

void Operation::notify(ID id) {
	//calculating operation result
	Token* resultToken = execute();

	//sending lines to Writer
	Time end = Scheduler::Instance()->getCurTime();
	Time start = end - delayTime;
	Writer::getInstance().saveLogData(id, start, end);

	//updating waiting, executing and completed pools
	//nextExpr are the expressions that are able to be calculated now
	vector<Expression*> nextExpr = Machine::getInstance().updatePools(resultToken);

	delete resultToken;

	//new events in the sheduler
	for (auto it : nextExpr) {
		//time menagement
		Event::create(it->getOp(), it->getOp()->getTime(), it->getId());
	}
}