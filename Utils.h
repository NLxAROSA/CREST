#include <string>
#include "zlib.h"

// gzip constants
#define MOD_GZIP_ZLIB_WINDOWSIZE 15
#define MOD_GZIP_ZLIB_CFACTOR    9
#define MOD_GZIP_ZLIB_BSIZE      8096

class Utils	{

public:

	// Returns true if the stringToSearch contains the stringToFind
	static 	bool contains(std::string stringToSearch, std::string stringToFind)	{
		if (stringToSearch.compare("") > 0)	{
			return ((int)stringToSearch.find(stringToFind) > (int)std::string::npos);
		}
		else{
			// stringToSearch is empty, return true (bit hacky, needs refactoring)
			return true;
		}
	}

	// gzips an input string with the supplied compression level
	static std::string gzipString(const std::string& str, int compressionlevel = Z_BEST_COMPRESSION)	{
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
		char* outbuffer = new char[32768];
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
			printf("Error during gzip compression: %s", zs.msg);
		}

		delete[] outbuffer;
		return outstring;
	}

};