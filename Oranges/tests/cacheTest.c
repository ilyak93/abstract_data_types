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

	List best=cacheDeliverOrangesWithBiggestWeight(cache);
	TEST_TRUE(result, best != NULL);
	TEST_TRUE(result, listGetSize(best) ==1);

	LIST_FOREACH(Orange,iter,best){
		char* const* companies = orangeGetFoodCompanies(iter);
		TEST_TRUE(result, strcmp(companies[0],"supersal") == 0);
		printf(   "   %s   ",companies[0]);
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
	Cache cache;
	TEST_TRUE(result,((cache=cacheCreate())!=NULL));
cacheDestroy(cache);

	return result;
}

static bool testCacheCopy(){
	bool result = true;
	Cache cache_cpy;
	TEST_TRUE(result,((cache_cpy=cacheCopy(NULL))==NULL));
	Cache src=cacheCreate();
	TEST_TRUE(result,((cache_cpy=cacheCopy(src))!=NULL));
	cacheDestroy(src);
	cacheDestroy(cache_cpy);

	return result;
}

static bool testCachePush(){
	bool result = true;
Cache cache=cacheCreate();
Orange orange10=orangeCreate(10,5,3,3);
Orange orange10a=orangeCreate(10,4,3,3);
Orange orange20=orangeCreate(20,5,3,3);
Orange orange256=orangeCreate(256,5,3,3);
Orange orange1=orangeCreate(1,5,3,3);
TEST_TRUE(result,((cachePush(NULL,orange10))==CACHE_ILLEGAL_ARGUMENT));
TEST_TRUE(result,((cachePush(cache,NULL))==CACHE_ILLEGAL_ARGUMENT));
TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
TEST_TRUE(result,((cachePush(cache,orange20))==CACHE_SUCCESS));
TEST_TRUE(result,((cachePush(cache,orange10a))==CACHE_SUCCESS));
TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));
TEST_TRUE(result,((cachePush(cache,orange1))==CACHE_SUCCESS));
TEST_TRUE(result,(listGetSize(cache->container[9]))==CACHE_OUT_OF_RANGE);
TEST_TRUE(result,(listGetSize(cache->container[0]))==CACHE_ILLEGAL_ARGUMENT);
TEST_TRUE(result,(listGetSize(cache->container[255]))==CACHE_ILLEGAL_ARGUMENT);
Orange orange1a=listGetFirst(cache->container[9]);
TEST_TRUE(result,(orange1a->expirationMonth)==4);
orange1a=listGetNext(cache->container[9]);
TEST_TRUE(result,(orange1->expirationMonth)==5);
orangeDestroy(orange10);
orangeDestroy(orange10a);
orangeDestroy(orange20);
orangeDestroy(orange256);
orangeDestroy(orange1);
cacheDestroy(cache);

	return result;
}

static bool testCacheFreeOrange(){
	bool result = true;
	Cache cache=cacheCreate();
	Orange orange10=orangeCreate(10,5,3,3);
	Orange orange10a=orangeCreate(10,4,3,3);
	Orange orange20=orangeCreate(20,5,3,3);
	Orange orange256=orangeCreate(256,5,3,3);
	Orange orange1=orangeCreate(1,5,3,3);
	TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange20))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange10a))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange1))==CACHE_SUCCESS));

	TEST_TRUE(result,((cacheFreeOrange(cache,257)==CACHE_OUT_OF_RANGE)));
	TEST_TRUE(result,((cacheFreeOrange(cache,0)==CACHE_OUT_OF_RANGE)));
	TEST_TRUE(result,((cacheFreeOrange(NULL,252)==CACHE_ILLEGAL_ARGUMENT)));
	TEST_TRUE(result,((cacheFreeOrange(cache,252)==CACHE_NO_ELEMENTS_IN_CELL)));
	TEST_TRUE(result,((cacheFreeOrange(cache,20)==CACHE_SUCCESS_LIST_EMPTY)));
	TEST_TRUE(result,((cacheFreeOrange(cache,20)==CACHE_NO_ELEMENTS_IN_CELL)));
	TEST_TRUE(result,((cachePush(cache,orange20))==CACHE_SUCCESS));
	TEST_TRUE(result,((cacheFreeOrange(cache,20)==CACHE_SUCCESS_LIST_EMPTY)));
	TEST_TRUE(result,((cacheFreeOrange(cache,256)==CACHE_SUCCESS_LIST_EMPTY)));
	TEST_TRUE(result,((cacheFreeOrange(cache,1)==CACHE_SUCCESS_LIST_EMPTY)));

	orangeDestroy(orange10);
	orangeDestroy(orange10a);
	orangeDestroy(orange20);

	orangeDestroy(orange256);
	orangeDestroy(orange1);
	cacheDestroy(cache);
	return result;
}

static bool testCacheGet(){
	bool result = true;
	Cache cache=cacheCreate();
	Orange orange10=orangeCreate(10,5,3,3);
	Orange orange10a=orangeCreate(10,4,3,3);
	Orange orange20=orangeCreate(20,5,3,3);
	Orange orange256=orangeCreate(256,5,3,3);
	Orange orange1=orangeCreate(1,5,3,3);
	TEST_TRUE(result,((cachePush(cache,orange1))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange10a))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange20))==CACHE_SUCCESS));
	TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));

	Orange orange_a,orange_b,orange_c,orange_d,orange_e;
	TEST_TRUE(result,((cacheGet(NULL,0,&orange_a))==CACHE_ILLEGAL_ARGUMENT));
	TEST_TRUE(result,((cacheGet(cache,0,&orange_a))==CACHE_OUT_OF_RANGE));
	TEST_TRUE(result,((cacheGet(cache,257,&orange_a))==CACHE_OUT_OF_RANGE));
	TEST_TRUE(result,((cacheGet(cache,252,&orange_a))==CACHE_NO_ELEMENTS_IN_CELL));
	TEST_TRUE(result,((cacheGet(cache,10,&orange_a))==CACHE_SUCCESS));
	TEST_TRUE(result,((cacheGet(cache,10,&orange_b))==CACHE_SUCCESS));
	TEST_TRUE(result,((cacheGet(cache,10,&orange_c))==CACHE_SUCCESS));
	TEST_TRUE(result,((cacheGet(cache,10,&orange_d))==CACHE_NO_ELEMENTS_IN_CELL));
	TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
	TEST_TRUE(result,((cacheGet(cache,10,&orange_d))==CACHE_SUCCESS));
	TEST_TRUE(result,((cacheGet(cache,10,&orange_e))==CACHE_NO_ELEMENTS_IN_CELL));

	orangeDestroy(orange_a);
	orangeDestroy(orange_b);
	orangeDestroy(orange_c);
	orangeDestroy(orange_d);

	orangeDestroy(orange10);
	orangeDestroy(orange10a);
	orangeDestroy(orange20);
	orangeDestroy(orange256);
	orangeDestroy(orange1);
	cacheDestroy(cache);

	return result;

}

static bool testCacheGetCurrent(){
	bool result = true;
	Cache cache=cacheCreate();
		Orange orange10=orangeCreate(10,5,3,3);
		Orange orange10a=orangeCreate(10,4,3,3);
		Orange orange256=orangeCreate(256,5,3,3);
		TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
		TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
		TEST_TRUE(result,((cachePush(cache,orange10a))==CACHE_SUCCESS));
		TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));

		cache->iterator=cache->container[9];
		TEST_TRUE(result, listGetSize(cacheGetCurrent(cache)) == 3 );
		cache->iterator=cache->container[255];
		TEST_TRUE(result, listGetSize(cacheGetCurrent(cache)) == 1 );
		cache->iterator=cache->container[3];
		TEST_TRUE(result, (listGetSize(cacheGetCurrent(cache))) == 0);
		TEST_TRUE(result, cacheGetCurrent(NULL) ==NULL);


		orangeDestroy(orange10);
		orangeDestroy(orange10a);
		orangeDestroy(orange256);
		cacheDestroy(cache);



	return result;
}

static bool testCacheGetOrangesOfCompanyFromCell(){
	bool result = true;

        Cache cache=cacheCreate();
			Orange orange10=orangeCreate(10,DEC,3,3);
			Orange orange10a=orangeCreate(10,DEC,3,3);
			Orange orange20=orangeCreate(20,DEC,3,3);
			Orange orange100=orangeCreate(100,DEC ,3,3);
			Orange orange100a=orangeCreate(100,DEC ,3,3);
			Orange orange100b=orangeCreate(100,DEC ,3,3);
			Orange orange256=orangeCreate(256,DEC,3,3);
			Orange orange1=orangeCreate(1,DEC,3,3);
			Orange expiredOrange=orangeCreate(1,FEB,3,3);
			TEST_TRUE(result, orangeAddFoodCompany(orange10,"company1") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange10a,"company1") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange20,"company1") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange100,"company1") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange100a,"company1") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange100a,"company2") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange256,"company2") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange256,"company1") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange256,"company3") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(orange1,"company1") == ORANGE_SUCCESS);
			TEST_TRUE(result, orangeAddFoodCompany(expiredOrange,"company1") == ORANGE_SUCCESS);

			TEST_TRUE(result,((cachePush(cache,expiredOrange))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,expiredOrange))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange1))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,expiredOrange))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange10a))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange100a))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange100a))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange100b))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange100))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));
			TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));


			List func10_check = cacheGetOrangesOfCompanyFromCell(cache,"company1",10);
				TEST_TRUE(result, func10_check != NULL);
				TEST_TRUE(result, listGetSize(func10_check) == 3);

			List func10_check2 = cacheGetOrangesOfCompanyFromCell(cache,"company1",10);
				TEST_TRUE(result, func10_check2 == NULL);
			List func10_check3 = cacheGetOrangesOfCompanyFromCell(cache,"company1",1);
				TEST_TRUE(result, func10_check3 != NULL);
				TEST_TRUE(result, listGetSize(func10_check3) == 1)
			List func10_check4 = cacheGetOrangesOfCompanyFromCell(cache,"company1",256);
				TEST_TRUE(result, func10_check4 != NULL);
				TEST_TRUE(result, listGetSize(func10_check4) == 3);
				Orange org_check=listGetFirst(func10_check4);
				TEST_TRUE(result, listGetSize(func10_check4) == 3);
				TEST_TRUE(result, !(strcmp(org_check->foodCompanies[0],"company1")));

				TEST_TRUE(result, listGetSize(cache->container[255]) == 0);
				List func10_check5 = cacheGetOrangesOfCompanyFromCell(cache,"company1",100);
				TEST_TRUE(result, func10_check5 != NULL);
				TEST_TRUE(result, listGetSize(func10_check5) == 3)

				listDestroy(func10_check);
				listDestroy(func10_check3);
				listDestroy(func10_check4);
				listDestroy(func10_check5);

			orangeDestroy(orange10);
			orangeDestroy(orange10a);
			orangeDestroy(orange20);
			orangeDestroy(orange100);
			orangeDestroy(expiredOrange);
            orangeDestroy(orange100a);
            orangeDestroy(orange100b);
			orangeDestroy(orange256);
			orangeDestroy(orange1);
			cacheDestroy(cache);


	return result;
}
static bool testcacheGetAllOrangesOfCompany(){
	bool result = true;
static bool testcacheGetAllOrangesOfCompany(){
    bool result = true;

    Cache cache=cacheCreate();
    Orange orange10=orangeCreate(10,JAN,3,10);
    Orange orange10a=orangeCreate(10,DEC,3,11);
    Orange orange20=orangeCreate(20,DEC,3,20);
    Orange orange100=orangeCreate(100,DEC ,3,100);
    Orange orange100a=orangeCreate(100,DEC ,3,101);
    Orange orange100b=orangeCreate(100,DEC ,3,102);
    Orange orange256=orangeCreate(256,DEC,3,256);
    Orange orange100ex=orangeCreate(100,JAN,3,103);
    Orange expiredOrange=orangeCreate(1,FEB,3,1);
    TEST_TRUE(result, orangeAddFoodCompany(orange10,"company 1") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange10a,"company 2") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange20,"company 3") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange100a,"company 1") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange100a,"company 2") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange100a,"company 3") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange256,"company 2") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange256,"company 1") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange256,"company 3") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange100ex,"company 1") == ORANGE_SUCCESS);
    TEST_TRUE(result, orangeAddFoodCompany(orange100ex,"company 3") == ORANGE_SUCCESS);

    TEST_TRUE(result,((cachePush(cache,orange100ex))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange10))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange10a))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange100a))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange100b))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange100))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));
    TEST_TRUE(result,((cachePush(cache,orange256))==CACHE_SUCCESS));

    List orangesOfCompany3 = cacheGetAllOrangesOfCompany(cache,"company 3");
    List orangesOfCompany1 = cacheGetAllOrangesOfCompany(cache,"company 1");
    List orangesOfCompany2 = cacheGetAllOrangesOfCompany(cache,"company 2");
    TEST_TRUE(result,(listGetSize(orangesOfCompany3) == 5));
    Orange cur_org = listGetFirst(orangesOfCompany3);
    TEST_TRUE(result,(cur_org->sellingPrice == 256));
    cur_org = listGetNext(orangesOfCompany3);
    TEST_TRUE(result,(cur_org->sellingPrice == 256));
    cur_org = listGetNext(orangesOfCompany3);
    TEST_TRUE(result,(cur_org->sellingPrice == 256));
    cur_org = listGetNext(orangesOfCompany3);
    TEST_TRUE(result,(cur_org->sellingPrice == 100));
    cur_org = listGetNext(orangesOfCompany3);
    TEST_TRUE(result,(cur_org->sellingPrice == 100));
    TEST_TRUE(result,(listGetSize(orangesOfCompany1) == 1));
    cur_org = listGetFirst(orangesOfCompany1);
    TEST_TRUE(result,(cur_org->sellingPrice == 10));
    TEST_TRUE(result,(listGetSize(orangesOfCompany1) == 1));
    TEST_TRUE(result,(listGetSize(orangesOfCompany2) == 0));

    orangeDestroy(orange10);
                orangeDestroy(orange10a);
                orangeDestroy(orange20);
                orangeDestroy(orange100);
                orangeDestroy(orange100a);
                orangeDestroy(orange100b);
                orangeDestroy(orange256);
                orangeDestroy(expiredOrange);
                cacheDestroy(cache);

    return result;

}

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

