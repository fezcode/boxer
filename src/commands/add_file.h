#include "command.h"

namespace boxer::commands {

	class Add_File : public Command {
	public:
		Add_File(string_t Name, stringvec_t Payload);
		virtual ~Add_File();
		auto execute() -> bool_t;
		auto buildup() -> bool_t;

	protected:
		bool_t destNameParamExists = false;

	};

}
