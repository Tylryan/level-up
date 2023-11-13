#include "./logger.h"

int
main(void)
{
	LOG(INFO, "Just lettin' you know");
	LOG(WARN, "Whoo there nelly!");
	LOG(ERR, "Sound the alarms!!");
}
