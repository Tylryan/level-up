#pragma once

#include <string>
#include <vector>

enum TOKEN_TYPE
{
	TOKEN_ID       ,
	TOKEN_STYLE    ,
	TOKEN_PARENT   ,
	TOKEN_VALUE    ,
	TOKEN_SOURCE   ,
	TOKEN_TARGET   ,
	TOKEN_CLASS    ,
	TOKEN_INTERFACE,
	TOKEN_LITERAL  ,
	TOKEN_ARROW    ,
	TOKEN_FIELD    ,
	TOKEN_METHOD   ,
	TOKEN_PACKAGE
};

struct token
{
	enum TOKEN_TYPE type;
	std::string key;
	std::string value;
};

struct lexer
{
	size_t line;
	int start;
	int current;
	bool has_packages;
	std::string file_path;
	std::string text;
	std::vector<struct token *> tokens;
	std::string current_word;
};

struct token * token_create(enum TOKEN_TYPE type, std::string value);
std::string token_to_string(struct token * token);
std::string token_type_to_string(enum TOKEN_TYPE type);
void token_destroy(struct token * t);

struct lexer * lexer_create(std::string file_path);
void lexer_destroy(struct lexer * self);
void lexer_lex(struct lexer *self);
