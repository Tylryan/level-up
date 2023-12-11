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
bool lexer_is_at_end(struct lexer * self);
bool lexer_skip(struct lexer * self, int times);
void lexer_add_token(struct lexer * self, struct token * t);
bool lexer_next(struct lexer * self);
struct token * lexer_create_token(struct lexer * self, std::string var, std::string val);
void lexer_set_curr_word(struct lexer * self, std::string word);
void lexer_skip_line(struct lexer * self);
void lexer_skip_until(struct lexer * self, char c);
void lexer_consume_string(struct lexer * self);
void lexer_handle_mxCell(struct lexer * self);
char lexer_peek(struct lexer * self);
char lexer_peek_next(struct lexer * self);
bool lexer_has_next(struct lexer * self);
void lexer_increment_line(struct lexer * self);
