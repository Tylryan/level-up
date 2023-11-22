#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "./vector.h"

/* Internal */
static bool v_below_limit(vector_t * vector);
static void v_expand(vector_t * vector);
static void v_contract(vector_t * vector);

vector_t
v_create(size_t capacity)
{
	vector_t vector = {0};
	vector.capacity = capacity;
	vector.array    = malloc(capacity * sizeof(void *));
	for (int i = 0;  i < capacity;i++)
	{
		vector.array[i] = NULL;
	}
	
	return vector;
}

void *
v_get(vector_t * vector, long index)
{
	if (index < 0)
	{
		index = vector->size + index;
	}

	if (index > vector->size)
	{
		fprintf(stdout, "vector.v_get() - index '%ld' is out of bounds\n", index);
	}

	return vector->array[index];
}

void
v_remove(vector_t * vector, long index)
{
	if (index < 0)
	{
		index = vector->size + index;
	}

	if (v_is_empty(vector))
	{
		printf("VECTOR WAS EMPTY");

		return;
	}

	if (index >= vector->size)
	{
		printf("INDEX OUT OF BOUDNS\n");
		return;
	}

	size_t left = index;
	size_t right = index+1;

	while (right < vector->size)
	{
		vector->array[left] = vector->array[right];
		right++;
		left++;
	}
	vector->array[left] = vector->array[right];

	vector->size--;
}

int
v_add(vector_t * vector, void * element)
{
	if (v_is_full(vector))
	{
		v_expand(vector);
	}

	vector->array[vector->size] = element;
	vector->size++;
		
	return 0;
}

bool
v_is_empty(vector_t * vector)
{
	return vector->size == 0;
}

bool
v_is_full(vector_t * vector)
{
	return vector->size == vector->capacity;
}

void
v_free(vector_t * vector)
{
	free(vector->array);
	vector->size = 0;
}

void
v_expand(vector_t * vector)
{
	size_t new_capacity = vector->size * EXPAND_MULTIPLIER;
	void ** new_array   = malloc(new_capacity * sizeof(void *));
	void ** old_array   = vector->array;

	for (int i = 0; i < new_capacity; i++)
	{
		new_array[i] = NULL;
	}

	for (int i = 0; i < vector->size; i++)
	{
		new_array[i] = old_array[i];
	}

	vector->capacity = new_capacity;

	free(vector->array);
	vector->array = new_array;
}

/*
 * DESCRIPTION
 *  This function doesn't actually free values in the array.
 *  It will just decrement the size of the vector. Freeing of
 *  the values will be done at once at the end.
 */
void *
v_pop(vector_t * vector)
{
	if (vector->size == 0)
	{
		return NULL;
	}
	if (v_below_limit(vector))
	{
		v_contract(vector);
	}

	void * val = v_get(vector, -1);
	vector->size--;
	return val;
}

bool
v_below_limit(vector_t * vector)
{
	return vector->size < (vector->capacity / EXPAND_MULTIPLIER);
}

void
v_contract(vector_t * vector)
{
	size_t new_capacity = vector->size / EXPAND_MULTIPLIER;
	void ** new_array   = malloc(new_capacity * sizeof(void*));
	void ** old_array   = vector->array;

	for (int i = 0; i < new_capacity; i++)
	{
		new_array[i] = NULL;
	}

	for (int i = 0; i < vector->size; i++)
	{
		new_array[i] = old_array[i];
	}

	vector->capacity = new_capacity;

	free(vector->array);
	vector->array = new_array;
}

void *
v_peek(vector_t * vector)
{
	return v_get(vector, -1);
}

void
v_shift_right(vector_t * vector, size_t index)
{

	int right = vector->size;
	int left = right -1;

	while (left > index)
	{
		vector->array[right] = vector->array[left];
		right--;
		left--;
	}
	vector->array[right] = vector->array[left];
	vector->array[index] = 0;
}

int
v_insert(vector_t * vector, void * element, long index)
{

	if (index < 0)
	{
		index = vector->size + index;
	}

	if (index == 0 && index == vector->size)
	{
		vector->array[0] = element;
		vector->size++;
		return 0;
		
	}

	if (v_is_full(vector))
	{
		v_expand(vector);
	}

	if (index > vector->size)
	{
		fprintf(stdout, "vector.v_insert() - INDEX OUT OF BOUNDS\n");
		return -1;
	}

	v_shift_right(vector, index);
	vector->array[index] = element;
	vector->size++;

	return 0;
}
