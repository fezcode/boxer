#pragma once
#include "../defs.h"
#include <unistd.h>

namespace boxer::commands {

	class Command {
	public:
		/* Since we are calling this ctor from its children,
		 * we should implement ctors and dtors
		 */
		Command(string_t Name, stringvec_t Payload) {
			this->name = Name;
			this->payload = Payload;
		}
		virtual ~Command() {}

		virtual auto execute() -> bool_t = 0;
		virtual auto buildup() -> bool_t = 0;
		

	protected:
		string_t name;
		stringvec_t payload;
		// This one determines if given command is valid.
		// There might be a couple of reason why command may not be valid.
		bool_t   isValid;

		inline auto fileExists(string_t filename) -> bool_t {
			return ( access(filename.c_str(), 0 ) == 0 );
		}
	};
}
