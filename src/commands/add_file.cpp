#include "add_file.h"
#include "../utils/string_utils.h"

namespace boxer::commands {

	Add_File::Add_File(stringvec_t Payload, string_t Working_Dir) : Command(ADD_FILE, Payload), working_dir(Working_Dir)
	{
		isValid = false;
	}

	Add_File::~Add_File() {}

	/**
	* Builds necessary stuff related to command's execution
	*/
	auto Add_File::buildup() -> bool_t {
		 
#ifdef DEBUG
		std::for_each(payload.begin(), payload.end(), [](const string_t &s){
				log_dbg(s);
				});
#endif
		if (payload.size() < 2) {
			log_err("ADD_FILE wrong format");
			return false;
		}

		if (boxer::string::isAbsolutePath(payload[1])) {
			source_filename = payload[1];
		} else {
			source_filename = working_dir + "/" + payload[1];
		}

		// Check if given string is actually exists and is file.
		if (exists(source_filename) && isFile(source_filename)) {
			log_dbg("file exists: " + source_filename);
		} else {
			log_err("file does NOT exist: " + source_filename);
			return false;
		}

		dest_filename = source_filename;

		log_inf("Source Filename: " + source_filename);
		log_inf("Destin Filename: " + dest_filename);

		if (boxer::string::beginsWith(dest_filename, "/")) {
			dest_filename = dest_filename.substr(1);
		}

		if (payload.size() == 2) {
			isValid = true;
			return true;
		
		} else if (payload.size() == 4) {
			if (boxer::string::iequals(payload[2], "AS")) { 
				dest_filename = payload[3];
				custom_dest_given = true;
				isValid = true;
				return true;
			}
		}
		
		return false;	
	}
	
	/** ADD_FILE <FILENAME> [ AS <DEST_FILENAME> ]
	 *  
	 * Scenario 1:
	 * File /foo/bar.ext is given with absolute path:
	 * <FILENAME> : /foo/bar.ext --> absolute path --> SOURCE_FILENAME = /foo/bar.ext ----- AS -----> DEST_FILENAME = {as given}
	 *                                                                               |
	 *                                                                               `----- NO AS --> DEST_FILENAME = /foo/bar.ext
	 * Scenario 2:
	 * File /foo/bar.ext is given with relative path:
	 * <FILENAME> : bar.ext --> relative path --> SOURCE_FILENAME = working_dir/bar.ext ----- AS -----> DEST_FILENAME = {as given}
	 *                                                                                 |
	 *                                                                                 `----- NO AS --> DEST_FILENAME = bar.ext
	 */
	auto Add_File::execute(const tar_t & tar) -> result::CommandResult {
		log_inf("Execute ADD_FILE");
		if (isValid) {
			bool_t addFileResult = tar->addFile(source_filename, dest_filename);
			return result::CommandResult(addFileResult);		
		}

		return result::CommandResult();	
	}
}

