
#include <vector>
#include <string>

struct arg_parser
{
	std::vector<std::string> args;
	std::string file_path;
};


struct arg_parser arg_parser_parse(size_t argc, char ** argv);
