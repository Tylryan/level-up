# CTEST
## Purpose
A few macros to help with basic testing.

## Compile
If you've already run `make` in the root directory, then you can just run `make example`.

## Example
```c
#include "../../include/ctest.h"

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
}
```

### Output
```bash
pass - example.c:one_equals_one(): '1 == 1'
fail - example.c:one_equals_two_fail(): '1 == 2' == false
pass - example.c:one_not_equals_two(): '1 == 2'
```
