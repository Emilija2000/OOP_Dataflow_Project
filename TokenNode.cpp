#include <queue>
#include "TokenNode.h"
using namespace std;

//iterative method for finding the depth of an unbalanced tree
unsigned TokenNode::depth(const TokenNode * subRoot){
	
	unsigned depth = 0; 

	//non-nullptr nodes on the same level 
	queue<const TokenNode*> sameLevelNodes; //queue = first-in-first-out
	//first level is just root
	if(subRoot != nullptr)
		sameLevelNodes.push(subRoot);

	//iterating through levels
	while (!sameLevelNodes.empty()) {

		//count levels
		depth++;

		//pop every node on this level from stack and push its nonempty children
		unsigned numOnThisLevel = sameLevelNodes.size();

		for (int i = 0; i < numOnThisLevel; i++) {
			const TokenNode* node = sameLevelNodes.front();
			sameLevelNodes.pop();
			if (node->getLeftChild())
				sameLevelNodes.push(node->getLeftChild());
			if (node->getRightChild())
				sameLevelNodes.push(node->getRightChild());
		}
	}

	return depth;
}

void TokenNode::swap(TokenNode*& one, TokenNode*& two) {
	TokenNode* p = one;
	one = two;
	two = p;

	/*int oneL = one->data.getName().length();
	int twoL = two->data.getName().length();
	if ((one->data.getName()[0] == 't') && (oneL > 1) &&
		(two->data.getName()[0] == 't') && (twoL > 1)) {
		Token::swapNames(one->data, two->data);
	}*/
}

void TokenNode::swap(TokenNode * node, CHILD_SIDE nodeChild, CHILD_SIDE childOfAChild) {
	if (nodeChild == LEFT && childOfAChild == LEFT)
		swap(node->leftChild, node->rightChild->leftChild);
	else if (nodeChild == LEFT && childOfAChild == RIGHT)
		swap(node->leftChild, node->rightChild->rightChild);
	else if (nodeChild == RIGHT && childOfAChild == LEFT)
		swap(node->rightChild, node->leftChild->leftChild);
	else if (nodeChild == RIGHT && childOfAChild == RIGHT)
		swap(node->rightChild, node->leftChild->rightChild);
}
