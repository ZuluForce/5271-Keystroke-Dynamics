#include "ditto.h"

void printUsage(char* progName) {
	std::cout << std::endl << "Usage: " << progName;
	std::cout << " [-l{type}] [-i{info}]" << std::endl << std::endl;
	std::cout << "-l: defines the profile type that ditto should load" << std::endl;
	std::cout << "-i: information about the profile that the loader will need" << std::endl << std::endl;
}

void initializeOptionStruct(DittoOptions* options) {
	options->profileType = NULL;
	options->profileInfo = NULL;

	options->initialized = true;
}

int destroyOptionStruct(DittoOptions* options) {
	if (options == NULL || !options->initialized)
		return 1;

	free(options->profileInfo);
	free(options->profileType);
	free(options);

	return 0;
}

int readFlagValue(char** dst, char* src, int flagLen) {
	if (flagLen < 0 || dst == NULL || src == NULL)
		return 1;

	size_t len = strlen(src);
	len -= (flagLen-1);
	*dst = (char*) malloc(len);

	strncpy(*dst, src+(flagLen), len);
	return 0;
}

DittoOptions* parseCmdLine(int argc, char* argv[]) {
    DittoOptions* options = (DittoOptions*) malloc(sizeof(DittoOptions));
    if (options == NULL) {
        std::cerr << "Failed to allocate DittoOptions struct" << std::endl;
        exit(-1);
    }
    initializeOptionStruct(options);

    int status;

	// Start at 1 to skip the program name
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-l", 2) == 0) {
        	if (options->profileType != NULL) {
        		std::cerr << "Found duplicate -l flag. Ignoring (";
        		std::cerr << argv[i] << ")" << std::endl;
        		continue;
        	}
			status = readFlagValue(&options->profileType, argv[i], 2);
			if (status) {
				std::cerr << "Error copying flag argument value" << std::endl;
				printUsage(argv[0]);
				exit(-1);
			}
        } else if (strncmp(argv[i], "-i",2) == 0) {
        	if (options->profileInfo != NULL) {
        		std::cerr << "Found duplicate -i flag. Ignoring (";
        		std::cerr << argv[i] << ")" << std::endl;
        		continue;
        	}
			status = readFlagValue(&options->profileInfo, argv[i], 2);
			if (status) {
				std::cerr << "Error copying flag argument value" << std::endl;
				printUsage(argv[0]);
				exit(-1);
			}
        } else {
        	std::cerr << "Unknown argument: " << argv[i] << std::endl;
        	printUsage(argv[0]);
        	exit(1);
        }
    }

    return options;
}

int checkRequiredOptions(DittoOptions* options) {
	if (options == NULL)
		return -1;


	bool good = true;
	if (options->profileType == NULL) {
		std::cerr << "Missing profile loader type" << std::endl;
		good = false;
	}

	if (options->profileInfo == NULL) {
		std::cerr << "Missing profile information for loader" << std::endl;
		good = false;
	}

	return (good ? 0 : -1);
}


int main(int argc, char* argv[]) {
	std::cout << "A wild ditto appeared" << std::endl;

    DittoOptions* options = parseCmdLine(argc, argv);

	//Check that the fields got set
	if (checkRequiredOptions(options)) {
		std::cerr << "Required parameters missing" << std::endl;
		printUsage(argv[0]);

		return 1;
	}

	std::cout << "Ditto Options: " << std::endl;
	std::cout << "\tProfile Type: " << options->profileType << std::endl;
	std::cout << "\tProfile Info: " << options->profileInfo << std::endl;

	// Get the profile loader
	DittoProfileLoader* loader = getProfileLoader(options->profileType, options->profileInfo);
	if (loader == NULL) {
		std::cout << "No profile loader found for type: " << options->profileType << std::endl;
		exit(1);
	}
	std::cout << "\nCurrent profile loader name: " << loader->getProfileLoaderName() << std::endl;

	KDProfile profile = KDProfile(100);

	int status;
	status = loader->loadProfile(profile);
	if (status) {
		std::cerr << "Failed to load profile" << std::endl;
		exit(status);
	}

	BasicProfileEnforcer enforcer;
	enforcer.enforce(profile);

    return 0;
}
