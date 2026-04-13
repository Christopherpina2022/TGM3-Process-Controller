#include <Windows.h>
#include <fstream>
#include <iostream>
#include "include/config.h"

// Write our passed arguments into a .bin config file for our DLL file to read
bool WriteConfig(const ResolutionConfig& cfg, const char* path) {
	std::ofstream file(path, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Error: Config setup has failed: could not open " << path << "\n";
		return false;
	}

	file.write(reinterpret_cast<const char*>(&cfg), sizeof(cfg));
	file.close();

	std::cout << "Config written successfully. \n";
	return true;
}