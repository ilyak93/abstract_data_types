#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "users.h"
#define USERNAME_LENGTH 9

/** Type for defining the map */
struct user_data_t {
	char user[9];
	int memory_limit;
	int cur_memory;
	Set trusted_users;
};

/* checks validity of a username*/
static bool userNameIsValid(UserName username) {
	if(strlen(username) != 8){
		return false;
	}
	for (int i = 0; i < USERNAME_LENGTH-1; i++) {
		if (!(((username[i] >= '0' && username[i] <= '9') || (username[i] >= 'a' && username[i] <= 'z')) || (username[i] >= 'A' && username[i] <= 'Z')))
			return false;
	}
	return true;
}

/** Function for copying key-element in the map */
static char* copyUserName(char* user) {
	if (!user) {
		return NULL;
	}
	char* copy = malloc(strlen(user) + 1);
	if (!copy) {
		return NULL;
	}
	return strcpy(copy, user);
}

/** Function to be used by the map for freeing key-elements */
static void freeUserName(char* user) {
	free(user);
}

/** Function for comparing two key-elements in the map */
static int compareUserNames(UserName user1, UserName user2) {
	return strcmp(user1, user2);
}

/** Function for copying data-element in the map */
static UserData copyUserData(UserData data) {
	if (!data) {
		return NULL;
	}
	return data;
}

/** Function to be used by the map for freeing data-elements */
static void freeUserData(UserData data) {
	setDestroy(data->trusted_users);
	free(data);
}


Users usersCreate() {
	Users users = mapCreate(
			(copyMapDataElements)copyUserData,
			(copyMapKeyElements)copyUserName,
			(freeMapDataElements)freeUserData,
			(freeMapKeyElements)freeUserName,
			(compareMapKeyElements)compareUserNames);
	if (!users) {
		return NULL;
	}
	return users;
}


void usersDestroy(Users users) {
	mapDestroy(users);
	return;
}

UserResult addUser(Users users, const char* const username, int memory_limit) {
	if (memory_limit < 1) {
		return USER_INVALID_ARGUMENT;
	} else if (!(username && userNameIsValid(username))) {
		return USER_ILLEGAL_USERNAME;
	} else if (mapGet(users, username)) {
		return USER_USERNAME_ALREADY_USED;
	}

	UserData user_data = (UserData)malloc(sizeof(struct user_data_t));
	if (!user_data) {
		return USER_OUT_OF_MEMORY;
	}
	strcpy(user_data->user, username);
	user_data->memory_limit = memory_limit;
	user_data->cur_memory = 0;
	Set trusted_users = setCreate(
			(copySetElements)copyUserName,
			(freeSetElements)freeUserName,
			(compareSetElements)compareUserNames);

	if (!trusted_users) {
		free(user_data);
		return USER_OUT_OF_MEMORY;
	}
	user_data->trusted_users = trusted_users;
	MapResult map_result = mapPut(users, username, user_data);
	if (map_result == MAP_OUT_OF_MEMORY) {
		setDestroy(trusted_users);
		free(user_data);
		return USER_OUT_OF_MEMORY;
	}
	return USER_SUCCESS;
}

bool userIsIn(Users users, UserName username) {
	if (!users || !username || !userNameIsValid(username)) {
		return false;
	} else if (mapGet(users, username)) {
		return true;
	}
	return false;
}

bool userIsInTrust(Users users, UserName username1, UserName username2) {
	if (!users) {
		return false;
	}
	UserData user_data = mapGet(users, username1);
	if (!user_data) {
		return false;
	} else if (setIsIn(user_data->trusted_users, (char*)username2)) {
		return true;
	}
	return false;
}

UserResult removeUser(Users users, const char* const username) {
	if (!username) {
		return USER_ILLEGAL_USERNAME;
	}
	UserData user_data = mapGet(users, username);
	if (!user_data) {
		return USER_USERNAME_NOT_FOUND;
	}
	mapRemove(users, username);
	return USER_SUCCESS;
}


UserResult userSetRelation(
		Users users,
		UserName username1,
		UserName username2,
		Relation relation) {
	if (!username1 || !username2) {
		return USER_USERNAME_NOT_FOUND;
	}
	UserData user_data = mapGet(users, username1);
	if (!(user_data && mapGet(users, username2))) {
		return USER_USERNAME_NOT_FOUND;;
	}
	if (relation == TRUST) {
		SetResult result = setAdd(user_data->trusted_users, (void*)username2);
		if (result == SET_OUT_OF_MEMORY)
			return USER_OUT_OF_MEMORY;
	}	else if((relation == UNTRUST)) {
		setRemove(user_data->trusted_users, (void*)username2);
	}
	return USER_SUCCESS;
}

/* only for tests use generally */
char* readUserName(UserData data) {
	if (!data) {
		return NULL;
	}
	return data->user;
}

void userChangeMemUse(Users users, UserName username, int size, Operation op) {
	UserData user_data = mapGet(users, username);
	if (user_data) {
		if (op == INCREASE) {
			user_data->cur_memory += size;
		} else if (op == REDUCE) {
			user_data->cur_memory -= size;
		}
	}
	return;
}

int userGetCurrentMemUse(Users users, UserName user) {
	UserData ptr = mapGet(users, user);
	if (!ptr) {
		return -1;
	}
	return ptr->cur_memory;
}

int userGetLimitMemUse(Users users, UserName user) {
	UserData ptr = mapGet(users, user);
	if (!ptr) {
		return -1;
	}
	return ptr->memory_limit;
}

void usersClear(Users users) {
	assert(users != NULL);
	mapClear(users);
}

UserData usersGetFirst(Users users) {
	if (!users) {
		return NULL;
	}
	UserName user = mapGetFirst(users);
	if (!user) {
		return NULL;
	}
	return mapGet(users, user);
}
UserData usersGetNext(Users users) {
	if (!users) {
		return NULL;
	}
	UserName user = mapGetNext(users);
	if (!user) {
		return NULL;
	}
	return mapGet(users, user);
}
