#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "testsMacros.h"
#include "cache.h"
#include "orange.h"
#include "list.h"


static bool testCache1(){
	bool result = true;
	Cache cache = cacheCreate();
	TEST_TRUE(result, cache != NULL);
	Orange orange10 = orangeCreate(10, JAN, 3, 22);
	Orange orange20 = orangeCreate(20, FEB, 3, 33);
	Orange orange30 = orangeCreate(30, MAR, 3, 44);
	Orange orange40 = orangeCreate(40, APR, 3, 55);

	TEST_TRUE(result, cachePush(cache,orange10) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange10) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange20) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange20) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange30) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange30) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange40) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange40) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange40) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange10) == CACHE_SUCCESS);

	int i=1;
	//iterate over cache cells:
	CACHE_FOREACH(iter, cache){
		switch(i){

			case 10:
				TEST_TRUE(result,listGetSize(iter) == 3);
				break;

			case 20:
				TEST_TRUE(result,listGetSize(iter) == 2);
				break;

			case 30:
				TEST_TRUE(result,listGetSize(iter) == 2);
				break;
			case 40:
				TEST_TRUE(result,listGetSize(iter) == 3);
				break;
			default:
				TEST_TRUE(result,listGetSize(iter) == 0);
				break;
		}
		i++;
	}

	TEST_TRUE(result, cacheFreeOrange(cache, 15) == CACHE_NO_ELEMENTS_IN_CELL);
	TEST_TRUE(result, cacheFreeOrange(cache, 40) == CACHE_SUCCESS);
	TEST_TRUE(result, cacheFreeOrange(cache, 40) == CACHE_SUCCESS);
	TEST_TRUE(result, cacheFreeOrange(cache, 40) == CACHE_SUCCESS_LIST_EMPTY);
	TEST_TRUE(result, cacheFreeOrange(cache, 40) == CACHE_NO_ELEMENTS_IN_CELL);


	Orange* org = malloc(sizeof(Orange));
	TEST_TRUE(result, org != NULL );
	*org = NULL;
	TEST_TRUE(result, cacheGet(cache,30,org) == CACHE_SUCCESS );
	TEST_TRUE(result, orangeGetExpirationMonth(*org) == MAR);
	TEST_TRUE(result, orangeGetSize(*org) == 30);
	orangeDestroy(*org);
	i = 1;
	CACHE_FOREACH(iter, cache){
			switch(i){

				case 10:
					TEST_TRUE(result,listGetSize(iter) == 3);
					break;

				case 20:
					TEST_TRUE(result,listGetSize(iter) == 2);
					break;

				case 30:
					TEST_TRUE(result,listGetSize(iter) == 1);
					break;
				case 40:
					TEST_TRUE(result,listGetSize(iter) == 0);
					break;
				default:
					TEST_TRUE(result,listGetSize(iter) == 0);
					break;
			}
			i++;
		}

	Orange orange200a = orangeCreate(200, JAN, 3, 33);
	Orange orange200b = orangeCreate(200,AUG,4,22);
	Orange orange200c = orangeCreate(200,JUN,4,33);

	TEST_TRUE(result, cachePush(cache,orange200a) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange200b) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange200c) == CACHE_SUCCESS);

	TEST_TRUE(result, cacheGet(cache,200,org) == CACHE_SUCCESS );
	TEST_TRUE(result, orangeGetExpirationMonth(*org) == JUN);
	orangeDestroy(*org);
	*org = NULL;
	TEST_TRUE(result, cacheGet(cache,200,org) == CACHE_SUCCESS );
	TEST_TRUE(result, orangeGetExpirationMonth(*org) == AUG);
		orangeDestroy(*org);
		*org = NULL;
	TEST_TRUE(result, cacheGet(cache,200,org) == CACHE_SUCCESS );
	TEST_TRUE(result, orangeGetExpirationMonth(*org) == JAN);
		orangeDestroy(*org);
		*org = NULL;
	orangeDestroy(orange20);
	orangeDestroy(orange40);
	orangeDestroy(orange30);
	orangeDestroy(orange10);
	orangeDestroy(orange200a);
	orangeDestroy(orange200b);
	orangeDestroy(orange200c);
	free(org);
	cacheDestroy(cache);
	return result;


}

static bool testCache2(){
	bool result = true;
	Cache cache = cacheCreate();
	TEST_TRUE(result, cache != NULL);
	Orange orange10 = orangeCreate(10, DEC, 3, 22);
	TEST_TRUE(result , orange10 != NULL);
	Orange orange20 = orangeCreate(20, DEC, 3, 33);
	TEST_TRUE(result , orange20 != NULL);
	Orange orange30 = orangeCreate(30, DEC, 3, 44);
	TEST_TRUE(result , orange30 != NULL);
	Orange orange40 = orangeCreate(40, DEC, 3, 55);
	TEST_TRUE(result , orange40 != NULL);
	Orange orange200a = orangeCreate(200, DEC, 3, 33);
	TEST_TRUE(result , orange200a != NULL);
	Orange orange200b = orangeCreate(200,DEC,4,22);
	TEST_TRUE(result , orange200b != NULL);
	Orange orange200c = orangeCreate(200,DEC,4,33);
	TEST_TRUE(result , orange200c != NULL);
	Orange expiredOrange = orangeCreate(200,JUN,5, 33);
	TEST_TRUE(result , expiredOrange != NULL);

	// Add food Companies :
	TEST_TRUE(result, orangeAddFoodCompany(orange10,"Eden-Teva") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange20,"supersal") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange20,"BIG") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange30,"BIG") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange30,"Eden-Teva") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange40,"supersal") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange200a,"BIG") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange200a,"Eden-Teva") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange200a,"supersal") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(orange200c,"Eden-Teva") == ORANGE_SUCCESS);
	TEST_TRUE(result, orangeAddFoodCompany(expiredOrange,"Eden-Teva") == ORANGE_SUCCESS);

	//Push oranges to cache:
	TEST_TRUE(result, cachePush(cache,orange10) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange20) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange30) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange30) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange30) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange40) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange200a) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange200b) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,orange200c) == CACHE_SUCCESS);
	TEST_TRUE(result, cachePush(cache,expiredOrange) == CACHE_SUCCESS);

	List orangesFromCellToEdenTeva = cacheGetOrangesOfCompanyFromCell(cache,"Eden-Teva",200);
	TEST_TRUE(result, orangesFromCellToEdenTeva != NULL);
	TEST_TRUE(result, listGetSize(orangesFromCellToEdenTeva) == 2);
	// Iterate over list:
	LIST_FOREACH(Orange,iter,orangesFromCellToEdenTeva){
		char* const* companies = orangeGetFoodCompanies(iter);
		TEST_TRUE(result, strcmp(companies[0],"Eden-Teva") == 0);
	}

	List allOrangesFromBig = cacheGetAllOrangesOfCompany(cache,"BIG");
	TEST_TRUE(result, allOrangesFromBig != NULL);
	TEST_TRUE(result, listGetSize(allOrangesFromBig) == 4);
	// Iterate over list:
	int W=0;
	LIST_FOREACH(Orange,iter,allOrangesFromBig){
		char* const* companies = orangeGetFoodCompanies(iter);
		TEST_TRUE(result, strcmp(companies[0],"BIG") == 0);
		W += orangeGetSize(iter);
	}
	TEST_TRUE(result, W == 110);

	List best = cacheDeliverOrangesWithBiggestWeight(cache);
	TEST_TRUE(result, best != NULL);
	TEST_TRUE(result, listGetSize(best) ==1);
	LIST_FOREACH(Orange,iter,best){
		char* const* companies = orangeGetFoodCompanies(iter);
		TEST_TRUE(result, strcmp(companies[0],"supersal") == 0);
		TEST_TRUE(result,orangeGetSize(iter) == 40);

	}

	listDestroy(best);
	listDestroy(allOrangesFromBig);
	listDestroy(orangesFromCellToEdenTeva);
	orangeDestroy(expiredOrange);
	orangeDestroy(orange20);
	orangeDestroy(orange40);
	orangeDestroy(orange30);
	orangeDestroy(orange10);
	orangeDestroy(orange200a);
	orangeDestroy(orange200b);
	orangeDestroy(orange200c);
	cacheDestroy(cache);
	return result;
}

static bool testCacheCreate(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

static bool testCacheCopy(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

static bool testCachePush(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

static bool testCacheFreeOrange(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

static bool testCacheGet(){
	bool result = true;

	// Complete your Code here ...

	return result;

}

static bool testCacheGetCurrent(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

static bool testCacheGetOrangesOfCompanyFromCell(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

static bool testcacheGetAllOrangesOfCompany(){
	bool result = true;

	// Complete your Code here ...

	return result;

}

static bool testcacheDeliverOrangesWithBiggestWeight(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(testCache1);
	RUN_TEST(testCache2);
	RUN_TEST(testCacheCreate);
	RUN_TEST(testCacheCopy);
	RUN_TEST(testCachePush);
	RUN_TEST(testCacheFreeOrange);
	RUN_TEST(testCacheGet);
	RUN_TEST(testCacheGetCurrent);
	RUN_TEST(testCacheGetOrangesOfCompanyFromCell);
	RUN_TEST(testcacheGetAllOrangesOfCompany);
	RUN_TEST(testcacheDeliverOrangesWithBiggestWeight);




	return 0;
}
