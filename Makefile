CC=g++
FLAGS=-Wall -Wextra -g

build: main.cpp processFile.cpp HuffmanTreeNode.cpp HuffmanTree.cpp
	$(CC) $(FLAGS) $^ -o comp
