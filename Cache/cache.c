#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

struct cache_t {
	Set *cells;
	FreeCacheElement freeElement;
	CopyCacheElement copyElement;
	CompareCacheElements compareElements;
	ComputeCacheKey computeKey;
	int cur_cell;
	int size;
};

static bool cacheIsEmpty(Cache cache) {
	CACHE_FOREACH(itr, cache) {
		if (setGetFirst(itr)) {
			return 0;
		}
	}
	return 1;
}

static Set* cellsAllocation(int size) {
	Set* cells = (Set*)malloc(sizeof(Set)*(size+1));
	if (!cells) {
		return NULL;
	}
	for (int i = 0; i <= size; i++)
		cells[i] = NULL;
	return cells;
}

/*Creates empty cache without allocating cells */
Cache cacheCreate(
	int size,
	FreeCacheElement free_element,
	CopyCacheElement copy_element,
	CompareCacheElements compare_elements,
	ComputeCacheKey compute_key) {
	Cache cache = (Cache)malloc(sizeof(struct cache_t));
	if (!cache || size < 1) {
		free(cache);
		return NULL;
	}
	cache->cells = NULL;
	cache->compareElements = compare_elements;
	cache->freeElement = free_element;
	cache->copyElement = copy_element;
	cache->computeKey = compute_key;
	cache->size = size;
	cache->cur_cell = -1;
	return cache;
}


/* Important: cachePush also allocates the cells themselves if it needed (first push)*/
CacheResult cachePush(Cache cache, CacheElement element) {
	if (!cache || !element) {
		return CACHE_NULL_ARGUMENT;
	} else if (!cache->cells) {
		cache->cells = cellsAllocation(cache->size);
		if (!cache->cells) {
			return CACHE_OUT_OF_MEMORY;
		}
	}
	int key = cache->computeKey(element);
	if (setIsIn(cache->cells[key], element)) {
		free(cache->cells);
		cache->cells = NULL;
		return CACHE_ITEM_ALREADY_EXISTS;
	}
	Set new_set = NULL;
	if (!cache->cells[key]) {
		new_set = setCreate(
				cache->copyElement,
				cache->freeElement,
				cache->compareElements);

		if (!new_set) {
			free(cache->cells);
			cache->cells = NULL;
			return CACHE_OUT_OF_MEMORY;
		}
		cache->cells[key] = new_set;
	}
	SetResult set_result = setAdd(cache->cells[key], element);
	if (set_result == SET_OUT_OF_MEMORY) {
		setDestroy(new_set);
		free(cache->cells);
		cache->cells = NULL;
		return CACHE_OUT_OF_MEMORY;
	}
	cache->cur_cell = -1;
	return CACHE_SUCCESS;
}

/* Important: cachePush also deallocates the cells themselves
 *  if it needed (last allocated block free)*/
CacheResult cacheFreeElement(Cache cache, CacheElement element) {
	if (!cache || !element) {
		return CACHE_NULL_ARGUMENT;
	}else if (!cache->cells) {
		return CACHE_ITEM_DOES_NOT_EXIST;
	}
	Set set_of_element = cache->cells[cache->computeKey(element)];
	if (!setIsIn(set_of_element, element)) {
		return CACHE_ITEM_DOES_NOT_EXIST;
	}
	setRemove(set_of_element, element);
	if (cacheIsEmpty(cache)) {
		free(cache->cells);
		cache->cells = NULL;
	}
	cache->cur_cell = -1;
	return CACHE_SUCCESS;
}

/* Important: cacheExtractElementByKey also deallocates the cells themselves
 *  if it needed (last allocated block extracted)*/
CacheElement cacheExtractElementByKey(Cache cache, int key) {
	if (!cache->cells) {
		return NULL;
	}
	CacheElement element = setGetFirst(cache->cells[key]);
	if (!element) {
		return NULL;
	}
	SetElement extracted_element = setExtract(cache->cells[key], element);
	if (cacheIsEmpty(cache)) {
		free(cache->cells);
		cache->cells = NULL;
	}
	cache->cur_cell = -1;
	return extracted_element;
}

bool cacheIsIn(Cache cache, CacheElement element) {
	if (!cache || !cache->cells || !element) {
		return false;
	}
	int key = cache->computeKey(element);
	if (!setIsIn(cache->cells[key], element)) {
		return false;
	}
	return true;
}

Set cacheGetFirst(Cache cache) {
	if (!cache || !cache->cells) {
		return NULL;
	}
	cache->cur_cell = -1;
	return cacheGetNext(cache);
}

Set cacheGetNext(Cache cache) {
	if (!cache || !cache->cells) {
		return NULL;
	}

	while (!cache->cells[++cache->cur_cell] && cache->cur_cell < cache->size) {
		;
	}
	if (cache->cur_cell > cache->size) {
		cache->cur_cell = -1;
		return NULL;
	}
	return cache->cells[cache->cur_cell];
}

Set cacheGetCurrent(Cache cache) {
	if (!cache || !cache->cells || cache->cur_cell == -1) {
		return NULL;
	}
	return cache->cells[cache->cur_cell];
}

/*Clear state of cache is when cells is deallocated and is NULL */
CacheResult cacheClear(Cache cache) {
	if (!cache) {
		return CACHE_NULL_ARGUMENT;
	} else if (cache->cells) {
		CACHE_FOREACH(itr, cache) {
			setDestroy(itr);
		}
		free(cache->cells);
		cache->cells = NULL;
	}
	return CACHE_SUCCESS;
}

void cacheDestroy(Cache cache) {
	if (cache) {
		cacheClear(cache);
	}
	free(cache);
}
