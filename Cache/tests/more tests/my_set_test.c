#include "test_utilities.h"
#include <stdlib.h>
#include "my_set/my_set.h"
#include <string.h>

#define ASSERT_TRUE(x) ASSERT_TEST(x);
#define ASSERT_EQUAL(x,y) ASSERT_TEST((x) == (y));
#define ASSERT_EQUAL_STR(x,y) ASSERT_TEST(strcmp((x),(y)) == 0);
#define ASSERT_SUCCESS(x) ASSERT_EQUAL((x), MY_SET_SUCCESS)
#define ASSERT_NOT_NULL(x) ASSERT_TRUE(x)
#define ASSERT_NULL(x) ASSERT_TRUE(!x)

static MySet my_set;
static char* strings[7] = {
    "Europe",
    "Asia",
    "North America",
    "South America",
    "Australia",
    "Africa",
    "Antarctica"
};
static char* sorted_strings[7] = {
    "Africa",
    "Antarctica",
    "Asia",
    "Australia",
    "Europe",
    "North America",
    "South America",
};
static int strings_num = 7;

/** Function to be used for copying an int into the set */
static MySetElement copyString(MySetElement str) {
	if (!str) {
		return NULL;
	}
	char* copy = malloc(strlen(str) + 1);
	if (!copy) {
		return NULL;
	}
	return strcpy(copy, str);
}

/** Function to be used by the set for freeing elements */
static void freeString(MySetElement str) {
	free(str);
}

/** Function for comparing to elements in the set */
static int compareStrings(MySetElement element1, MySetElement element2) {
	return strcmp(element1, element2);
}

static bool longerThanSix(MySetElement element) {
  return strlen(element) > 6;
}

bool setUpMySet(char* str[], int num) {
  my_set = mySetCreate(copyString, freeString, compareStrings);
  if (my_set == NULL) {
    return false;
  }
  for (int i = 0; i < num; i++) {
    ASSERT_SUCCESS(mySetAdd(my_set, str[i]));
  }
  return true;
}

void tearDownMySet() {
  mySetDestroy(my_set);
}

bool testMySetCopySuccess() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));
  MySet copy = mySetCopy(my_set);
  ASSERT_EQUAL(mySetGetSize(copy), mySetGetSize(my_set));
  MY_SET_FOREACH(char*, string, my_set) {
    ASSERT_TRUE(mySetIsIn(copy, string));
  }
  MY_SET_FOREACH(char*, string, copy) {
    ASSERT_TRUE(mySetIsIn(my_set, string));
  }
  ASSERT_SUCCESS(mySetRemove(my_set, "Africa"));
  ASSERT_TRUE(!mySetIsIn(my_set, "Africa"));
  ASSERT_TRUE(mySetIsIn(copy, "Africa"));

  ASSERT_NOT_NULL(copy);
  mySetDestroy(copy);
  tearDownMySet();
  return true;
}

bool testMySetCopyNull() {
  ASSERT_NULL(mySetCopy(NULL));
  return true;
}

bool testMySetGetSize() {
  ASSERT_TRUE(setUpMySet(NULL, 0));
  ASSERT_EQUAL(mySetGetSize(my_set), 0);
  tearDownMySet();

  ASSERT_TRUE(setUpMySet(strings, strings_num));
  ASSERT_EQUAL(mySetGetSize(my_set), 7);
  tearDownMySet();

  ASSERT_EQUAL(mySetGetSize(NULL), -1);

  return true;
}

bool testMySetIsIn() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));
  ASSERT_TRUE(!mySetIsIn(my_set, NULL));
  ASSERT_TRUE(!mySetIsIn(NULL, "string"));
  for (int i = 0; i < strings_num; i++) {
    ASSERT_TRUE(mySetIsIn(my_set, strings[i]));
    ASSERT_SUCCESS(mySetRemove(my_set, strings[i]));
    ASSERT_TRUE(!mySetIsIn(my_set, strings[i]));
  }
  tearDownMySet();
  return true;
}

bool testMySetRemoveSuccess() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));

  for (int i = 0; i < strings_num; i++) {
    ASSERT_SUCCESS(mySetRemove(my_set, strings[i]));
    ASSERT_TRUE(!mySetIsIn(my_set, strings[i]));
  }
  ASSERT_EQUAL(mySetGetSize(my_set), 0);

  tearDownMySet();
  return true;
}

bool testMySetAddAndRemoveFailure() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));

  ASSERT_EQUAL(mySetAdd(NULL, "string"), MY_SET_NULL_ARGUMENT);
  ASSERT_EQUAL(mySetRemove(NULL, "string"), MY_SET_NULL_ARGUMENT);
  ASSERT_EQUAL(mySetAdd(my_set, NULL), MY_SET_NULL_ARGUMENT);
  ASSERT_EQUAL(mySetRemove(my_set, NULL), MY_SET_NULL_ARGUMENT);

  ASSERT_EQUAL(mySetAdd(my_set, "Asia"), MY_SET_ITEM_ALREADY_EXISTS);
  ASSERT_EQUAL(mySetRemove(my_set, "string"), MY_SET_ITEM_DOES_NOT_EXIST);

  tearDownMySet();
  return true;
}

bool testMySetExtractSuccess() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));

  for (int i = 0; i < strings_num; i++) {
    char* extracted_string = NULL;
    extracted_string = mySetExtract(my_set, strings[i]);
    ASSERT_NOT_NULL(extracted_string);
    ASSERT_EQUAL_STR(extracted_string, strings[i]);
    ASSERT_TRUE(!mySetIsIn(my_set, strings[i]));
    freeString(extracted_string);
  }
  ASSERT_EQUAL(mySetGetSize(my_set), 0);

  tearDownMySet();
  return true;
}

bool testMySetExtractFailure() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));

  ASSERT_EQUAL(mySetExtract(my_set, "string"), NULL);
  ASSERT_EQUAL(mySetExtract(NULL, "string"), NULL);
  ASSERT_EQUAL(mySetExtract(my_set, NULL), NULL);

  tearDownMySet();
  return true;
}

bool testMySetFilter() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));

  MySet filtered = mySetFilter(my_set, longerThanSix);
  ASSERT_NOT_NULL(filtered);
  ASSERT_TRUE(filtered != my_set);

  MY_SET_FOREACH(char*, string, my_set) {
    if (longerThanSix(string)) {
      ASSERT_TRUE(mySetIsIn(filtered, string));
    } else {
      ASSERT_TRUE(!mySetIsIn(filtered, string));
    }
  }

  mySetDestroy(filtered);
  tearDownMySet();
  return true;
}

bool testMySetIteration() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));
  int i = 0;
  MY_SET_FOREACH(char*, string, my_set) {
    ASSERT_EQUAL(string, mySetGetCurrent(my_set));
    ASSERT_EQUAL_STR(string, sorted_strings[i]);
    i++;
  }
  ASSERT_EQUAL(i, 7);
  ASSERT_NULL(mySetGetCurrent(my_set));

  tearDownMySet();
  return true;
}

bool testMySetIterationEmpty() {
  ASSERT_TRUE(setUpMySet(NULL, 0));
  ASSERT_NULL(mySetGetFirst(my_set));
  ASSERT_NULL(mySetGetCurrent(my_set));
  ASSERT_NULL(mySetGetNext(my_set));

  tearDownMySet();
  return true;
}

bool testMySetClear() {
  ASSERT_TRUE(setUpMySet(strings, strings_num));
  ASSERT_SUCCESS(mySetClear(my_set));
  ASSERT_EQUAL(mySetGetSize(my_set), 0);
  ASSERT_EQUAL(mySetClear(NULL), MY_SET_NULL_ARGUMENT);
  tearDownMySet();
  return true;
}

bool mySetTests(int num)
{
	switch(num)
	{
	case 0:
		RUN_TEST(testMySetAddAndRemoveFailure);
		RUN_TEST(testMySetRemoveSuccess);
		RUN_TEST(testMySetClear);
		RUN_TEST(testMySetCopyNull);
		RUN_TEST(testMySetCopySuccess);
		RUN_TEST(testMySetExtractFailure);
		RUN_TEST(testMySetExtractSuccess);
		RUN_TEST(testMySetFilter);
		RUN_TEST(testMySetGetSize);
		RUN_TEST(testMySetIsIn);
		RUN_TEST(testMySetIteration);
		RUN_TEST(testMySetIterationEmpty);
		break;
	case 1:
		RUN_TEST(testMySetAddAndRemoveFailure);
		break;
	case 2:
		RUN_TEST(testMySetRemoveSuccess);
		break;
	case 3:
		RUN_TEST(testMySetClear);
		break;
	case 4:
		RUN_TEST(testMySetCopyNull);
		break;
	case 5:
		RUN_TEST(testMySetCopySuccess);
		break;
	case 6:
		RUN_TEST(testMySetExtractFailure);
		break;
	case 7:
		RUN_TEST(testMySetExtractSuccess);
		break;
	case 8:
		RUN_TEST(testMySetFilter);
		break;
	case 9:
		RUN_TEST(testMySetGetSize);
		break;
	case 10:
		RUN_TEST(testMySetIsIn);
		break;
	case 11:
		RUN_TEST(testMySetIteration);
		break;
	case 12:
		RUN_TEST(testMySetIterationEmpty);
		break;
	default:
		printf("ERROR in my_set test\n");
		return false;
		break;
	}
	return true;
}

