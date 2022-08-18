#include "processFile.h"

int main()
{
	compressFile("in/ana.txt");
	printf("FILE COMPRESSED\n");
	decompressFile("in/ana.txt.cmp");
	return 0;
}