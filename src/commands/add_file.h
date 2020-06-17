#include "command.h"
#include <unistd.h>

namespace boxer::commands {

	class Add_File : public Command {
	public:
		Add_File(string_t Name, stringvec_t Payload);
		virtual ~Add_File();
		auto execute() -> bool_t;
		auto buildup() -> bool_t;

	protected:
		inline auto fileExists(string_t filename) -> bool_t {
			return ( access(filename.c_str(), 0 ) == 0 );
		}

	};

}
