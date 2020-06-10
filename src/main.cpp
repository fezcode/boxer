//===========================================
// Ahmed Samil Bulbul
// Boxer parser
//===========================================
#include "main.h"
#include <getopt.h>

auto main(int argc, char **argv) -> int {
	string_t filename;
	static struct option long_options[] =
	{
		/* These options set a flag. */
		{"help", no_argument, 0, 'h'},
		/* These options don’t set a flag. We distinguish them by their indices. */
		{"file",  required_argument, 0, 'f'},
		{0, 0, 0, 0}
	};

	int c;
	/* getopt_long stores the option index here. */
	int option_index = 0;


	/* Detect the end of the options. */
  	while ((c = getopt_long (argc, argv, "hf:", long_options, &option_index)) != -1) {
		switch(c) {
		case 'h':
		      std::cout << "Help is requested but cannot give a shit about it right now." << std::endl;
		      break;

		case 'f':
		      printf ("Given file `%s'\n", optarg);
		      filename = optarg;
		      break;
		}
	}

	auto parser = std::make_shared<boxer::parser::Parser>(filename);
	parser->processFile();

	return 0;
}
