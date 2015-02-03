#include <sstream>
#include "SharedMemory.h"

class JsonResponseBuilder	{
public:
	// No-args constructor
	JsonResponseBuilder();
	// Generates a JSON over HTTP response for the given connection
	void buildResponse(std::stringstream& ss, const SharedMemory* sharedData, std::string queryString);
};