#include "loaders/fs_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <iostream>

#define TOTAL_CHARS 127

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " [output]" << std::endl;
		return 1;
	}

	srand(time(NULL));

	FILE* out = fopen(argv[1], "wb");
	if (out == NULL) {
		std::cerr << "Failed to open output file: " << argv[1] << std::endl;
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return -1;
	}

	FSProfileUnit unit;
	size_t unitSize = sizeof(FSProfileUnit);

	for (int i = 0; i < TOTAL_CHARS; ++i) {
		for (int j = 0; j < TOTAL_CHARS; ++j) {
			unit.from_key = i;
			unit.to_key = j;

			// Make a random interkeystroke time from 0-1 second
			unit.time_in_ms = (rand() % 1000);

			fwrite(&unit, unitSize, 1, out);
		}
	}

	fclose(out);

	std::cout << "Finished creating mock profile for the fs_loader: " << argv[1] << std::endl;
	return 0;
}
