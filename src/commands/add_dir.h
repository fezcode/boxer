#include "command.h"

namespace boxer::commands {

	/** ADD_DIR 
	 * Format ADD_DIR <FILENAME> [AS <OUTPUT_FILENAME> ]
	 */
	class Add_Dir : public Command {
	public:
		Add_Dir(stringvec_t Payload, string_t Working_Dir);
		virtual ~Add_Dir();
		auto execute(const tar_t & tar) -> result::CommandResult;
		auto buildup() -> bool_t;

	protected:
		string_t working_dir;
		string_t source_filename;
		string_t dest_filename;
		bool_t custom_dest_given = false;
	};

}
