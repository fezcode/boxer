#include "command.h"

namespace boxer::commands {

	class Archive_Name : public Command {
	public:
		Archive_Name(stringvec_t Payload);
		virtual ~Archive_Name();
		auto execute(const tar_t & tar) -> result::CommandResult;
		auto buildup() -> bool_t;
	};

}
