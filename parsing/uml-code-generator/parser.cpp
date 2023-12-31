#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <vector>

#include "./parser.h"
#include "./lexer.h"

/* Private functions */
struct parser parser_create(std::vector<struct token*> tokens, bool has_packages);
void parser_destroy(struct parser * self);
void parser_destroy_object(union p_object * self);

struct token * parser_peek(struct parser *self);
struct token * parser_peek_next(struct parser * self);
struct token * parser_peek_prev(struct parser * self);
struct token * parser_peek_at(struct parser *self, size_t val);
bool parser_has_next(struct parser * self);
void parser_next(struct parser * self);
bool parser_skip(struct parser * self, size_t n);
void parser_parse_class(struct parser * self);
void parser_parse_field(struct parser * self);
void parser_parse_method(struct parser * self);
void parser_parse_interface(struct parser * self);
void parser_parse_arrow(struct parser * self, enum TOKEN_TYPE arrow_type);
void parser_parse_arrow_comp(struct parser * self, enum TOKEN_TYPE arrow_type);
void parser_parse_arrow_rel(struct parser * self, enum TOKEN_TYPE arrow_type);

std::vector<struct klass>
parser_parse(struct parser * self)
{
	struct token * current_token;
	
	while (parser_has_next(self))
	{
		current_token        = parser_peek(self);
		enum TOKEN_TYPE type = current_token->type;

		if      (type == TOKEN_CLASS)     { parser_parse_class(self);           }
		else if (type == TOKEN_FIELD)     { parser_parse_field(self);           }
		else if (type == TOKEN_METHOD)    { parser_parse_method(self);          }
		else if (type == TOKEN_INTERFACE) { parser_parse_interface(self);       }
		else if (type == TOKEN_ARROW)     { parser_parse_arrow(self, type);     }
		else if (type == TOKEN_ARROW_REL) { parser_parse_arrow_rel(self, type); }
		else if (type == TOKEN_ARROW_COMP){ parser_parse_arrow_comp(self, type);}

		parser_next(self);
	}
	
	return std::vector<struct klass>();
}

void
parser_destroy(struct parser * self)
{
	for (union p_object po: self->parser_objects)
	{
	    parser_destroy_object(&po);
	}
}

void
parser_destroy_object(union p_object * self)
{
	switch (self->common->type)
	{
		case PARSER_KLASS:     { delete self->klass; return;     }
		case PARSER_FIELD:     { delete self->field; return;     }
		case PARSER_METHOD:    { delete self->method; return;    }
		case PARSER_INTERFACE: { delete self->inter; return;     }
		case PARSER_PACKAGE:   { delete self->package; return;   }
		case PARSER_ARROW:     { delete self->arrow_gen; return;     }
		case PARSER_ARROW_COMP:{ delete self->arrow_comp; return;}
		case PARSER_ARROW_REL: { delete self->arrow_rel; return; }
	}
}

void
parser_parse_class(struct parser * self)
{
	struct token * current_token = parser_peek(self);

	struct klass * k = new klass();
	k->type  = PARSER_KLASS;
	k->value = current_token->value;

	if (self->has_packages)
	{
		parser_next(self);
		k->package = parser_peek(self)->value;

		parser_next(self);
		k->id = parser_peek(self)->value;
	}
	else
	{
		k->id = parser_peek_prev(self)->value;
	}

	/* Skip the style token */
	parser_skip(self, 2);
	k->pid = parser_peek(self)->value;

	/* TODO(tyler) need to free these */
	union p_object po = {.klass = k};

	self->parser_objects.push_back(po);
}

void
parser_parse_field(struct parser * self)
{
	struct token * current_token = parser_peek(self);
	struct token * prev_token    = parser_peek_prev(self);

	struct field * f = new field();
	f->type  = PARSER_FIELD;
	f->value = current_token->value;
	f->id    = parser_peek_prev(self)->value;

	/* Skip the style token */
	parser_skip(self, 2);
	f->pid = parser_peek(self)->value;

	/* TODO(tyler) need to free these */
	union p_object po = {.field = f};

	self->parser_objects.push_back(po);
}

void
parser_parse_method(struct parser * self)
{
	struct token * current_token = parser_peek(self);
	struct token * prev_token    = parser_peek_prev(self);

	struct method * m = new method();
	m->type  = PARSER_METHOD;
	m->value = current_token->value;

	enum P_TYPE prev_object_type = self->parser_objects
		.back()
		.common
		->type;

	if (prev_object_type == PARSER_INTERFACE)
	{
	    parser_skip(self, 2);
	    m->id    = parser_peek(self)->value;
	    parser_skip(self, 2);
	    m->pid = parser_peek(self)->value;
	}

	else
	{
	    m->id = parser_peek_prev(self)->value;
	    /* Skip the style token */
	    parser_skip(self, 2);
	    m->pid = parser_peek(self)->value;
	}


	/* TODO(tyler) need to free these */
	union p_object po = {.method = m};

	self->parser_objects.push_back(po);
}

void
parser_parse_interface(struct parser * self)
{
	struct token * current_token = parser_peek(self);
	struct token * prev_token    = parser_peek_prev(self);

	struct inter * i = new inter();
	i->type  = PARSER_INTERFACE;
	i->value = current_token->value;
	assert(i->value.empty() == false);

	/* TODO(tyler) BETTER ERROR HANDLING.
	   If a user specifies packages for one object,
	   they must do the same for all. If they don't,
	   inheritance can be lost.
	 */
	if (self->has_packages)
	{
		parser_next(self);
		i->package = parser_peek(self)->value;
		std::string import_path = i->package;
		import_path.append(".");
		import_path.append(i->value);
		i->import_path = import_path;
		parser_next(self);
		i->id = parser_peek(self)->value;
	}
	else
	{
	    i->id = parser_peek_prev(self)->value;
	}

	/* Skip the style token */
	parser_skip(self, 2);
	i->pid = parser_peek(self)->value;

	/* TODO(tyler) need to free these */
	union p_object po = {.inter = i};

	self->parser_objects.push_back(po);
}

/* I know an arrow should probably be a union */
void
parser_parse_arrow(struct parser * self, enum TOKEN_TYPE arrow_type)
{
	struct token * current_token = parser_peek(self);
	struct token * prev_token    = parser_peek_prev(self);

	struct arrow_gen * ag = new arrow_gen();
	std::string id        = parser_peek_prev(self)->value;
	std::string source_id = parser_peek_at(self, 2)->value;
	std::string target_id = parser_peek_at(self, 3)->value;

	/* Skip the style token */
	parser_skip(self, 2);
	std::string pid = parser_peek(self)->value;

	ag->type      = PARSER_ARROW;
	ag->id        = id;
	ag->pid       = pid;
	ag->source_id = source_id;
	ag->target_id = target_id;

	union p_object po = {.arrow_gen = ag};

	self->parser_objects.push_back(po);
}

void
parser_parse_arrow_comp(struct parser * self, enum TOKEN_TYPE arrow_type)
{
	struct token * current_token = parser_peek(self);
	struct token * prev_token    = parser_peek_prev(self);

	struct arrow_comp * ac = new arrow_comp();

	std::string id        = parser_peek_prev(self)->value;
	std::string source_id = parser_peek_at(self, 2)->value;
	std::string target_id = parser_peek_at(self, 3)->value;

	/* Skip the style token */
	parser_skip(self, 2);
	std::string pid = parser_peek(self)->value;

	ac->type      = PARSER_ARROW_COMP;
	ac->id        = id;
	ac->pid       = pid;
	ac->source_id = source_id;
	ac->target_id = target_id;

	union p_object po = {.arrow_comp = ac};

	self->parser_objects.push_back(po);
}

void
parser_parse_arrow_rel(struct parser * self, enum TOKEN_TYPE arrow_type)
{
	struct token * current_token = parser_peek(self);
	struct token * prev_token    = parser_peek_prev(self);

	struct arrow_rel * ar = new arrow_rel();
	std::string id        = parser_peek_prev(self)->value;
	std::string source_id = parser_peek_at(self, 2)->value;
	std::string target_id = parser_peek_at(self, 3)->value;

	/* Skip the style token */
	parser_skip(self, 2);
	std::string pid = parser_peek(self)->value;

	ar->type      = PARSER_ARROW_REL;
	ar->id        = id;
	ar->pid       = pid;
	ar->source_id = source_id;
	ar->target_id = target_id;

	union p_object po = {.arrow_rel = ar};

	self->parser_objects.push_back(po);
}

struct parser
parser_create(std::vector<struct token*> tokens, bool has_packages)
{
	struct parser p;
	p.current      = 0;
	p.tokens       = tokens;
	p.has_packages = has_packages;
	
	return p;
}

bool
parser_has_next(struct parser * self)
{
	return self->current +1 <= self->tokens.size();
}

struct token *
parser_peek(struct parser *self)
{
	return self->tokens[self->current];
}

struct token *
parser_peek_next(struct parser * self)
{
	if (parser_has_next(self) == false)
	{
		return NULL;
	}
	return self->tokens[self->current+1];
}

struct token *
parser_peek_prev(struct parser * self)
{
	if (self->current == 0)
	{
		return NULL;
	}

	return self->tokens[self->current-1];
}

struct token *
parser_peek_at(struct parser *self, size_t val)
{
	if (self->current + val > self->tokens.size())
	{
		return NULL;
	}
	return self->tokens[self->current+val];
}
void
parser_next(struct parser * self)
{
	self->current++;
}

bool
parser_skip(struct parser * self, size_t n)
{
	if (self->current+n > self->tokens.size())
	{
		return false;
	}

	self->current+=n;
	return true;
}

std::string
p_object_to_str(union p_object * po)
{
	char buff[150];
	switch (po->common->type)
	{
	case PARSER_KLASS:
	{
		sprintf(buff, "KLASS: id: %s, pid: %s, value: %s",
			po->klass->id.c_str(),
			po->klass->pid.c_str(),
			po->klass->value.c_str());
		break;
	}
	case PARSER_METHOD:
	{
		sprintf(buff, "METHOD: id: %s, pid: %s, value: %s",
			po->method->id.c_str(),
			po->method->pid.c_str(),
			po->method->value.c_str());
		break;
	}
	case PARSER_FIELD:
	{
		sprintf(buff, "FIELD: id: %s, pid: %s, value: %s",
			po->field->id.c_str(),
			po->field->pid.c_str(),
			po->field->value.c_str());
		break;
	}
	case PARSER_INTERFACE:
	{
		sprintf(buff, "INTERFACE: id: %s, pid: %s, value: %s",
			po->inter->id.c_str(),
			po->inter->pid.c_str(),
			po->inter->value.c_str());
		break;
	}
	case PARSER_ARROW:
	{
		sprintf(buff, "ARROW: id: %s, pid: %s, source: %s, target: %s",
			po->arrow_gen->id.c_str(),
			po->arrow_gen->pid.c_str(),
			po->arrow_gen->source_id.c_str(),
			po->arrow_gen->target_id.c_str());
		break;
	}
	case PARSER_ARROW_REL:
	{
		sprintf(buff, "ARROW_REL: id: %s, pid: %s, source: %s, target: %s",
			po->arrow_rel->id.c_str(),
			po->arrow_rel->pid.c_str(),
			po->arrow_rel->source_id.c_str(),
			po->arrow_rel->target_id.c_str());
		break;
	}
	case PARSER_ARROW_COMP:
	{
		sprintf(buff, "ARROW_COMP: id: %s, pid: %s, source: %s, target: %s",
			po->arrow_comp->id.c_str(),
			po->arrow_comp->pid.c_str(),
			po->arrow_comp->source_id.c_str(),
			po->arrow_comp->target_id.c_str());
		break;
	}
	default:
		sprintf(buff,"");
		break;
		
	}
	return std::string(buff);
}
