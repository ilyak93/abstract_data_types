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

#define ASSERT_TRUE(x) ASSERT_TEST(x);
#define ASSERT_EQUAL(x,y) ASSERT_TEST((x) == (y));
#define ASSERT_EQUAL_STR(x,y) ASSERT_TEST(strcmp((x),(y)) == 0);
#define ASSERT_SUCCESS(x) ASSERT_EQUAL((x), MEMCACHE_SUCCESS)
#define ASSERT_NOT_NULL(x) ASSERT_TRUE(x)
#define ASSERT_NULL(x) ASSERT_TRUE(!x)

static MemCache memcache = NULL;
static char* not_found_user = "imnotfound";
static char* james_bond = "James007";
static char* daniel = "daniel99";
static char* usernames[] = {"James007", "daniel99"};
static int usernames_num = 2;
static int memory_limit = 1000;

/** Function to be used for initializing the memcache */
static bool setUp(char** users, int num) {
  ASSERT_NOT_NULL(memcache = memCacheCreate());
  for (int i = 0; i < num; i++) {
    ASSERT_SUCCESS(memCacheAddUser(memcache, users[i], memory_limit));
  }
  return true;
}

/** Function to be called before exiting the test */
static int tearDown() {
  memCacheDestroy(memcache);
  return 0;
}

/** Functions to be used for checking blocks' status */
static bool checkBlock(void* ptr, int size, char mod, char* username, char* data) {
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

static bool blockIsAllocated(MemCache memcache, void* ptr) {
  for(void* iterator = memCacheGetFirstAllocatedBlock(memcache);
    iterator;
      iterator = memCacheGetNextAllocatedBlock(memcache)) {
    if (ptr == iterator) {
      return true;
    }
  }
  return false;
}

static bool blockIsFree(MemCache memcache, void* ptr) {
  for(void* iterator = memCacheGetFirstFreeBlock(memcache);
      iterator;
      iterator = memCacheGetNextFreeBlock(memcache)) {
    if (ptr == iterator) {
      return true;
    }
  }
  return false;
}

bool memCacheTestAddUser() {
  if(!setUp(NULL, 0))
    return false;

  ASSERT_SUCCESS(memCacheAddUser(memcache, daniel, 1000));
  ASSERT_SUCCESS(memCacheAddUser(memcache, james_bond, 1000));

  ASSERT_EQUAL(memCacheAddUser(memcache, daniel, 700),
      MEMCACHE_USERNAME_ALREADY_USED);
  ASSERT_EQUAL(memCacheAddUser(memcache, james_bond, 250),
      MEMCACHE_USERNAME_ALREADY_USED);

  ASSERT_EQUAL(memCacheAddUser(memcache, "james007", -12),
      MEMCACHE_INVALID_ARGUMENT);
  ASSERT_EQUAL(memCacheAddUser(memcache, "daniel@9", 1000),
      MEMCACHE_ILLEGAL_USERNAME);
  ASSERT_EQUAL(memCacheAddUser(memcache, "daniel", 1000),
      MEMCACHE_ILLEGAL_USERNAME);
  ASSERT_EQUAL(memCacheAddUser(memcache, "daniel1999", 1000),
      MEMCACHE_ILLEGAL_USERNAME);
  ASSERT_EQUAL(memCacheAddUser(NULL, daniel, 200),
      MEMCACHE_NULL_ARGUMENT);

  tearDown();
  return true;
}

bool memCacheTestSetBlockMod() {
  if(!setUp(usernames, usernames_num))
    return false;
  char *ptr;
  ASSERT_NOT_NULL(ptr = memCacheAllocate(memcache, james_bond, 10));
  char *str = "Not allocated block";
  ASSERT_SUCCESS(memCacheSetBlockMod(memcache, james_bond, ptr, 'G'));
  ASSERT_TRUE(checkBlock(ptr, 10, 'G', james_bond, NULL));
  ASSERT_SUCCESS(memCacheSetBlockMod(memcache, james_bond, ptr, 'A'));
  ASSERT_TRUE(checkBlock(ptr, 10, 'A', james_bond, NULL));
  ASSERT_SUCCESS(memCacheSetBlockMod(memcache, james_bond, ptr, 'U'));
  ASSERT_TRUE(checkBlock(ptr, 10, 'U', james_bond, NULL));
  ASSERT_EQUAL(memCacheSetBlockMod(NULL, james_bond, ptr, 'G'),
      MEMCACHE_NULL_ARGUMENT)
  ASSERT_EQUAL(memCacheSetBlockMod(memcache, not_found_user, ptr, 'G'),
      MEMCACHE_USER_NOT_FOUND);
  ASSERT_EQUAL(memCacheSetBlockMod(memcache, james_bond, str, 'G'),
      MEMCACHE_BLOCK_NOT_ALLOCATED);
  ASSERT_EQUAL(memCacheSetBlockMod(memcache, daniel, ptr, 'G'),
      MEMCACHE_PERMISSION_DENIED);
  ASSERT_EQUAL(memCacheSetBlockMod(memcache, james_bond, ptr, 'K'),
      MEMCACHE_INVALID_ARGUMENT);
  ASSERT_TRUE(checkBlock(ptr, 10, 'U', james_bond, NULL));

  tearDown();
  return true;
}

bool memCacheTestTrustAndUntrust() {
  if(!setUp(usernames, usernames_num))
    return false;
  ASSERT_SUCCESS(memCacheTrust(memcache, daniel, james_bond));
  ASSERT_SUCCESS(memCacheTrust(memcache, daniel, james_bond));
  ASSERT_SUCCESS(memCacheTrust(memcache, james_bond, daniel));
  ASSERT_EQUAL(memCacheTrust(NULL, james_bond, daniel),
      MEMCACHE_NULL_ARGUMENT);
  ASSERT_EQUAL(memCacheTrust(memcache, not_found_user, daniel),
      MEMCACHE_USER_NOT_FOUND);
  ASSERT_EQUAL(memCacheTrust(memcache, daniel, not_found_user),
      MEMCACHE_USER_NOT_FOUND);

  ASSERT_SUCCESS(memCacheUntrust(memcache, daniel, james_bond));
  ASSERT_SUCCESS(memCacheUntrust(memcache, daniel, james_bond));
  ASSERT_SUCCESS(memCacheUntrust(memcache, james_bond, daniel));
  ASSERT_EQUAL(memCacheUntrust(NULL, james_bond, daniel),
      MEMCACHE_NULL_ARGUMENT);
  ASSERT_EQUAL(memCacheUntrust(memcache, not_found_user, daniel),
      MEMCACHE_USER_NOT_FOUND);
  ASSERT_EQUAL(memCacheUntrust(memcache, daniel, not_found_user),
      MEMCACHE_USER_NOT_FOUND);

  tearDown();
  return true;
}

bool memCacheTestAllocateAndFree() {
  if(!setUp(usernames, usernames_num))
    return false;
  char *ptr1, *ptr2, *ptr3;

  ASSERT_NULL(memCacheGetFirstAllocatedBlock(memcache));

  ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, james_bond, 10));
  ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, daniel, 40));
  ASSERT_NOT_NULL(ptr3 = memCacheAllocate(memcache, daniel, 500));

  ASSERT_TRUE(checkBlock(ptr1, 10, 'U', james_bond, NULL));
  ASSERT_TRUE(checkBlock(ptr2, 40, 'U', daniel, NULL));
  ASSERT_TRUE(checkBlock(ptr3, 500, 'U', daniel, NULL));

  ASSERT_TRUE(blockIsAllocated(memcache, ptr1));
  ASSERT_TRUE(blockIsAllocated(memcache, ptr2));
  ASSERT_TRUE(blockIsAllocated(memcache, ptr3));

  ASSERT_NULL(memCacheGetFirstFreeBlock(memcache));

  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr1));
  ASSERT_EQUAL(ptr1, memCacheGetFirstFreeBlock(memcache));

  ASSERT_SUCCESS(memCacheFree(memcache, daniel, ptr2));
  ASSERT_SUCCESS(memCacheFree(memcache, daniel, ptr3));

  ASSERT_EQUAL(ptr1, memCacheGetFirstFreeBlock(memcache));
  ASSERT_EQUAL(ptr2, memCacheGetNextFreeBlock(memcache));
  ASSERT_NULL(memCacheGetNextFreeBlock(memcache));

  tearDown();
  return true;
}

bool memCacheTestAllocateAndFreeFailure() {
  if(!setUp(usernames, usernames_num))
    return false;
  char *ptr1 = NULL;

  ptr1 = memCacheAllocate(NULL, james_bond, 10);
  ASSERT_NULL(ptr1);
  ptr1 = memCacheAllocate(memcache, james_bond, -10);
  ASSERT_NULL(ptr1);
  ptr1 = memCacheAllocate(memcache, james_bond, 0);
  ASSERT_NULL(ptr1);
  ptr1 = memCacheAllocate(memcache, not_found_user, 10);
  ASSERT_NULL(ptr1);
  ptr1 = memCacheAllocate(memcache, daniel, memory_limit + 10);
  ASSERT_NULL(ptr1);

  ASSERT_NULL(memCacheGetFirstAllocatedBlock(memcache));
  ASSERT_NULL(memCacheGetFirstFreeBlock(memcache));

  ASSERT_EQUAL(memCacheFree(NULL, daniel, ptr1), MEMCACHE_NULL_ARGUMENT);
  ASSERT_EQUAL(memCacheFree(memcache, not_found_user, ptr1),
      MEMCACHE_USER_NOT_FOUND);
  ASSERT_EQUAL(memCacheFree(memcache, daniel, ptr1),
      MEMCACHE_BLOCK_NOT_ALLOCATED);

  ASSERT_NULL(memCacheGetFirstAllocatedBlock(memcache));
  ASSERT_NULL(memCacheGetFirstFreeBlock(memcache));

  tearDown();
  return true;
}

bool memCacheTestAllocateAndFreeLargeBlocks() {
  if(!setUp(usernames, usernames_num))
    return false;
  char *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL;

  ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, daniel, 500));
  ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, daniel, 257));
  ASSERT_NOT_NULL(ptr3 = memCacheAllocate(memcache, james_bond, 500));

  ASSERT_TRUE(checkBlock(ptr1, 500, 'U', daniel, NULL));
  ASSERT_TRUE(checkBlock(ptr2, 257, 'U', daniel, NULL));
  ASSERT_TRUE(checkBlock(ptr3, 500, 'U', james_bond, NULL));

  ASSERT_TRUE(blockIsAllocated(memcache, ptr1));
  ASSERT_TRUE(blockIsAllocated(memcache, ptr2));
  ASSERT_TRUE(blockIsAllocated(memcache, ptr3));

  ASSERT_SUCCESS(memCacheFree(memcache, daniel, ptr1));
  ASSERT_SUCCESS(memCacheFree(memcache, daniel, ptr2));
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr3));

  ASSERT_NULL(memCacheGetFirstFreeBlock(memcache));

  tearDown();
  return true;
}

bool memCacheTestAllocateAndFreeSameSize() {
  if(!setUp(usernames, usernames_num))
    return false;
  char *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL;

  ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, james_bond, 10));
  ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, daniel, 10));

  ASSERT_SUCCESS(memCacheSetBlockMod(memcache, james_bond, ptr1, 'A'));

  ASSERT_TRUE(checkBlock(ptr1, 10, 'A', james_bond, NULL));
  ASSERT_TRUE(checkBlock(ptr2, 10, 'U', daniel, NULL));

  ptr3 = ptr1;
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr1));
  ASSERT_EQUAL(ptr3, memCacheGetFirstFreeBlock(memcache));

  ASSERT_NOT_NULL(ptr4 = memCacheAllocate(memcache, daniel, 10));
  ASSERT_EQUAL(ptr3, ptr4);
  ASSERT_TRUE(checkBlock(ptr4, 10, 'U', daniel, NULL));
  ASSERT_NULL(memCacheGetFirstFreeBlock(memcache));

  tearDown();
  return true;
}

bool memCacheTestPermissions() {
  if(!setUp(usernames, usernames_num))
    return false;
  char *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL;

  ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, james_bond, 10));
  ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, daniel, 40));
  ASSERT_NOT_NULL(ptr3 = memCacheAllocate(memcache, james_bond, 70));
  ASSERT_NOT_NULL(ptr4 = memCacheAllocate(memcache, daniel, 500));

  ASSERT_TRUE(checkBlock(ptr1, 10, 'U', james_bond, NULL));
  ASSERT_TRUE(checkBlock(ptr2, 40, 'U', daniel, NULL));
  ASSERT_TRUE(checkBlock(ptr3, 70, 'U', james_bond, NULL));
  ASSERT_TRUE(checkBlock(ptr4, 500, 'U', daniel, NULL));

  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr1));
  ASSERT_EQUAL(memCacheFree(memcache, james_bond, ptr2),
      MEMCACHE_PERMISSION_DENIED);
  ASSERT_EQUAL(memCacheFree(memcache, james_bond, ptr4),
      MEMCACHE_PERMISSION_DENIED);
  ASSERT_EQUAL(memCacheFree(memcache, daniel, ptr3),
      MEMCACHE_PERMISSION_DENIED);

  ASSERT_SUCCESS(memCacheSetBlockMod(memcache, daniel, ptr4, 'G'));
  ASSERT_EQUAL(memCacheFree(memcache, james_bond, ptr4),
      MEMCACHE_PERMISSION_DENIED);

  ASSERT_SUCCESS(memCacheTrust(memcache, daniel, james_bond));
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr4));

  ASSERT_EQUAL(memCacheFree(memcache, james_bond, ptr2),
      MEMCACHE_PERMISSION_DENIED);

  ASSERT_SUCCESS(memCacheSetBlockMod(memcache, daniel, ptr2, 'A'));
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr2));

  ASSERT_EQUAL(memCacheFree(memcache, daniel, ptr3),
      MEMCACHE_PERMISSION_DENIED);
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr3));

  tearDown();
  return true;
}

bool memCacheTestReset() {
  if(!setUp(usernames, usernames_num))
    return false;
  char *ptr1, *ptr2;
  ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, james_bond, 10));
  ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, james_bond, 10));
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr1));
  ASSERT_TRUE(blockIsAllocated(memcache, ptr2));
  ASSERT_TRUE(blockIsFree(memcache, ptr1));
  ASSERT_EQUAL(memCacheReset(NULL), MEMCACHE_NULL_ARGUMENT);
  ASSERT_SUCCESS(memCacheReset(memcache));
  ASSERT_TRUE(!blockIsAllocated(memcache, ptr2));
  ASSERT_TRUE(!blockIsFree(memcache, ptr1));
  tearDown();
  return true;
}

bool memCacheTestIteration() {
  if(!setUp(usernames, usernames_num))
    return false;

  char *ptr1, *ptr2, *ptr3;
  int ptr_visited[3] = {0};

  ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, james_bond, 10));
  ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, james_bond, 20));
  ASSERT_NOT_NULL(ptr3 = memCacheAllocate(memcache, james_bond, 30));

  for (void* iterator = memCacheGetFirstAllocatedBlock(memcache);
      iterator;
      iterator = memCacheGetNextAllocatedBlock(memcache)) {
    if (iterator == ptr1) {
      ASSERT_TRUE(!ptr_visited[0]);
      ptr_visited[0] = 1;
    } else if (iterator == ptr2) {
      ASSERT_TRUE(!ptr_visited[1]);
      ptr_visited[1] = 1;
    } else if (iterator == ptr3) {
      ASSERT_TRUE(!ptr_visited[2]);
      ptr_visited[2] = 1;
    } else {
      ASSERT_TRUE(0);
    }
  }
  ASSERT_TRUE(ptr_visited[0] && ptr_visited[1] && ptr_visited[2]);

  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr1));
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr2));
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr3));

  for (void* iterator = memCacheGetFirstFreeBlock(memcache);
      iterator;
      iterator = memCacheGetNextFreeBlock(memcache)) {
    if (iterator == ptr1) {
      ASSERT_TRUE(ptr_visited[0]);
      ptr_visited[0] = 0;
    } else if (iterator == ptr2) {
      ASSERT_TRUE(ptr_visited[1]);
      ptr_visited[1] = 0;
    } else if (iterator == ptr3) {
      ASSERT_TRUE(ptr_visited[2]);
      ptr_visited[2] = 0;
    } else {
      ASSERT_TRUE(0);
    }
  }
  ASSERT_TRUE(!ptr_visited[0] && !ptr_visited[1] && !ptr_visited[2]);

  tearDown();
  return true;
}

bool memCacheTestIterationSparse() {
  if(!setUp(usernames, usernames_num))
    return false;

  char *ptr1, *ptr2, *ptr3;

  ASSERT_NOT_NULL(ptr1 = memCacheAllocate(memcache, james_bond, 2));
  ASSERT_NOT_NULL(ptr2 = memCacheAllocate(memcache, james_bond, 100));
  ASSERT_NOT_NULL(ptr3 = memCacheAllocate(memcache, james_bond, 237));

  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr1));
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr2));
  ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr3));

  ASSERT_EQUAL(ptr1, memCacheGetFirstFreeBlock(memcache));
  ASSERT_EQUAL(ptr2, memCacheGetNextFreeBlock(memcache));
  ASSERT_EQUAL(ptr3, memCacheGetNextFreeBlock(memcache));
  ASSERT_NULL(memCacheGetNextFreeBlock(memcache));

  tearDown();
  return true;
}

bool memCacheTestIterationDense() {
  if(!setUp(usernames, usernames_num))
    return false;

  char *ptr[9];
  bool visited_size_2[5] = {0};
  bool visited_size_3[2] = {0};
  bool visited_size_110[2] = {0};

  ASSERT_NOT_NULL(ptr[0] = memCacheAllocate(memcache, james_bond, 2));
  ASSERT_NOT_NULL(ptr[1] = memCacheAllocate(memcache, james_bond, 2));
  ASSERT_NOT_NULL(ptr[2] = memCacheAllocate(memcache, james_bond, 2));
  ASSERT_NOT_NULL(ptr[3] = memCacheAllocate(memcache, james_bond, 2));
  ASSERT_NOT_NULL(ptr[4] = memCacheAllocate(memcache, james_bond, 2));
  ASSERT_NOT_NULL(ptr[5] = memCacheAllocate(memcache, james_bond, 3));
  ASSERT_NOT_NULL(ptr[6] = memCacheAllocate(memcache, james_bond, 3));
  ASSERT_NOT_NULL(ptr[7] = memCacheAllocate(memcache, james_bond, 110));
  ASSERT_NOT_NULL(ptr[8] = memCacheAllocate(memcache, james_bond, 110));

  for (int i = 0; i < 9; i++)
    ASSERT_SUCCESS(memCacheFree(memcache, james_bond, ptr[i]));

  char* current = memCacheGetFirstFreeBlock(memcache);
  for (int i = 0; i < 5; i++)
    if (current == ptr[i])
      visited_size_2[i] = true;
  for (int j = 0; j < 4; j++) {
    current = memCacheGetNextFreeBlock(memcache);
    for (int i = 0; i < 5; i++)
      if (current == ptr[i])
          visited_size_2[i] = true;
  }
  for (int i = 0; i < 5; i++)
    ASSERT_TRUE(visited_size_2[i]);

  for (int j = 0; j < 2; j++) {
    current = memCacheGetNextFreeBlock(memcache);
    for (int i = 5; i < 7; i++)
      if (current == ptr[i])
          visited_size_3[i - 5] = true;
  }
  for (int i = 0; i < 2; i++)
    ASSERT_TRUE(visited_size_3[i]);

  for (int j = 0; j < 2; j++) {
    current = memCacheGetNextFreeBlock(memcache);
    for (int i = 7; i < 9; i++)
      if (current == ptr[i])
          visited_size_110[i - 7] = true;
  }
  for (int i = 0; i < 2; i++)
    ASSERT_TRUE(visited_size_110[i]);

  ASSERT_NULL(memCacheGetNextFreeBlock(memcache));

  tearDown();
  return true;
}

bool memCacheTestIterationEmpty() {
  if(!setUp(usernames, usernames_num))
    return false;

  ASSERT_NULL(memCacheGetFirstAllocatedBlock(memcache));
  ASSERT_NULL(memCacheGetFirstFreeBlock(memcache));

  ASSERT_NULL(memCacheGetNextAllocatedBlock(memcache));
  ASSERT_NULL(memCacheGetNextFreeBlock(memcache));

  tearDown();
  return true;
}

bool memCacheTests(int num)
{
	switch(num)
	{
	case 0:
	RUN_TEST(memCacheTestAddUser);
	RUN_TEST(memCacheTestSetBlockMod);
	RUN_TEST(memCacheTestTrustAndUntrust);
	RUN_TEST(memCacheTestAllocateAndFree);
	RUN_TEST(memCacheTestAllocateAndFreeFailure);
	RUN_TEST(memCacheTestAllocateAndFreeLargeBlocks);
	RUN_TEST(memCacheTestAllocateAndFreeSameSize);
	RUN_TEST(memCacheTestPermissions);
	RUN_TEST(memCacheTestReset);
	RUN_TEST(memCacheTestIteration);
	RUN_TEST(memCacheTestIterationSparse);
	RUN_TEST(memCacheTestIterationDense);
	RUN_TEST(memCacheTestIterationEmpty);
	break;
	case 19:
		RUN_TEST(memCacheTestAddUser);
		break;
	case 20:
		RUN_TEST(memCacheTestSetBlockMod);
		break;
	case 21:
		RUN_TEST(memCacheTestTrustAndUntrust);
		break;
	case 22:
		RUN_TEST(memCacheTestAllocateAndFree);
		break;
	case 23:
		RUN_TEST(memCacheTestAllocateAndFreeFailure);
		break;
	case 24:
		RUN_TEST(memCacheTestAllocateAndFreeLargeBlocks);
		break;
	case 25:
		RUN_TEST(memCacheTestAllocateAndFreeSameSize);
		break;
	case 26:
		RUN_TEST(memCacheTestPermissions);
		break;
	case 27:
		RUN_TEST(memCacheTestReset);
		break;
	case 28:
		RUN_TEST(memCacheTestIteration);
		break;
	case 29:
		RUN_TEST(memCacheTestIterationSparse);
		break;
	case 30:
		RUN_TEST(memCacheTestIterationDense);
		break;
	case 31:
		RUN_TEST(memCacheTestIterationEmpty);
		break;
	default:
		printf("ERROR in memcache test\n");
		return false;
		break;
	}
	return true;
}

