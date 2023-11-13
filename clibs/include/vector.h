#pragma once

#include <stdlib.h>
#include <stdbool.h>

static size_t EXPAND_MULTIPLIER = 2;

//enum vector_type
//{
//	INT,
//	LONG,
//	FLOAT,
//	STRING,
//	CLASS
//};

typedef struct {
	size_t size;
	size_t el_size;
	size_t capacity;
	//vector_type type;
	void ** array;
} vector_t;

vector_t Vector(size_t capacity);
void * v_get(vector_t * vector, long index);
void * v_pop(vector_t * vector);
int v_append(vector_t * vector, void * element);
bool v_is_full(vector_t * vector);
bool v_is_empty(vector_t * vector);
void v_free(vector_t * vector);
void v_expand(vector_t * vector);
void v_contract(vector_t * vector);
bool v_below_limit(vector_t * vector);
void v_remove(vector_t * vector, long index);
void v_shift_right(vector_t * vector, size_t index);
int v_insert(vector_t * vector, void * element, long index);
void * v_peek(vector_t * vector);
