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
	auto CommandFactory::createCommand(stringvec_t payload) -> std::shared_ptr<Command> {
		string_t name = payload[0];
		std::shared_ptr<Command> command;
		// ADD_FILE
		if (boxer::string::iequals(name, "ADD_FILE")) {
			command = std::make_shared<Add_File>(name, payload);
			command->buildup();
		}

		// ADD_DIR
		else if (boxer::string::iequals(name, "ADD_DIR")) {

		}

		return command;
	}

} //namespace boxer::commands
