#include "./logger.h"

int
main(void)
{
	LOG(INFO, "Just lettin' you know: %d", 5);
	LOG(WARN, "Whoo there nelly!");
	LOG(ERR, "Sound the alarms!!");
}
