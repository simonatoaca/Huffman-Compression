#pragma once
#include <cstdint>
#include <stdio.h>
#include <queue>
#include <map>
#include "HuffmanTreeNode.h"

class HuffmanTree {
private:
	uint64_t nodeCount;
	HuffmanTreeNode* root;
	std::vector<HuffmanSerializedNode> serializedNodes;
	typedef enum {LEFT, RIGHT} direction;

	HuffmanTreeNode* unifyNodes(HuffmanTreeNode* left, HuffmanTreeNode* right);
	void serializeNode(HuffmanTreeNode* node, long long int* currentNodePos);
	void __serializeTree(HuffmanTreeNode* node, long long int* currentNodePos, direction dir);
	void __encodeSymbols(HuffmanTreeNode* node, std::vector<bool> symbolCode,
						 std::map<uint8_t, std::vector<bool>>* symbols, direction dir);
	void __freeTree(HuffmanTreeNode* node);
public:
	HuffmanTree();
	~HuffmanTree();

	uint64_t getNumberOfNodes();
	std::vector<HuffmanSerializedNode> getSerializedNodes();
	
	void constructTree(std::priority_queue<HuffmanTreeNode>* nodes);
	
	void serializeTree();
	std::map<uint8_t, std::vector<bool>>* encodeSymbols();
};