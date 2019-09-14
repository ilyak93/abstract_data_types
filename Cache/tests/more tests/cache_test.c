#include "test_utilities.h"
#include <stdlib.h>
#include "cache.h"
#include <string.h>

#define ASSERT_TRUE(x) ASSERT_TEST(x);
#define ASSERT_EQUAL(x,y) ASSERT_TEST((x) == (y));
#define ASSERT_EQUAL_STR(x,y) ASSERT_TEST(strcmp((x),(y)) == 0);
#define ASSERT_SUCCESS(x) ASSERT_EQUAL((x), CACHE_SUCCESS)
#define ASSERT_NOT_NULL(x) ASSERT_TRUE(x)
#define ASSERT_NULL(x) ASSERT_TRUE(!x)

#define LETTERS_NUM 26

static Cache cache = NULL;

static char* dollar_sign = "$tring";
static char* five = "5tring";

static char* australia = "australia";
static char* austria = "Austria";
static char* argentina = "argentina";
static char* japan = "Japan";
static char* jordan = "jordan";
static char* south_africa = "South africa";
static char* south_korea = "south korea";
static char* zimbabwe = "Zimbabwe";

static char* all_countries[] = {"australia",
                                "Austria",
                                "argentina",
                                "Japan",
                                "jordan",
                                "South africa",
                                "south korea",
                                "Zimbabwe"};

/**
* Tests the generic cache ADT by creating and manipulating
* a cache of strings with a container of size 256, where
* the i'th cell in the container contains all strings
* that start with the i'th letter in the alphabet.
*/

/** Function to be used for copying a string into the cache */
static CacheElement copyString(CacheElement str) {
  if (!str) {
    return NULL;
  }
  char* copy = malloc(strlen(str) + 1);
  if (!copy) {
    return NULL;
  }
  return strcpy(copy, str);
}

/** Function to be used by the cache for freeing elements */
static void freeString(CacheElement str) {
  free(str);
}

/** Function for comparing two elements in the cache */
static int compareStrings(CacheElement element1, CacheElement element2) {
  return strcmp(element1, element2);
}

/** Function to compute the key for an element in the cache */
static int getFirstLetter(CacheElement element) {
  char first_char = *(char*)element;
  if (first_char <= 'Z' && first_char >= 'A')
    return first_char - 'A';
  else
    return first_char - 'a';
}

bool setUpCache(char* strings[], int num) {
  cache = cacheCreate(LETTERS_NUM,
                      freeString,
                      copyString,
                      compareStrings,
                      getFirstLetter);
  ASSERT_NOT_NULL(cache);

  for (int i = 0; i < num; i++) {
    ASSERT_SUCCESS(cachePush(cache, strings[i]));
  }
  return true;
}

void tearDownCache() {
  cacheDestroy(cache);
}

bool cacheTestPushAndFree() {
  if (!setUpCache(NULL, 0))
    return false;
  ASSERT_SUCCESS(cachePush(cache, australia));
  ASSERT_TRUE(cacheIsIn(cache, australia));
  ASSERT_SUCCESS(cachePush(cache, austria));
  ASSERT_TRUE(cacheIsIn(cache, austria));
  ASSERT_SUCCESS(cachePush(cache, argentina));
  ASSERT_TRUE(cacheIsIn(cache, argentina));

  ASSERT_EQUAL(cachePush(cache, austria), CACHE_ITEM_ALREADY_EXISTS);

  ASSERT_SUCCESS(cachePush(cache, japan));
  ASSERT_TRUE(cacheIsIn(cache, japan));
  ASSERT_SUCCESS(cachePush(cache, jordan));
  ASSERT_TRUE(cacheIsIn(cache, jordan));

  ASSERT_SUCCESS(cachePush(cache, south_africa));
  ASSERT_TRUE(cacheIsIn(cache, south_africa));
  ASSERT_SUCCESS(cachePush(cache, south_korea));
  ASSERT_TRUE(cacheIsIn(cache, south_korea));

  ASSERT_SUCCESS(cachePush(cache, zimbabwe));
  ASSERT_TRUE(cacheIsIn(cache, zimbabwe));

  ASSERT_EQUAL(cachePush(cache, zimbabwe), CACHE_ITEM_ALREADY_EXISTS);

  ASSERT_SUCCESS(cacheFreeElement(cache, australia));
  ASSERT_TRUE(!cacheIsIn(cache, australia));
  ASSERT_SUCCESS(cacheFreeElement(cache, austria));
  ASSERT_TRUE(!cacheIsIn(cache, austria));
  ASSERT_SUCCESS(cacheFreeElement(cache, argentina));
  ASSERT_TRUE(!cacheIsIn(cache, argentina));

  ASSERT_EQUAL(cacheFreeElement(cache, austria), CACHE_ITEM_DOES_NOT_EXIST);

  ASSERT_SUCCESS(cacheFreeElement(cache, japan));
  ASSERT_TRUE(!cacheIsIn(cache, japan));
  ASSERT_SUCCESS(cacheFreeElement(cache, jordan));
  ASSERT_TRUE(!cacheIsIn(cache, jordan));

  ASSERT_SUCCESS(cacheFreeElement(cache, south_africa));
  ASSERT_TRUE(!cacheIsIn(cache, south_africa));
  ASSERT_SUCCESS(cacheFreeElement(cache, south_korea));
  ASSERT_TRUE(!cacheIsIn(cache, south_korea));

  ASSERT_SUCCESS(cacheFreeElement(cache, zimbabwe));
  ASSERT_TRUE(!cacheIsIn(cache, zimbabwe));

  ASSERT_EQUAL(cacheFreeElement(cache, zimbabwe), CACHE_ITEM_DOES_NOT_EXIST);

  cacheDestroy(cache);
  return true;
}

bool cacheTestPushFailure() {
  if (!setUpCache(NULL, 0))
    return false;

  ASSERT_EQUAL(cachePush(NULL, australia), CACHE_NULL_ARGUMENT);

  ASSERT_EQUAL(cachePush(cache, five), CACHE_OUT_OF_RANGE);
  ASSERT_EQUAL(cachePush(cache, dollar_sign), CACHE_OUT_OF_RANGE);

  tearDownCache();
  return true;
}

bool cacheTestFreeElementFailure() {
  if (!setUpCache(NULL, 0))
    return false;

  ASSERT_EQUAL(cacheFreeElement(NULL, australia), CACHE_NULL_ARGUMENT);

  ASSERT_TRUE(
        cacheFreeElement(cache, dollar_sign) == CACHE_ITEM_DOES_NOT_EXIST);

  tearDownCache();
  return true;
}

bool cacheTestExtractElementByKey() {
  if (!setUpCache(all_countries, 8))
    return false;

  char* starts_with_a;
  bool visited_australia = false;
  bool visited_austria = false;
  bool visited_argentina = false;
  while ((starts_with_a = cacheExtractElementByKey(cache, 'a' - 'a'))
          != NULL) {
    if (strcmp(starts_with_a, australia) == 0) {
      ASSERT_TRUE(!visited_australia);
      ASSERT_TRUE(!cacheIsIn(cache, australia));
      visited_australia = true;
    } else if (strcmp(starts_with_a, austria) == 0) {
      ASSERT_TRUE(!visited_austria);
      ASSERT_TRUE(!cacheIsIn(cache, austria));
      visited_austria = true;
    } else if (strcmp(starts_with_a, argentina) == 0) {
      ASSERT_TRUE(!visited_argentina);
      ASSERT_TRUE(!cacheIsIn(cache, argentina));
      visited_argentina = true;
    } else
      ASSERT_TRUE(0);
    freeString(starts_with_a);
  }
  ASSERT_TRUE(visited_australia);
  ASSERT_TRUE(visited_austria);
  ASSERT_TRUE(visited_argentina);

  ASSERT_NULL(cacheExtractElementByKey(cache, 'a' - 'a'));
  char* starts_with_z = cacheExtractElementByKey(cache, 'z' - 'a');
  ASSERT_EQUAL_STR(zimbabwe, starts_with_z);
  freeString(starts_with_z);
  tearDownCache();
  return true;
}

bool cacheTestExtractElementByKeyFailure() {
  if (!setUpCache(NULL, 0))
    return false;

  ASSERT_NULL(cacheExtractElementByKey(NULL, 0));

  ASSERT_NULL(cacheExtractElementByKey(cache, 50));
  ASSERT_NULL(cacheExtractElementByKey(cache, -12));
  ASSERT_NULL(cacheExtractElementByKey(cache, 26));

  tearDownCache();
  return true;
}

bool cacheTestClear() {
  if (!setUpCache(NULL, 0))
    return false;

  ASSERT_EQUAL(cacheClear(NULL), CACHE_NULL_ARGUMENT);

  ASSERT_SUCCESS(cachePush(cache, australia));

  ASSERT_SUCCESS(cacheClear(cache));
  ASSERT_TRUE(!cacheIsIn(cache, australia));

  ASSERT_SUCCESS(cacheClear(cache));

  tearDownCache();
  return true;
}

bool cacheTests(int num)
{
	switch(num)
	{
	case 0:
		RUN_TEST(cacheTestPushAndFree);
		RUN_TEST(cacheTestPushFailure);
		RUN_TEST(cacheTestFreeElementFailure);
		RUN_TEST(cacheTestExtractElementByKey);
		RUN_TEST(cacheTestExtractElementByKeyFailure);
		RUN_TEST(cacheTestClear);
		break;
	case 13:
		RUN_TEST(cacheTestPushAndFree);
		break;
	case 14:
		RUN_TEST(cacheTestPushFailure);
		break;
	case 15:
		RUN_TEST(cacheTestFreeElementFailure);
		break;
	case 16:
		RUN_TEST(cacheTestExtractElementByKey);
		break;
	case 17:
		RUN_TEST(cacheTestExtractElementByKeyFailure);
		break;
	case 18:
		RUN_TEST(cacheTestClear);
		break;
	default:
		printf("ERROR in cache test\n");
		return false;
		break;
	}
	return true;
}

