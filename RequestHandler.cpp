// Dependencies
#include "RequestHandler.h"
#include "JsonResponseBuilder.h"
#include "sharedmemory.h"
#include "fossa.h"
#include <sstream>
#include <vector>
#include "zlib.h"
#include "Utils.h"
// Constants
#define MAP_OBJECT_NAME "$pcars$"
#define HTTP_RESPONSE_503 "{\"status\": \"503 Service unavailable, is Project CARS running and is Shared Memory enabled?\"}"
#define HTTP_RESPONSE_409 "{\"status\": \"409 Conflict, are CREST and Project CARS both at the latest version?\"}"

#define MOD_GZIP_ZLIB_WINDOWSIZE 15
#define MOD_GZIP_ZLIB_CFACTOR    9
#define MOD_GZIP_ZLIB_BSIZE      8096

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

std::string gzipString(const std::string& str,	int compressionlevel = Z_BEST_COMPRESSION)	{
	z_stream zs;
	memset(&zs, 0, sizeof(zs));

	if (deflateInit2(&zs,
		compressionlevel,
		Z_DEFLATED,
		MOD_GZIP_ZLIB_WINDOWSIZE + 16,
		MOD_GZIP_ZLIB_CFACTOR,
		Z_DEFAULT_STRATEGY) != Z_OK
		) {
		throw(std::runtime_error("deflateInit2 failed while compressing."));
	}

	zs.next_in = (Bytef*)str.data();
	zs.avail_in = str.size();

	int ret;
	char outbuffer[32768];
	std::string outstring;

	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = deflate(&zs, Z_FINISH);

		if (outstring.size() < zs.total_out) {
			outstring.append(outbuffer,
				zs.total_out - outstring.size());
		}
	} while (ret == Z_OK);

	deflateEnd(&zs);

	if (ret != Z_STREAM_END) {
		std::ostringstream oss;
		oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
		throw(std::runtime_error(oss.str()));
	}

	return outstring;
}

bool shouldGzipResponse(struct http_message *hm)	{
	std::string http_message(hm->message.p, hm->message.len);

	if (contains(http_message, "Accept-Encoding") && contains(http_message, "gzip"))	{
		return true;
	}

	return false;
}

void buildResponse(struct ns_connection *nc, const SharedMemory* sharedData, struct http_message *hm)  {

	// Build the JSON response
	std::stringstream ss;
	jsonResponseBuilder.buildResponse(ss, sharedData, getQueryString(hm));

	//const char* deflatedResponse = compress_string(ss.str().data());

	std::string responseJson = ss.str();
	std::string response;

	bool gzipResponse = shouldGzipResponse(hm);

	if (gzipResponse)	{
		response = gzipString(responseJson);
	}
	else{
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