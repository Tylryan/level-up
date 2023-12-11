#include <cctype>
#include <cstdio>
#include <string>
#include <cstring>
#include <boost/algorithm/string.hpp>

#include "./io.h"
#include "./lexer.h"
#include "./parser.h"
#include "./generator.h"
#include "./writer.h"
/*
  1) Root has an id of 1 and a parent id of 0
  2) Anything not inside another object has a parent ID of 1.
  3) Anything inside another object has that other obect's id as their own parent id.

  - Class    ; when style == swimlane
  - package  ; when style == folder
  - Interface; when value contains <<interface>>

  Class ends when next value's style is neither text nor strokeWidth
*/


int
main(void)
{
	std::string file_path = "file.txt";
	struct lexer * lex = lexer_create(file_path);
	lexer_lex(lex);

	//for (int i = 0; i < lex->tokens.size();i++)
	//{
	//	struct token * t = lex->tokens[i];
	//	printf("%s\n",token_to_string(t).c_str());
	//}

	//struct parser * parser = parser_create(lex->tokens);
	struct parser parser = parser_create(lex->tokens, lex->has_packages);
	parser_parse(&parser);

	struct generator gen = gen_create(&parser.parser_objects);
	gen_generate(&gen);

	writer_write(&gen.klasses);

	//lexer_destroy(lex);
	delete lex;
	//parser_destroy(parser);
	//std::string contents = read_file_as_string(file_path);
}
