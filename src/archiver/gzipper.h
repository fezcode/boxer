//=================================================================================================
// gzipper.h
//=================================================================================================
//

#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <memory>
#include <unistd.h>

#include <assert.h>
#include <zlib/zlib.h>

namespace asbuzz::archiver {

#define CHUNK 16384
#define windowBits 15
#define GZIP_ENCODING 16	

	class GZipper {
		
	private:
		std::string source_filename;
		std::string dest_filename;
		size_t filesize;

		// methods
		auto fileExists() -> bool;
		auto get_error(int ret) -> void;
		auto def() -> int;
	public:
		GZipper(std::string SourceFilename, std::string DistFilename) :
					source_filename(SourceFilename), dest_filename(DistFilename) { }

		~GZipper() {}

		auto compress() -> bool;
	
	};
	
	// private methods
	inline auto GZipper::fileExists() -> bool {
		return ( access(source_filename.c_str(), 0) == 0 );
	}

	 inline auto GZipper::def() -> int {
		if (!fileExists()) {
			std::cerr << "Source file " << source_filename << "does NOT exist" << std::endl;
			return Z_ERRNO;
		}

		FILE *source = fopen(source_filename.c_str(), "rb");
		FILE *dest   = fopen(dest_filename.c_str(), "w");
		int level    = Z_DEFAULT_COMPRESSION; 
		
		int ret, flush;
    	unsigned int have;
    	z_stream strm;
    	unsigned char in[CHUNK];
    	unsigned char out[CHUNK];

    	/* allocate deflate state */
    	strm.zalloc = Z_NULL;
    	strm.zfree = Z_NULL;
    	strm.opaque = Z_NULL;
		ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, windowBits | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY);
    	// ret = deflateInit(&strm, level);
    	if (ret != Z_OK)
    	    return ret;

    	/* compress until end of file */
    	do {
    	    strm.avail_in = fread(in, 1, CHUNK, source);
    	    if (ferror(source)) {
    	        (void)deflateEnd(&strm);
				fclose(source);
				fclose(dest);
    	        return Z_ERRNO;
    	    }
    	    flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
    	    strm.next_in = in;

    	    /* run deflate() on input until output buffer not full, finish
    	       compression if all of source has been read in */
    	    do {
    	        strm.avail_out = CHUNK;
    	        strm.next_out = out;
    	        ret = deflate(&strm, flush);    /* no bad return value */
    	        assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
    	        have = CHUNK - strm.avail_out;
    	        if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
    	            (void)deflateEnd(&strm);
					fclose(source);
					fclose(dest);
    	            return Z_ERRNO;
    	        }
    	    } while (strm.avail_out == 0);
    	    assert(strm.avail_in == 0);     /* all input will be used */

    	    /* done when last data in file processed */
    	} while (flush != Z_FINISH);
    	assert(ret == Z_STREAM_END);        /* stream will be complete */

    	/* clean up and return */
    	(void)deflateEnd(&strm);
		fclose(source);
		fclose(dest);
    	return Z_OK;
	}

	inline auto GZipper::get_error(int ret) -> void {
		switch (ret)
		{
		case Z_ERRNO:
			std::cerr << "file error" << std::endl;
			break;
		case Z_STREAM_ERROR:
			std::cerr << "invalid compression level" << std::endl;
			break;
		case Z_DATA_ERROR:
			std::cerr << "invalid or incomplete deflate data" << std::endl;
			break;
		case Z_MEM_ERROR:
			std::cerr << "out of memory" << std::endl;
			break;
		case Z_VERSION_ERROR:
			std::cerr << "zlib version mismatch!" << std::endl;
		default:
			std::cerr << "error occurred" << std::endl;
			break;
		}
	}

	// public method
	inline auto GZipper::compress() -> bool {
		if (def() == Z_OK)
			return true;
		return false;
	}
		
} // namespace asbuzz::archiver
