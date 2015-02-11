#include <string>
#include "fossa.h"

class FossaUtils	{
public:

	// Returns the value of the header with the given name from the supplied HTTP message,
	// or empty string if no header with that name present.
	static std::string getHeaderValue(std::string name, struct http_message *hm)	{
		std::string result;
		for (size_t i = 0; i < ARRAY_SIZE(hm->header_names); i++)	{
			ns_str headerName = hm->header_names[i];
			std::string headerNameString(headerName.p, headerName.len);

			if (headerNameString.compare(name) == 0)	{
				ns_str headerValue = hm->header_values[i];
				result = std::string(headerValue.p, headerValue.len);
			}
		}
		return result;
	}

};