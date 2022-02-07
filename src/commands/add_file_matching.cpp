#include "add_file_matching.h"
#include <regex>

namespace boxer::commands {

	/**
	 * ADD_FILE_MATCHING
	 * Format ADD_FILE_MATCHING <REGULAR_EXPRESSION> IN <DIRECTORY> GET <ALL|LATEST|FIRST|LAST> [AS <OUTPUT_FILENAME> ] TO <DESTINATION>
	 * Payload[0] = ADD_FILE_MATCHING	: [must] not used, already set
	 * Payload[1] = REGEX				: [must] is used to get matching files.
	 * Payload[2] = IN					: [must] 
	 * Payload[3] = DIRECTORY			: [must] search regex in.
	 * Payload[4] = GET					: [must] 
	 * Payload[5] = SELECTOR			: [must]
	 *										ALL: 	Add all files matching.
	 *										LATEST:	Get only latest modified.
	 *										FIRST:	Get first match.
	 *										LAST:	Get last match.
	 * Payload[6] = TO					: [must]
	 * Payload[7] = DESTINATION			: [must] destination directory.
	 * Payload[8] = AS					: [optional] is used to get matching files.
	 * Payload[9] = OUTPUT_FILENAME		: [optional] must exists if AS is given.
	 * 
	 * If ALL is selected then AS option is omitted.
	 * 
	 */
	Add_File_Matching::Add_File_Matching(stringvec_t Payload, string_t Working_Dir) : Command(ADD_FILE_MATCHING, Payload), working_dir(Working_Dir)
	{
		isValid = false;
	}

	Add_File_Matching::~Add_File_Matching() {}

	/**
	* Builds necessary stuff related to command's execution
	*/
	auto Add_File_Matching::buildup() -> bool_t {
		 
#ifdef DEBUG
		std::for_each(payload.begin(), payload.end(), [](const string_t &s){
				log_dbg(s);
				});
#endif
		if (payload.size() < 8) {
			log_err("ADD_FILE_MATCHING wrong format");
			return false;
		}

		regex_str = payload[1];

		if (boxer::string::iequals(payload[2], "in")) {
			search_directory = payload[3];
		} else {
			log_err("IN is not given.");
			return false;
		}

		if (!boxer::string::isAbsolutePath(search_directory)) {
			search_directory = working_dir + "/" + search_directory;
		}

		// Check if given string is actually exists and is file.
		if (exists(search_directory) && isDirectory(search_directory)) {
			log_dbg("dir exists: " + search_directory);
		} else {
			log_err("dir does NOT exist: " + search_directory);
			return false;
		}


		// Concat trailing / to create a standard.
		if (!boxer::string::endsWith(search_directory, "/"))
			search_directory = search_directory + "/";

		if (boxer::string::iequals(payload[4], "get")) {
			convertToType(payload[5]);
		} else {
			log_err("GET selector is not given.");
			return false;
		}

		if (boxer::string::iequals(payload[6], "to")) {
			save_directory = payload[7];
		} else {
			log_err("TO directory is not given");
			return false;
		}

		// Concat trailing / to create a standard.
		if (!boxer::string::endsWith(save_directory, "/"))
			save_directory = save_directory + "/";

		if (boxer::string::beginsWith(save_directory, "/")) {
			save_directory = save_directory.substr(1);
		}

		// at least two more parameters exists.
		if (payload.size() >= 10) {
			if (boxer::string::iequals(payload[8], "as")) {
				output_filename = payload[9];
				if (!boxer::string::isFilenameValid(output_filename)) {
					log_war("Given ouptut filename is not valid.");
					return false;
				}
				custom_dest_given = true;
			} else {
				log_err("Given parameter at position 9 [" + payload[8] + "] is not correct.");
				return false;
			}
		}
	 
		log_inf("Regex       : " + regex_str);
		log_inf("Source Dir  : " + search_directory);
		log_inf("Save Dir    : " + save_directory);
		log_inf("Output Name : " + output_filename);

		isValid = true;
		return true;	
	}
	
	/** ADD_FILE_MATCHING
	 * Output filename is only basename of file. It has to be valid filename.
	 * If ALL selector is given output filename is basename of each file. So Output filename becomes useless.
	 * 
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
	auto Add_File_Matching::execute(const tar_t & tar) -> result::CommandResult {
		log_inf("Execute ADD_FILE_MATCHING");
		if (isValid) {
			stringvec_t matching_files;
			bool_t addFileResult = true;
			// Get files in a directory.
			fs::path dirpath(search_directory);
			if (fs::is_empty(dirpath)) {
				log_war("Directory is empty: " + search_directory);
				return result::CommandResult();
			}

			// Add every file matching regex in directory to vector.
			for (auto& p : fs::directory_iterator(dirpath) ) {
				string_t new_source = p.path().string();
				string_t new_source_base = boxer::string::getBasename(new_source);

				if (std::regex_match(new_source_base, std::regex(regex_str))) {
					matching_files.push_back(new_source);
					log_dbg("Matched: " + new_source);
				}
			}

			string_t current_src = matching_files[0];  // set for LATEST
			string_t current_dst;
		
			if (matching_files.size() == 0 ) {
				log_inf("No files match");
				return result::CommandResult(true);
			}

			switch (selector)
			{
			case ALL:
				for (auto & p : matching_files) {
					current_src = p;
					current_dst = save_directory + boxer::string::getBasename(p);
					addFileResult &= tar->addFile(current_src, current_dst);
				}				
				break;
				
			case LATEST:
				for (auto & p : matching_files) {
					if (fs::last_write_time(fs::path(p)) > fs::last_write_time(fs::path(current_src))) {
						current_src = p;
						if (custom_dest_given) {
							current_dst = save_directory + output_filename;
						} else {
							current_dst = save_directory + boxer::string::getBasename(p);
						}
					}
				}	

				addFileResult &= tar->addFile(current_src, current_dst);
				break;

			case FIRST:
				current_src = matching_files[0];

				if (custom_dest_given) {
					current_dst = save_directory + output_filename;
				} else {
					current_dst = save_directory + boxer::string::getBasename(current_src);
				}
					
				addFileResult &= tar->addFile(current_src, current_dst);
				break;
			
			case LAST:
				current_src = matching_files[matching_files.size()-1];

				if (custom_dest_given) {
					current_dst = save_directory + output_filename;
				} else {
					current_dst = save_directory + boxer::string::getBasename(current_src);
				}
					
				addFileResult &= tar->addFile(current_src, current_dst);
				break;

			default:
				break;
			}

			return result::CommandResult(addFileResult);

		}
		// 	bool_t addFileResult = tar->addFile(source_filename, dest_filename);
		// 	return result::CommandResult(addFileResult);		
		// }

		return result::CommandResult();	
	}
}

