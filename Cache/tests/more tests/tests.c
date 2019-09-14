#include <stdio.h>
#include <stdlib.h>
#include "tests.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("ERROR in the tests main\n");
		return 0;
	}
	int testNum = atoi(argv[1]);
	if ((testNum == 0 && mySetTests(testNum) && cacheTests(testNum) && memCacheTests(testNum)) ||
			(testNum >= 1 && testNum <= 12 && mySetTests(testNum)) ||
			(testNum >= 13 && testNum <= 18 && cacheTests(testNum)) ||
			(testNum >= 19 && testNum <= 31 && memCacheTests(testNum)))
	{
		printf("TEST COMPLETED SUCCESSFULLY\n");
	}
	else
	{
		printf("TEST FAILED\n");
	}

	return 0;
}
