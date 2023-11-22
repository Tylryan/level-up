#pragma once

#include <stdlib.h>
#include <stdbool.h>

static size_t EXPAND_MULTIPLIER = 2;

typedef struct {
	size_t size;
	size_t el_size;
	size_t capacity;
	//vector_type type;
	void ** array;
} vector_t;


/* Creates a vector and allocates an interal array on the heap.
   Thus it needs to be freed.

   Additionally, if the user adds heap allocated data to this
   structure, each allocatoin will also need to be freed.
*/
vector_t v_create(size_t capacity);
/* Frees only the array within the vector. If elements within
   the vector are allocated to the heap, each element will
   also need to be freed.
*/
void v_free(vector_t * vector);
/* Get an object from a vector by index */
void * v_get(vector_t * vector, long index);
/* Pop the last element off the vector and return it*/
void * v_pop(vector_t * vector);
/* Add an element to the end of the vector */
int v_add(vector_t * vector, void * element);
/* Determines whether the size of the element is equal to its capacity */
bool v_is_full(vector_t * vector);
bool v_is_empty(vector_t * vector);
/* Remove an element from the vector by its index */
void v_remove(vector_t * vector, long index);
void v_shift_right(vector_t * vector, size_t index);
/* Insert an element in the vector by its index */
int v_insert(vector_t * vector, void * element, long index);
/* Return the last element in the vector, but do not remove it */
void * v_peek(vector_t * vector);
