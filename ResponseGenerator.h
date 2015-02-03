class ResponseGenerator	{
public:
	// No-args constructor
	ResponseGenerator();
	// Generates a JSON over HTTP response for the given connection
	void generateResponse(struct ns_connection *nc, struct http_message *hm);
};

