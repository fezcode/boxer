#pragma once
#include "../defs.h"
#include "../archiver/tarballer.h"
#include "result/command_result.h"
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace boxer::commands {

	enum CommandType {
		COMMAND_NONE,
		ARCHIVE_NAME,
		ADD_FILE,
		ADD_DIR,
		ADD_FILE_MATCHING,
		COMMAND_MAX
	};

	static auto type_str(CommandType type) -> string_t {
		switch (type)
		{
		case COMMAND_NONE: 		return "COMMAND_NONE";		break;
		case ARCHIVE_NAME: 		return "ARCHIVE_NAME";		break;
		case ADD_FILE: 			return "ADD_FILE";			break;
		case ADD_DIR: 			return "ADD_DIR";			break;
		case ADD_FILE_MATCHING: return "ADD_FILE_MATCHING";	break;
		case COMMAND_MAX: 		return "COMMAND_MAX";		break;
		default: 				return "ERR";	 			break;
		}
	}

	class Command {
	public:
		/* Since we are calling this ctor from its children,
		 * we should implement ctors and dtors
		 */
		Command(CommandType Type, stringvec_t Payload) {
			this->type    = Type;
			this->payload = Payload;
		}
		virtual ~Command() {}

		virtual auto execute(const tar_t & tar) -> result::CommandResult = 0;
		virtual auto buildup() -> bool_t = 0;
		inline auto getName() -> string_t { 
			return type_str(type);
		}
		inline auto getType() -> CommandType {
			return type;
		}
		

	protected:
		CommandType type;
		stringvec_t payload;

		// This one determines if given command is valid.
		// There might be a couple of reason why command may not be valid.
		bool_t   isValid;

		inline auto exists(string_t filename) -> bool_t {
			return fs::exists(fs::path(filename));
			//return ( access(filename.c_str(), 0 ) == 0 );
		}

		inline auto isFile(string_t filename) -> bool_t {
			return fs::is_regular_file(fs::path(filename));
		}

		inline auto isDirectory(string_t filename) -> bool_t  {
			return fs::is_directory(fs::path(filename));
		}
	};
}
