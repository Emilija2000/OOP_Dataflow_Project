#include <cctype>
#include <fstream>
#include <queue>
#include <stack>
#include "Compiler.h"
#include "Writer.h"

OperationSyntax Compiler::myOperations[] = {
	OperationSyntax("+", 2, A_NONE, 0),
	OperationSyntax("*", 3, A_NONE, 0),
	OperationSyntax("^", 5, A_RIGHT, 0),
	OperationSyntax("=", 1, A_RIGHT, 0)
};

Compiler & Compiler::setInstance(Time ta, Time tm, Time te, Time tw, int nw) {
	static Compiler instance(ta,tm,te,tw,nw); //instance made on first use
	return instance;
}

//Getter for the only, already created instance
Compiler& Compiler::getInstance() {
	return setInstance(0,0,0,0,0); //instance made on first use
}

//Private constructor
Compiler::Compiler(Time ta, Time tm, Time te, Time tw, int nw):simultAssign(nw) {
	myOperations[0].delay = ta;
	myOperations[1].delay = tm;
	myOperations[2].delay = te;
	myOperations[3].delay = tw;
}

//Destructor
Compiler::~Compiler() {
	this->clear();
}

void Compiler::clear() {

	Writer::getInstance().clear();

	Token::completeReset();

	for (auto it : this->syntaxTrees) {
		//postorder
		TokenNode* curr = it, *lastVisited = nullptr;
		stack<TokenNode*> unvisited;

		while (!unvisited.empty() || curr) {
			if (curr) {
				unvisited.push(curr);
				curr = curr->getLeftChild();
			}
			else {
				TokenNode* topNode = unvisited.top(); //parent node to curr
													  //parent node is still not popped from stack
													  //if right child is still not deleted
				if (topNode->getRightChild() != nullptr && topNode->getRightChild() != lastVisited) {
					curr = topNode->getRightChild();
				}
				//if both of helper's children are deleted
				else {
					lastVisited = topNode;
					unvisited.pop();
					delete topNode;
				}
			}
		}
	}
	syntaxTrees.clear();
}


//Processing
void Compiler::compile(const string& filename, CompileMode mode) {

	//if called again it is cleared first
	this->clear();

	//reading and saving info into tree
	if (mode == SIMPLE)
		simpleRead(filename);
	else if (mode == ADVANCED)
		advancedRead(filename);
	else 
		throw CompilerError();

	//changing the file extension
	size_t position = filename.find(".");
	string outputFileName = filename.substr(0, position);
	outputFileName += ".imf";

	//writing the output
	save(outputFileName);
}

//reading - precedenceCheck varies depending on compile mode
void Compiler::read(const string & inputFileName, bool precedenceCheck(const OperationSyntax*, const OperationSyntax*)) {
	fstream inputFile(inputFileName, ios::in);

	int numOfLines = 0;
	//reading expressions line by line
	for (std::string line; getline(inputFile, line); ) {

		//skip blank lines
		if (line.empty())
			continue;

		//remembering read parts on stack - different for numbers and operators
		stack<TokenNode*> operandStack;
		stack<string> operatorStack;

		for (string::iterator it = line.begin(); it != line.end();) {

			string readerToken;
			//using helper function readOneToken - return value is true if the token is operator
			if (readOneToken(line, it, readerToken)) {
				//while there are operators on stack that should be done first
				while (!operatorStack.empty() && operatorStack.top() != "(" &&
					precedenceCheck(getOp(operatorStack.top()), getOp(readerToken))) {
					//poping operator from stack and inserting into a tree
					TokenNode* newToken = this->nextInsertForTree(operandStack, operatorStack);
				}
				operatorStack.push(readerToken);
			}
			//brackets
			else if(readerToken == "(")
				operatorStack.push(readerToken);
			else if (readerToken == ")") {
				while (!operatorStack.empty() && operatorStack.top() != "(") {
					//poping operator from the stack and inserting into a tree
					TokenNode* newToken = this->nextInsertForTree(operandStack, operatorStack);
				}
				if (operatorStack.empty())
					throw MismatchedBrackets();
				operatorStack.pop();
			}
			//blank symbol
			else if (readerToken.empty())
				continue;
			//identifier or number
			else {
				TokenNode* newToken = new TokenNode("=", readerToken);
				operandStack.push(newToken);
			}
		}

		//if something is left on stack
		TokenNode* head;
		while (!operatorStack.empty()) {
			if (operatorStack.top() == "(")
				throw MismatchedBrackets();
			head = this->nextInsertForTree(operandStack, operatorStack);
		}
		this->syntaxTrees.push_back(head);
		operandStack.pop(); //head


		if (!operandStack.empty() || !operatorStack.empty())
			throw Mismatched();

		numOfLines++;
	}

	inputFile.close();
}

//help while reading
bool Compiler::readOneToken(const string & line, string::iterator & it, string& readerToken) {
	char c = *it;

	//skiping blank spaces
	if (c == ' ' || c == '\t') {
		it++;
		return false;
	}

	//brackets
	else if (c == '(' || c == ')') {
		readerToken += c;
		it++;
		return false;
	}

	//numbers - read until next character is not digit
	else if (isdigit(c) || c == '-' || c == '.') {
		readerToken += c;
		it++;
		//skip blank signs after minus sign
		if (c == '-') {
			while (it != line.end() && (*it == ' ')) it++;
			if (it == line.end())
				throw BadInput();
		}
		//if it is not last character in this line
		while (it != line.end() && (isdigit(*(it)) || (*it == '.'))) {
			c = *it;
			readerToken += c;
			it++;
		}

		return false;
	}

	//identifiers - always one letter
	else if (isalpha(c)) {
		readerToken += c;
		it++;
		return false;
	}

	//operations - read until next character is something mentioned above
	else {
		readerToken += c;
		while (it != line.end() && (*(it + 1) != ' ') && (*(it + 1)) != '\t' &&
			!isdigit(*(it + 1)) && !isalpha(*(it + 1)) && (*(it + 1) != '(') && (*(it + 1) != ')')) {
			c = *(++it);
			readerToken += c;
		}
		//check if operation symbol is valid
		if (getOp(readerToken) == nullptr)
			throw  NonexistentOperation();
	}
	it++;
	return true;
}

TokenNode* Compiler::nextInsertForTree(stack<TokenNode*>& operandStack, stack<string>& operatorStack) {
	//pop from the stack and make new token
	TokenNode* newToken = new TokenNode(operatorStack.top());
	operatorStack.pop();

	//reverse order of children - left operand was first in -> last out
	if (operandStack.empty())
		throw Mismatched();
	newToken->setRightChild(operandStack.top());
	operandStack.pop();
	if (operandStack.empty())
		throw Mismatched();
	newToken->setLeftChild(operandStack.top());
	operandStack.pop();

	//saving new token among operands
	operandStack.push(newToken);

	return newToken;
}

const OperationSyntax* Compiler::getOp(const string & symbol) {
	for (int i = 0; i < sizeof(Compiler::myOperations) / sizeof(Compiler::myOperations[0]); i++) {
		if (myOperations[i].symbol == symbol)
			return myOperations + i;
	}
	return nullptr;
}

//check if op1 should be done before op2
bool simpleCheck(const OperationSyntax* op1, const OperationSyntax* op2) {
	if((op1->precedence > op2->precedence) ||
		(op1->precedence == op2->precedence) && (op1->assoc == A_LEFT) ||
		(op1->precedence == op2->precedence) && (op1->assoc == A_NONE))
		return true;
	return false;
}
bool advancedCheck(const OperationSyntax* op1, const OperationSyntax* op2) {
	if ((op1->precedence > op2->precedence) ||
		(op1->precedence == op2->precedence) && (op1->assoc == A_LEFT))
		return true;
	return false;
}

//makes vector that shows order in which variables should be calculated
vector<string> Compiler::variableOrder() const {
	vector<string> vars;
	for (auto it : syntaxTrees) {
		string s = it->getLeftChild()->getData()->getName();
		vars.push_back(s);
	}
	return vars;
}
int Compiler::findVarInOrder(const TokenNode * node, vector<string> order) {
	int count = 0;
	for (auto it : order) {
		if (it == node->getData()->getName()) {
			return count;
		}
		count++;
	}
	return -1;
}

int Compiler::nodeWeight(TokenNode * node, vector<string> order) {
	int weight = 0, depth = 0;
	//non-nullptr nodes on the same level 
	queue<const TokenNode*> sameLevelNodes; //queue = first-in-first-out
											//first level is just root
	if (node != nullptr)
		sameLevelNodes.push(node);

	//iterating through levels
	while (!sameLevelNodes.empty()) {
		//count levels
		depth++;
		//pop every node on this level from stack and push its nonempty children
		int numOnThisLevel = sameLevelNodes.size();
		//for every variable count weight += depth*operationDelay* foundOrder
		for (int i = 0; i < numOnThisLevel; i++) {
			const TokenNode* node = sameLevelNodes.front();

			int index;
			if ((index = findVarInOrder(node, order)) != -1)
				weight += Compiler::getOp(node->getData()->getOperationSymbol())->delay * (index + 1) /Compiler::getInstance().simultAssign;
			else
				weight += getOp(node->getData()->getOperationSymbol())->delay;

			sameLevelNodes.pop();
			if (node->getLeftChild())
				sameLevelNodes.push(node->getLeftChild());
			if (node->getRightChild())
				sameLevelNodes.push(node->getRightChild());
		}
	}

	return weight;
}

//checks if one goes before two in order - if one should be lower
bool Compiler::nodeBeforeOtherVars(TokenNode * one, TokenNode * two, vector<string> order) {
	//check for vars
	int index1 = findVarInOrder(one, order);
	int index2 = findVarInOrder(two, order);
	
	//both nodes are variables
	if (index1 >= 0 && index2 >= 0 && index1 < index2)
		return true;
	
	//if one is number it is before
	if ((index1 == -1) && (index2 != -1) && (one->getData()->isNumber()))
		return true;
	return false;
}

bool Compiler::nodeBeforeOtherDepths(TokenNode * one, TokenNode * two, vector<string> order) {

	//depth check
	//tree depths of interest
	unsigned upperDepth = TokenNode::depth(one);
	unsigned lowerDepth = TokenNode::depth(two);
	//swap children to balance this tree
	if (lowerDepth > upperDepth)
		return true;
	return false;
}

bool Compiler::nodeBeforeOtherWeights(TokenNode * one, TokenNode * two, vector<string> order) {
	int w1 = nodeWeight(one, order);
	int w2 = nodeWeight(two, order);
	if (w1 < w2)
		return true;
	return false;
}

//return if swap happened
bool Compiler::advancedSortSwap(TokenNode* node, bool checkFunction(TokenNode*, TokenNode*, vector<string>)) {

	vector<string> varOrder = this->variableOrder();

	//case 1: left child exists and it is the same (non_assoc) operator as current node
	if (node->getLeftChild() &&
		(node->getData()->getOperationSymbol() == node->getLeftChild()->getData()->getOperationSymbol()) &&
		(getOp(node->getData()->getOperationSymbol())->assoc == A_NONE)) {
		
		if(checkFunction(node->getRightChild(), node->getLeftChild()->getLeftChild(), varOrder)) {
			TokenNode::swap(node, TokenNode::RIGHT, TokenNode::LEFT);
			return true;
		}
		else if (checkFunction(node->getRightChild(), node->getLeftChild()->getRightChild(), varOrder)) {
			TokenNode::swap(node, TokenNode::RIGHT, TokenNode::RIGHT);
			return true;
		}
	}

	//case 2: right child has the same operation as this node
	if (node->getRightChild() &&
		(node->getData()->getOperationSymbol() == node->getRightChild()->getData()->getOperationSymbol()) &&
		(getOp(node->getData()->getOperationSymbol())->assoc == A_NONE)) {
		//swap children to balance this tree
		if (checkFunction(node->getLeftChild(), node->getRightChild()->getLeftChild(), varOrder)) {
			TokenNode::swap(node, TokenNode::LEFT, TokenNode::LEFT);
			return true;
		}
		else if (checkFunction(node->getLeftChild(), node->getRightChild()->getRightChild(), varOrder)) {
			TokenNode::swap(node, TokenNode::LEFT, TokenNode::RIGHT);
			return true;
		}
	}
	return false;
}

//sorting token node tree for advanced reed
void Compiler::advancedSort(bool check(TokenNode*, TokenNode*, vector<string>)) {
	//for every line from input file
	for (auto it : this->syntaxTrees) {

		TokenNode* node = it, *nodeSub = node;
		stack<TokenNode*> unvisitedNodes, unvisitedNodesSub;
		/*//preorder
		while (!unvisitedNodes.empty() || node != nullptr) {
			if (node != nullptr) {
				unvisitedNodes.push(node->getRightChild());

				//preorder of the whole subtree
				nodeSub = node;
				while (!unvisitedNodesSub.empty() || nodeSub) {
					if (nodeSub) {
						unvisitedNodesSub.push(nodeSub->getRightChild());
						if (!advancedSortSwap(nodeSub, check))
							nodeSub = nodeSub->getLeftChild();
					}
					else {
						nodeSub = unvisitedNodesSub.top();
						unvisitedNodesSub.pop();
					}
				}
				
				node = node->getLeftChild();
			}
			else {
				//next node 
				node = unvisitedNodes.top();
				unvisitedNodes.pop();
			}
		}*/
		//postorder
		TokenNode* lastVisited = nullptr;
		while (!unvisitedNodes.empty() || node != nullptr) {
			if (node) {
				if (lastVisited != node->getRightChild()) {
					unvisitedNodes.push(node);
					unvisitedNodes.push(node->getRightChild());
					node = node->getLeftChild();
				}
				else {
					//preorder of the whole subtree
					nodeSub = node;
					while (!unvisitedNodesSub.empty() || nodeSub) {
						if (nodeSub) {
							unvisitedNodesSub.push(nodeSub->getRightChild());
							if (!advancedSortSwap(nodeSub, check))
								nodeSub = nodeSub->getLeftChild();
						}
						else {
							nodeSub = unvisitedNodesSub.top();
							unvisitedNodesSub.pop();
						}
					}
					//move to next node
					lastVisited = node;
					if (!unvisitedNodes.empty()) {
						node = unvisitedNodes.top();
						unvisitedNodes.pop();
					}
					else
						node = nullptr;
				}
			}
			else {
				//next node 
				lastVisited = node;
				node = unvisitedNodes.top();
				unvisitedNodes.pop();
			}
		}
	}
}

//Two modes for reading from input file into parsed tree
//simpleRead does linear parsing - how the expressions are written
void Compiler::simpleRead(const string & inputFileName) {
	read(inputFileName, simpleCheck);
}
void Compiler::advancedRead(const string & inputFileName) {
	read(inputFileName, advancedCheck);
	advancedSort(nodeBeforeOtherVars);
	//advancedSort(nodeBeforeOtherDepths);
	advancedSort(nodeBeforeOtherWeights);
}


void Compiler::save(const string & outputFileName) const {

	//for every expression from input file
	for (auto it : this->syntaxTrees) {
		//postorder
		TokenNode* curr = it, *lastVisited = nullptr;
		stack<TokenNode*> unvisited;

		while (!unvisited.empty() || curr) {
			if (curr) {
				//first time visiting this node
				if (!(lastVisited && lastVisited == curr->getRightChild()) &&
					!(curr->getLeftChild() == nullptr && curr->getRightChild() == nullptr)) {
					unvisited.push(curr);
					unvisited.push(curr->getRightChild());
					curr = curr->getLeftChild();
				}
				else {
					if(!(curr->getLeftChild() == nullptr && curr->getRightChild() == nullptr))
						Writer::getInstance().addImfLine(curr->getData(), curr->getLeftChild()->getData(), curr->getRightChild()->getData());
					lastVisited = curr;
					if (!unvisited.empty()) {
						curr = unvisited.top();
						unvisited.pop();
					}
					else curr = nullptr;
				}
			}
			else {
				//end of one branch 
				//popping nullptr AND the node without children
				if (!unvisited.empty()) {
					curr = unvisited.top();
					unvisited.pop();
				}
				else
					curr = nullptr;
			}
		}
	}

	Writer::getInstance().writeFile(outputFileName);
}