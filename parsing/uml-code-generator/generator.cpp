#include <boost/algorithm/string/trim.hpp>
#include <cstdlib>
#include <string>

#include "./generator.h"
#include "parser.h"

void handle_fields(struct generator * self, struct a_array aa, union p_object * k, struct Klass * klass);
void handle_methods(struct generator * self, struct a_array aa, union p_object * k, struct Klass * klass);
std::string gen_translate_method(struct generator * self, union p_object * field);

void
gen_generate(struct generator * self)
{
	std::vector<Klass> klasses;
	struct a_array aa = a_array_create(self);

	for (int i = 0; i < aa.classes.size();i++)
	{
		union p_object * k = aa.classes[i];

		struct Klass klass;
		klass.type    = OBJ_KLASS;
		klass.name    = k->klass->value;
		klass.package = k->klass->package;

		handle_relations(self, aa, k, &klass);
		handle_fields(self, aa, k, &klass);
		handle_methods(self, aa, k, &klass);

		klasses.push_back(klass);
	}

	for (int i = 0; i < aa.interfaces.size(); i++)
	{
		printf("\n\nparser: HANDLING INTERFACES\n");
		union p_object * k = aa.interfaces[i];

		struct Klass klass;
		klass.type = OBJ_INTERFACE;
		klass.name = k->inter->value;
		klass.package = k->inter->package;

		handle_relations(self, aa, k, &klass);
		handle_fields(self, aa, k, &klass);
		handle_methods(self, aa, k, &klass);

		klasses.push_back(klass);
	}

	self->klasses = klasses;
}

struct generator
gen_create(std::vector<union p_object> * p_objects)
{
	struct generator gen = generator();
	gen.p_objects = p_objects;

	return gen;
}

std::string gen_translate_field(struct generator *self, union p_object *field);

struct a_array
a_array_create(struct generator * self)
{
	struct a_array array = a_array();

	while (gen_has_next(self))
	{
		union p_object * po = gen_peek(self);
		enum P_TYPE type    = po->common->type;

		if      (type == PARSER_KLASS)     { array.classes.push_back(po);   }
		else if (type == PARSER_INTERFACE) { array.interfaces.push_back(po);}
		else if (type == PARSER_ARROW)     { array.arrows.push_back(po);    }
		else if (type == PARSER_ARROW_REL) { array.arrows.push_back(po);    }
		else if (type == PARSER_ARROW_COMP){ array.arrows.push_back(po);    }
		else if (type == PARSER_FIELD)
		{
			po->field->value = gen_translate_field(self, po);
			array.fields.push_back(po);
		}
		else if (type == PARSER_METHOD)
		{
			po->method->value = gen_translate_method(self, po);
			array.methods.push_back(po);
		}
		/* These are probably arrows */
		gen_next(self);
	}

	return array;
}

void
gen_next(struct generator * self)
{
	self->current++;
}

bool
gen_has_next(struct generator * self)
{
	if (self->current >= self->p_objects->size())
	{
		return false;
	}
	return true;
}

union p_object *
gen_peek(struct generator * self)
{
	if (gen_has_next(self) == false)
	{
		return NULL;
	}

	return &self->p_objects->at(self->current);
}

union p_object *
gen_peek_next(struct generator * self)
{
	if (self->current +1 >= self->p_objects->size())
	{
		return NULL;
	}

	return &self->p_objects->at(self->current+1);
}

union p_object *
gen_peek_prev(struct generator * self)
{
	if (self->current < self->p_objects->size())
	{
		return NULL;
	}

	return &self->p_objects->at(self->current-1);
}

void
gen_set_cindex(struct generator * self, size_t index)
{
	self->current = index;
}

void handle_arrows_gen();
void handle_arrows_rel();
void handle_arrows_comp();
/*
  ARROW: Target should be the object being pointed to
  ARROW_REL: Target should be the object being inherited
  ARROW_COMP: Target should be the object being used
*/
void
handle_relations(struct generator * self,
		 struct a_array aa      ,
		 union p_object * k     ,
		 struct Klass * klass)
{
	printf("parser: AA.ARROWS SIZE: %lu\n", aa.arrows.size());
	for (union p_object * ao: aa.arrows)
	{

		printf("\n\nparser: HANDLING RELATIONS\n");

		std::string klass_id = k->common->id;
		P_TYPE atype = ao->common->type;
		union p_object * o;

		if (atype == PARSER_ARROW){
			if (ao->arrow_gen->source_id == klass_id)
			{
			    o = find_p_object(self, ao->arrow_gen->target_id);
			}
			else {continue;}
		}
		else if (atype == PARSER_ARROW_COMP)
		{
			/* compare the source_id with this object's parent */
			union p_object * obj = find_p_object(self, ao->arrow_comp->source_id);
			union p_object * parent_obj = find_p_object(self, obj->common->pid);

			if (parent_obj->common->id != k->common->id)
			{
				continue;
			}
				
			o = find_p_object(self, ao->arrow_comp->target_id);
		}
		else if (atype == PARSER_ARROW_REL)
		{
			if (ao->arrow_rel->source_id == klass_id)
			{
			    o = find_p_object(self, ao->arrow_rel->target_id);
			}
			else {continue;}
		}
		else { continue; }

		if (o == NULL)
		{
			printf("[info] generator.handle_relations() - could not find matching relation\n");
			continue;
		}

		enum P_TYPE otype = o->common->type;
		/* Find the class for the method/field */
		if (otype == PARSER_METHOD || otype == PARSER_FIELD)
		{
			/* Find parent object */
			o = find_p_object(self, o->common->pid);
			if (otype == PARSER_FIELD)
			{
			    klass->imports.push_back(o->inter->import_path);
			}
			otype = o->common->type;
		}

		if (otype == PARSER_KLASS)
		{
			/* TODO(tyler): NEVER REACHES HERE */
			//klass->interfaces.push_back(o->klass);
		}

		else if (otype == PARSER_INTERFACE)
		{
			printf("parser: FOUND INTERFACE\n");
			klass->interfaces.push_back(o->inter);

			klass->imports.push_back(o->inter->import_path);
		}

	}
}

void
handle_fields(struct generator * self,
	      struct a_array aa,
	      union p_object * k,
	      struct Klass * klass)
{
	for (auto po: aa.fields)
	{
		if (po->field->pid != k->common->id)
		{
			continue;
		}

		klass->fields.push_back(po->field);
	}
}


void
handle_methods(struct generator * self, struct a_array aa, union p_object * k, struct Klass * klass)
{
	for (auto po: aa.methods)
	{
		if (po->method->pid != k->common->id)
		{
			continue;
		}
		klass->methods.push_back(po->method);
	}
}

/* Find a p_object with the specified id */
union p_object *
find_p_object(struct generator * self, std::string id)
{
	size_t og_index = self->current;
	gen_set_cindex(self, 0);

	while (gen_has_next(self))
	{
		union p_object * co = gen_peek(self);

		if (co->common->id == id)
		{
			gen_set_cindex(self, og_index);
			return co;
			
		}
		gen_next(self);
	}

	printf("[info] generator.find_p_object() - did not find a match for '%s'\n",
	       id.c_str());

	gen_set_cindex(self, og_index);
	return NULL;
}

/* TODO(tyler) make this more readable */
std::string
gen_translate_field(struct generator *self,  union p_object *field)
{
	std::string value = field->field->value;
	std::string res = std::string();

	if (value[0] == '+')
	{
		res.append("public ");
	}
	else if (value[0] == '-')
	{
		res.append("private ");
		
	}
	else if (value[0] == '~')
	{
		res.append("");
	}

	int colon = value.find_first_of(':');

	if (colon < 0)
	{
                fprintf(stdout, "[error] invalid FIELD. missing ':' in '%s'.\n",
			value.c_str());
		exit(EXIT_FAILURE);

	}

	/* Field type */
	for (int i = colon +1; i < value.size(); i++)
	{
		if (value[i] == ' ')
		{
			continue;
		}
		res.push_back(value[i]);
	}
	res.push_back(' ');
	/* Field name */
	for (int i = 1; i < value.size() && i < colon; i++)
	{
		if (value[i] == ' ')
		{
			continue;
		}
		res.push_back(value[i]);
	}
	res.push_back(';');

	return res;
}    

/* TODO(tyler) Make this more readable */
std::string
gen_translate_method(struct generator * self, union p_object * field)
{
	std::string value = field->field->value;
	std::string res = std::string();

	if (value[0] == '+')
	{
		res.append("public ");
	}
	else if (value[0] == '-')
	{
		res.append("private ");
	}
	else if (value[0] == '~')
	{
		res.append("");
	}

	int fcolon    = value.find_first_of(':');
	int lcolon    = value.find_last_of(':');
	int beg_paren = value.find_first_of('(');
	int end_paren = value.find_first_of(')');
	if (lcolon < 0 || value[end_paren+1] != ':')
	{
                fprintf(stdout, "[error] invalid METHOD. missing ':' in '%s'.\n",
			value.c_str());
		exit(EXIT_FAILURE);
	}
	

	/* Method type */
	int i = lcolon +1;
	for (; i < value.size(); i++)
	{
		if (value[i] == ' ')
		{
			continue;
		}
		res.push_back(value[i]);
	}
	res.push_back(' ');

	/* method name */
	std::string method_name = std::string();
	i = 1;
	for (; i <= beg_paren; i++)
	{
		if (value[i] == ' ')
		{
			continue;
		}
		method_name.push_back(value[i]);
	}
	res.append(method_name);

	/* at this point the program is either inside a ()
	   or it's at the end ')'
	*/

	/* method params */
	int j = i;

	if (value[j] == ')')
	{
		res.push_back(')');
		return res;
	}

	while (j < value.size())
	{
	    std::string name;
	    std::string tipe;

	    /* get param name */
	    while (value[j] != ':')
	    {
		    if (value[j] == ' ')
		    {
			    j++;
			    continue;
		    }
		    name.push_back(value[j]);
		    j++;
	    }


	    j++;

	    /* get param type */
	    while (value[j] != ')' && j < value.size())
	    {
		    if (value[j] == ' ')
		    {
			    j++;
			    continue;
		    }

		    if (value[j] == ',')
		    {
			    name.append(", ");
			    j++;
			    break;
		    }

		    tipe.push_back(value[j]);
		    j++;
	    }

	    res.append(tipe + " ");
	    res.append(name);

	    /* why I can't just put if statement in the
	       while loop, I guess I'll never know.
	    */
	    if (value[j] == ')')
	    {
		    break;
	    }

	    j++;
	}

	res.push_back(')');
	return res;
}
