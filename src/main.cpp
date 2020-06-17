//===========================================
// Ahmed Samil Bulbul
// Boxer parser
//===========================================
#include "main.h"
#include <getopt.h>
#include <unistd.h>
// #include <zlib/zlib.h>
#include "archiver/archiver.h"

auto getHelp() -> void {
	log_err("Help is requested but cannot give a shit about it right now.")
	exit(1);
}

auto getAvailableCommands()-> void {
	string_t commands = R"(
 COMMANDS
 ------------------------------------------------------------------------------
  
 ADD_FILE [FILENAME] [ABSOLUTE | RELATIVE]
     Adds current `FILENAME` to box. ABSOLUTE uses absolute path of file to store
     in box while RELATIVE uses directory of Boxerfile's directory.
     Default is RELATIVE.

 )";
	std::cout << commands << std::endl;
}

auto main(int argc, char **argv) -> int {
	boxer::archiver::createTar();
	exit(1);





	string_t filename;
	static struct option long_options[] =
	{
		/* These options set a flag. */
		{"help", no_argument, 0, 'h'},
		{"commands", no_argument, 0, 'c'},
		/* These options don’t set a flag. We distinguish them by their indices. */
		{"file",  required_argument, 0, 'f'},
		{0, 0, 0, 0}
	};

	int c;
	/* getopt_long stores the option index here. */
	int option_index = 0;


	/* Detect the end of the options. */
  	while ((c = getopt_long (argc, argv, "hcf:", long_options, &option_index)) != -1) {
		switch(c) {
		case 'h':
			getHelp();
			break;
		case 'c':
			getAvailableCommands();
			break;

		case 'f':
			printf ("Given file `%s'\n", optarg);
			filename = optarg;
			break;
		}
	}

	if (filename.empty())
	{
		log_err("Filename is not given.")
		exit(1);
	} 
	else if ( access(filename.c_str(), 0) != 0 ) {
		log_err("Given file does NOT exist")
		exit(1);
	}


	auto cwd = std::filesystem::current_path();
	log_dbg("Current Working Directory: " + string_t(cwd))
	
	auto fullPath = string_t(realpath(filename.c_str(), NULL));
	log_dbg("Working Filename: " + fullPath)

	auto parser = std::make_shared<boxer::parser::Parser>(fullPath);
	parser->processFile();

	return 0;
}
