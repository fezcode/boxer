//============================
// Parser.cpp
//============================
//

#include "parser.h"

namespace boxer::parser {

	// ctor
	/**
	 * Preconditions:
	 * 	- File exists.
	 */
	Parser::Parser(string_t path) : filename(path){ 
		const size_t last_slash_idx = path.rfind('/');
		if (std::string::npos != last_slash_idx)
		{
		       working_dir = path.substr(0, last_slash_idx);
		}

		log_dbg("File:" + filename + "WD:" + working_dir);
		 
		commandFactory = std::make_shared<boxer::commands::CommandFactory>();
		
	}

	// Start point of processing file.
	bool_t Parser::processFile() {
		if(!readFile()) {
			log_war("Can NOT process file");
			exit(1);
		}

		// printContents();
		
		std::for_each(contentsOfFile.begin(), contentsOfFile.end(),
				[&](const string_t& fileline) {
					getCommandFromLine(fileline);
				});		
		
		// start making sense of each line

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
			log_err("Cannot open the File : " + filename);
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
			log_dbg(c);
	}

	auto Parser::getCommandFromLine(string_t line) -> void {
		// Ignore line if it is a comment.
		if (boxer::string::beginsWith(line, "#")) {
			log_inf("Ignore comment");
			return;
		}

		stringvec_t words = boxer::string::split(line, " ");

		if (words.size() <= 0) {
			return;
		}

		log_dbg("Line:");
		log_dbg(line);
		log_dbg("Words:");
	        std::for_each(words.begin(),words.end(), [](const string_t &s){
				log_dbg(s);
				});

		log_dbg("");
		if (words.size() < 1) {
			return; 
		}	

		commandFactory->createCommand(words);
	}

} // namespace boxer::parser


