#include "./writer.h"
#include "generator.h"
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <sys/stat.h>


void writer_write_sig(FILE * fptr, struct Klass * k);
void writer_write_fields(FILE * fptr, struct Klass * k);
std::string writer_get_ret_val(std::string method_sig);
void writer_write_methods(FILE * fptr, struct Klass * k);
FILE * writer_create_file(struct Klass * k);
void writer_close_file(FILE * fptr);
void writer_end(FILE * fptr);
void writer_mkdir(std::string dir_name);

void
writer_mkdirs(struct Klass * klass)
{
	/* TODO(tyler) Implement */
}    

void
writer_write(std::vector<Klass> * Klasses )
{
	writer_mkdir("src");

	/* TODO(tyler) if no package, just put it in 'src' */
	for (int i = 0; i < Klasses->size(); i++)
	{
		struct Klass k = Klasses->at(i);

		writer_mkdirs(&k);
		FILE * fptr = writer_create_file(&k);
		writer_write_sig(fptr, &k);
		writer_write_fields(fptr, &k);
		writer_write_methods(fptr, &k);
		writer_end(fptr);
	}
}

void
writer_write_sig(FILE * fptr, struct Klass * k)
{
	std::string class_name = k->name;

	if (k->type == OBJ_KLASS)
	{
	    fprintf(fptr, "public class %s", class_name.c_str());
	}
	else if (k->type == OBJ_INTERFACE)
	{
	    fprintf(fptr, "public interface %s", class_name.c_str());
	}

	if (k->interfaces.size() > 0)
	{
		fprintf(fptr, " implements %s", k->interfaces[0].c_str());
	}

	if (k->interfaces.size() > 1)
	{
		for (int i = 1; i < k->interfaces.size();i++)
		{
			printf("INTERFACE: %s\n", k->interfaces[i].c_str());
			fprintf(fptr, ", %s", k->interfaces[i].c_str());
		}
	}
	fprintf(fptr, " {\n");
}

void
writer_write_fields(FILE * fptr, struct Klass * k)
{
	for (int i = 0; i < k->fields.size();i++)
	{
		fprintf(fptr, "    %s\n", k->fields[i].c_str());
	}
	fprintf(fptr, "\n");
}

std::string
writer_get_ret_val(std::string method_sig)
{
	int spc_one = method_sig.find(' ');
	std::string ret_type = method_sig.substr(0, spc_one);

	bool has_access_mod = ret_type == "private" || ret_type == "public";

	if (has_access_mod)
	{
		int spc_two = method_sig.find(' ', spc_one+1);
		ret_type = method_sig.substr(spc_one + 1, spc_two - spc_one -1);
	}

	if      (ret_type == "int")    {return "0";     }
	else if (ret_type == "float")  { return "0.0";  }
	else if (ret_type == "byte")   { return "0";    }
	else if (ret_type == "short")  { return "0";    }
	else if (ret_type == "long")   { return "0";    }
	else if (ret_type == "double") { return "0";    }
	else if (ret_type == "boolean"){ return "false";}
	else if (ret_type == "char")   { return "a";    }
	else if (ret_type == "void")   { return "";     }

	return "null";
}

void
writer_write_methods(FILE * fptr, struct Klass * k)
{
	for (int i = 0; i < k->methods.size();i++)
	{
		std::string method_sig = k->methods[i];
		if (k->type == OBJ_KLASS)
		{
			fprintf(fptr, "    %s {\n", method_sig.c_str());
			fprintf(fptr, "        //TODO implement method \n");
			fprintf(fptr, "        return %s;\n", writer_get_ret_val(method_sig).c_str());
			fprintf(fptr, "    }\n");
		}
		else if(k->type == OBJ_INTERFACE)
		{
			/* TODO(tyler) never reaches here */
			fprintf(fptr, "    %s;\n", method_sig.c_str());
		}
	}
}

FILE *
writer_create_file(struct Klass * k)
{
	std::string file_name = "src/" + k->name + ".java";
	FILE * fptr;
	fptr = fopen(file_name.c_str(), "w");
	return fptr;
}

void
writer_close_file(FILE * fptr)
{
	fclose(fptr);
}

void
writer_end(FILE * fptr)
{
	fprintf(fptr, "}\n");
	writer_close_file(fptr);
}

void
writer_mkdir(std::string dir_name)
{
	if (mkdir(dir_name.c_str(), 0777) < 0)
	{
		fprintf(stdout, "[info] failed to create src dir\n");
	}
}
