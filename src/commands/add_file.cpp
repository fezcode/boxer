#include "add_file.h"

namespace boxer::commands {

	Add_File::Add_File(string_t Name, stringvec_t Payload) : Command(Name, Payload) 
	{
		isValid = false;
	}

	Add_File::~Add_File() {}

	auto Add_File::buildup() -> bool_t {
		 
		std::for_each(payload.begin(), payload.end(), [](const string_t &s){
				log_dbg(s)
				});
		
		if (fileExists(payload[1])) {
			log_dbg("file exists: " + payload[1])
		} else {
			log_err("file does NOT exist: " + payload[1])
		}

		if (payload.size() > 2) {

		}

		return false;	

	}
	
	auto Add_File::execute() -> bool_t {
		return false;	
	}


}

