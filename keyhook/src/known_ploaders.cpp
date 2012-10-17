#include "known_ploaders.h"

DittoProfileLoader* getProfiler(std::string name, std::string data) {
	DittoProfileLoader* loader = NULL;

	// General filesystem profile loader.
	if (!name.compare("fs")) {
		loader = new FSProfileLoader(data);
	}

	return loader;
}
