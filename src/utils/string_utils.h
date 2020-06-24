//============================================
// String Operations and Utils Static Methods
//============================================

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../defs.h"
#include <ctime>
#include <random>
#include <functional>
#include <chrono>
#include <string.h>
#include <filesystem>

namespace boxer::string {

	static const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

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

	static auto endsWith(const string_t &str, const string_t &pattern ) -> bool_t {
		return str.rfind(pattern) == std::abs( (long long int)(str.size() - pattern.size()) );
	}

	// What is mkstemp, what is tmpnam...
	static auto generateRandomString(const size_t & length ) -> string_t {
		charvec_t randomName;
		size_t newLength = length;

		if (length > 512) {
			log_dbg("512 is the maximum size for random name. Let's not overdo this.");
			newLength = 512;
		}
		 
		// use high res clock in order to provide actual seed. time(0) sucks big time.
		std::mt19937::result_type seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto integer_rand = std::bind(std::uniform_int_distribution<int>(0,61), std::mt19937(seed));

		for (uint i = 0; i < newLength; ++i )
			randomName.push_back(letters[integer_rand()]);
	
		return string_t(randomName.begin(), randomName.end()); 
	}

	static auto getBasename(string_t path) -> string_t {
		return string_t(basename(path.c_str()));
	}

	static auto getParentPath(string_t path) -> string_t {
		std::filesystem::path p(path);
		return p.parent_path().string();
	}

	static auto isAbsolutePath(string_t path) -> bool_t {
		std::filesystem::path p(path);
		return p.is_absolute();
	}

	static auto isFilenameValid(string_t name) -> bool_t {
		char illegals[] = {'\0', '\\', '/', ':', '*', '\"', '<', '>', '|', '$', '&' };
		for (char c : illegals) {
			if (name.find(c) != std::string::npos)
				return false;
		}
		
		return true;
	}

} //namespace boxer::string
