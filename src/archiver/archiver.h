//====================================
// Compressor
//====================================

#pragma once

#include "../defs.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "lindenb/io/tarball.h"

namespace boxer::archiver {
	
	static auto createTar() -> bool_t {
		/* open file for writing */
		std::fstream out("archive.tar",std::ios::out);
		if(!out.is_open())
			{
			std::cerr << "Cannot open out" << std::endl;
			return EXIT_FAILURE;
			}
		/* create the tar file */
		lindenb::io::Tar tarball(out);
		/* add item 1 */
		tarball.putFile("../../test-files/tempfile.txt","tempfile.txt");
		/* finalize the tar file */
		tarball.finish();
		/* close the file */
		out.close();
		/* we're done */
		return true;

	}
}

