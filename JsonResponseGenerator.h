// Constants
#define MAP_OBJECT_NAME "$pcars$"

#define HTTP_HEADER_NO_CONTENT "Content-Length:0\r\n"
#define HTTP_RESPONSE_503 "HTTP/1.1 503 Service Unavailable\r\n"
#define HTTP_RESPONSE_409 "HTTP/1.1 409 Conflict\r\n"
#define HTTP_RESPONSE_200 "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"

class JsonResponseGenerator	{
public:
	// No-args constructor
	JsonResponseGenerator();
	// Generates a JSON over HTTP response for the given connection
	void generateResponse(struct ns_connection *nc);
};

