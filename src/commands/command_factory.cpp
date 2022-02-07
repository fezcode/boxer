//==================================
// Command Factory Implementation
//==================================
//

#include "command_factory.h"

namespace boxer::commands {

	/** 
	 * Conditions:
	 * 	- payload's first string must be name of command
	 * 	- payload's size should be greater than 0
	 */
	auto CommandFactory::createCommand(stringvec_t payload, string_t working_dir) -> command_ptr {
		string_t name = payload[0];
		std::shared_ptr<Command> command;
		
		// ARCHIVE_NAME
		if (boxer::string::iequals(name, type_str(ARCHIVE_NAME))) {
			command = std::make_shared<Archive_Name>(payload);
		}

		// ADD_FILE
		else if (boxer::string::iequals(name, type_str(ADD_FILE))) {
			command = std::make_shared<Add_File>(payload, working_dir);
		}

		// ADD_DIR
		else if (boxer::string::iequals(name,  type_str(ADD_DIR))) {
			command = std::make_shared<Add_Dir>(payload, working_dir);
		}

		
		// ADD_DIR
		else if (boxer::string::iequals(name,  type_str(ADD_FILE_MATCHING))) {
			command = std::make_shared<Add_File_Matching>(payload, working_dir);
		}

		command->buildup();

		return command;
	}

} //namespace boxer::commands
