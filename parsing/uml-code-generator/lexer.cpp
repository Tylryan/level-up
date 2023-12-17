
#include <boost/algorithm/string/trim.hpp>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string.h>

#include "./io.h"
#include "./lexer.h"

#define KB 1024
#define MB KB * KB

/* Private functions */
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

void
lexer_lex(struct lexer *self)
{
	while (lexer_is_at_end(self) == false)
	{
		char l = lexer_peek(self);
		std::string cw = self->text.substr(
			self->start,
			self->current - self->start + 1);

		lexer_set_curr_word(self, cw);
		if ('\n' == l || ' ' == l)
		{
			if ('\n' == l) lexer_increment_line(self);

			if (lexer_skip(self, 1) == false)
			{
				break;
			}
		}


		bool is_mxCell      = "<mxCell" == self->current_word;
		bool is_object_cell = "<object" == self->current_word;

		if (is_mxCell || is_object_cell)
		{
			if (is_object_cell)
			{
				self->has_packages = true;
			}

			if (lexer_skip(self, 2) == false)
			{
				break;
			}
			lexer_handle_mxCell(self);
		}

		else { lexer_next(self); }
	}
}

void
lexer_increment_line(struct lexer * self)
{
	self->line++;
}

/* TODO(tyler): Token objects don't need to be malloced */
struct token *
token_create(enum TOKEN_TYPE type, std::string key, std::string value)
{
	struct token * t = new token();
	t->type  = type;
	t->key   = key;
	t->value = value;

	return t;
}

void
token_destroy(struct token * t)
{
	delete t;
}

struct lexer *
lexer_create(std::string file_path)
{
	std::string contents = read_file_as_string(file_path);

	struct lexer * l = new lexer();
	l->current      = 0;
	l->start        = 0;
	l->text         = contents;
	l->line         = 1;
	l->file_path    = file_path;
	l->has_packages = false;
	return l;
};

void
lexer_destroy(struct lexer * self)
{
	for (int i = 0; i < self->tokens.size(); i++)
	{
		token_destroy(self->tokens[i]);
	}

	delete self;
}

bool
lexer_is_at_end(struct lexer * self)
{
	return self->current == self->text.size();
}

bool
lexer_skip(struct lexer * self, int times)
{
	if (self->current + times >= self->text.size())
	{
		printf("info: could not skip that far\n");
		return false;
	}
	self->current+=times;
	self->start = self->current;
	return true;
}

void
lexer_reset_start(struct lexer * self)
{
	self->start = self->current;
}

char
lexer_peek(struct lexer * self)
{
	return self->text[self->current];
}

void
lexer_add_token(struct lexer * self, struct token * t)
{
	self->tokens.push_back(t);
}

bool
lexer_next(struct lexer * self)
{
	if (self->current + 1 >= self->text.size())
	{
		printf("info: lexer_next() reached the end\n");
		return false;
	}

	self->current++;
	return true;
}

char
lexer_peek_next(struct lexer * self)
{
	if (lexer_has_next(self) == false)
	{
		return 0;
	}

	return self->text[self->current + 1];
}

bool
lexer_has_next(struct lexer *self)
{
	return self->current +1 < self->text.size();
}

struct token *
lexer_create_token(struct lexer * self, std::string var, std::string val)
{
	if (var == "id")
		return token_create(TOKEN_ID, var, val);
	else if (var == "parent")
		return token_create(TOKEN_PARENT, var, val);
	else if (var == "style")
	{
		/* Relation arrow */
		if (strstr(val.c_str(), "endArrow=block"))
		{
			return token_create(TOKEN_ARROW_REL, var, val);
		}
		/* Composition arrow */
		else if (strstr(val.c_str(), "endArrow=open"))
		{
			return token_create(TOKEN_ARROW_COMP, var, val);
		}
		/* Any other arrow */
		else if (strstr(val.c_str(), "edgeStyle"))
		{
			return token_create(TOKEN_ARROW, var, val);
		}
		else
		{
		    return token_create(TOKEN_STYLE, var, val);
		}
		
	}
	else if (var == "source")
		return token_create(TOKEN_SOURCE, var, val);
	else if (var == "target")
		return token_create(TOKEN_TARGET, var, val);
	else if (var == "package")
		return token_create(TOKEN_PACKAGE, var, val);
	else if (var == "value" || var == "label")
	{
		bool is_class = strstr(val.c_str(), "class")
			|| strstr(val.c_str(), "Class");
		bool is_interface = strstr(val.c_str(), "interface")
			|| strstr(val.c_str(), "Interface");

		if (is_class)
		{
			val = val.substr(val.find(":")+1);
			boost::trim(val);
			return token_create(TOKEN_CLASS, var, val);
		}

		if (is_interface)
		{
			val = val.substr(val.find(":")+1);
			boost::trim(val);
			return token_create(TOKEN_INTERFACE, var, val);
		}

		bool is_field_or_method = strchr(val.c_str(), '+')
			|| strchr(val.c_str(), '-')
			|| strchr(val.c_str(), '~');

		bool is_method = strchr(val.c_str(), '(');

		if (is_field_or_method && is_method)
		{
		    return token_create(TOKEN_METHOD, var, val);
		}

		else if (is_field_or_method)
		{
		    return token_create(TOKEN_FIELD, var, val);
		}

		//else if (val != "")
		//{
		//	fprintf(stdout,
		//		"[error] invalid field or method. "
		//		"missing access modifier.\n"
		//		"    line %lu: %s\n",
		//		self->line,
		//		val.c_str());

		//	exit(EXIT_FAILURE);
		//}

		return token_create(TOKEN_VALUE, var, val);
	}

	return NULL;
}

void
lexer_set_curr_word(struct lexer * self, std::string word)
{
	self->current_word = word;
}

void
lexer_skip_line(struct lexer * self)
{
	char l = lexer_peek(self);
	while ('\n' != l)
	{
		if (lexer_skip(self, 1) == false)
		{
			break;
		}
	}
	lexer_next(self);
	lexer_increment_line(self);
}

void
lexer_skip_until(struct lexer * self, char c)
{
	char l = lexer_peek(self);
	while (l != c)
	{
		lexer_next(self);
		l = lexer_peek(self);
	}
}

bool
lexer_is_empty_string(struct lexer * self)
{
	return self->text[self->current -1] == '"'
		&& lexer_peek(self) == '"';
}

void
lexer_consume_string(struct lexer * self)
{
	char l = lexer_peek(self);
	while (l != '"' && self->current < self->text.size())
	{

		lexer_next(self);
		l = lexer_peek(self);
	}
}

void
lexer_handle_mxCell(struct lexer * self)
{
	while ("</mxCell>" != self->current_word)
	{
		char l = lexer_peek(self);
		std::string cw = self->text.substr(self->start,
						   self->current - self->start);
		lexer_set_curr_word(self, cw);

		if (l == '\n' )
		{
			lexer_skip_until(self, '<');
			self->start = self->current;
			lexer_increment_line(self);
			break;
		}

		if ("<mxGeometry" == self->current_word)
		{
			lexer_skip_line(self);
		}

		if (l == '=')
		{
			std::string var = self->current_word;
			//Jump into the string skipping the '=' and '"'
			if (lexer_skip(self, 2) == false)
			{
				break;
			}

			lexer_consume_string(self);

			std::string val  = self->text.substr(self->start,
							     self->current - self->start);
			if (val.find('&') != -1)
			{
				printf("\n\n[ERROR] An error occured when reading the value [%s] on line [%lu]. Perhaps [%s] is corrupted. Please try another file.\n", val.c_str(), self->line, self->file_path.c_str());
				printf("%s\n", val.c_str());
				exit(EXIT_FAILURE);
			}

			struct token * t = lexer_create_token(self, var, val);


			if (t)
			{
			    lexer_add_token(self, t);
			    printf("lexer: %s\n", token_to_string(t).c_str());
			}

			lexer_skip(self, 2);
			continue;
		} /* end of '=' */
		lexer_next(self);
	} /* end of <mxCell> */
	lexer_reset_start(self);
}

std::string
token_type_to_string(enum TOKEN_TYPE type)
{
	switch (type)
	{
	        case TOKEN_CLASS:     { return "TOKEN_CLASS";      }
	        case TOKEN_INTERFACE: { return "TOKEN_INTERFACE";  }
	        case TOKEN_ID:        { return "TOKEN_ID";         }
	        case TOKEN_STYLE:     { return "TOKEN_STYLE";      }
	        case TOKEN_PARENT:    { return "TOKEN_PARENT";     }
	        case TOKEN_VALUE:     { return "TOKEN_VALUE";      }
	        case TOKEN_SOURCE:    { return "TOKEN_SOURCE";     }
	        case TOKEN_TARGET:    { return "TOKEN_TARGET";     }
	        case TOKEN_FIELD:     { return "TOKEN_FIELD";      }
	        case TOKEN_METHOD:    { return "TOKEN_METHOD";     }
	        case TOKEN_PACKAGE:   { return "TOKEN_PACKAGE";    }
	        case TOKEN_ARROW:     { return "TOKEN_ARROW";      }
	        case TOKEN_ARROW_REL: { return "TOKEN_ARROW_REL";  }
	        case TOKEN_ARROW_COMP:{ return "TOKEN_ARROW_COMP"; }

		default:              { return "INVALID TOKEN TYPE"; }
	}
}

std::string
token_to_string(struct token * token)
{
	char buff[KB];
	sprintf(buff, "{ TOKEN: { TYPE: %s, KEY: %s, VAL: %s } }\n",
		token_type_to_string(token->type).c_str(),
	        token->key.c_str(),
	        token->value.c_str());
	return std::string(buff);
}
