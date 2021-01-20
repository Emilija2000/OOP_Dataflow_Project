#ifndef COMPILER_H
#define COMPILER_H

#include <stack>
#include <string>
#include <vector>
#include "Token.h"
#include "TokenNode.h"
#include "Writer.h"

using namespace std;

/*Compiler views the code as text only - doesn't need to know what operations 'mean', just the syntax
  The rest of the program doesn't need to know about textual part, only the info that compiler provides*/

//Two modes of compilation
enum CompileMode { SIMPLE, ADVANCED };

//Everything compiler has to know about operations
enum associativity { A_NONE, A_LEFT, A_RIGHT };

struct OperationSyntax {
	OperationSyntax(string symbol, int precedence, associativity assoc, Time t) :
		symbol(symbol), precedence(precedence), assoc(assoc), delay(t) {};

	string symbol;
	int precedence;
	associativity assoc;
	Time delay;
};


//Compiler translates program file (without configuration) into intermediate form. Singleton.
class Compiler {
public:
	//Singleton DP - getter for the only instance
	//As compiler needs to know the writer there is also setup function
	static Compiler& setInstance(Time ta, Time tm, Time te, Time tw, int nw);
	static Compiler& getInstance();
	Compiler(Compiler&) = delete;
	Compiler& operator=(Compiler&) = delete;

	//Complete translation, saving .imf 
	void compile(const string& filename, CompileMode mode);

protected:
	//Singleton DP - constructor
	Compiler(Time ta, Time tm, Time te, Time tw, int nw);
	~Compiler();

	void clear();

	//Reading of the file that should be translated
	void read(const string & inputFileName, bool precedenceCheck(const OperationSyntax*, const OperationSyntax*));
	void simpleRead(const string& inputFileName);
	void advancedRead(const string& inputFileName);
	//Saving compiled file with intermediate form (.imf)
	void save(const string& outputFileName) const;

private:

	//Helper functions
	//Reading one token (identifier, operation symbol,...) from line of input file
	//Returns if read token is operation symbol or not
	bool readOneToken(const string& line, string::iterator& it, string& token);
	//Insert one token into the tree from the information on the stack, return the reference of new tree head
	TokenNode* nextInsertForTree(stack<TokenNode*>& operandStack, stack<string>& operatorStack);
	//Sort tree after reading it - balancing syntaxTrees (using check function)
	void advancedSort(bool check(TokenNode*, TokenNode*, vector<string>));
	bool advancedSortSwap(TokenNode* node, bool check(TokenNode*, TokenNode*, vector<string>));
	//sort variables - which should be written in memory first
	vector<string> variableOrder() const;
	static int findVarInOrder(const TokenNode* node, vector<string> order);
	static int nodeWeight(TokenNode* node, vector<string> order);
	static bool nodeBeforeOtherVars(TokenNode* one, TokenNode* other, vector<string> order); //just var check
	static bool nodeBeforeOtherDepths(TokenNode* one, TokenNode* other, vector<string> order); //just depth check
	static bool nodeBeforeOtherWeights(TokenNode * one, TokenNode * two, vector<string> order); //weights - depth&&var check

	//Atributes
	std::vector<TokenNode*> syntaxTrees; 
	int simultAssign;

	//Connect operation symbol to everything compiler knows about it
	static const OperationSyntax* getOp(const string& symbol);
	//Defined operators for this compiler
	static OperationSyntax myOperations[];

};

#endif
