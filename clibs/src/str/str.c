#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./str.h"

struct str_t *
str_create(char * val)
{
	struct str_t * string = (struct str_t*) malloc(sizeof(struct str_t));

	int32_t size  = strlen(val);
	string->val  = (char*) malloc((size+1) * sizeof(char*));
	string->size = size;

	memcpy(string->val, val, strlen(val) + 1);
	return string;
}

struct str_t *
str_create_empty()
{
	struct str_t * string = (struct str_t*) malloc(sizeof(struct str_t));

	int32_t size  = 0;
	string->val  = (char*) malloc((size+1) * sizeof(char*));
	string->size = size;

	memcpy(string->val, "", 1);
	return string;
}

void
str_destroy(struct str_t * self)
{
	free(self->val);
	free(self);
}

int32_t str_handle_index(struct str_t * self, long index)
{
	if (index < 0)
	{
		index += self->size;
	}

	if (index >= (long)self->size)
	{
		fprintf(stderr, "error: index out of bounds\n");
		abort();
	}

	return index;
}

char
str_get(struct str_t * self, long index)
{
	index = str_handle_index(self, index);
	return self->val[index];
}

char
str_at(struct str_t * self, long index)
{
	return str_get(self, index);
}

void
str_set(struct str_t * self, long index, char val)
{
	index = str_handle_index(self, index);
	self->val[index] = val;
}

bool
str_equals(struct str_t * self, char * other)
{
	return strcmp(self->val, other) == 0;
}

bool
str_equalsic(struct str_t * self, char * other)
{
	if (self->size != strlen(other))
	{
		return false;
	}

	for (int32_t i = 0; i < self->size; i++)
	{
		char left = tolower(self->val[i]);
		char right = tolower(other[i]);

		if (left != right)
		{
			return false;
		}
	}

	return true;
}

void
str_reset(struct str_t * self)
{
	str_destroy(self);
	self = str_create_empty();
}

void str_append(struct str_t * self, char * other)
{
	str_sappend(self, other);
}

void
str_sappend(struct str_t * self, char * other)
{
	int32_t new_size = self->size + strlen(other);
	self = realloc(self, new_size + 1);
	strncat(self->val, other, strlen(other) + 1);
	self->size = new_size;
}

void
str_push_back(struct str_t * self, char other)
{
	str_cappend(self, other);
}

void
str_cappend(struct str_t * self, char other)
{
	int32_t new_size = self->size + 1;
	self = realloc(self, new_size);
	strncat(self->val, &other, + 1);
	self->size = new_size;
}

long
str_cfind(struct str_t * self, char needle)
{
	for (int32_t i = 0; i < self->size; i++)
	{
		if (self->val[i] == needle)
		{
			return i;
		}
	}
	return -1;
}

struct str_t *
str_substr(struct str_t * self, int32_t start, int32_t end)
{
	if (start == end)
	{
		return str_create("");
	}

	int32_t size = end - start + 1;
	char buff[size];
	strncpy(buff, self->val + start, size -1);
	return str_create(buff);
}

long
str_sfind(struct str_t * self, char * needle)
{
	int32_t l, r, start;
	l = r = start = 0;

	struct str_t * sub = str_create("");
	while (r < self->size)
	{
		while (self->val[r] == needle[l])
		{
			str_cappend(sub, self->val[r]);
			if (strcmp(sub->val, needle) == 0)
			{
				str_destroy(sub);
				return start;
			}
			r++;
			l++;
		}

		str_reset(sub);

		r++;
		start++;
	}

	str_destroy(sub);
	return -1;
}

bool
str_scontains(struct str_t * self, char * needle)
{
	if (str_sfind(self, needle) == -1)
	{
		return false;
	}
	return true;
}

bool
str_ccontains(struct str_t * self, char needle)
{
	if (str_cfind(self, needle) == -1)
	{
		return false;
	}

	return true;
}

struct str_t *
str_reversed(struct str_t * self)
{
	struct str_t * rev = str_create_empty();
	for (long i = (long)self->size - 1; i >= 0; i--)
	{
		str_cappend(rev, str_get(self, i));
	}

	return rev;
}
