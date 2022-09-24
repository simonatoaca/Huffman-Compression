#include "processFile.h"
#include <ctime>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Not enough args. Usage: ./comp -d <file>.cmp or ./comp -c <file>");
		return -1;
	}

	if (!strcmp(argv[1], "-c")) {
		compressFile(argv[2]);
	} else if (!strcmp(argv[1], "-d")) {
		decompressFile(argv[2]);
	} else {
		fprintf(stderr, "Incorrect args. Usage: ./comp -d <file>.cmp or ./comp -c <file>");
	}

	return 0;
}