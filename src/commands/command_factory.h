//=======================================
// Command Factory
//=======================================
//

#pragma once

#include "command.h"
#include "../defs.h"
#include "../utils/string_utils.h"
#include "all_commands.h"

namespace boxer::commands {

	class CommandFactory {
	public:
		auto createCommand(stringvec_t payload) -> command_ptr; 
	};

} // namespace boxer::commands
