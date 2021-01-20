#ifndef TOKEN_NODE_H
#define TOKEN_NODE_H

#include<string>
#include"Token.h"

//Node for binary tree of tokens that compiler is reading
class TokenNode {
public:
	TokenNode(const string& operationSymbol, const string& name) :
		leftChild(nullptr), rightChild(nullptr), data(operationSymbol, name) {}
	TokenNode(const string& operationSymbol) :leftChild(nullptr), rightChild(nullptr), 
		data(operationSymbol){}
	~TokenNode() { this->leftChild = nullptr; this->rightChild = nullptr; }

	//getters
	TokenNode* getLeftChild() const { return leftChild; }
	TokenNode* getRightChild() const { return rightChild; }
	const Token* getData() const { return &data; }

	//setters
	void setLeftChild(TokenNode* child) { this->leftChild = child; }
	void setRightChild(TokenNode* child) { this->rightChild = child; }

	//helper functions
	static unsigned depth(const TokenNode*);
	//swap left/right child of this node with left/right child of other child of this node
	enum CHILD_SIDE{LEFT, RIGHT};
	static void swap(TokenNode* node, CHILD_SIDE nodeChild, CHILD_SIDE childOfAChild);

private:

	static void swap(TokenNode*&, TokenNode*&);

	Token data;
	TokenNode *leftChild, *rightChild;
};

#endif