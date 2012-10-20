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
		return -1;
	}

	size_t read, numRead;
	size_t unitSize = sizeof(FSProfileUnit);
	FSProfileUnit unit;

	int from_index, to_index, max_index;
	max_index = profile.size();

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
		from_index = (int) unit.from_key;
		to_index = (int) unit.to_key;
		if (from_index >= max_index || to_index >= max_index) {
			std::cerr << "Invalid from/to_key value: from = " << from_index;
			std::cerr << " to = " << to_index << " max = " << max_index << std::endl;
			fclose(input);
			return -1;
		}

		profile.at(from_index).at(to_index) = unit.time_in_ms;
		++numRead;
	}

	std::cout << "\tFSProfileLoader finished loading" << std::endl;
	fclose(input);
	return 0;
}
