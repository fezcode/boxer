//===========================================
// Ahmed Samil Bulbul
// Boxer parser
//===========================================
#include "main.h"

auto main() -> int {
	auto parser = std::make_shared<boxer::parser::Parser>("/root/workspace/boxer/boxer.bxr");
	parser->processFile();

	return 0;
}
