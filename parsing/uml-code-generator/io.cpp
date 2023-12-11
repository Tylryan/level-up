
#include <string>
#include <sstream>
#include <fstream>

std::string
read_file_as_string(std::string file_path)
{
	std::ifstream file;

	file.open(file_path);

	if (file.fail())
	{
		fprintf(stdout, "error: failed to open file '%s'\n", file_path.c_str());
		return std::string();
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}
