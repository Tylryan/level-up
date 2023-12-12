#pragma once

#include <vector>
#include "./generator.h"

struct writer
{
	std::string source_dir;
	
};
void writer_write(std::vector<Klass> * Klasses);
