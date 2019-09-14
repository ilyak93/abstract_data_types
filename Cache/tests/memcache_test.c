/*
* memcache_test.c
*
*  Created on: Nov 26, 2015
*      Author: tamer.mour
*/

#include "test_utilities.h"
#include <stdlib.h>
#include "memcache.h"
#include <string.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


#define ASSERT_TRUE(x) ASSERT_TEST(x);
#define ASSERT_EQUAL(x,y) ASSERT_TEST((x) == (y));
#define ASSERT_EQUAL_STR(x,y) ASSERT_TEST(strcmp((x),(y)) == 0);
#define ASSERT_SUCCESS(x) ASSERT_EQUAL((x), MEMCACHE_SUCCESS)
#define ASSERT_NOT_NULL(x) ASSERT_TRUE(x)
#define ASSERT_NULL(x) ASSERT_TRUE(!x)

/** Functions to be used for checking blocks' status */
static bool checkBlock(void* ptr, int size, char mod, const char* const username, char* data) {
	char* block = ptr;
	block -= (sizeof(int) + 1 + 8 + 3);
	ASSERT_EQUAL(size, *(int*)block);
	block += sizeof(int);
	ASSERT_EQUAL(*block, '\0');
	block += 1;
	ASSERT_EQUAL(*block, mod);
	block += 1;
	ASSERT_EQUAL_STR(username, block);
	block += strlen(username) + 1;
	ASSERT_EQUAL(*block, '\0');
	block += 1;
	if (data != NULL)
		ASSERT_EQUAL_STR(data, block);
	return true;
}

bool memCacheExampleTest() {
	MemCache memcache;
	ASSERT_NOT_NULL(memcache = memCacheCreate());

	ASSERT_SUCCESS(memCacheAddUser(memcache, "jbond007", 1000));
	ASSERT_SUCCESS(memCacheAddUser(memcache, "j0walker", 500));

	char *ptr1 = NULL;
	ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, "jbond007", 10));

	char *ptr2 = NULL;
	ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, "j0walker", 50));

	strcpy(ptr1, "A string.");
	strcpy(ptr2, "Some other string.");

	ASSERT_SUCCESS(memCacheSetBlockMod(memcache, "jbond007", ptr1, 'G'));

	ASSERT_TRUE(checkBlock(ptr1, 10, 'G', "jbond007", "A string."));
	ASSERT_TRUE(checkBlock(ptr2, 50, 'U', "j0walker", "Some other string."));

	ASSERT_SUCCESS(memCacheTrust(memcache, "jbond007", "j0walker"));

	ASSERT_EQUAL(memCacheFree(memcache, "jbond007", ptr2),
		MEMCACHE_PERMISSION_DENIED);
	ASSERT_SUCCESS(memCacheFree(memcache, "j0walker", ptr1));

	ASSERT_NOT_NULL(memCacheGetFirstFreeBlock(memcache));
	ASSERT_NOT_NULL(memCacheGetFirstAllocatedBlock(memcache));
	memCacheDestroy(memcache);
	return true;
}


bool memCacheMyTest() {
	MemCache memcache;
	ASSERT_NOT_NULL(memcache = memCacheCreate());
	ASSERT_SUCCESS(memCacheAddUser(memcache, "Ilya1993", 300));
	ASSERT_SUCCESS(memCacheAddUser(memcache, "Ilya1994", 500));
	ASSERT_EQUAL(memCacheAddUser(memcache, "Ilya1994", 0), MEMCACHE_INVALID_ARGUMENT);
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1994", "Ilya1993"));
	ASSERT_SUCCESS(memCacheAddUser(memcache, "Ilya1995", 700));
	ASSERT_EQUAL(memCacheAddUser(memcache, "Ilya1996", 1000), MEMCACHE_SUCCESS);
	ASSERT_EQUAL(memCacheTrust(memcache, "Tereza99", "Claudia5"), MEMCACHE_USER_NOT_FOUND);
	ASSERT_EQUAL(memCacheUntrust(memcache, "Ilya1993", "Ilya1994"), MEMCACHE_SUCCESS);
	ASSERT_EQUAL(memCacheUntrust(memcache, "Ilya1993", "Ilya1994"), MEMCACHE_SUCCESS);
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1994", "Ilya1995"));
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1994", "Ilya1996"));
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1995", "Ilya1996"));
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1996", "Ilya1993"));
	char *ptr1 = NULL;
	char *ptr2 = NULL;
	char *ptr3 = NULL;
	char *ptr4 = NULL;
	char *ptr5 = NULL;
	char *ptr1_tmp = NULL;
	ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, "Ilya1993", 300));
	ASSERT_EQUAL(ptr1_tmp = memCacheAllocate(memcache, "Ilya1993", 1), NULL);
	ASSERT_NOT_NULL(ptr3 = memCacheAllocate(memcache, "Ilya1996", 255));
	ASSERT_SUCCESS(memCacheSetBlockMod(memcache, "Ilya1996", ptr3, 'G'))
		ASSERT_NOT_NULL(ptr4 = memCacheAllocate(memcache, "Ilya1996", 500));
	ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, "Ilya1995", 256));
	ASSERT_SUCCESS(memCacheSetBlockMod(memcache, "Ilya1995", ptr2, 'G'))
		ASSERT_SUCCESS(memCacheFree(memcache, "Ilya1993", ptr3));
	ASSERT_NOT_NULL(ptr5 = memCacheAllocate(memcache, "Ilya1994", 200));

	strcpy(ptr1, "Ilya1993");
	strcpy(ptr2, "Ilya1995");
	strcpy(ptr3, "Ilya1996");
	strcpy(ptr4, "Ilya19962");
	strcpy(ptr5, "Ilya1994");

	ASSERT_TRUE(checkBlock(ptr1, 300, 'U', "Ilya1993", "Ilya1993"));
	ASSERT_TRUE(checkBlock(ptr2, 256, 'G', "Ilya1995", "Ilya1995"));
	ASSERT_TRUE(checkBlock(ptr3, 255, 'G', "Ilya1996", "Ilya1996"));
	ASSERT_TRUE(checkBlock(ptr4, 500, 'U', "Ilya1996", "Ilya19962"));
	ASSERT_TRUE(checkBlock(ptr5, 200, 'U', "Ilya1994", "Ilya1994"));

	ASSERT_NOT_NULL(memCacheGetFirstFreeBlock(memcache));
	char *cur_block = NULL;
	ASSERT_SUCCESS(memCacheFree(memcache, "Ilya1996", ptr2));//
	ASSERT_NULL((cur_block = memCacheGetCurrentFreeBlock(memcache)));
	ASSERT_NOT_NULL(cur_block = memCacheGetFirstFreeBlock(memcache));
	ASSERT_NOT_NULL((cur_block = memCacheGetCurrentFreeBlock(memcache)));
	ASSERT_NOT_NULL(cur_block = memCacheGetNextFreeBlock(memcache));
	ASSERT_NOT_NULL(cur_block = memCacheGetCurrentFreeBlock(memcache));
	ASSERT_NULL(memCacheGetNextAllocatedBlock(memcache));

	ASSERT_NOT_NULL(memCacheGetFirstAllocatedBlock(memcache));
	cur_block = NULL;
	ASSERT_NOT_NULL(cur_block = memCacheGetCurrentAllocatedBlock(memcache));
	ASSERT_NOT_NULL(cur_block = memCacheGetNextAllocatedBlock(memcache));
	ASSERT_NOT_NULL(cur_block = memCacheGetCurrentAllocatedBlock(memcache));
	ASSERT_NOT_NULL(cur_block = memCacheGetNextAllocatedBlock(memcache));
	ASSERT_NOT_NULL(cur_block = memCacheGetCurrentAllocatedBlock(memcache));
	ASSERT_NULL(memCacheGetNextAllocatedBlock(memcache));
	memCacheReset(memcache);
	memCacheGetCurrentAllocatedBlock(memcache);
	memCacheGetNextAllocatedBlock(memcache);
	memCacheGetCurrentFreeBlock(memcache);
	memCacheGetNextFreeBlock(memcache);

	//Same operations after reseting memcache
	ASSERT_NOT_NULL(memcache = memCacheCreate());
	ASSERT_SUCCESS(memCacheAddUser(memcache, "Ilya1993", 300));
	ASSERT_SUCCESS(memCacheAddUser(memcache, "Ilya1994", 500));
	ASSERT_EQUAL(memCacheAddUser(memcache, "Ilya1994", 0), MEMCACHE_INVALID_ARGUMENT);
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1994", "Ilya1993"));
	ASSERT_SUCCESS(memCacheAddUser(memcache, "Ilya1995", 700));
	ASSERT_EQUAL(memCacheAddUser(memcache, "Ilya1996", 1000), MEMCACHE_SUCCESS);
	ASSERT_EQUAL(memCacheTrust(memcache, "Tereza99", "Claudia5"), MEMCACHE_USER_NOT_FOUND);
	ASSERT_EQUAL(memCacheUntrust(memcache, "Ilya1993", "Ilya1994"), MEMCACHE_SUCCESS);
	ASSERT_EQUAL(memCacheUntrust(memcache, "Ilya1993", "Ilya1994"), MEMCACHE_SUCCESS);
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1994", "Ilya1995"));
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1994", "Ilya1996"));
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1995", "Ilya1996"));
	ASSERT_SUCCESS(memCacheTrust(memcache, "Ilya1996", "Ilya1993"));
	ptr1 = NULL;
	ptr2 = NULL;
	ptr3 = NULL;
	ptr4 = NULL;
	ptr5 = NULL;
	ptr1_tmp = NULL;
	ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, "Ilya1993", 300));
	ASSERT_EQUAL(ptr1_tmp = memCacheAllocate(memcache, "Ilya1993", 1), NULL);
	ASSERT_NOT_NULL(ptr3 = memCacheAllocate(memcache, "Ilya1996", 255));
	ASSERT_SUCCESS(memCacheSetBlockMod(memcache, "Ilya1996", ptr3, 'G'))
		ASSERT_NOT_NULL(ptr4 = memCacheAllocate(memcache, "Ilya1996", 500));
	ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, "Ilya1995", 256));
	ASSERT_SUCCESS(memCacheSetBlockMod(memcache, "Ilya1995", ptr2, 'G'))
		ASSERT_SUCCESS(memCacheFree(memcache, "Ilya1993", ptr3));
	ASSERT_NOT_NULL(ptr5 = memCacheAllocate(memcache, "Ilya1994", 200));

	strcpy(ptr1, "Ilya1993");
	strcpy(ptr2, "Ilya1995");
	strcpy(ptr3, "Ilya1996");
	strcpy(ptr4, "Ilya19962");
	strcpy(ptr5, "Ilya1994");

	ASSERT_TRUE(checkBlock(ptr1, 300, 'U', "Ilya1993", "Ilya1993"));
	ASSERT_TRUE(checkBlock(ptr2, 256, 'G', "Ilya1995", "Ilya1995"));
	ASSERT_TRUE(checkBlock(ptr3, 255, 'G', "Ilya1996", "Ilya1996"));
	ASSERT_TRUE(checkBlock(ptr4, 500, 'U', "Ilya1996", "Ilya19962"));
	ASSERT_TRUE(checkBlock(ptr5, 200, 'U', "Ilya1994", "Ilya1994"));
	memCacheDestroy(memcache);
	return true;
}

int main() {
	RUN_TEST(memCacheExampleTest);
	RUN_TEST(memCacheMyTest);
	return 0;
}
