#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "memcache.h"

#define USERNAME_LENGTH 9


struct memcache_t {
	Cache allocated_mem;
	Cache free_mem;
	Users users;
	Set cur_allocated_blocks;
	Set cur_free_blocks;
	void *cur_allocated_block;
	void *cur_free_block;

};



/** Data element data type for block */
typedef void* MemcacheBlock;
typedef const void* const constMemcacheBlock;


/** Calculate and pulls out the size of the block */
static int getSizeOfBlock(MemcacheBlock block) {
	char *ptr = block;
	return *((int*)(ptr - (sizeof(int) + 12)));
}

/** casts address to integer */
static uintptr_t ptrToInt(void *ptr) {
	uintptr_t number = (uintptr_t)ptr;
	return number;
}

/** Function for copying a block - returns the element itself to be insert to cache */
static MemcacheBlock copyBlock(MemcacheBlock ptr) {
	if (!ptr) {
		return NULL;
	}
	return ptr;
}

/** Function for free a block */
static void freeBlock(MemcacheBlock ptr) {
	if (!ptr) {
		return;
	}
	char *block = ptr;
	block -= (sizeof(int) + 12);
	free(block);
}

/** Function for comparing 2 blocks */
static unsigned long cmpBlocks(MemcacheBlock ptr1, MemcacheBlock ptr2) {
	return (ptrToInt(ptr1) - ptrToInt(ptr2));
}

/** calculates and pulls out the mode of the block */
static char getModOfBlock(MemcacheBlock block) {
	char *ptr = block;
	ptr -= 11;
	return *ptr;
}

/** calculates and pulls out the user of the block */
static char* getUserOfBlock(MemcacheBlock block) {
	char *ptr = block;
	ptr -= 10;
	return ptr;

}

static void changeFields(void *block, UserName username) {
	char *ptr = block;
	ptr -= 11;
	*ptr = 'U';
	ptr++;
	strcpy(ptr, username);
}

static void changeAllFields(void *block, UserName username, int size) {
	char *ptr = block;
	*((int*)ptr) = size;
	ptr += sizeof(int);
	*ptr = '\0';
	ptr++;
	*ptr = 'U'; // By Default ???????
	ptr++;
	strcpy(ptr, username);
	ptr += 9;
	*ptr = '\0';
	ptr += (size + 1);
	*ptr = '\0';
}

static void setModOfBlock(void *block, char mod) {
	char *ptr = block;
	ptr -= 11;
	*ptr = mod;
}

static int computeKeyForAllocated(void *ptr) {
	uintptr_t number = (uintptr_t)ptr;
	return number % ADDRESSES;
}

static int computeKeyForFree(void *ptr) {
	return getSizeOfBlock(ptr);
}

static bool userNameIsValid(UserName username) {
	if (strlen(username) != (USERNAME_LENGTH - 1)) {
		return false;
	}
	for (int i = 0; i < USERNAME_LENGTH - 1; i++) {
		if (!(((username[i] >= '0' && username[i] <= '9') || (username[i] >= 'a' && username[i] <= 'z')) || (username[i] >= 'A' && username[i] <= 'Z')))
			return false;
	}
	return true;
}

static void cacheExtractElement(Cache cache, CacheElement element) {
	CACHE_FOREACH(itr, cache) {
		if (setIsIn(itr, element)) {
			setExtract(itr, element);
			break;
		}
	}
}

static void *findOrAllocateBlock(MemCache memcache, UserName username, int size) {
	void *block = NULL;
	if (size <= FREE_LIMIT && (block = cacheExtractElementByKey(
			memcache->free_mem,
			size))) {
		changeFields(block, username);
	} else {
		block = malloc(sizeof(int) + (sizeof(char)*(size + 13)));
		if (!block) {
			return NULL;
		}
		changeAllFields(block, username, size);
		void **ptr1 = &block; //iterates address to the beginning of the content.
		char** ptr2 = (char**)ptr1;
		*ptr2 += (sizeof(int) + 12);

	}
	return block;
}

MemCache memCacheCreate() {
	MemCache memcache = (MemCache)malloc(sizeof(struct memcache_t));
	if (!memcache) {
		return NULL;
	}
	memcache->allocated_mem = cacheCreate(ADDRESSES,
			(FreeCacheElement)freeBlock,
			(CopyCacheElement)copyBlock,
			(CompareCacheElements)cmpBlocks,
			(ComputeCacheKey)computeKeyForAllocated);

	memcache->free_mem = cacheCreate(FREE_LIMIT,
			(FreeCacheElement)freeBlock,
			(CopyCacheElement)copyBlock,
			(CompareCacheElements)cmpBlocks,
			(ComputeCacheKey)computeKeyForFree);

	memcache->users = usersCreate();
	if (!memcache ||
		!memcache->free_mem ||
		!memcache->allocated_mem ||
		!memcache->users) {
			free(memcache->users);
			free(memcache->allocated_mem);
			free(memcache->free_mem);
			free(memcache);
			return NULL;
		}
	memcache->cur_allocated_blocks = NULL;
	memcache->cur_free_blocks = NULL;
	memcache->cur_allocated_block = NULL;
	memcache->cur_free_block = NULL;
	return memcache;
}

void memCacheDestroy(MemCache memcache) {
	usersDestroy(memcache->users);
	cacheDestroy(memcache->allocated_mem);
	cacheDestroy(memcache->free_mem);
	free(memcache);
}

MemCachResult memCacheAddUser(MemCache memcache, const char* const username, int memory_limit) {
	if (!memcache) {
		return MEMCACHE_NULL_ARGUMENT;
	}
	UserResult user_result = addUser(memcache->users, username, memory_limit);
	if (user_result == USER_INVALID_ARGUMENT) {
		return MEMCACHE_INVALID_ARGUMENT;
	} else if (user_result == USER_ILLEGAL_USERNAME) {
		return MEMCACHE_ILLEGAL_USERNAME;
	} else if (user_result == USER_USERNAME_ALREADY_USED) {
		return MEMCACHE_USERNAME_ALREADY_USED;
	} else if (user_result == USER_OUT_OF_MEMORY) {
		return MEMCACHE_OUT_OF_MEMORY;
	}
	return MEMCACHE_SUCCESS;
}

MemCachResult memCacheSetBlockMod(
		MemCache memcache,
		const char* const username,
		void* ptr, char mod) {
	if (!memcache) {
		return MEMCACHE_NULL_ARGUMENT;
	} else if (!userIsIn(memcache->users, username)) {
		return MEMCACHE_USER_NOT_FOUND;
	} else if (!cacheIsIn(memcache->allocated_mem, ptr)) {
		return MEMCACHE_BLOCK_NOT_ALLOCATED;
	}
	UserName user = getUserOfBlock(ptr);
	if (strcmp(user, username)) {
		return MEMCACHE_PERMISSION_DENIED;
	} else if (!(mod == 'A' || mod == 'G' || mod == 'U')) {
		return MEMCACHE_INVALID_ARGUMENT;
	}
	setModOfBlock(ptr, mod);
	return MEMCACHE_SUCCESS;
}

MemCachResult memCacheTrust(
		MemCache memcache,
		const char* const username1,
		const char* const username2) {
	if (!memcache) {
		return MEMCACHE_NULL_ARGUMENT;
	}
	UserResult user_result = userSetRelation(
			memcache->users,
			username1,
			username2,
			TRUST);
	if (user_result == USER_USERNAME_NOT_FOUND) {
		return MEMCACHE_USER_NOT_FOUND;
	}else if (user_result == USER_OUT_OF_MEMORY) {
		return MEMCACHE_OUT_OF_MEMORY;
	}
	return MEMCACHE_SUCCESS;
}


MemCachResult memCacheUntrust(MemCache memcache, const char* const username1, const char* const username2) {
	if (!memcache) {
		return MEMCACHE_NULL_ARGUMENT;
	}
	UserResult user_result = userSetRelation(memcache->users, username1, username2, UNTRUST);
	if (user_result == USER_ILLEGAL_USERNAME) {
		return MEMCACHE_ILLEGAL_USERNAME;
	} else if (user_result == USER_USERNAME_NOT_FOUND) {
		return MEMCACHE_USER_NOT_FOUND;
	}
	return MEMCACHE_SUCCESS;

}

void* memCacheAllocate(MemCache memcache, const char* const username, int size) {
	if (!memcache || !username || (size < 1) ||
		!userNameIsValid(username) ||
		!userIsIn(memcache->users, username)||
		(userGetCurrentMemUse(memcache->users, username) + size)
			>
		userGetLimitMemUse(memcache->users, username)) {
		return NULL;
	}

	void *block = findOrAllocateBlock(memcache, username, size);
	CacheResult cache_result = cachePush(memcache->allocated_mem, block);

	if (cache_result == CACHE_SUCCESS) {
		userChangeMemUse(memcache->users, username, size, INCREASE);
		memcache->cur_allocated_blocks = NULL;
		memcache->cur_allocated_block = NULL;
		memcache->cur_free_blocks = NULL;
		memcache->cur_free_block = NULL;
		return block;
	}
	return NULL;
}

MemCachResult memCacheFree(MemCache memcache,
		const char* const username,
		void* ptr) {
	if (!memcache) {
		return MEMCACHE_NULL_ARGUMENT;
	} else if (!userIsIn(memcache->users, username)) {
		return MEMCACHE_USER_NOT_FOUND;
	} else if (!cacheIsIn(memcache->allocated_mem, ptr)) {
		return MEMCACHE_BLOCK_NOT_ALLOCATED;
	}
	UserName user = getUserOfBlock(ptr);
	char mod_of_block = getModOfBlock(ptr);
	if (!((mod_of_block == 'A') ||
		(mod_of_block == 'G' &&
		(userIsInTrust(memcache->users, user, username))) ||
		(!strcmp(user, username)))) {
		return MEMCACHE_PERMISSION_DENIED;
	}
	userChangeMemUse(memcache->users, user, getSizeOfBlock(ptr), REDUCE);
	memcache->cur_allocated_blocks = NULL;
	memcache->cur_allocated_block = NULL;
	memcache->cur_free_blocks = NULL;
	memcache->cur_free_block = NULL;

	if (getSizeOfBlock(ptr) > FREE_LIMIT) {
		cacheFreeElement(memcache->allocated_mem, ptr);
		return MEMCACHE_SUCCESS;
	}
	CacheResult cache_result = cachePush(memcache->free_mem, ptr);
	if (cache_result == CACHE_OUT_OF_MEMORY) {
		return MEMCACHE_OUT_OF_MEMORY;
	}
	cacheExtractElement(memcache->allocated_mem, ptr);
	return MEMCACHE_SUCCESS;
}

void* memCacheGetFirstAllocatedBlock(MemCache memcache) {
	if (!memcache) {
		return NULL;
	}
	memcache->cur_allocated_block = NULL;
	memcache->cur_allocated_blocks = cacheGetFirst(memcache->allocated_mem);
	if (!memcache->cur_allocated_blocks) {
		return NULL;
	}
	return memCacheGetNextAllocatedBlock(memcache);
}

void* memCacheGetNextAllocatedBlock(MemCache memcache) {
	if (!(memcache && memcache->cur_allocated_blocks)) {
		return NULL;
	} else if (memcache->cur_allocated_blocks) {
		if (!memcache->cur_allocated_block) {
			memcache->cur_allocated_block = setGetFirst(
					memcache->cur_allocated_blocks);
		} else {
			memcache->cur_allocated_block = setGetNext(
					memcache->cur_allocated_blocks);
		}

		if (!memcache->cur_allocated_block) {
			memcache->cur_allocated_blocks = cacheGetNext(
					memcache->allocated_mem);
			Set set_itr = setGetFirst(memcache->cur_allocated_blocks);
			while (!set_itr && memcache->cur_allocated_blocks) {
				memcache->cur_allocated_blocks = cacheGetNext(
						memcache->allocated_mem);
				if (!memcache->cur_allocated_blocks) {
					return NULL;
				}
				set_itr = setGetFirst(memcache->cur_allocated_blocks);
			}
			memcache->cur_allocated_block = set_itr;
		}
	}
	return memcache->cur_allocated_block;
}

void* memCacheGetCurrentAllocatedBlock(MemCache memcache) {
	if (!memcache) {
		return NULL;
	}
	return memcache->cur_allocated_block;
}

void* memCacheGetFirstFreeBlock(MemCache memcache) {
	if (!memcache) {
		return NULL;
	}
	memcache->cur_free_block = NULL;
	memcache->cur_free_blocks = cacheGetFirst(memcache->free_mem);
	if (!memcache->cur_free_blocks) {
		return NULL;
	}
	return memCacheGetNextFreeBlock(memcache);
}
void* memCacheGetNextFreeBlock(MemCache memcache) {
	if (!(memcache && memcache->cur_free_blocks)) {
		return NULL;
	} else if (memcache->cur_free_blocks) {
		if (!memcache->cur_free_block) {
			memcache->cur_free_block = setGetFirst(memcache->cur_free_blocks);
		} else {
			memcache->cur_free_block = setGetNext(memcache->cur_free_blocks);
		}
		if (!memcache->cur_free_block) {
			memcache->cur_free_blocks = cacheGetNext(memcache->free_mem);
			Set set_itr = setGetFirst(memcache->cur_free_blocks);
			while (!set_itr && memcache->cur_free_blocks) {
				memcache->cur_free_blocks = cacheGetNext(memcache->free_mem);
				if (!memcache->cur_free_blocks) {
					return NULL;
				}
				set_itr = setGetFirst(memcache->cur_free_blocks);
			}
			memcache->cur_free_block = set_itr;
		}
	}
	return memcache->cur_free_block;
}

void* memCacheGetCurrentFreeBlock(MemCache memcache) {
	if (!memcache) {
		return NULL;
	}
	return memcache->cur_free_block;
}

MemCachResult memCacheReset(MemCache memcache) {
	if (!memcache) {
		return MEMCACHE_NULL_ARGUMENT;
	}
	usersClear(memcache->users);
	cacheClear(memcache->allocated_mem);
	cacheClear(memcache->free_mem);
	memcache->cur_allocated_block = NULL;
	memcache->cur_allocated_blocks = NULL;
	memcache->cur_free_blocks = NULL;
	memcache->cur_free_block = NULL;
	return MEMCACHE_SUCCESS;
}
