// Constants
#define MAP_OBJECT_NAME "$pcars$"

#define HTTP_RESPONSE_503 "HTTP/1.1 503 Service unavailable\r\nContent-Length: 0\r\n\r\n"
#define HTTP_RESPONSE_409 "HTTP/1.1 409 Conflict\r\nContent-Length: 0\r\n\r\n"

class JsonResponseGenerator	{
public:
	// No-args constructor
	JsonResponseGenerator();
	// Generates a JSON over HTTP response for the given connection
	void generateResponse(struct ns_connection *nc);
};

