#include "HuffmanTree.h"

HuffmanTree::HuffmanTree()
{
	this->nodeCount = 0;
}

uint16_t HuffmanTree::getNumberOfNodes()
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

void HuffmanTree::serializeNode(HuffmanTreeNode* node, int* currentNodePos)
{
	(*currentNodePos)++;
	//HuffmanSerializedNode* serializedNode = new HuffmanSerializedNode();
	HuffmanSerializedNode serializedNode = {0, 0};
	if (!node->leftChild && !node->rightChild) {
		serializedNode.isTerminal = 1;
		serializedNode.value = node->value;
	} else {
		serializedNode.isTerminal = 0;
		serializedNode.childData.leftChild = *currentNodePos + 1;
	}

	// Add to vector
	this->serializedNodes.insert(this->serializedNodes.end(), serializedNode);
}

void HuffmanTree::__serializeTree(HuffmanTreeNode* node, int* currentNodePos,
								  int* parentNodePos, direction dir)
{
	if (!node)
		return;

	serializeNode(node, currentNodePos);

	if (dir == RIGHT) {
		this->serializedNodes[*parentNodePos].childData.rightChild = *currentNodePos;
		//printf("RIGHT: CURR: %d, ROOT: %d\n", *currentNodePos, *parentNodePos);
		(*parentNodePos)--;
	} else {
		*parentNodePos = *currentNodePos - 1;
		//printf("LEFT: CURR: %d, ROOT: %d\n", *currentNodePos, *parentNodePos);
	}

	__serializeTree(node->leftChild, currentNodePos, parentNodePos, LEFT);
	__serializeTree(node->rightChild, currentNodePos, parentNodePos, RIGHT);
}

void HuffmanTree::serializeTree()
{
	int currentNodePos = -1;
	int parentNodePos = 0;

	serializeNode(this->root, &currentNodePos);
	__serializeTree(this->root->leftChild, &currentNodePos, &parentNodePos, LEFT);
	__serializeTree(this->root->rightChild, &currentNodePos, &parentNodePos, RIGHT);
}

void HuffmanTree::__encodeSymbols(HuffmanTreeNode* node, std::vector<bool> symbolCode,
								  std::map<uint8_t, std::vector<bool>>* symbols, direction dir)
{
	if (!node)
		return;

	symbolCode.push_back(dir);

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

void __freeTree(HuffmanTreeNode *node)
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