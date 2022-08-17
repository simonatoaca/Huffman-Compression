#pragma once
#include <cstdint>
#include <stdio.h>
#include <queue>
#include <map>
#include "HuffmanTreeNode.h"

class HuffmanTree {
private:
	uint16_t nodeCount;
	HuffmanTreeNode* root;
	std::vector<HuffmanSerializedNode> serializedNodes;
	typedef enum {LEFT, RIGHT} direction;

public:
	HuffmanTree();
	~HuffmanTree();

	uint16_t getNumberOfNodes();
	std::vector<HuffmanSerializedNode> getSerializedNodes();
	HuffmanTreeNode* unifyNodes(HuffmanTreeNode* left, HuffmanTreeNode* right);
	void constructTree(std::priority_queue<HuffmanTreeNode>* nodes);
	void serializeNode(HuffmanTreeNode* node, int* currentNodePos);
	void __serializeTree(HuffmanTreeNode* node, int* currentNodePos,
						 int* rootNodePos, direction dir);
	void serializeTree();

	void __encodeSymbols(HuffmanTreeNode* node, std::vector<bool> symbolCode,
						 std::map<uint8_t, std::vector<bool>>* symbols, direction dir);
	std::map<uint8_t, std::vector<bool>>* encodeSymbols();
};