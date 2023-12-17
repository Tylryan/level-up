#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <sys/stat.h>
#include <format>

#include "./writer.h"
#include "./generator.h"

/* Private functions */
void writer_write_sig(FILE * fptr, struct Klass * k);
void writer_write_fields(FILE * fptr, struct Klass * k);
void writer_write_methods(FILE * fptr, struct Klass * k);
void writer_write_package(FILE * fptr, struct Klass * k);
void writer_write_imports(FILE * fptr, struct Klass * k);
std::string writer_get_ret_val(std::string method_sig);
FILE * writer_create_file(struct Klass * k);
void writer_close_file(FILE * fptr);
void writer_end(FILE * fptr);
void writer_mkdir(std::string dir_name);
void writer_mkdirs(struct Klass * klass);
std::string writer_replace(std::string s, char o, char n);

void
writer_write(std::vector<Klass> * Klasses )
{
	/* TODO(tyler) if no package, just put it in 'src' */
	writer_mkdir("src");
	for (int i = 0; i < Klasses->size(); i++)
	{
		struct Klass k = Klasses->at(i);

		writer_mkdirs(&k);
		FILE * fptr = writer_create_file(&k);
		writer_write_package(fptr, &k);
		writer_write_imports(fptr, &k);
		writer_write_sig(fptr, &k);
		writer_write_fields(fptr, &k);
		writer_write_methods(fptr, &k);
		writer_end(fptr);
	}
}

void
writer_write_package(FILE * fptr, struct Klass * k)
{
	fprintf(fptr, "package %s;\n\n", k->package.c_str());
}

void
writer_write_imports(FILE * fptr, struct Klass * k)
{
	for (int i = 0; i < k->interfaces.size(); i++)
	{
		fprintf(fptr,
			"import %s.%s;\n",
			k->interfaces[i]->package.c_str(),
			k->interfaces[i]->value.c_str());
	}
	fprintf(fptr, "\n\n");
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
		fprintf(fptr, " implements %s", k->interfaces[0]->value.c_str());
	}

	if (k->interfaces.size() > 1)
	{
		for (int i = 1; i < k->interfaces.size();i++)
		{
			fprintf(fptr, ", %s", k->interfaces[i]->value.c_str());
		}
	}
	fprintf(fptr, " {\n");
}

void
writer_write_fields(FILE * fptr, struct Klass * k)
{
	for (int i = 0; i < k->fields.size();i++)
	{
		fprintf(fptr, "    %s\n", k->fields[i]->value.c_str());
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
	else if (ret_type == "float")  { return "0.0f";  }
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
		std::string method_sig = k->methods[i]->value;
		if (k->type == OBJ_KLASS)
		{
			fprintf(fptr, "    %s {\n", method_sig.c_str());
			fprintf(fptr, "        //TODO implement method \n");
			fprintf(fptr, "        return %s;\n", writer_get_ret_val(method_sig).c_str());
			fprintf(fptr, "    }\n");
		}
		else if(k->type == OBJ_INTERFACE)
		{
			fprintf(fptr, "    %s;\n", method_sig.c_str());
		}
	}
}

FILE *
writer_create_file(struct Klass * k)
{
	std::string p_path = writer_replace(k->package, '.', '/');
	p_path.push_back('/');
	std::string file_name = "src/" + p_path + k->name + ".java";

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
	if (mkdir(dir_name.c_str(), 0777) < 0 && dir_name != "src")
	{
		//fprintf(stdout, "[info] failed to create %s dir\n", dir_name.c_str());
	}
}

std::string
writer_replace(std::string s, char o, char n)
{
	std::string res = std::string();
	for (int i = 0; i < s.size();i++)
	{
		if (s[i] == o)
		{
			res.push_back(n);
			continue;
		}
		res.push_back(s[i]);
	}
	
	return res;
}
    
void
writer_mkdirs(struct Klass * klass)
{
	std::string p_path = writer_replace(klass->package, '.', '/');
	p_path.push_back('/');

	int i = 0, j = 0;
	while (j <= p_path.size())
	{
		if (p_path[j] == '/')
		{
			std::string d_path = "src/" + p_path.substr(i, j - i);
			writer_mkdir(d_path);
		}
		j++;
	}
}    
