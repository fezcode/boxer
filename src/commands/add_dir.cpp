#include "add_dir.h"
#include "../utils/string_utils.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace boxer::commands {

	/**
	 * Working_Dir: absolute path of the dir containing boxer file.
	 */
	Add_Dir::Add_Dir(stringvec_t Payload, string_t Working_Dir) : Command(ADD_DIR, Payload), working_dir(Working_Dir)
	{
		isValid = false;
	}

	Add_Dir::~Add_Dir() {}

	/**
	 * Builds necessary stuff related to command's execution
	 */
	auto Add_Dir::buildup() -> bool_t {
		 
#ifdef DEBUG
		std::for_each(payload.begin(), payload.end(), [](const string_t &s){
				log_dbg(s);
				});
#endif

		if (payload.size() < 2) {
			log_err("ADD_DIR wrong format");
			return false;
		}

		if (boxer::string::isAbsolutePath(payload[1])) { 
			source_filename = payload[1];
		} else {
			source_filename = working_dir + "/" + payload[1];
		}

		// Concat trailing / to create a standard.
		if (!boxer::string::endsWith(source_filename, "/"))
			source_filename = source_filename + "/";

		// Check if given string is actually exists and is file.
		if (exists(source_filename) && isDirectory(source_filename)) {
			log_dbg("dir exists: " + source_filename);
		} else {
			log_err("dir does NOT exist: " + source_filename);
			return false;
		}

		dest_filename = source_filename;

		log_inf("Source Dir: " + source_filename);
		log_inf("Destin Dir: " + dest_filename);

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
	
	/** ADD_DIR <DIRNAME> [ AS <DEST_FILENAME> ]
	 *  
	 * Scenario 1:
	 * Folder /foo/bar.ext is given with absolute path:
	 * <FILENAME> : /foo/bar.ext --> absolute path --> SOURCE_FILENAME = /foo/bar.ext ----- AS -----> DEST_FILENAME = {as given}
	 *                                                                               |
	 *                                                                               `----- NO AS --> DEST_FILENAME = /foo/bar.ext
	 * Scenario 2:
	 * File /foo/bar.ext is given with relative path:
	 * <FILENAME> : bar.ext --> relative path --> SOURCE_FILENAME = working_dir/bar.ext ----- AS -----> DEST_FILENAME = {as given}
	 *                                                                                 |
	 *                                                                                 `----- NO AS --> DEST_FILENAME = bar.ext
	 */
	auto Add_Dir::execute(const tar_t & tar) -> result::CommandResult {
		log_inf("Execute ADD_DIR");
		if (isValid) {
			bool_t addFileResult;
			// Add files in directory
			fs::path dirpath(source_filename);
			if (fs::is_empty(dirpath)) {
				log_war("Directory is empty: "+source_filename);
				return result::CommandResult(true);
			}

			for (auto& p : fs::directory_iterator(dirpath)) {
				string_t new_source = p.path().string();
				string_t new_dest = new_source;

				if(custom_dest_given) {
					new_dest = dest_filename + "/" + boxer::string::getBasename(new_source);
					log_dbg("dest_filename is NOT empty");
				} else {
					new_dest = new_dest.substr(1);
					log_dbg("dest_filename is empty");
				}

				log_dbg("SRC:" + new_source);
				log_dbg("DST:" + new_dest);

				addFileResult = tar->addFile(new_source, new_dest);	
				if (!addFileResult) {
					break;
				}
			}

			return result::CommandResult(addFileResult);
		}

		return result::CommandResult();	
	}
}

