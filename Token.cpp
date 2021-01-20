#include "OperationBase.h"
#include "Token.h"


const string Token::getOperationSymbol() const {
	return OperationBase::getOperationSymbol(this->operationId);
}

//constructor
Token::Token(const string& operationSym, const string& newName):
	valueCalculated(false), operationId(OperationBase::symbolToOperation(operationSym)), 
	name(checkNameDatabase(newName)) {}

Token::Token(const string & operationSymbol) :
	Token(operationSymbol, ((operationSymbol == "=") ? newAssignmentName() : newTokenName())) {}


//new names
void Token::resetNameCreator() {
	newAssignmentCounter = 0;
	newTokenCounter = 0;
}

void Token::completeReset() {
	resetNameCreator();
	nameDatabase.clear();
}

void Token::swapNames(Token &one, Token &two) {
	std::swap(one.name, two.name);
}

set<string> Token::nameDatabase;

const string & Token::checkNameDatabase(const string& newName) const {
	if(nameDatabase.count(newName))
		throw RepeatedName();
	return newName;
}

int Token::newTokenCounter = 0;
int Token::newAssignmentCounter = 0;

string Token::newTokenName() {
	string newName = "t" + to_string(++newTokenCounter);
	return newName;
}
string Token::newAssignmentName() {
	string newName = "a" + to_string(++newAssignmentCounter);
	return newName;
}
