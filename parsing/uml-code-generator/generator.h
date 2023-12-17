#pragma once

#include <cstddef>
#include <vector>
#include <string>

#include "./parser.h"

enum OBJECT_TYPE
{
	OBJ_KLASS,
	OBJ_INTERFACE
};

struct Klass
{
	enum OBJECT_TYPE type;
	std::string name;
	std::string package;
	std::vector<std::string> imports;
	std::vector<struct inter*> interfaces;
	std::vector<struct field*> fields;
	std::vector<struct method*> methods;
};

struct generator
{
	size_t current{};
	std::vector<union p_object> * p_objects;
	std::vector<struct Klass> klasses;
};

struct a_array
{
	std::vector<union p_object*> classes;
	std::vector<union p_object*> interfaces;
	std::vector<union p_object*> fields;
	std::vector<union p_object*> methods;
	std::vector<union p_object*> arrows;
};


struct generator gen_create(std::vector<union p_object> * p_objects);
void gen_generate(struct generator * self);

struct a_array a_array_create(struct generator * self);

bool gen_has_next(struct generator * self);
void gen_next(struct generator * self);
union p_object * gen_peek(struct generator * self);
union p_object * gen_peek_next(struct generator * self);
union p_object * gen_peek_prev(struct generator * self);
void gen_set_cindex(struct generator * self, size_t index);

union p_object * find_p_object(struct generator * self, std::string id);
void handle_relations(struct generator *self, struct a_array aa, union p_object * k, struct Klass * klass);
