// Dependencies
#include "RequestHandler.h"
#include "JsonResponseBuilder.h"
#include "sharedmemory.h"
#include "fossa.h"
#include <sstream>
#include <vector>

// Constants
#define MAP_OBJECT_NAME "$pcars$"
#define HTTP_RESPONSE_503 "{\"status\": \"503 Service unavailable, is Project CARS running and is Shared Memory enabled?\"}"
#define HTTP_RESPONSE_409 "{\"status\": \"409 Conflict, are CREST and Project CARS both at the latest version?\"}"

static JsonResponseBuilder jsonResponseBuilder = JsonResponseBuilder();

RequestHandler::RequestHandler(){};

void sendServiceUnavailable(struct ns_connection *nc)    {
	// Send HTTP 503
	ns_printf(nc, "HTTP/1.1 503 Service unavailable\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Length: %d\r\n\r\n%s",
		(int)strlen(HTTP_RESPONSE_503), HTTP_RESPONSE_503);
}

void sendConflict(struct ns_connection *nc)    {
	// Send HTTP 409
	ns_printf(nc, "HTTP/1.1 409 Conflict\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Length: %d\r\n\r\n%s",
		(int)strlen(HTTP_RESPONSE_409), HTTP_RESPONSE_409);
}

std::string getQueryString(struct http_message *hm)	{
	if (hm->query_string.len > 0)	{
		std::string queryString (hm->query_string.p, hm->query_string.len);
		return queryString;
	}else{
		return "";
	}
}

void buildResponse(struct ns_connection *nc, const SharedMemory* sharedData, struct http_message *hm)  {

	// Build the JSON response
	std::stringstream ss;
	jsonResponseBuilder.buildResponse(ss, sharedData, getQueryString(hm));

	// build HTTP OK response with JSON response body
	ns_printf(nc, "HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Length: %d\r\n\r\n%s",
		(int)strlen(ss.str().c_str()), ss.str().c_str());
}

void processSharedMemoryData(struct ns_connection *nc, const SharedMemory* sharedData, struct http_message *hm)   {
	// Ensure we're sync'd to the correct data version
	if (sharedData->mVersion != SHARED_MEMORY_VERSION)	{
		// build conflict response
		sendConflict(nc);
		printf("Data version mismatch, please make sure that your pCARS version matches your CREST version\n");
	}
	else{
		buildResponse(nc, sharedData, hm);
	}

}

void processFile(struct ns_connection *nc, HANDLE fileHandle, struct http_message *hm)    {

	const SharedMemory* sharedData = (SharedMemory*)MapViewOfFile(fileHandle, PAGE_READONLY, 0, 0, sizeof(SharedMemory));

	if (sharedData == NULL)	{
		// File found, but could not be mapped to shared memory data
		sendServiceUnavailable(nc);
	}
	else{
		// Process file
		processSharedMemoryData(nc, sharedData, hm);
		// Unmap file
		UnmapViewOfFile(sharedData);
	}

}

void RequestHandler::handleRequest(struct ns_connection *nc, struct http_message *hm)	{
	// Open the memory mapped file
	HANDLE fileHandle = OpenFileMappingA(PAGE_READONLY, FALSE, MAP_OBJECT_NAME);

	if (fileHandle == NULL)	{
		// File is not available, build service unavailable response
		sendServiceUnavailable(nc);
	}
	else{
		// File is available, process the file
		processFile(nc, fileHandle, hm);
		// Close the file
		CloseHandle(fileHandle);
	}

}