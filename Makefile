CC=g++
FLAGS=-Wall -Wextra -g -O2

build: main.cpp processFile.cpp HuffmanTreeNode.cpp HuffmanTree.cpp
	$(CC) $(FLAGS) $^ -o comp
