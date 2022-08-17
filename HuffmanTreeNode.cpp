#include "HuffmanTreeNode.h"

HuffmanTreeNode::HuffmanTreeNode()
{
	this->leftChild = nullptr;
	this->rightChild = nullptr;
	this->value = 0;
	this->probability = 0.0f;
}

HuffmanTreeNode::HuffmanTreeNode(HuffmanTreeNode *left, HuffmanTreeNode *right)
{
	this->leftChild = new HuffmanTreeNode();
	this->leftChild->leftChild = left->leftChild;
	this->leftChild->rightChild = left->rightChild;
	this->leftChild->probability = left->probability;
	this->leftChild->value = left->value;

	this->rightChild = new HuffmanTreeNode();
	this->rightChild->leftChild = right->leftChild;
	this->rightChild->rightChild = right->rightChild;
	this->rightChild->probability = right->probability;
	this->rightChild->value = right->value;

	this->probability = left->probability + right->probability;
	this->value = left->value < right->value ? left->value : right->value;
}