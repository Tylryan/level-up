/**
 * DESCRIPTION
 *   A logger macro that will print out the file, function, and line
 *   where the macro was called.
 *
 * USAGE
 *  LOG(INFO, "My helpful log message);
 *  $ INFO: main.c:some_func():12 - My helpful log message
 *
 * 2023, Tyler Ryan
 */
#pragma once
#include <stdio.h>

enum LOG_LEVEL 
{
	INFO,
	WARN,
	ERR,
	
};

#define LOG(LOG_LEVEL, msg...)						\
	switch (LOG_LEVEL) 							   	   \
	{										   \
	case INFO: 									   \
	{										   \
		printf("| INFO | [%s] | %s:%s():%d\t| ", __TIMESTAMP__, __FILE__, __FUNCTION__, __LINE__); \
		printf(msg);						\
		printf("\n");						\
		break; 								    	   \
	}										   \
	case WARN: 									   \
	{										   \
		printf("| WARN | [%s] | %s:%s():%d\t| ", __TIMESTAMP__, __FILE__, __FUNCTION__, __LINE__); \
		printf(msg);						                   \
		printf("\n");                                                             \
		break; 								    	   \
	}										   \
	case ERR: 									   \
	{										   \
		printf("| ERR | [%s] | %s:%s():%d\t| ", __TIMESTAMP__, __FILE__, __FUNCTION__, __LINE__); \
		printf(msg);						                   \
		printf("\n");                                                             \
		break; 								    	   \
	}										   \
	default:									   \
	{										   \
		printf("INTERNAL ERROR | [%s] | %s:%s():%d\t| MESSAGE - \"incorrect use of 'LOG()'\"\n", __TIMESTAMP__, __FILE__, __FUNCTION__, __LINE__); \
		break; 									   \
	}										   \
	}
