#include "add_file.h"
#include "../utils/string_utils.h"

namespace boxer::commands {

	Add_File::Add_File(string_t Name, stringvec_t Payload) : Command(Name, Payload) 
	{
		isValid = false;
	}

	Add_File::~Add_File() {}

	auto Add_File::buildup() -> bool_t {
		 
		std::for_each(payload.begin(), payload.end(), [](const string_t &s){
				log_dbg(s);
				});
		
		if (fileExists(payload[1])) {
			log_dbg("file exists: " + payload[1]);
		} else {
			log_err("file does NOT exist: " + payload[1]);
			return false;
		}

		if (payload.size() == 2) {
			isValid = true;
			return true;
		
		} else if (payload.size() == 4) {
			if (boxer::string::iequals(name, "AS")) {
				destNameParamExists = true;
				isValid = true;
				return true;
			}
		}
		
		return false;	
	}
	
	auto Add_File::execute() -> bool_t {

		return false;	
	}
}

