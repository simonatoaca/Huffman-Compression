#pragma once
#include <cstdint>
#include <stdio.h>
#include <queue>

typedef struct HuffmanTreeNode {
	float probability;

	uint8_t value;
	HuffmanTreeNode* leftChild;
	HuffmanTreeNode* rightChild;

	HuffmanTreeNode();
	HuffmanTreeNode(HuffmanTreeNode *left, HuffmanTreeNode *right);
	friend bool operator<(const HuffmanTreeNode& left, const HuffmanTreeNode& right)
	{
		if (left.probability != right.probability) {
			return left.probability > right.probability;
		} else {
			return left.value > right.value;
		}
	}
} HuffmanTreeNode;

typedef struct {
  	uint8_t isTerminal;
  	union {
	    uint8_t value;
	    struct {
	      uint16_t leftChild;
	      uint16_t rightChild;
	    } __attribute__((__packed__)) childData;
  	} __attribute__((__packed__));
} __attribute__((__packed__)) HuffmanSerializedNode;