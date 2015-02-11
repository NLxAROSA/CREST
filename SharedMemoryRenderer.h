#include <sstream>
#include "SharedMemory.h"

class SharedMemoryRenderer	{
public:
	// No-args constructor
	SharedMemoryRenderer();
	// Renders the shared memory into a string representation
	// The queryString determines which sections are rendered or not
	std::string render(const SharedMemory* sharedData, std::string queryString);
};