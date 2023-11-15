
#include "ctest.h"

void
one_equals_one(void)
{
	ASSERT_TRUE(1 == 1);
}

void
one_equals_two_fail(void)
{
	ASSERT_TRUE( 1 == 2);
}

void
one_not_equals_two(void)
{
	ASSERT_FALSE( 1 == 2);
}
int
main(void)
{
	one_equals_one();
	one_equals_two_fail();
	one_not_equals_two();
	test_summary();
}
