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

	//ScanCode codes[] = {SCANCODE_0, SCANCODE_1, SCANCODE_2, SCANCODE_3};

	int num_flys, num_presses;
	num_flys = num_presses = 0;
/*
	for (int i = 0; i < TOTAL_CHARS; ++i) {
		for (int j = 0; j < TOTAL_CHARS; ++j) {
			unit.from_key = codes[rand() % 4];
			unit.to_key = codes[rand() % 4];

			// Make a random interkeystroke time from 0-1 second
			unit.time_in_ms = (rand() % 1000);

			if (rand() % 2) {
				unit.time_type = FLY_TIME;
				++num_flys;
			} else {
				unit.time_type = PRESS_TIME;
				++num_presses;
			}

			fwrite(&unit, unitSize, 1, out);
		}
	}
	*/

	unit.from_key = SCANCODE_A;
	unit.to_key = SCANCODE_N;
	unit.time_in_ms = 500;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	unit.from_key = SCANCODE_N;
	unit.to_key = SCANCODE_D;
	unit.time_in_ms = 100;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	unit.from_key = SCANCODE_D;
	unit.to_key = SCANCODE_R;
	unit.time_in_ms = 1000;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	unit.from_key = SCANCODE_R;
	unit.to_key = SCANCODE_E;
	unit.time_in_ms = 600;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	unit.from_key = SCANCODE_E;
	unit.to_key = SCANCODE_W;
	unit.time_in_ms = 50;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	unit.from_key = SCANCODE_W;
	unit.to_key = SCANCODE_SPACE;
	unit.time_in_ms = 200;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	unit.from_key = SCANCODE_SPACE;
	unit.to_key = SCANCODE_H;
	unit.time_in_ms = 500;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	unit.from_key = SCANCODE_H;
	unit.to_key = SCANCODE_E;
	unit.time_in_ms = 100;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	unit.from_key = SCANCODE_E;
	unit.to_key = SCANCODE_L;
	unit.time_in_ms = 175;
	unit.time_type = FLY_TIME;
	fwrite(&unit, unitSize, 1, out);

	fclose(out);

	std::cout << "Finished creating mock profile for the fs_loader: " << argv[1] << std::endl;
	std::cout << "\tKey Presses: " << num_presses << std::endl;
	std::cout << "\tKey Flys: " << num_flys << std::endl;
	return 0;
}
