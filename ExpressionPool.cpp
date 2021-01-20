#include "ExpressionPool.h"
#include "Machine.h"

void Pool::add(Expression* e) {
	collection.push_back(e);
}

void Pool::remove(Expression* e) {
	vector<Expression*>::iterator it;
	for (it = collection.begin(); it != collection.end(); it++) {
		if ((*it)->getId() == e->getId())
			break;
	}
	if (it < collection.end())
		collection.erase(it);
}

void Pool::clear() {
	for(auto it:collection) {
		delete it;
	}
	collection.clear();
}

bool Pool::empty() const {
	return this->collection.empty();
}

Token * Pool::getTokenByName(const string & name) {
	for (auto t : collection) {
		if (t->getResultName() == name) {
			return t->getResult();
		}
	}
	return nullptr;
}

Expression * Pool::getExprByName(const string & name) {
	for (auto t : collection) {
		if (t->getResultName() == name) {
			return t;
		}
	}
	return nullptr;
}

void Pool::updateTokenValues(Token * tok) {
	for (auto it : collection) {
		if (it->getResultName() == tok->getName())
			it->setResultValue(tok->getValue());
		if (it->getLeftOpName() == tok->getName())
			it->setLeftValue(tok->getValue());
		if (it->getRightOpName() == tok->getName())
			it->setRightValue(tok->getValue());
	}
}

vector<Expression*> Pool::forExecution() {
	vector<Expression*> returnExpr;

	for (auto it : collection) {
		it->getOp()->setInput(it->getLeftOp());
		it->getOp()->setInput(it->getRightOp());
		if (it->getOp()->executionAllowed()) {
			returnExpr.push_back(it);
			//memory menagement
			if (it->getOp()->getOpType() == ASSIGNMENT)
				Machine::getInstance().startVariableInput();
		}
		else {
			it->getOp()->clearInputs();
		}
	}

	return returnExpr;
}
