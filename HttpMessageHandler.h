class HttpMessageHandler	{
public:
	// No-args constructor
	HttpMessageHandler();
	// Handles the given HTTP message, using the supplied connection
	// to provide output (if there is output)
	void handle(struct ns_connection *nc, struct http_message *hm);
};

