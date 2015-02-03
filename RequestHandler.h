class RequestHandler	{
public:
	// No-args constructor
	RequestHandler();
	// Generates a JSON over HTTP response for the given connection
	void handleRequest(struct ns_connection *nc, struct http_message *hm);
};

