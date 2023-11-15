#pragma once

#include <stdio.h>
#include <stdbool.h>
extern int test_count;
extern int test_pass;
extern int test_fail;

#define STRINGIFY(x) #x

#define ASSERT_TRUE(expr)					\
	if ((expr) == true)						\
	{								\
		printf("pass - %s:%s(): '%s'\n", __FILE__, __FUNCTION__, STRINGIFY(expr)); \
		test_pass+=1;						\
	}								\
	else								\
	{								\
		printf("fail - %s:%s(): '%s' == false\n", __FILE__, __FUNCTION__, STRINGIFY(expr)); \
		test_fail+=1;						\
        }								\
	test_count+=1;					

#define ASSERT_FALSE(expr)					\
	if ((expr) == false)						\
	{								\
		printf("pass - %s:%s(): '%s'\n", __FILE__, __FUNCTION__, STRINGIFY(expr)); \
		test_pass+=1;						\
	}								\
	else								\
	{								\
		printf("fail - %s:%s(): '%s' == true\n", __FILE__, __FUNCTION__, STRINGIFY(expr)); \
		test_fail+=1;						\
        }								\
	test_count+=1;					

void ctest_summary(void);
void ctest_init(void);
