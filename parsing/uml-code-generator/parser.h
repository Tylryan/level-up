#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "./lexer.h"

enum P_TYPE
{
	PARSER_INTERFACE ,
	PARSER_KLASS     ,
	PARSER_METHOD    ,
	PARSER_FIELD     ,
	PARSER_PACKAGE   ,
	PARSER_ARROW_COMP,
	PARSER_ARROW_REL ,
	PARSER_ARROW
};

struct klass
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string value;
	std::string package;
	std::string import_path;
};

struct inter
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string value;
	std::string package;
	std::string import_path;
};

struct package
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string value;
};

struct field
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string value;
};

struct method
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string value;
};

struct arrow_comp
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string source_id;
	std::string target_id;
};

struct arrow_rel
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string source_id;
	std::string target_id;
};

struct arrow_gen
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string source_id;
	std::string target_id;
};

struct arrow_com
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
	std::string source_id;
	std::string target_id;
};

struct common
{
	enum P_TYPE type;
	std::string id;
	std::string pid;
};

union p_object
{
	struct common      * common;
	struct klass       * klass;
	struct field       * field;
	struct method      * method;
	struct inter       * inter;
	struct package     * package;
	struct arrow_gen   * arrow_gen;
	struct arrow_rel   * arrow_rel;
	struct arrow_comp  * arrow_comp;
};

struct parser
{
	size_t current;
	bool has_packages;
	std::vector<union p_object> parser_objects;
	std::vector<struct token*> tokens;
};

struct parser parser_create(std::vector<struct token*> tokens, bool has_packages);
void parser_destroy(struct parser * parser);

std::vector<struct klass> parser_parse(struct parser * self);    

std::string p_object_to_str(union p_object * po);
