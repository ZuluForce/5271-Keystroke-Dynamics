#include "loaders/fs_loader.h"

FSProfileLoader::FSProfileLoader(std::string data)
: DittoProfileLoader(data) {
	return;
}

std::string FSProfileLoader::getProfileLoaderName() {
	return "FileSystem binary format profile loader";
}

std::vector<std::string>* FSProfileLoader::enumerateAvailableProfiles() {
	std::vector<std::string>* avail = new std::vector<std::string>();
	if (avail == NULL) return NULL;

	avail->push_back("Profile enumeration not supported");

	return avail;
}

// This will use whatever data was passed in as the profile to load
int FSProfileLoader::loadProfile(KDProfile& profile) {
	KDProfileID newId(this->profileData);

	return this->loadProfile(profile, newId);
}

int FSProfileLoader::loadProfile(KDProfile& profile, KDProfileID& id) {
	std::cout << "\nFSProfileLoader loading " << id.getProfileName() << std::endl;

	FILE* input = fopen(id.getProfileName().c_str(), "rb");
	if (input == NULL) {
		// Should throw some sort of exception here
		std::cerr << "Failed to open profile file" << std::endl;
		std::cerr << "Error: " << strerror(errno) << std::endl;
		std::cerr << "Trying with .ditto extension" << std::endl;

		std::string name = id.getProfileName() + ".ditto";
		input = fopen(name.c_str(), "rb");
		if (input == NULL) {
			std::cerr << "Failed to open again" << std::endl;
			return -1;
		}
	}

	size_t read, numFly, numPress, numRead;
	read = numFly = numPress = numRead = 0;

	size_t unitSize = sizeof(FSProfileUnit);
	FSProfileUnit unit;

	int from_index, to_index;

	while (!feof(input)) {
		read = fread(&unit, unitSize, 1, input);
		if (read != 1) {
			if (feof(input))
				break; //End of input

			std::cerr << "\n\n!!! ----------------------------- !!!" << std::endl;
			std::cerr << "Profile unit missing or incomplete: unit " << numRead+1 << std::endl;
			std::cerr << "Byte offset into file: " << numRead*unitSize << std::endl;
			fclose(input);
			return -1;
		}
		//Otherwise we got what we believe is a valid profile
		from_index = (int) unit.from_key; // Originally a ScanCode enum value
		to_index = (int) unit.to_key;

		if (unit.time_type == FLY_TIME) {
			profile.addFlyTime(from_index, to_index, unit.time_in_ms * MICRO_IN_MILLI);
			++numFly;
		} else if (unit.time_type == PRESS_TIME) {
			profile.addPressTime(from_index, unit.time_in_ms * MICRO_IN_MILLI);
			++numPress;
		} else {
			std::cerr << "Unknown time tpye in the loaded FSProfileUnit: " << unit.time_type << std::endl;
			fclose(input);
			return -1;
		}

		++numRead;
	}

	std::cout << "\tFSProfileLoader finished loading" << std::endl;
	std::cout << "\tTotal Read: " << numRead << std::endl;
	std::cout << "\tNum Fly Times: " << numFly << std::endl;
	std::cout << "\tNum Press Times: " << numPress << std::endl;
	std::cout << "\tAverage Fly Time: " << profile.getAverageFlyTimeMs() << " ms" << std::endl;
	std::cout << "\tAverage Press Time: " << profile.getAveragePressTimeMs() << " ms" << std::endl;
	fclose(input);
	return 0;
}
