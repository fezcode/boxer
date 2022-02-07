//=================================================================================================
// Header-only TAR archive creator for C++17 and above
// Uses the standards defined in
// 		https://www.gnu.org/software/tar/manual/html_node/Standard.html
//
// This project uses parts of lindend project which is released under MIT License
// Go to link below for full license
// 		https://github.com/lindenb/cclindenb/blob/master/LICENSE.txt
//
//=================================================================================================
// 
// MIT License
//
// Copyright (c) 2020
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
//=================================================================================================


#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <memory>
#include <unistd.h>

// #include <cstdlib>

namespace fezcode::archiver {

	struct posix_header
	{                              /* byte offset */
		char name[100];               /*   0 */
	  	char mode[8];                 /* 100 */
	  	char uid[8];                  /* 108 */
	  	char gid[8];                  /* 116 */
	  	char size[12];                /* 124 */
	  	char mtime[12];               /* 136 */
	  	char chksum[8];               /* 148 */
	  	char typeflag;                /* 156 */
	  	char linkname[100];           /* 157 */
	  	char magic[6];                /* 257 */
	  	char version[2];              /* 263 */
	  	char uname[32];               /* 265 */
	  	char gname[32];               /* 297 */
	  	char devmajor[8];             /* 329 */
	  	char devminor[8];             /* 337 */
	  	char prefix[155];             /* 345 */
        char pad[12];				  /* 500 */
									  /* 512 */
	};

	#define TMAGIC   "ustar"        /* ustar and a null */
	#define TMAGLEN  6
	#define TVERSION "00"           /* 00 and no null */
	#define TVERSLEN 2
	
	/* Values used in typeflag field.  */
	#define REGTYPE  '0'            /* regular file */
	#define AREGTYPE '\0'           /* regular file */
	#define LNKTYPE  '1'            /* link */
	#define SYMTYPE  '2'            /* reserved */
	#define CHRTYPE  '3'            /* character special */
	#define BLKTYPE  '4'            /* block special */
	#define DIRTYPE  '5'            /* directory */
	#define FIFOTYPE '6'            /* FIFO special */
	#define CONTTYPE '7'            /* reserved */
	
	#define XHDTYPE  'x'            /* Extended header referring to the
	                                   next file in the archive */
	#define XGLTYPE  'g'            /* Global extended header */
	
	/* Bits used in the mode field, values in octal.  */
	#define TSUID    04000          /* set UID on execution */
	#define TSGID    02000          /* set GID on execution */
	#define TSVTX    01000          /* reserved */
	                                /* file permissions */
	#define TUREAD   00400          /* read by owner */
	#define TUWRITE  00200          /* write by owner */
	#define TUEXEC   00100          /* execute/search by owner */
	#define TGREAD   00040          /* read by group */
	#define TGWRITE  00020          /* write by group */
	#define TGEXEC   00010          /* execute/search by group */
	#define TOREAD   00004          /* read by other */
	#define TOWRITE  00002          /* write by other */
	#define TOEXEC   00001          /* execute/search by other */
	
	/* This class is used to create the tar file */
	class Tar {

		private:
			/* This flag is used to check whether initialization
			   of tar file; started, used internally */
			bool finished;
			bool startable;
			bool started;
			std::string outputFilename;
			std::ofstream output;

			auto record_init(const std::shared_ptr<posix_header> header) -> void;
			auto record_checksum(std::shared_ptr<posix_header> header) -> void;
			auto record_filesize(std::shared_ptr<posix_header> header, std::size_t filesize) -> void;
			auto record_filename(std::shared_ptr<posix_header> header, std::string filename) -> void;
			auto record_finalize(std::size_t len) -> void;


		public:
			Tar() : startable(false), started(false) { };
			Tar(std::string filename) : outputFilename(filename), startable(true), started(false) { };
			~Tar() {
				if(output.is_open()) {
					output.close();
				}
#ifdef FEZCODE_ARCHIVER_DEBUG	
				if(!finished) std::cerr << "archive creation did NOT finished properly" << std::endl;
#endif
			};

			auto open() -> bool;
			auto close() -> bool;
			auto isStarted() -> bool;
			auto setName(std::string name) -> void;
			auto getName() -> std::string;
			auto addFile(std::string original_filename, std::string archive_filename) -> bool;		
	};

	// Initalize a record.
	inline auto Tar::record_init(const std::shared_ptr<posix_header> header) -> void {
		std::strcpy(header->magic,   TMAGIC);
		std::strcpy(header->version, TVERSION);
		std::sprintf(header->mtime,  "%011lo", time(NULL));
	    std::sprintf(header->mode,   "%07o",   0100777);
	    std::sprintf(header->uid,    "%07o",   0);
	    std::sprintf(header->gid,    "%07o",   0);

		char login[256] = "";
		getlogin_r(login, 32);

		std::snprintf(header->uname, 32, "%s", login);
		// std::sprintf(header->gname, "%s", "users");
	}

	// Calculate checksum of header.
	inline auto Tar::record_checksum(std::shared_ptr<posix_header> header) -> void {
		int sum = 0;
		char *p = (char *) header.get();
		char *q = p + sizeof(posix_header);

		while (p < header->chksum)
			sum += *p++ & 0xff;

		for (int i=0; i < 8; ++i) {
			sum += ' ';
			++p;
		}

		while (p < q)
			sum += *p++ & 0xff;

		/*
		for (unsigned int i = 0; i < sizeof(posix_header); ++i) {
			sum += p[i];
		}
		*/

		std::sprintf(header->chksum, "%06o", sum);	
	}

	/* Write filesize to header */
	inline auto Tar::record_filesize(std::shared_ptr<posix_header> header, std::size_t filesize) -> void {
		std::sprintf(header->size, "%011llo", (long long unsigned int)filesize);
	}

	/* Write filename to header */
	inline auto Tar::record_filename(std::shared_ptr<posix_header> header, std::string filename) -> void {
		if (filename.empty() || filename.length() > 100) {
			std::cerr << "Archive name is invalid" << std::endl;
		}

		std::snprintf(header->name, 100, "%s", filename.c_str());
	}

	/* To make record size multiple of our block size = 512 */
	inline auto Tar::record_finalize(std::size_t len) -> void {
		char c='\0';

		while ( (len % sizeof(posix_header)) != 0 ) {
			output.write(&c, sizeof(char));
			++len;
		}
	}


	// Public Interface Methods

	/* Open tar file */
	inline auto Tar::open() -> bool {
		if (!startable) {
			std::cerr << "File name is not given. Call setName method first." << std::endl ;
			return false;
		}

		std::string arch_filename = (outputFilename.empty() ? "temp_archive.tar" : outputFilename);
		output = std::ofstream(arch_filename);

		if (!output.is_open()) {
			std::cerr << "Cannot create archive file." << std::endl;
			return false;
		}

		started = true;

		return true;
	}

	/* To complete a tar archive, append 2 empty blocks to the end of stream*/
	inline auto Tar::close() -> bool {
		finished = true;
		posix_header empty_header;
		
		std::memset((void*)&empty_header,0,sizeof(posix_header));

		output.write( (const char*) &empty_header, sizeof(posix_header) );
		output.write( (const char*) &empty_header, sizeof(posix_header) );
		output.flush();

		output.close();

		return true;
	}

	inline auto Tar::isStarted() -> bool {
		return started;
	}

	inline auto Tar::setName(std::string name) -> void {
		outputFilename = name;
	}

	inline auto Tar::getName() -> std::string {
		return outputFilename;
	}

	inline auto Tar::addFile(std::string original_filename, std::string archive_filename) -> bool {
		std::ifstream fs(original_filename.c_str(), std::ifstream::in | std::ifstream::binary );

		if (!fs.is_open()) {
			std::cerr << "`" << original_filename << "` cannot be opened. Skipping." << std::endl;
			return false;
		}

		fs.seekg(0, fs.end);
		std::streamsize length = fs.tellg();
		fs.seekg(0, fs.beg);

		// std::cout << "Input length:" << length << std::endl;

		std::shared_ptr<posix_header> header = std::make_shared<posix_header>();
		record_init(header);
		record_filename(header, archive_filename);
		header->typeflag = REGTYPE;
		record_filesize(header, length);
		record_checksum(header);
		output.write( (const char*)header.get(), sizeof(posix_header) );

		output << fs.rdbuf();
		fs.close();

		record_finalize(length);

		return true;
	}

}

