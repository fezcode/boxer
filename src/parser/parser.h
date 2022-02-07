//===================================
// Parser.h
//===================================

#pragma once
#include "defs.h"
#include "utils/string_utils.h"
#include <algorithm>
#include "commands/command_factory.h"

namespace boxer::parser {

	class Parser {
	public:
		Parser(string_t path);
		~Parser() { log_inf("~Parser"); }
		auto processFile() -> bool_t;
		auto retrieveCommands() -> command_list_t;
		auto getWorkingDir() -> string_t;

	private:
		string_t filename;
		string_t working_dir;
		stringvec_t contentsOfFile;
		command_list_t commands_list;
		std::shared_ptr<boxer::commands::CommandFactory> commandFactory;

		// functions
		auto readFile() -> bool_t;
		auto printContents() -> void;
		auto getCommandFromLine(string_t line) -> void;
		
	};
}

