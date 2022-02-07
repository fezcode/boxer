#include "archive_name.h"
#include "../utils/string_utils.h"

namespace boxer::commands {

	Archive_Name::Archive_Name(stringvec_t Payload) : Command(ARCHIVE_NAME, Payload) 
	{
		isValid = false;
	}

	Archive_Name::~Archive_Name() {}

	auto Archive_Name::buildup() -> bool_t {

		log_inf("Archive filename = " + payload[1]);
		 
		std::for_each(payload.begin(), payload.end(), [](const string_t &s){
				log_dbg(s);
				});

		if (payload.size() >= 2) {
			isValid = true;
			return true;
		}

		return false;	
	}
	
	auto Archive_Name::execute(const tar_t & tar) -> result::CommandResult {
		log_inf("Execute ARCHIVE_NAME");
		if (isValid) {
			if (boxer::string::isFilenameValid(payload[1])) {
				return result::CommandResult(true, stringvec_t{ payload[1] });	
			}
			log_err("Archive name is not valid");		
		}
		return result::CommandResult();	
	}
}