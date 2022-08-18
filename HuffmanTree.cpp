#include "HuffmanTree.h"

HuffmanTree::HuffmanTree()
{
	this->nodeCount = 0;
}

uint64_t HuffmanTree::getNumberOfNodes()
{
	return this->nodeCount;
}

std::vector<HuffmanSerializedNode> HuffmanTree::getSerializedNodes()
{
	return this->serializedNodes;
}

HuffmanTreeNode* HuffmanTree::unifyNodes(HuffmanTreeNode* left, HuffmanTreeNode* right)
{
	HuffmanTreeNode* commonNode = new HuffmanTreeNode(left, right);
	this->nodeCount++;
	return commonNode;
}

void HuffmanTree::constructTree(std::priority_queue<HuffmanTreeNode>* nodes)
{
	this->nodeCount = nodes->size();
	while (nodes->size() > 2) {
		HuffmanTreeNode node1 = nodes->top();
		nodes->pop();
		HuffmanTreeNode node2 = nodes->top();
		nodes->pop();

		HuffmanTreeNode *commonNode = unifyNodes(&node1, &node2);
		nodes->push(*commonNode);
		delete commonNode;
	}

	HuffmanTreeNode node1 = nodes->top();
	nodes->pop();
	HuffmanTreeNode node2 = nodes->top();
	nodes->pop();

	this->root = unifyNodes(&node1, &node2);
	delete nodes;
}

void HuffmanTree::serializeNode(HuffmanTreeNode* node, long long int* currentNodePos)
{
	(*currentNodePos)++;
	HuffmanSerializedNode serializedNode = {0, 0};
	if (!node->leftChild && !node->rightChild) {
		serializedNode.isTerminal = 1;
		serializedNode.value = node->value;
	} else {
		serializedNode.isTerminal = 0;
		serializedNode.childData.leftChild = *currentNodePos + 1;
		serializedNode.childData.rightChild = 0;
	}

	// Add to vector
	this->serializedNodes.emplace_back(serializedNode);
}

void HuffmanTree::__serializeTree(HuffmanTreeNode* node, long long int* currentNodePos, direction dir)
{
	if (!node)
		return;

	serializeNode(node, currentNodePos);

	if (dir == RIGHT) {
		// Search for the last node that has no rightChild (and is not terminal)
		for (long long int i = *currentNodePos - 1; i >= 0; i--) {
			if (!this->serializedNodes[i].isTerminal && !this->serializedNodes[i].childData.rightChild) {
				this->serializedNodes[i].childData.rightChild = *currentNodePos;
				break;
			} 
		}
	}

	__serializeTree(node->leftChild, currentNodePos, LEFT);
	__serializeTree(node->rightChild, currentNodePos, RIGHT);
}

void HuffmanTree::serializeTree()
{
	long long int currentNodePos = -1;

	this->serializedNodes.reserve(this->nodeCount);

	serializeNode(this->root, &currentNodePos);
	__serializeTree(this->root->leftChild, &currentNodePos, LEFT);
	__serializeTree(this->root->rightChild, &currentNodePos, RIGHT);
}

void HuffmanTree::__encodeSymbols(HuffmanTreeNode* node, std::vector<bool> symbolCode,
								  std::map<uint8_t, std::vector<bool>>* symbols, direction dir)
{
	if (!node)
		return;

	symbolCode.emplace_back(dir);

	if (!node->leftChild && !node->rightChild) {
		symbols->insert(std::pair<uint8_t, std::vector<bool>>(node->value, symbolCode));
	}

	__encodeSymbols(node->leftChild, symbolCode, symbols, LEFT);
	__encodeSymbols(node->rightChild, symbolCode, symbols, RIGHT);
}

std::map<uint8_t, std::vector<bool>>* HuffmanTree::encodeSymbols()
{
	std::map<uint8_t, std::vector<bool>>* symbols = new std::map<uint8_t, std::vector<bool>>;
	std::vector<bool> symbolCode;

	__encodeSymbols(this->root->leftChild, symbolCode, symbols, LEFT);
	__encodeSymbols(this->root->rightChild, symbolCode, symbols, RIGHT);

	return symbols;
}

void HuffmanTree::__freeTree(HuffmanTreeNode *node)
{
	if (!node)
		return;

	__freeTree(node->leftChild);
	__freeTree(node->rightChild);
	delete node;
}

HuffmanTree::~HuffmanTree()
{
	 __freeTree(this->root);
}