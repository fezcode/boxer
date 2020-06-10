//===================================
// Parser.h
//===================================

#include "defs.h"

namespace boxer::parser {

	class Parser {
	public:
		Parser(string_t path);
		bool_t processFile();
	private:
		string_t filename;
		stringvec_t contentsOfFile;

		// functions
		bool_t readFile();
		auto printContents() -> void;
	};
}

