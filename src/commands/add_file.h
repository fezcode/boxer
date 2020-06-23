#include "command.h"

namespace boxer::commands {

	/** ADD_FILE 
	 * Format ADD_FILE <FILENAME> [AS <OUTPUT_FILENAME> ]
	 */
	class Add_File : public Command {
	public:
		Add_File(stringvec_t Payload);
		virtual ~Add_File();
		auto execute(const tar_t & tar) -> result::CommandResult;
		auto buildup() -> bool_t;

	protected:
		bool_t destNameParamExists = false;

	};

}
