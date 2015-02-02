// Constants
#define MAP_OBJECT_NAME "$pcars$"

#define HTTP_RESPONSE_503 "{\"status\": \"503 Service unavailable, is Project CARS running?\"}"
#define HTTP_RESPONSE_409 "{\"status\": \"409 Conflict, are CREST and Project CARS both at the latest version?\"}"

class JsonResponseGenerator	{
public:
	// No-args constructor
	JsonResponseGenerator();
	// Generates a JSON over HTTP response for the given connection
	void generateResponse(struct ns_connection *nc);
};

