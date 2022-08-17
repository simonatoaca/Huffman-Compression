#pragma once
#include <cstdint>
#include <map>
#include "HuffmanTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

std::priority_queue<HuffmanTreeNode>* parseFile(char *fileName, uint32_t* fileSize);
std::vector<uint8_t>* encodeFile(char *fileName, std::map<uint8_t, std::vector<bool>> symbols);
void compressFile(char *fileName);
void decompressFile(char *fileName);