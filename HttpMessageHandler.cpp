// Dependencies
#include "HttpMessageHandler.h"
#include "FossaUtils.h"
#include "SharedMemoryRenderer.h"
#include "Utils.h"
#include "sharedmemory.h"
#include "fossa.h"
#include <sstream>

// Constants
#define MAP_OBJECT_NAME "$pcars$"
#define HTTP_RESPONSE_503 "{\"status\": \"503 Service unavailable, is Project CARS running and is Shared Memory enabled?\"}"
#define HTTP_RESPONSE_409 "{\"status\": \"409 Conflict, are CREST and Project CARS both at the latest version?\"}"
#define GZIP_THRESHOLD 128

static SharedMemoryRenderer sharedMemoryRenderer = SharedMemoryRenderer();

HttpMessageHandler::HttpMessageHandler(){};

// Outputs an HTTP 503 on the supplied connection
void sendServiceUnavailable(struct ns_connection *nc)    {
	ns_printf(nc, "HTTP/1.1 503 Service unavailable\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Length: %d\r\n\r\n%s",
		(int)strlen(HTTP_RESPONSE_503), HTTP_RESPONSE_503);
}

// Outputs an HTTP 409 on the supplied connection
void sendConflict(struct ns_connection *nc)    {
	// Send HTTP 409
	ns_printf(nc, "HTTP/1.1 409 Conflict\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Length: %d\r\n\r\n%s",
		(int)strlen(HTTP_RESPONSE_409), HTTP_RESPONSE_409);
}

// Extracts the query string from the given HTTP message
std::string getQueryString(struct http_message *hm)	{
	if (hm->query_string.len > 0)	{
		std::string queryString (hm->query_string.p, hm->query_string.len);
		return queryString;
	}else{
		return "";
	}
}

// Returns true if the response to the given HTTP message should
// be gzipped, based on the value of the Accept-Encoding header
// and the size of the uncompressed response
bool shouldGzipResponse(struct http_message *hm, int responseLength)	{
	return Utils::contains(FossaUtils::getHeaderValue("Accept-Encoding", hm), "gzip") && responseLength > GZIP_THRESHOLD;
}

// Renders the response
void renderResponse(struct ns_connection *nc, const SharedMemory* sharedData, struct http_message *hm)  {

	std::string responseJson = sharedMemoryRenderer.render(sharedData, getQueryString(hm));
	std::string response;

	bool gzipResponse = shouldGzipResponse(hm, responseJson.size());

	if (gzipResponse)	{
		response = Utils::gzipString(responseJson);
	}else{
		response = responseJson;
	}

	// build HTTP OK response with JSON response body
	ns_printf(nc, "HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n");
	if (gzipResponse)	{
		ns_printf(nc, "Content-Encoding: gzip\r\n");
	}
	ns_printf(nc, "Content-Length: %d\r\n\r\n",
		(int)response.size());
	ns_send(nc, response.data(), response.size());

}

// Processes the shared memory
void processSharedMemoryData(struct ns_connection *nc, const SharedMemory* sharedData, struct http_message *hm)   {
	// Ensure we're sync'd to the correct data version
	if (sharedData->mVersion != SHARED_MEMORY_VERSION)	{
		// build conflict response
		sendConflict(nc);
		printf("Data version mismatch, please make sure that your pCARS version matches your CREST version\n");
	}else{
		renderResponse(nc, sharedData, hm);
	}

}

// Processes the memory mapped file
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

void HttpMessageHandler::handle(struct ns_connection *nc, struct http_message *hm)	{
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