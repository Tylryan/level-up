#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**************************************
A simple size based string library.
 **************************************/

struct str_t
{
	char * val;
	int32_t size;
};

struct str_t * str_create(char * val);
struct str_t * str_create_empty();
void str_destroy(struct str_t * self);

/* str_reset(): sets the value of the string to "" */
void str_reset(struct str_t * self);

/* str_at(): same as str_get(). tries to replicates c++ function */
char str_at(struct str_t * self, long index);
char str_get(struct str_t * self, long index);
void str_set(struct str_t * self, long index, char val);

void str_sappend(struct str_t * self, char * other);
/* str_sappend(): same as sappend. replicates c++ function */
void str_append(struct str_t * self, char * other);
void str_cappend(struct str_t * self, char other);
/* str_push_back(): same as cappend. replicates c++ function */
void str_push_back(struct str_t * self, char other);
long str_cfind(struct str_t * haystack, char needle);
struct str_t * str_substr(struct str_t * self, int32_t start, int32_t end);
long str_sfind(struct str_t * haystack, char * needle);
bool str_scontains(struct str_t * self, char * needle);
bool str_ccontains(struct str_t * self, char needle);
bool str_equals(struct str_t * self, char * other);
/* str_equalsic(): returns true if strings equal ignoring case and false otherwise */
bool str_equalsic(struct str_t * self, char * other);
struct str_t * str_reversed(struct str_t * self);
