#include "processFile.h"

HuffmanTreeNode* makeNode(uint8_t value, uint64_t occurences, uint64_t fileSize)
{
	HuffmanTreeNode* newNode = new HuffmanTreeNode();
	newNode->value = value;
	newNode->probability = ((double)occurences / fileSize);
	return newNode;
}

std::priority_queue<HuffmanTreeNode>* parseFile(char *fileName, uint32_t* fileSize)
{
	FILE *inputFile = fopen(fileName, "rb");
	std::map<uint8_t, uint32_t> symbolMap;

	uint8_t value;
	while (fread(&value, sizeof(value), 1, inputFile)) {
		if (symbolMap.count(value)) {
			symbolMap[value]++;
		} else {
			symbolMap.insert(std::pair<uint8_t, uint32_t>(value, 1));
		}

		(*fileSize)++;
	}

	std::priority_queue<HuffmanTreeNode> *nodes = new std::priority_queue<HuffmanTreeNode>();
	std::map<uint8_t, uint32_t>::iterator it;

	for (it = symbolMap.begin(); it != symbolMap.end(); ++it) {
		HuffmanTreeNode *node = makeNode(it->first, it->second, *fileSize);
		nodes->push(*node);
		delete node;
	}

	fclose(inputFile);
	return nodes;
}

uint8_t getByte(std::vector<bool> bitArray, size_t startPos)
{
	uint8_t byte = 0;
	for (size_t i = 0; i < 8; i++) {
		byte |= (bitArray[startPos + i] << i);
	}

	return byte;
}

std::vector<uint8_t>* encodeFile(char *fileName, std::map<uint8_t, std::vector<bool>> symbols)
{
	FILE *inputFile = fopen(fileName, "rb");

	std::vector<bool> bitArray;
	uint8_t value;
	while (fread(&value, sizeof(value), 1, inputFile)) {

		// Concatenate bit codes
		bitArray.insert(bitArray.end(), symbols[value].begin(), symbols[value].end());
	}

	// Add 0 padding
	while (bitArray.size() % 8) {
		bitArray.emplace_back(0);
	}

	std::vector<uint8_t>* encoding = new std::vector<uint8_t>;
	encoding->reserve(bitArray.size() / 8);
	for (size_t pos = 0; pos < bitArray.size(); pos += 8) {
		encoding->emplace_back(getByte(bitArray, pos));
	}

	fclose(inputFile);
	return encoding;
}

void compressFile(char *fileName)
{
	/* COMPRESSING DATA */
	HuffmanTree tree;
	uint32_t fileSize = 0;
	std::priority_queue<HuffmanTreeNode>* nodes = parseFile(fileName, &fileSize);

	tree.constructTree(nodes);
	tree.serializeTree();

	/* WRITING THE COMPRESSED DATA */
	char outputFile[strlen(fileName) + 5];
	sprintf(outputFile, "%s.cmp", fileName);

	FILE *out = fopen(outputFile, "wb");

	// Number of nodes to be read
	uint64_t nodeCount = tree.getNumberOfNodes();
	fwrite(&nodeCount, sizeof(uint8_t), 1, out);

	// Write serialized nodes
	std::vector<HuffmanSerializedNode> serializedNodes = tree.getSerializedNodes();
	for (uint64_t i = 0; i < nodeCount; i++) {
		HuffmanSerializedNode serializedNode = serializedNodes[i];
	 	fwrite(&serializedNode, sizeof(serializedNode), 1, out);
	}

	// Write original file size
	fwrite(&fileSize, sizeof(uint32_t), 1, out);

	// Write encoded bytes
	std::map<uint8_t, std::vector<bool>>* symbols = tree.encodeSymbols();
	std::vector<uint8_t>* encoding = encodeFile(fileName, *symbols);
	fwrite(encoding->data(), sizeof(uint8_t), encoding->size(), out);

	delete symbols;
	delete encoding;
	fclose(out);
}

std::vector<bool>* decodeByte(uint8_t byte)
{
	std::vector<bool>* decodedByte = new std::vector<bool>;
	for (size_t i = 0; i < 8; i++) {
		bool bit = (byte & (1 << i)) >> i;
		decodedByte->push_back(bit);
	}

	return decodedByte;
}

uint8_t getSymbol(std::vector<HuffmanSerializedNode> serializedNodes, int* nodePos, std::vector<bool> bitArray, int* bitPos)
{
	if (serializedNodes[*nodePos].isTerminal) {
		return serializedNodes[*nodePos].value;
	}

	if (bitArray[*bitPos]) {
		*nodePos = serializedNodes[*nodePos].childData.rightChild;
	} else {
		*nodePos = serializedNodes[*nodePos].childData.leftChild;
	}
	(*bitPos)++;
	
	return getSymbol(serializedNodes, nodePos, bitArray, bitPos);
}

void decompressFile(char *fileName)
{
	char outputFile[100];

	sprintf(outputFile, "%s.decomp", fileName);

	FILE *in = fopen(fileName, "rb");
	FILE *out = fopen(outputFile, "wb");

	uint8_t nodeCount = 0;
	if (!fread(&nodeCount, sizeof(uint8_t), 1, in))
		return;

	std::vector<HuffmanSerializedNode> serializedNodes;
	serializedNodes.reserve(nodeCount);
	for (int i = 0; i < nodeCount; i++) {
		HuffmanSerializedNode serializedNode = {0, 0};
		if (!fread(&serializedNode, sizeof(HuffmanSerializedNode), 1, in))
			return;
		serializedNodes.emplace_back(serializedNode);
	}

	uint32_t fileSize = 0;
	if (!fread(&fileSize, sizeof(uint32_t), 1, in))
		return;

	std::vector<uint8_t> encoding;
	uint8_t byte;
	while (fread(&byte, sizeof(uint8_t), 1, in)) {
		encoding.emplace_back(byte);
	}

	std::vector<bool> bitArray;
	for (uint8_t byte : encoding) {
		std::vector<bool>* decodedByte = decodeByte(byte);
		bitArray.insert(bitArray.end(), decodedByte->begin(), decodedByte->end());
		delete decodedByte;
	}

	int bitPos = 0;
	for (uint32_t i = 0; i < fileSize; i++) {
		int nodePos = 0;
		uint8_t symbol = getSymbol(serializedNodes, &nodePos, bitArray, &bitPos);
		fwrite(&symbol, sizeof(uint8_t), 1, out);
	}

	fclose(in);
	fclose(out);
}