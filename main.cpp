#include "processFile.h"

int main()
{
	compressFile("in/ana.txt");
	decompressFile("in/ana.txt.cmp");
	return 0;
}