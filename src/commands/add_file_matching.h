#include "command.h"
#include "../utils/string_utils.h"

namespace boxer::commands {

	/** ADD_FILE_MATCHING
	 * Format ADD_FILE_MATCHING <REGULAR_EXPRESSION> IN <DIRECTORY> GET <ALL|LATEST|FIRST|LAST> [AS <OUTPUT_FILENAME> ]
	 */
	class Add_File_Matching : public Command {
	public:
		Add_File_Matching(stringvec_t Payload, string_t Working_Dir);
		virtual ~Add_File_Matching();
		auto execute(const tar_t & tar) -> result::CommandResult;
		auto buildup() -> bool_t;

		enum GET_TYPE {
			NONE,
			ALL,	
			LATEST,
			FIRST,	
			LAST	
		};

	protected:
		string_t working_dir;
		string_t regex_str;
		string_t search_directory;
		GET_TYPE selector = LATEST;
		string_t save_directory;
		
		// AS option is given.
		bool_t custom_dest_given = false;
		string_t output_filename;
		
	private:
		inline auto convertToType(string_t type_str) -> void {
			if (boxer::string::iequals(type_str, "ALL")) {
				selector = ALL;
			} else if (boxer::string::iequals(type_str, "LATEST")) {
				selector = LATEST;
			} else if (boxer::string::iequals(type_str, "FIRST")) {
				selector = FIRST;
			} else if (boxer::string::iequals(type_str, "LAST")) {
				selector = LAST;
			} else {
				selector = NONE;
			}
		}

	};

}
