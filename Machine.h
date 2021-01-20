#ifndef MACHINE_H
#define MACHINE_H

#include <string>
#include <vector>
#include "Expression.h"
#include "ExpressionPool.h"
#include "Sched.h"
#include "OperationBase.h"

class Machine {
public:
	//getter and setter for Machine instance
	static Machine& getInstance();
	static Machine& setInstance(Time Ta, Time Tm, Time Te, Time Tw); //must be called

	//execute program in filename
	void exec(const string& filename);

	//returns expressions that can be done when newResult is calculated
	vector<Expression*> updatePools(Token* newResult);

	//memory menagement - input duration
	void startVariableInput();
	void endVariableInput();

private:
	//private constructor. sigleton dp
	Machine(Time Ta, Time Tm, Time Te, Time Tw);
	~Machine();

	//reading
	void readImfFile(const string& filename);
	Expression* readLine(string& line);
	double readNumber(const string&);

	//flag
	static bool initialized;

	//expression pools
	Pool waiting;
	Pool executing;
	Pool completed;

	//remembering variable names
	set<string> varNames;

	//everything about operations
	OperationBase myOperations;
};

#endif