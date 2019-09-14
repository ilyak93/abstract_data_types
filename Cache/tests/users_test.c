#include "test_utilities.h"
#include <stdlib.h>
#include <string.h>
#include "users.h"

#define ASSERT_TRUE(x) ASSERT_TEST(x);
#define ASSERT_EQUAL(x,y) ASSERT_TEST((x) == (y));
#define ASSERT_EQUAL_STR(x,y) ASSERT_TEST(strcmp((x),(y)) == 0);
#define ASSERT_SUCCESS(x) ASSERT_EQUAL((x), USER_SUCCESS)
#define ASSERT_NOT_NULL(x) ASSERT_TRUE(x)
#define ASSERT_NULL(x) ASSERT_TRUE(!x)

bool usersExampleTest() {
	Users users = NULL;;
	ASSERT_NOT_NULL(users = usersCreate());

	ASSERT_SUCCESS(addUser(users, "jbond007", 1000));
	ASSERT_SUCCESS(addUser(users, "j0walker", 500));
	ASSERT_TRUE(userIsIn(users, "jbond007"));
	ASSERT_TRUE(userIsIn(users, "j0walker"));
	ASSERT_TRUE(!strcmp(readUserName(usersGetFirst(users)),"j0walker"));
	ASSERT_TRUE(!strcmp(readUserName(usersGetNext(users)),"jbond007"));
	ASSERT_SUCCESS(removeUser(users, "j0walker"));
	ASSERT_TRUE(!userIsIn(users, "j0walker"));
	ASSERT_TRUE(!strcmp(readUserName(usersGetFirst(users)),"jbond007"));
	ASSERT_NULL(usersGetNext(users));
	ASSERT_SUCCESS(addUser(users, "j0walker", 500));

	ASSERT_EQUAL(userGetCurrentMemUse(users, "j0walker"),0);
	ASSERT_EQUAL(userGetLimitMemUse(users, "j0walker"),500);
	userChangeMemUse(users, "j0walker", 200, INCREASE);
	ASSERT_EQUAL(userGetCurrentMemUse(users, "j0walker"),200);
	ASSERT_EQUAL(userGetLimitMemUse(users, "j0walker"),500);
	userChangeMemUse(users, "j0walker", 150,REDUCE);
	ASSERT_EQUAL(userGetCurrentMemUse(users, "j0walker"),50);
	ASSERT_EQUAL(userGetLimitMemUse(users, "j0walker"),500);

	ASSERT_SUCCESS(userSetRelation(users, "jbond007", "j0walker", TRUST));
	ASSERT_EQUAL(userSetRelation(users, "j0walker", "Alex9999", TRUST), USER_USERNAME_NOT_FOUND);
	ASSERT_SUCCESS(userSetRelation(users, "j0walker", "jbond007", UNTRUST));
	ASSERT_SUCCESS(userSetRelation(users, "j0walker", "jbond007", TRUST));
	ASSERT_SUCCESS(userSetRelation(users, "j0walker", "jbond007", UNTRUST));
	
	usersClear(users);
	ASSERT_TRUE(!userIsIn(users, "jbond007"));
	ASSERT_NULL(usersGetFirst(users));
	ASSERT_SUCCESS(addUser(users, "jbond007", 1000));
	ASSERT_TRUE(userIsIn(users, "jbond007"));
	ASSERT_TRUE(!strcmp(readUserName(usersGetFirst(users)),"jbond007"));

	usersDestroy(users);
	return true;
}

int main() {
	RUN_TEST(usersExampleTest);
	return 0;
}
