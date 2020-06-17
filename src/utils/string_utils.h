//============================================
// String Operations and Utils Static Methods
//============================================

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../defs.h"

namespace boxer::string {

	static stringvec_t split (string_t s, string_t delimiter) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	        string_t token;
		stringvec_t res;
	
		while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
		        token = s.substr (pos_start, pos_end - pos_start);
		        pos_start = pos_end + delim_len;
			res.push_back (token);
		}
		
		res.push_back (s.substr (pos_start));
		return res;
	}

	static auto iequals(const string_t & a, const string_t &b ) -> bool_t {
		return std::equal(a.begin(), a.end(),
				  b.begin(), b.end(),
				  [](char a, char b) {
					return tolower(a) == tolower(b);
				  });
	}

	static auto beginsWith(const string_t &str, const string_t &pattern ) -> bool_t {
		return (str.rfind(pattern, 0) == 0);
	}

} //namespace boxer::string
