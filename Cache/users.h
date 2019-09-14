
#ifndef USERS_
#define USERS_

#include <stdbool.h>
#include "map.h"
#include "set.h"


/**
* Users Container
*
* The following functions are available:
*
*   usersCreate				- Creates a new empty users container.
*  							- This resets the internal iterator.
*
*   usersDestroy			- Deletes an existing users container
*   						- and frees all resources
*
*   userGetCurrentMemUse	- Returns the current memory usage of a given user
*
*   userGetLimitMemUse		- Returns the limit memory usage of a given user.
*
*   userChangeMemUse		- Returns the limit memory usage of a given user.
*
*   addUser		    		- Adds a new user to users container.
*   				  		If the user exists, the data isn't overridden.
*   				 		 This resets the internal iterator.
*
*   removeUser				- Removes a user from users container.
*                   		- This resets the internal iterator.
*
*   userGetFirst			- Sets the internal iterator to the first user in the
*   				 		 users container, and returns it.
*
*   userGetNext				- Advances the internal iterator to the next user and
*   				  		returns it.
*
*	 usersClear				- Clears the contents of the users container.
*	 						- the container using the free function.
*
* 	 USER_FOREACH			- A macro for iterating over the map's elements
* 	 						 - for inner use.
*/

/** Type for defining the user record */
typedef struct user_data_t *UserData;

/** Type used for returning error codes from users functions */
typedef enum UserResult_t {
	USER_SUCCESS,
	USER_INVALID_ARGUMENT,
	USER_ILLEGAL_USERNAME,
	USER_USERNAME_ALREADY_USED,
	USER_USERNAME_NOT_FOUND,
	USER_OUT_OF_MEMORY,
} UserResult;

/** Type for defining the user name */
typedef const char* const UserName;

/** Type for defining users container */
typedef Map Users;

/** Type for defining the differing key for setRelation function */
typedef enum set_relation_t {
	TRUST = 10,
	UNTRUST = 11,
} Relation;

/** Type for defining the differing key for moderating memory usage function */
typedef enum arithmetic_operation_t {
	INCREASE = 12,
	REDUCE = 13,
} Operation;

/**
* usersCreate: Allocates a new empty users-map.
* sets iterator as undefined
* @return
* 	NULL - if allocations failed.
* 	A new Users Container in case of success.
*/
Users usersCreate();

/**
* usersDestroy: Deallocates an existing users container.
*
* 	@param users - Target users container to be deallocated.
* 	If users container is NULL nothing will be
* 	done
*/
void usersDestroy(Users users);

/**
* addUser: Adds a new user by allocating a user with data
* 		   and push it to the users container.
*
*iterator undefined afted operation
*
* @param users - Target users container.
* @param username - a new username to add to users container.
* @param memory_limit - maximal memory_limit can be allocated by user.

* @return
* 	USER_ILLEGAL_USERNAME if a NULL was sent.
*	USER_USERNAME_ALREADY_USED if the user is already in the users container
*	USER_INVALID_ARGUMENT if the argument is non-positive number
*	USER_OUT_OF_MEMORY if allocation failed
*   USER_SUCCESS if user created and pushed into Target.
*/
UserResult addUser(Users users, UserName username, int memory_limit);


/**
* removeUser: Removes a user by deallocating a user and his data
* 			  from the users container.
*
* iterator undefined after operation
*
* @param users - Target users container.
* @param username - a new username to remove from users container.
*
* @return
* 	USER_ILLEGAL_USERNAME if a NULL was sent.
*	USER_USERNAME_NOT_FOUND if user isn't found in users container
*   USER_SUCCESS if user removed and deleted.
*/
UserResult removeUser(Users users, UserName username);

/**
*	userSetRelation: Adds or removes a user to/from another's trusted users.
*
* @param users - The user container in which both users
* @param username1 - user in which trusted users will be made the change
* @param username2 - the user to add/delete to/from first user's trusteded users.
  @param relation - a value defined for differing the two operations add/remove.
*
* By the relation param will be called the optional function
* and add or remove to/from user
*
* @return
* 	USER_ILLEGAL_USERNAME if a NULL was sent as username params
* 	USER_USERNAME_NOT_FOUND if one of the users not found
* 	USER_OUT_OF_MEMORY if allocation failed
*	USER_SUCCESS if username2 added or removed from username1 trusted users.
*/
UserResult userSetRelation(
		Users users,
		UserName username1,
		UserName username2,
		Relation relation);


/**
*	userChangeMemUse: increase/reduce of memory usage of user
*
* @param users - The users container for which to get
* 				the current memory usage of user from.
* @param username - username of which memory usage to change
* @param size - the value to increase/reduce from the current value
* @param op - a value defined to differing from increase/reduce options.
*
* the function will increase or reduce the current memory usage
* by the Operation param.
*
*/
void userChangeMemUse(Users users, UserName username1, int size, Operation op);

/**
*	userGetCurrentMemUse: returns current memory usage of user
*
* @param users - The users container for which to get
*  				 the current memory usage of user from.
* @param user - username of which memory usage to return
*
* @return
* 	-1 if user not found or users invalid argument
* 	the function returns current memory usage of user otherwise
*/
int userGetCurrentMemUse(Users users, UserName user);

/**
*	userGetCurrentMemUse: returns limit memory usage of user
*
* @param users - The users container for which to get
* 				 the limit memory usage of user from.
* @param user - username of which limit of memory usage to return
*
@return
* 	-1 if user not found or users invalid argument
* 	the function returns limit memory usage of user otherwise
*/
int userGetLimitMemUse(Users users, UserName user);

/**
* userIsIn: Checks if a user exists in users-map.
*
* @param users - Target users-map.
* @param username - a username to check.

* @return
* 	false if a NULL was sent or if the user isn't in the users-map
*	true otherwise
*/
bool userIsIn(Users users, UserName username);

/**
* userIsInTrust: Checks if one user is in trust list of another.
*
* @param users - Target users-map.
* @param username1 - a username in which trust-list to check.
* @param username2 - a username trusted or not to check by the first.

* @return
* 	false if a NULL was sent or if the user isn't in the users-map
* 	or the user isn't trusted
*	true otherwise
*/
bool userIsInTrust(Users users, UserName username1, UserName username2);

/**
*	readUserName: returns username of a data-element founded by username
*	clariffication: only for testing usage
* @param data - The users-data-element for which to get username of current data
*
* return username as a string.
*/
char* readUserName(UserData data);

/**
* usersClear: Removes all users and their data from the users container
* @param users
* 	Target users container to remove all users and data from.
*/
void usersClear(Users users);

/**
* usersGetFirst: returns the first user's data.
*
* @param users - The users container to get from the user's data.
*
*return NULL if users invalid users-map or empty
*		first user's data-element otherwise
*/
UserData usersGetFirst(Users users);

/**
* usersGetNext: returns the next user's data
*
* @param users - The users container to get from the user's data
*
*return NULL if users invalid users-map or empty
*		next user's data-element otherwise
*/
UserData usersGetNext(Users users);

#endif /* USERS_ */
