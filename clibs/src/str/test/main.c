#include <stdio.h>
#include <assert.h>

#include "../str.h"

int
main(void)
{
	struct str_t * str = str_create("HELLER");
	assert(strcmp(str->val, "HELLER") == 0);
	assert(str->size == strlen("HELLER"));

	str_sappend(str, " THERE");
	assert(str->size == strlen("HELLER THERE"));
	assert(strcmp(str->val, "HELLER THERE") == 0);

	str_sappend(str, " GUY");
	assert(strcmp(str->val, "HELLER THERE GUY") == 0);
	assert(str->size == strlen("HELLER THERE GUY"));

	char c = 'A';
	str_cappend(str, c);
	assert(str->size == strlen("HELLER THERE GUY") + 1);

	struct str_t * s = str_create("");
	assert(s->size == 0);
	assert(strcmp(s->val, "") == 0);
	str_cappend(s, 'H');
	str_cappend(s, 'I');

	assert(strcmp(s->val, "HI") == 0);

	assert(str_cfind(str, '0') == -1);
	assert(str_cfind(str, 'R') == 5);

	struct str_t * sub = str_substr(str, 0, 2);
	assert(strcmp(sub->val, "HE") == 0);
	assert(sub->size == 2);

	assert(str_sfind(str, "LLO") == -1);
	assert(str_sfind(str, "LLER") == 2);

	struct str_t * emp = str_create_empty();
	assert(emp->size == 0);
	assert(strcmp(emp->val, "") == 0);

	assert(str_ccontains(str, 'H') == true);
	assert(str_ccontains(str, 'h') == false);
	assert(str_scontains(str, "HELL") == true);
	assert(str_scontains(str, "HELLO") == false);

	struct str_t * e = str_create("HI");
	assert(str_equals(e, "HI") == true);
	assert(str_equals(e, "hi") == false);
	assert(str_equals(e, "HELLO") == false);

	assert(str_equalsic(e, "hi") == true);
	assert(str_equalsic(e, "hello") == false);

	assert(str_at(e, 0) == 'H');
	assert(str_at(e, -1) == 'I');

	str_set(e, 0, 'F');
	assert(strcmp(e->val, "FI") == 0);
	assert(e->size == strlen(e->val));

	str_set(e, -1, 'O');
	assert(strcmp(e->val, "FO") == 0);
	assert(e->size == strlen(e->val));

	struct str_t * rev = str_reversed(e);
	assert(strcmp(rev->val, "OF") == 0);

	str_destroy(e);
	str_destroy(s);
	str_destroy(str);
	str_destroy(sub);
	str_destroy(emp);

	printf("STR TESTS PASSED\n");
}
