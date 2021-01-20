#ifndef EXPRESSION_POOL
#define EXPRESSION_POOL

#include <vector>
#include "Expression.h"
using namespace std;

class Pool {
public:
	Pool() = default;

	void add(Expression* e);
	void remove(Expression* e);
	void clear();

	//check
	bool empty() const;

	//searching 
	Token* getTokenByName(const string& name);
	Expression* getExprByName(const string& name);

	//change tokens with the same name, in every expression
	void updateTokenValues(Token* t);

	//return all expressions that can be executed
	vector<Expression*> forExecution();

private:
	vector<Expression*> collection;
};

#endif