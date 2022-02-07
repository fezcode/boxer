//============================
// Parser.cpp
//============================
//

#include "parser.h"

namespace boxer::parser {

	/**
	 * Ctor
	 * Preconditions:
	 * 	- File exists.
	 */
	Parser::Parser(string_t path) : filename(path){ 
		working_dir = boxer::string::getParentPath(path);
		log_dbg("File:" + filename);
		log_dbg("Working Directory:" + working_dir);
		commandFactory = std::make_shared<boxer::commands::CommandFactory>();
	}

	// Start point of processing file.
	bool_t Parser::processFile() {
		if(!readFile()) {
			log_war("Can NOT process file");
			exit(1);
		}

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
		// Read the next line from File until it reaches the end.
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
			log_dbg("Ignore comment");
			return;
		}

		stringvec_t words = boxer::string::split(line, " ");

		if (words.size() <= 0) {
			return;
		}

		log_dbg("Line: " + line);
		
		// log_dbg("Words:");
	    //     std::for_each(words.begin(),words.end(), [](const string_t &s){
		// 		log_dbg(s);
		// 		});

		log_dbg("");
		if (words.size() < 1) {
			return; 
		}	

		auto command = commandFactory->createCommand(words, working_dir);
		commands_list.push_back(command);
	}

	auto Parser::retrieveCommands() -> command_list_t {
		return commands_list;
	}

	auto Parser::getWorkingDir() -> string_t {
		return working_dir;
	}

} // namespace boxer::parser
