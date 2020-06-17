//===================================
// Parser.h
//===================================

#include "defs.h"
#include "utils/string_utils.h"
#include <algorithm>
#include "commands/command_factory.h"

namespace boxer::parser {

	enum Commands {
		COMMAND_NONE,
		ADD_FILE,
		ADD_DIR,
		COMMAND_MAX
	};

	class Parser {
	public:
		Parser(string_t path);
		bool_t processFile();

	private:
		string_t filename;
		string_t working_dir;
		stringvec_t contentsOfFile;
		pairListKeyString(Commands) commands;
		std::shared_ptr<boxer::commands::CommandFactory> commandFactory;

		// functions
		auto readFile() -> bool_t;
		auto printContents() -> void;
		auto getCommandFromLine(string_t line) -> void;
	};
}

