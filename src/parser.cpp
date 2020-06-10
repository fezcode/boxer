//============================
// Parser.cpp
//============================
//

#include "parser.h"

namespace boxer::parser {

	// ctor
	Parser::Parser(string_t path) : filename(path) { }

	// Start point of processing file.
	bool_t Parser::processFile() {
		if(!readFile()) {
			std::cout << "Can NOT process file" << std::endl;
			exit(1);
		}

		printContents();

		return true;
	}


	// private functions
	// -----------------
	
	/**
	 * Reads file line by line and puts them into contentsOfFile string vector
	 */	
	bool_t Parser::readFile() {
		// Open the File
		ifstream_t in{filename.c_str()};
		 
		// Check if object is valid
		if(!in) {
			std::cerr << "Cannot open the File : " << filename << std::endl;
			return false;
		}
	 								 
	 	string_t str;
		// Read the next line from File untill it reaches the end.
		//
		while (std::getline(in, str)) {
		// Line contains string of length > 0 then save it in vector
		if(str.size() > 0)
			contentsOfFile.push_back(str);
		}

		//Close The File
		in.close();
		return true;
	}

	auto Parser::printContents() -> void {
		for (auto const& c : contentsOfFile)
			std::cout << c << std::endl;
	}

}
