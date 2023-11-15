
#include <stdio.h>
#include <stdbool.h>
#include "./ctest.h"

int test_count = 0;
int test_pass  = 0;
int test_fail  = 0;


void
ctest_summary(void)
{
	printf("=====================================\n");
	printf("         TEST SUMMMARY: %s           \n", __FILE__);
	printf("=====================================\n");
	printf("TOTAL TESTS: %d\n", test_count);
	printf("TESTS PASSED: %d\n", test_pass);
	printf("TESTS FAILED: %d\n", test_fail);
	printf("=====================================\n");
	test_count = test_pass = test_fail = 0;
}

void
ctest_init(void)
{
	printf("======== TESTING: %s ==========\n", __FILE__);
}
