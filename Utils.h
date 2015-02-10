#include <string>

static 	bool contains(std::string stringToSearch, std::string stringToFind)	{
	// Returns true if the stringToSearch contains the stringToFind
	if (stringToSearch.compare("") > 0)	{
		return ((int)stringToSearch.find(stringToFind) > (int)std::string::npos);
	}
	else{
		// stringToSearch is empty, return true (bit hacky, needs refactoring)
		return true;
	}
}
