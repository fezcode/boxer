#include "command.h"

namespace boxer::commands {

	/** ADD_FILE 
	 * Format ADD_FILE <FILENAME> [AS <OUTPUT_FILENAME> ]
	 */
	class Add_File : public Command {
	public:
		Add_File(stringvec_t Payload, string_t Working_Dir);
		virtual ~Add_File();
		auto execute(const tar_t & tar) -> result::CommandResult;
		auto buildup() -> bool_t;

	protected:
		string_t working_dir;
		string_t source_filename;
		string_t dest_filename;
		bool_t custom_dest_given = false;
		
	};

}
