#include <cctype>
#include <cstdio>
#include <string>
#include <cstring>
#include <boost/algorithm/string.hpp>

#include "./arg_parser.h"
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
main(int argc, char ** argv)
{
	struct arg_parser ap = arg_parser_parse(argc, argv);

	std::string file_path = ap.file_path;
	/* TODO(tyler): Lex object doesn't need to be malloced */
	struct lexer * lex = lexer_create(file_path);
	lexer_lex(lex);

	struct parser parser = parser_create(lex->tokens, lex->has_packages);
	parser_parse(&parser);

	struct generator gen = gen_create(&parser.parser_objects);
	gen_generate(&gen);

	writer_write(&gen.klasses);

	lexer_destroy(lex);
	parser_destroy(&parser);
}
