#pragma once

#include <stdlib.h>
#include <stdbool.h>

/**************************************
A simple size based string library.
 **************************************/
typedef struct __str
{
	char * string;
	size_t length;
} str_t;

/* Constructor */
str_t str(const char * string);

void sfree(str_t string);

/* String to char pointer */
char * stcp(const str_t * string);
/* String from char pointer */
str_t sfcp(const char * string);

/* I believe this will still have the null term at the end */
void sappend(str_t * string, const char * cp);

str_t ssub(const str_t * string, size_t start, size_t end);
