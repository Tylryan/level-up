#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "vector.h"

#include "ctest.h"

typedef struct {
	char * name;
} test_person_t;

void
tests_summarize()
{
	printf("******** TEST SUMMMARY **********\n");
	printf("TOTAL TESTS: %d\n", test_count);
	printf("TOTAL PASSED: %d\n", test_pass);
	printf("TOTAL FAILED: %d\n", test_fail);
	printf("*********************************\n");
}

void
test_vector_creation()
{
	vector_t hector = Vector(2);
	ASSERT_TRUE(hector.array != NULL);
	ASSERT_TRUE(hector.array != NULL);
	ASSERT_TRUE(hector.array[0] == 0);
	ASSERT_TRUE(hector.capacity == 2);
	ASSERT_TRUE(hector.size == 0    );
	ASSERT_TRUE(hector.el_size == 0 );

	v_free(&hector);
}

void
test_append_int()
{
	vector_t hector = Vector(2);
	v_append(&hector, (void*)5);
	ASSERT_TRUE(hector.size != 0);
	ASSERT_TRUE(hector.size == 1);
	ASSERT_TRUE((int*) hector.array[0] == (int*)5);

	v_append(&hector, (void*) 7);
	ASSERT_TRUE((int*) hector.array[1] == (int*)7);

	v_free(&hector);
}

void
test_append_string()
{
	vector_t hector = Vector(2);
	v_append(&hector, "HI");
	ASSERT_TRUE(hector.size != 0);
	ASSERT_TRUE(hector.size == 1);
	ASSERT_TRUE(strcmp((char*) hector.array[0], "HI") == 0);

	v_append(&hector, "THERE");
	ASSERT_TRUE(strcmp((char*) hector.array[1], "THERE") == 0);

	v_free(&hector);
}

void
test_append_class()
{
	test_person_t * p1 = malloc(sizeof(test_person_t));
	test_person_t * p2 = malloc(sizeof(test_person_t));
	test_person_t * p3 = malloc(sizeof(test_person_t));
	p1->name = "GUY";
	p2->name = "SHAWN";
	p3->name = "DAWN";

	vector_t hector = Vector(2);
	v_append(&hector, p1);
	ASSERT_TRUE(hector.size != 0);
	ASSERT_TRUE(hector.size == 1);

	v_append(&hector, p2);
	v_append(&hector, p3);

	test_person_t * p_one   = (test_person_t*) hector.array[0];
	test_person_t * p_two   = (test_person_t*) hector.array[1];
	test_person_t * p_three = (test_person_t*) hector.array[2];
	ASSERT_TRUE(strcmp(p_one->name, "GUY")   == 0);
	ASSERT_TRUE(strcmp(p_two->name, "SHAWN") == 0);
	ASSERT_TRUE(strcmp(p_three->name, "DAWN")   == 0);

	//assert(strcmp((char*) hector.array[1], "THERE") == 0);


	v_free(&hector);
	free(p_one);
	free(p_two);
	free(p_three);
}

void
test_remove()
{
	vector_t hector = Vector(2);
	v_append(&hector, (void*)1);
	v_append(&hector, (void*)-2);
	v_append(&hector, (void*)3);
	v_append(&hector, (void*)4);

	/* Remove the last element */
	v_remove(&hector, -1);
	ASSERT_TRUE(hector.size ==3);
	/* It's a pain to convert a void * to an int */
	int * value = (int*) v_get(&hector, -1);
	ASSERT_TRUE(value == (int*)3);

	/*Remove the last element */
	v_remove(&hector, -1);
	ASSERT_TRUE(hector.size == 2);
	ASSERT_TRUE(v_get(&hector, -1) == (void*)-2);

	/* Remove the first element */
	v_remove(&hector, 0);
	ASSERT_TRUE(hector.size == 1);
	ASSERT_TRUE(v_get(&hector, 0) == (int*)-2);

	v_free(&hector);
}

/*
 * DESCRIPTION
 *  Should return the last value in the vector, but not remove it.
 */
void
test_peek()
{
	vector_t hector = Vector(2);
	v_append(&hector, (void*)1);
	v_append(&hector, (void*)-2);
	
	void * last = v_peek(&hector);
	ASSERT_TRUE(last == (void*)-2);
	ASSERT_TRUE(v_get(&hector, -1) == (void*) -2);
}

void
test_expand()
{
	vector_t hector = Vector(2);
	hector.size = 2;
	v_expand(&hector);

	ASSERT_TRUE(hector.capacity == 4);
	ASSERT_TRUE(v_get(&hector, 0) == NULL);
	ASSERT_TRUE(v_get(&hector, -1) == NULL);
}

void
test_shift_right()
{
	vector_t hector = Vector(2);
	v_append(&hector, (void*)1);
	v_append(&hector, (void*)2);
	v_append(&hector, (void*)3);
	v_append(&hector, (void*)4);

	v_shift_right(&hector, 1);

	ASSERT_TRUE(v_get(&hector, -1) != (void*)4);
	ASSERT_TRUE(v_get(&hector, -1) == (void*)3);
	ASSERT_TRUE(v_get(&hector, 0) == (void*)1);
	ASSERT_TRUE(v_get(&hector, 1) == (void*)0);
	ASSERT_TRUE(v_get(&hector, 1) == (void*) 0);
	ASSERT_TRUE(v_get(&hector, -1) == (void*)4);

	v_free(&hector);
}

void
test_insert()
{
	vector_t hector = Vector(2);
	v_insert(&hector, (void*) 1, 0);
	v_insert(&hector, (void*) 10, 0);
	v_insert(&hector, (void*) 0, 1);
	v_insert(&hector, (void*) 5, 1);
	v_insert(&hector, (void*) 2, -1);


	ASSERT_TRUE(v_get(&hector, 0) == (void*) 10);
	ASSERT_TRUE(v_get(&hector, 1) == (void*) 5);
	ASSERT_TRUE(v_get(&hector, 2) == (void*) 0);
	ASSERT_TRUE(v_get(&hector, 3) == (void*) 2);
	ASSERT_TRUE(v_get(&hector, -1) == (void*) 1);

	v_free(&hector);
}

int
main(void)
{
	printf("========== LIBVECTOR =========\n");
	test_vector_creation();
	test_append_int();
	test_append_string();
	test_append_class();
	test_remove();
	test_peek();
	test_expand();
	//test_shift_right();
	test_insert();
	
	tests_summarize();

}
