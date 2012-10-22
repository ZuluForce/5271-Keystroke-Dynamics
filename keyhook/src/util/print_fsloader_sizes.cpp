#include "loaders/fs_loader.h"

int main(int argc, char* argv[]) {
	char fmt_str[] = "%d %d";

	fprintf(stdout, fmt_str, sizeof(FSProfileUnit), sizeof(FSProfileTimeType));

	return 0;
}
