#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>


#include "./arg_parser.h"

struct arg_parser
arg_parser_parse(size_t argc, char ** argv)
{
	struct arg_parser ap = arg_parser();

	if (argc < 2)
	{
		printf("[error] arg_parser.arg_parser() - need to enter a file path\n");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < argc; i++)
	{
		ap.args.push_back(argv[i]);
	}
	ap.file_path = ap.args[1];
	return ap;
}
