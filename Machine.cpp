#include <fstream>
#include "Exceptions.h"
#include "Machine.h"
#include "Memory.h"
#include "Writer.h"

bool Machine::initialized = false;

Machine::Machine(Time Ta, Time Tm, Time Te, Time Tw) :
	waiting(), executing(), completed(), myOperations(Ta, Tm, Te, Tw) {}

Machine::~Machine() {
	waiting.clear();
	executing.clear();
	completed.clear();
	varNames.clear();
}

Machine & Machine::getInstance() {
	if (!initialized)
		throw Uninitialized();
	return setInstance(-1, -1, -1, -1);
}

Machine& Machine::setInstance(Time Ta, Time Tm, Time Te, Time Tw) {
	static Machine instance(Ta, Tm, Te, Tw);
	initialized = true;
	return instance;
}

//reading from imf file into pools
void Machine::readImfFile(const string & filename) {
	fstream inputFile(filename, ios::in);

	//reading line by line
	for (std::string line; getline(inputFile, line); ) {
		//new Expression from everything read - notice: expression should delete everything
		Expression* newExpr = readLine(line);

		//remember variable names
		if (newExpr->getOp()->getOpType() == ASSIGNMENT)
			varNames.insert(newExpr->getLeftOp()->getName());

		//initial setup for execution
		newExpr->getOp()->setInput(newExpr->getLeftOp());
		newExpr->getOp()->setInput(newExpr->getRightOp());
		if (newExpr->getOp()->executionAllowed()) {
			executing.add(newExpr);
			//put event on schedule - time management
			Event::create(newExpr->getOp(), newExpr->getOp()->getTime(), newExpr->getId());
			if(newExpr->getOp()->getOpType() == ASSIGNMENT)
			//start tracking in memory - memory menagement
			startVariableInput();
		}
		else {
			newExpr->getOp()->clearInputs();
			waiting.add(newExpr);
		}
}

	inputFile.close();
}

Expression* Machine::readLine(string& line) {
	
	//iterator through this line
	string::iterator it = line.begin();
	//information needed for Expression object from this line
	//expression id
	ID id = 0;
	while (it != line.end()) {
		if (isdigit(*it)) {
			while (isdigit(*it)) {
				id = id * 10 + (*it) - '0';
				it++;
			}
			break;
		}
		it++;
	}

	//not important characters
	while (it != line.end() && *it != ']') it++;
	it++;
	while (it != line.end() && *it == ' ') it++;
	//operator sign
	string opSign;
	while (it != line.end() && *it != ' ') {
		opSign += *it;
		it++;
	}
	//skip blank
	while (it != line.end() && *it == ' ') it++;
	//result token
	string result;
	if (opSign != "=") {
		while (it != line.end() && *it != ' ') {
			result += *it;
			it++;
		}
	}
	//skip blank
	while (it != line.end() && *it == ' ') it++;
	//first operand
	string firstOperand;
	while (it != line.end() && *it != ' ') {
		firstOperand += *it;
		it++;
	}
	if (opSign == "=") result = firstOperand;
	//skip blank
	while (it != line.end() && *it == ' ') it++;
	//second operand
	string secondOperand;
	while (it != line.end() && *it != ' ') {
		secondOperand += *it;
		it++;
	}


	//new Tokens for operands
	Token* leftOperand = new Token("=", firstOperand);
	try {
		double num = readNumber(firstOperand);
		leftOperand->setValue(num);
	}
	catch (NotANumber& e) { /*just skip */ }

	Token* rightOperand = new Token("=", secondOperand);
	try {
		double num = readNumber(secondOperand);
		rightOperand->setValue(num);
	}
	catch (NotANumber& e) { /* skip */ }

	Token* resultToken = new Token(opSign, result);

	//new Operation
	Operation* op = myOperations.getOp(opSign);
	op->setOutputName(resultToken->getName());

	//new Expression from everything read - notice: expression should delete everything
	Expression* newExpr = new Expression(id, leftOperand, rightOperand, resultToken, op);

	return newExpr;
}

double Machine::readNumber(const string & num) {
	double integer = 0, decimal = 0;
	bool negative = false;
	if (isdigit(num[0]) || num[0] == '-') {
		int i = 0;
		if (num[i] == '-') {
			negative = true; i++;
		}
		for (; i < num.length() && num[i] != '.'; i++) {
			integer = 10 * integer + num[i] - '0';
		}
		if (num[i] == '.') {
			i++;
			int decPoint = 1;
			for (; i < num.length(); i++) {
				decimal = decimal * 10 + num[i] - '0';
				decPoint *= 10;
			}
			decimal = decimal / decPoint;
		}
	}
	else 
		throw NotANumber();

	double n = integer + decimal;
	if (negative) n = -n;
	return n;
}

//execution of all of the read expressions
void Machine::exec(const string & filename) {
	readImfFile(filename);

	//calculate everything in right order
	bool flag = !waiting.empty();
	while (!waiting.empty() || flag) {
		flag = Scheduler::Instance()->processNow();
	}

	//log file	
	string outputFileName = filename.substr(0, filename.find(".")) + ".log";
	Writer::getInstance().writeLogFile(outputFileName);

	//mem file
	for (auto it : varNames) {
		Writer::getInstance().addMemLine(it, Memory::getInstance().get(it));
	}
	outputFileName = filename.substr(0, filename.find(".")) + ".mem";
	Writer::getInstance().writeFile(outputFileName);
}

vector<Expression*> Machine::updatePools(Token * newResult) {
	//update values
	executing.updateTokenValues(newResult);
	waiting.updateTokenValues(newResult);

	//transfering calculated expression from one pool to another
	if (Expression* expr = executing.getExprByName(newResult->getName())) {
		executing.remove(expr);
		completed.add(expr);
	}

	//pointers to expressions that can be executed now
	vector<Expression*> nextExpressions = waiting.forExecution();

	//transfering those from waiting to executing
	for (auto it : nextExpressions) {
		waiting.remove(it);
		executing.add(it);
	}

	return nextExpressions;
}

void Machine::startVariableInput() {
	if (Memory::getInstance().isFree())
		Memory::getInstance().currentInputs = Memory::getInstance().currentInputs + 1;
	else
		throw MemoryInputNotPossible();
}

void Machine::endVariableInput() {
	Memory::getInstance().currentInputs = Memory::getInstance().currentInputs - 1;
}