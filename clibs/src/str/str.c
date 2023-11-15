#include <stdio.h>
#include <string.h>

#include "str.h"
#include <vector.h>

str_t
str(const char * string)
{
	size_t string_len = strlen(string);
	char * copy = (char*) malloc(string_len * sizeof(char));

	if (string == NULL)
		copy = "";
	else 
	    strncpy(copy, string, strlen(string));

	str_t str = {
		.string = copy,
		.length = string_len
	};

	return str;
}

void
sfree(str_t string)
{
	free(string.string);
	string.length = 0;
}

void
sappend(str_t * string, const char * cp)
{
	size_t cp_length = strlen(cp);
	size_t old_length = string->length;
	size_t new_length = cp_length + old_length;
	char * old_string = string->string;
	string->string = (char *) realloc(string->string, new_length * sizeof(char));
	strncpy(string->string, old_string, old_length);
	strncat(string->string, cp, cp_length);

	printf("NEW STRING: %s\n", string->string);
	string->length = new_length;
}

str_t
ssub(const str_t *string, size_t start, size_t end)
{
	char sub[end - start];
	strncpy(sub, string->string+start, end);
	sub[end] = '\0';
	return str(sub);
}


enum FLGS
{
	N = 0,
	A = 1,
	B = 2,
	C = 4,
	D = 8
};

int
main(void)
{

	int something = N | B | C | D;
	//something |=1;
	/* something |=B; */
	/* something |=C; */
	/* something |=D; */


	printf("0x%02x\n", something);

	if (something & B)
	{
		printf("YUP\n");
		
	}
					    


	return 0;
}
