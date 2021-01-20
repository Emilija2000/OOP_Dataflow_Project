#ifndef TOKEN_H
#define TOKEN_H

#include <set>
#include <string>
#include "Exceptions.h"
using namespace std;

enum OperationType;

class Token {
public:
	//Constructor - copy is forbidden because name must be unique
	Token(const string& operationSymbol, const string& name);
	Token(const string& operationSymbol);
	Token(Token&) = delete;
	Token& operator=(Token&) = delete;
	~Token() = default;

	//Setters
	void setValue(double);

	//Getters
	const string getName() const;
	double getValue() const;
	const string getOperationSymbol() const;

	//flag checkers
	bool isCalculated() const;
	bool isNumber() const;

	static void resetNameCreator();
	static void completeReset();

	static void swapNames(Token&, Token&);

private:
	//Helper methods for initialization
	//Returns given string if it is not in name database
	const string& checkNameDatabase(const string& newName) const;

	//Menaging new token names
	static string newTokenName();
	static string newAssignmentName();
	static int newTokenCounter;
	static int newAssignmentCounter;

	//Properties
	string name;
	const OperationType operationId; //operation that produced this token
	double value;

	//flags
	bool valueCalculated;

	static set<string> nameDatabase;
};

//inline methods of class Token
inline const string Token::getName() const {
	return this->name;
}
inline double Token::getValue() const {
	return this->value;
}
inline void Token::setValue(double val) {
	this->value = val;
	valueCalculated = true;
}
inline bool Token::isCalculated() const {
	return valueCalculated;
}
inline bool Token::isNumber() const {
	return isdigit(this->name[0]);
}

#endif