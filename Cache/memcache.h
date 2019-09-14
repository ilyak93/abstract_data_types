/*
* map.h
*
*  Created on: 15/03/2012
*      Author: nsegev
*/

#ifndef MEMCACHE_
#define MEMCACHE_

#include <stdbool.h>
#include "cache.h"
#include "users.h"
#include "map.h"
#include "set.h"
#include "list.h"

#define ADDRESSES 1024
#define FREE_LIMIT 256
/**
* Memory Cache
*
* Implements a memory cache container type.
* The memory cache has an internal iterator for external use.
* For all functions where the state of the iterator
* after calling that function is not stated,
* it is undefined. That is you cannot assume anything about it.
*
* The following functions are available:
*   memCacheCreate		- Creates a new empty memory cache.
*
*   memCacheDestroy		- Deletes an existing memory cache
*   					- and frees all resources
*
*   memCacheAddUser		- Adds a user to system what allows the system
*   					- allocate blocks by him.
*   					- This resets the internal users's iterator.
*
*   memCacheSetBlockMod	- Changes access mod of memory block into
*   					- one of possible states:
*   					- 'A' for all user access, 'G' for trusted user access,
*   					- 'U' for user who created the block only
*
*   memCacheTrust		- Set one user in another's trusted list, which allows
*   					- the first in the right access mod of the block
*   					- manipulate the memory block.
*
*   memCacheUntrust		- Remove one user from another's trusted list.
*
*   memCacheAllocate  	- Allocates a memory block of given size and
*   					- belonging to the user, who allocated.
*					  	- Iterator status undefined after operation.
*
*   memCacheFree		- Deallocate a memory block by given address
*                   	- Iterator status undefined after operation.
*
*   memCacheGetFirstAllocatedBlock		- Sets the internal iterator
*    									- to the first allocated block
*    									- in the memory cache, and returns it.
*
*   memCacheGetNextAllocatedBlock		- Sets the internal iterator
*    									- to the next allocated block
*    									- in the memory cache, and returns it.
*
*   memCacheGetCurrentAllocatedBlock	- returns the current iterator value
*   									- as current allocated block.
*
*   memCacheGetFirstFreeBlock			- Sets the internal iterator
*    									- to the first free block
*    									- in the memory cache, and returns it.
*
*   memCacheGetNextFreeBlock			- Sets the internal iterator
*    									- to the next free block
*    									- in the memory cache, and returns it.
*
*   memCacheGetCurrentFreeBlock			- returns the current iterator value
*   									- as current free block.
*
*	 memCacheClear			- Clears the contents of the Memory cache.
*	 						- Frees all the allocated blocks.
*
* 	 MAP_ALLOCATED_FOREACH	- A macro for iterating over the memcache's
* 	 						- allocated blocks.
*
* 	 MAP_FREE_FOREACH		- A macro for iterating over
* 	 						- the memcache's allocated blocks.
*
*/

/** Type for defining the memory cache */
typedef struct memcache_t *MemCache;

/** Type used for returning error codes from memory cache functions */
typedef enum MemCachResult_t {
	MEMCACHE_SUCCESS,
	MEMCACHE_NULL_ARGUMENT,
	MEMCACHE_USER_NOT_FOUND,
	MEMCACHE_BLOCK_NOT_ALLOCATED,
	MEMCACHE_INVALID_ARGUMENT,
	MEMCACHE_PERMISSION_DENIED,
	MEMCACHE_ILLEGAL_USERNAME,
	MEMCACHE_USERNAME_ALREADY_USED,
	MEMCACHE_OUT_OF_MEMORY
} MemCachResult;

/** Data element data type for memory cache container */
typedef void* MemcacheElement;

/**
* memCacheCreate: Allocates a new empty memory cache.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new memcache in case of success.
*/
MemCache memCacheCreate();

/**
* mapDestroy: Deallocates an existing map. Clears all elements by using the
* stored free functions.
*
* @param map - Target map to be deallocated. If map is NULL nothing will be
* 		done
*/
void memCacheDestroy(MemCache memcache);

/**
* memCacheAddUser: Adds a new user to the memory cache system
*
* @param memcache - Target memory cache.
* @param username - a new username to add to memory cache users.
* @param memory_limit - maximal memory_limit can be allocated by user.

* @return
* 	MEMCACHE_NULL_ARGUMENT if NULL was sent as memcache param
*	MEMCACHE_INVALID_ARGUMENT if the argument is non-positive number
*	MEMCACHE_ILLEGAL_USERNAME if a NULL was sent or illegal username
*	MEMCACHE_USERNAME_ALREADY_USED if the user is already in the memcache users
*	MEMCACHE_OUT_OF_MEMORY if allocation failed.
*	MEMCACHE_SUCCESS if user created successfully.
*/
MemCachResult memCacheAddUser(
		MemCache memcache,
		const char* const username,
		int memory_limit);

/**
* memCacheSetBlockMod: Set mod of memory block to one of the states: 'U','G','A'
* @param memcache - The map which size is requested
* @param username - the creator of the block
* @param ptr - address of the block
* @param ptr - mod to which current mod will be changed.
* @return
* 	MEMCACHE_NULL_ARGUMENT if a NULL pointer was sent.
* 	MEMCACHE_USER_NOT_FOUND if user isn't exist in the system
* 	MEMCACHE_BLOCK_NOT_ALLOCATED if block isn't allocated
* 	MEMCACHE_PERMISSION_DENIED if the user isn't the creator of the memory block
* 	MEMCACHE_INVALID_ARGUMENT if mod argument is invalid
* 	MEMCACHE_SUCCESS if mod of block was set successfully.
*/
 MemCachResult memCacheSetBlockMod(
		 MemCache memcache,
		 const char* const username,
		 void* ptr, char mod);

 /**
 *	memCacheTrust: Adds user to another's trusted users.
 *
 * @param memcache - memory cache system in which both of users are
 * @param username1 - user in which trusted users will be made the addition
 * @param username2 - the user to add to first user's trusted users.
 *
 * @return
 * 	MEMCACHE_ILLEGAL_USERNAME
 * 	USER_USERNAME_NOT_FOUND if one of the users not found
 * 							(including NULL param was sent)
 * 	USER_OUT_OF_MEMORY if allocation failed
 *	USER_SUCCESS if username2 added to username1 trusted users.
 */
 MemCachResult memCacheTrust(
		 MemCache memcache,
		 const char* const username1,
		 const char* const username2);


 /**
  *	memCacheUntrust: Removes user from another's trusted users.
  *
  * @param memcache - memory cache system in which both of users are
  * @param username1 - user in which trusted users will be made the removal
  * @param username2 - the user to remove from first user's trusted users.
  *
  * @return
  * 	MEMCACHE_ILLEGAL_ARGUMENT if NULL was sent as memcache param
  * 	USER_USERNAME_NOT_FOUND if one of the users not found
  * 	(including NULL was sent as user)
  * 	USER_SUCCESS if username2 removed from username1 trusted users.
  */
 MemCachResult memCacheUntrust(
		 MemCache memcache,
		 const char* const username1,
		 const char* const username2);

/**
*	memCacheAllocate: Allocates and stores a new sized memory block. Returns a
*	pointer to the first cell of the block to the user.
*  Iterator's value is undefined after this operation.
*  @param memcache - Target memcache system
*  @param username - user which creates the block
*  @param size - size of block
*
* @return
* 	NULL if a one of the parameters was illegal
* 	pointer of the block otherwise, as mentioned.
*/
void* memCacheAllocate(MemCache memcache, const char* const username, int size);

/**
*	memCacheFree: Deallocates a stored memory block.
*  Iterator's value is undefined after this operation.
*  @param memcache - Target memcache system
*  @param username - user which try to deallocate a block
*  (can be the user himself or a trusted one)
*  @param ptr - pointer of the block as already described.
*
* @return
* 	MEMCACHE_NULL_ARGUMENT if a NULL ws sent as memcache param
* 	MEMCACHE_USER_NOT_FOUND if user wasn't found in the system
* 	MEMCACHE_BLOCK_NOT_ALLOCATED if block wasn't allocated
* 	MEMCACHE_PERMISSION_DENIED if user doesn't have permission to free the block
* 	MEMCACHE_OUT_OF_MEMORY if inner allocation failed
* 	MEMCACHE_SUCCESS if block free successfully.
*/
MemCachResult memCacheFree(
		MemCache memcache,
		const char* const username,
		void* ptr);

/**
 * Sets the internal iterator to the first available allocated block
 * and retrieves it.
 *
 * @param memcache - memcache to iterate the allocated blocks.
 *
 * @return NULL if a NULL pointer was passed or wasn't allocated any blocks
 * or the iterator is in undefined position.
 *  The first available  of the allocated blocks in memcache otherwise.
 */
void* memCacheGetFirstAllocatedBlock(MemCache memcache);

/**
 * Sets the internal iterator to the next available allocated block
 * and retrieves it.
 *
 * @param memcache - memcache to iterate the allocated blocks.
 *
 * @return NULL if a NULL pointer was passed or wasn't allocated any blocks
 * or the iterator is in undefined position.
 *  The next available of the allocated blocks in memcache otherwise.
 */
void* memCacheGetNextAllocatedBlock(MemCache memcache);

/**
 * retrieves the current allocated block which the iternal iterator points.
 *
 * @param memcache - memcache to iterate the allocated blocks.
 *
 * @return NULL if a NULL pointer was passed
 * or the iterator passed all the allocated block
 * or is in undefined position.
 *  The current available block of the allocated blocks in memcache otherwise.
 */
void* memCacheGetCurrentAllocatedBlock(MemCache memcache);

/**
 * Sets the internal iterator to the first available free block and retrieves it.
 *
 * @param memcache - memcache to iterate the free blocks.
 *
 * @return NULL if a NULL pointer was passed
 * or the iterator is in undefined position
 * or no blocks were free.
 *  The first available of the allocated blocks in memcache otherwise.
 */
void* memCacheGetFirstFreeBlock(MemCache memcache);

/**
 * Sets the internal iterator to the next available free block and retrieves it.
 *
 * @param memcache - memcache to iterate the free blocks.
 *
 * @return NULL if a NULL pointer was sent or iterator passed all the free blocks
 * or the iterator is in undefined position.
 *  The next available block of the free blocks in memcache otherwise.
 */
void* memCacheGetNextFreeBlock(MemCache memcache);

/**
 * retrieves the current free block which the iternal iterator points.
 *
 * @param memcache - memcache to iterate the free blocks.
 *
 * @return NULL if a NULL pointer was passed or the iterator passed al the free
 * block or is in undefined position.
 *  The current available block of the free blocks in memcache otherwise.
 */
void* memCacheGetCurrentFreeBlock(MemCache memcache);

/**
* memCacheReset: Removes all users, allocated and free blocks
* 	from target memcache.
* The elements are deallocated. New operation on users and blocks
* 	can be made after.
* Iterator reseted.
*
* @param memcache
* 	Target memcache to remove all elements from.
* @return
* 	MEMCACHE_NULL_ARGUMENT - if a NULL pointer was sent.
* 	MEMCACHE_SUCCESS - Otherwise.
*/
MemCachResult memCacheReset(MemCache memcache);

/*!
* Macros for iterating over allocated and free blocks.
* Declares a new iterator for the loop for each macro.
*/
#define MEMCACHE_ALLOCATED_FOREACH(cur_allocated_block,memcache_src) \
	for(char *cur_allocated_block = (char*) memCacheGetFirstAllocated(memcache_src) ; \
	cur_allocated_block ;\
	cur_allocated_block = memCacheGetNextAllocated(memcache_src))

#define MEMCACHE_FREE_FOREACH(cur_free_block,memcache_src) \
	for(char *cur_free_block = (char*) memCacheGetFirstFree(memcache_src) ; \
	cur_free_block ;\
	cur_free_block = memCacheGetNextFree(memcache_src))

#endif /* MEMCACHE_ */
