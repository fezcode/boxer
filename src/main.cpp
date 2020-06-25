//===========================================
// Ahmed Samil Bulbul
// Boxer parser
//===========================================

#include "defs.h"
#include <iostream>
#include <filesystem>
#include <getopt.h>
#include <unistd.h>
#include "archiver/tarballer.h"
#include "archiver/gzipper.h"
#include "utils/string_utils.h"
#include "parser/parser.h"
#include "executor/executor.h"

auto getHelp() -> void {
	log_err("Help is requested but cannot give a shit about it right now.");
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

#ifdef TEST_IS_REQ
	std::cout << std::boolalpha << boxer::string::isAbsolutePath("/asdasd/sdfasdf*") << std::endl;
	exit (0);

	std::cout << std::boolalpha << boxer::string::isFilenameValid("bar") << std::endl;
	std::cout << std::boolalpha << boxer::string::isFilenameValid("$$bilyo.ext") << std::endl;
	
	exit(1);

	std::cout << boxer::string::getBasename("/foo/bar") << "==" << boxer::string::getParentPath("/foo/bar") << std::endl;
	std::cout << boxer::string::getBasename("/foo/bar.exe") << "==" << boxer::string::getParentPath("/foo/bar.exe") << std::endl;
	std::cout << boxer::string::getBasename("/foo/zaza/") << "==" << boxer::string::getParentPath("/foo/zaza/") << std::endl;

	exit(1);

	string_t randomString = boxer::string::generateRandomString(600);
	log_dbg("Random String");
	log_dbg(randomString);

	exit(1);

	asbuzz::archiver::Tar tar("my_arch.tar");
	tar.open();

	// Beware of leading `/` in archive filenames
	tar.addFile("/mnt/c/Repos/boxer/test-files/test_wrong.txt", "test_wrong.txt");
	tar.addFile("/mnt/c/Repos/boxer/test-files/test1.txt", "cc/test1.txt");
	tar.addFile("/mnt/c/Repos/boxer/test-files/test2.txt", "test2.txt");
	tar.close();

	asbuzz::archiver::GZipper gzip("my_arch.tar", "my_arch.tar.gz");
	gzip.compress();
	gzip.cleanup();

	exit(1);
#endif

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
		log_err("Filename is not given.");
		exit(1);
	} 
	else if ( access(filename.c_str(), 0) != 0 ) {
		log_err("Given file does NOT exist");
		exit(1);
	}

	auto cwd = std::filesystem::current_path();
	log_dbg("Current Working Directory: " + string_t(cwd));
	
	auto fullPath = string_t(realpath(filename.c_str(), NULL));
	log_dbg("Boxer File: " + fullPath);

	auto parser = std::make_shared<boxer::parser::Parser>(fullPath);
	parser->processFile();

	command_list_t all_commands = parser->retrieveCommands();
	string_t temp_tar_filename = boxer::string::generateRandomString(20);
	log_inf("Temp Tar Name = " + temp_tar_filename);
	auto executor = std::make_shared<boxer::executor::Executor>(all_commands, temp_tar_filename , parser->getWorkingDir());
	
	executor->execute();
		
	return 0;
}
