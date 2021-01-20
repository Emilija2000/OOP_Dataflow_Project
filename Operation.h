#ifndef OPERATION_H
#define OPERATION_H

#include "Interfaces.h"
#include "Token.h"

enum OperationType { FIRST = -1, ADD, MULTIPLY, EXPONENT, ASSIGNMENT, LAST };

class Operation : public ITimedElement {
public:
	Operation(OperationType id, int numInpu, Time delayTime);
	virtual ~Operation();

	virtual void notify(ID id) override;

	//execution
	virtual bool executionAllowed() const; //returns if there is a token on every input port
	Token* execute();			   //returns value is a result - is not responsible for deleting it

	//input-output management
	void setInput(const Token*);
	void clearInputs();
	void setOutputName(const string& tokenName);

	//delay
	double getTime() const;

	//get
	OperationType getOpType() const;

protected:
	//check
	virtual bool portsFull() const;

	//specific execution function - override in derived classes
	virtual Token* specificOperation() = 0;
	const string& getResultName();
	//input ports - constant size numOfInputs
	const Token** inputPorts;

private:

	//properties
	OperationType id;
	int numOfInputs;
	Time delayTime;
	string resultName;
};


//inline methods
inline Time Operation::getTime() const {
	return this->delayTime;
}

inline OperationType Operation::getOpType() const{
	return this->id;
}

inline const string& Operation::getResultName() {
	return resultName;
}

#endif