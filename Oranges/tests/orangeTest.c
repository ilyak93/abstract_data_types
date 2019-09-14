/*
 * orangeTest.c
 *
 *  Created on: 23 בנוב 2015
 *      Author: tal
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "testsMacros.h"
#include "orange.h"


static bool testOrange1() {
	bool result = true;
	Orange orange = orangeCreate(80,3,3,33);
	TEST_DIFFERENT(result, orange, NULL);
	char company[] = { 's', 'u', 'p', 'e', 'r', 's', 'a', 'l', '\0' };
	TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 0);
	TEST_EQUALS(result, orangeAddFoodCompany(orange, company), ORANGE_SUCCESS);
	company[5] = 'z';
	company[6] = 'o';
	company[7] = 'l';
	TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 1);
	TEST_EQUALS(result, orangeAddFoodCompany(orange, company), ORANGE_SUCCESS);
	TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 2);
	TEST_EQUALS(result, orangeAddFoodCompany(orange, company), ORANGE_COMPANY_ALREADY_EXISTS);
	TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 2);
	TEST_EQUALS(result, orangeAddFoodCompany(orange, "Eden-Teva"), ORANGE_SUCCESS);
	TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 3);
	TEST_EQUALS(result, orangeAddFoodCompany(orange, "Tivtam"),
			ORANGE_NO_PLACE_FOR_COMPANY);
	TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 3);
	TEST_EQUALS(result, orangeSetFoodCompanyForDelivery(orange,"Eden-Teva"), ORANGE_SUCCESS);
	TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 3);//לבדוק שזה במקום הראשון והכל מוזז כמו שצריך ושהאחרון מת
	char* const* companies = orangeGetFoodCompanies(orange);
	TEST_DIFFERENT(result, companies, NULL);
	TEST_EQUALS(result, strcmp(companies[0], "Eden-Teva"), 0);
	TEST_DIFFERENT(result, companies[1], NULL);
	TEST_EQUALS(result, strcmp(companies[1], "supersal"), 0);
	TEST_DIFFERENT(result, companies[2], NULL);
	TEST_EQUALS(result, strcmp(companies[2], "superzol"), 0);
	orangeDestroy(orange);
	return result;
}

static bool testOrangeCreate(){
	bool result = true;
    TEST_EQUALS(result, orangeCreate(270,3,3,33), NULL);
    TEST_EQUALS(result, orangeCreate(0,3,3,33), NULL);
    TEST_EQUALS(result, orangeCreate(80,12,3,33), NULL);
    TEST_EQUALS(result, orangeCreate(80,-1,3,33), NULL);
    TEST_EQUALS(result, orangeCreate(80,3,-5,33), NULL);
    TEST_EQUALS(result, orangeCreate(80,3,3,-1), NULL);
    Orange orange = orangeCreate(80,3,3,33);
    TEST_EQUALS(result, orange->expirationMonth, 3);
    TEST_EQUALS(result, orange->size, 80);
    TEST_EQUALS(result, orange->maxNumberOfFoodCompanies, 3);
    TEST_EQUALS(result, orange->sellingPrice, 33);
    TEST_DIFFERENT(result, orange, NULL);
    for(int i=0; i<orange->maxNumberOfFoodCompanies;i++)
        TEST_EQUALS(result, orange->foodCompanies[i], NULL);

	return result;
}

static bool testOrangeCopy(){
	bool result = true;
    TEST_EQUALS(result, orangeCopy(NULL), NULL);
    Orange orange = orangeCreate(80,3,3,33);
    orangeAddFoodCompany(orange, "company 1");
    orangeAddFoodCompany(orange, "company 2");
    Orange orange_cpy=orangeCopy(orange);
    TEST_EQUALS(result,orangeGetNumberOfFoodCompanies(orange),orangeGetNumberOfFoodCompanies(orange_cpy));
    TEST_EQUALS(result,orange->size ,orange_cpy->size);
    TEST_EQUALS(result,orange->expirationMonth ,orange_cpy->expirationMonth);
    TEST_EQUALS(result,orange->maxNumberOfFoodCompanies ,orange_cpy->maxNumberOfFoodCompanies);
    TEST_EQUALS(result,orange->sellingPrice ,orange_cpy->sellingPrice);
    char* const* companies = orangeGetFoodCompanies(orange_cpy);
    TEST_DIFFERENT(result, orange->foodCompanies, companies);
    for(int i=0;i<orangeGetNumberOfFoodCompanies(orange);i++)
    {
    	TEST_EQUALS(result, strcmp(companies[i], orange->foodCompanies[i]), 0);
    }

    orangeAddFoodCompany(orange, "company 3");
    TEST_DIFFERENT(result, companies[2], orange->foodCompanies[2]);
    TEST_EQUALS(result, orangeCopy(NULL), NULL);
    orangeDestroy(orange);
    orangeDestroy(orange_cpy);
	return result;
}

static bool testOrangeCompareBySize(){
	bool result = true;
	//Orange orange1=orangeCreate(80,10,5,30);
	//Orange orange2=orangeCreate(70,10,5,30);
	//orangeCompareBySize(orange1,orange2);
	//TEST_TRUE(result,a);


	return result;
}

static bool testOrangeAddFoodCompany(){
    bool result = true;
    Orange orange1 = orangeCreate(80,10,3,30);
    char *str = NULL;
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "abc"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "abcd"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "abcde"), ORANGE_SUCCESS);
    TEST_EQUALS(result,orangeGetNumberOfFoodCompanies(orange1),3);
    str = "abcd"; //EQUAL CHECK
    TEST_EQUALS(result, strcmp(orange1->foodCompanies[1],str), 0);
    str = "abcde"; // DIFF CHEACK
    //TEST_DIFFERENT(result,(!(strcmp(orange1->foodCompanies[0])), 0);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, NULL), ORANGE_ILLEGAL_ARGUMENT);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1,"abcd"), ORANGE_COMPANY_ALREADY_EXISTS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1,"Best"), ORANGE_NO_PLACE_FOR_COMPANY);
    orangeDestroy(orange1);
    return result;
}

static bool testOrangeRemoveFoodCompany(){
    bool result = true;
    Orange orange1= orangeCreate(80,10,3,30);
    TEST_EQUALS(result, orangeRemoveFoodCompany(orange1, "abcde"), ORANGE_COMPANY_DOESNT_EXISTS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "abc"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "abcd"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "abcde"), ORANGE_SUCCESS);
    Orange orange_cpy = orangeCopy(orange1);
    TEST_EQUALS(result, orangeRemoveFoodCompany(orange1, "abcde"), ORANGE_SUCCESS);
    TEST_DIFFERENT(result,orangeGetNumberOfFoodCompanies(orange1),orangeGetNumberOfFoodCompanies(orange_cpy));
    TEST_DIFFERENT(result,orange1->foodCompanies[2],orange_cpy->foodCompanies[2]);
    TEST_EQUALS(result, orangeRemoveFoodCompany(orange1, "BEST"), ORANGE_COMPANY_DOESNT_EXISTS);
    TEST_EQUALS(result, orangeRemoveFoodCompany(orange1, "abcde"), ORANGE_COMPANY_DOESNT_EXISTS);
    TEST_EQUALS(result, orangeRemoveFoodCompany(orange1, NULL), ORANGE_ILLEGAL_ARGUMENT);
    TEST_EQUALS(result, orangeRemoveFoodCompany(NULL, NULL), ORANGE_ILLEGAL_ARGUMENT);
    TEST_EQUALS(result, orangeRemoveFoodCompany(NULL, "abcd"), ORANGE_ILLEGAL_ARGUMENT);
    TEST_EQUALS(result, orangeRemoveFoodCompany(orange1, "abcd"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "abcd"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "abcd"), ORANGE_COMPANY_ALREADY_EXISTS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1, "Best"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeAddFoodCompany(orange1,"Compa"), ORANGE_NO_PLACE_FOR_COMPANY);
    orangeDestroy(orange1);
    return result;
}


static bool testOrangeSetFoodCompanyForDelivery(){
	bool result = true;

    Orange orange = orangeCreate(80,3,4,33);
    orangeSetFoodCompanyForDelivery(orange, "company 2");
    orangeSetFoodCompanyForDelivery(orange, "company 1");
    TEST_EQUALS(result, strcmp(orange->foodCompanies[0], "company 1" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[1], "company 2" ) ,0);
    TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 2);
    TEST_EQUALS(result, orangeSetFoodCompanyForDelivery(NULL,"company_new"), ORANGE_ILLEGAL_ARGUMENT);
    TEST_EQUALS(result, orangeSetFoodCompanyForDelivery(orange,NULL), ORANGE_ILLEGAL_ARGUMENT);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[0], "company 1" ) ,0);
    TEST_EQUALS(result, orangeSetFoodCompanyForDelivery(orange,"company_new"), ORANGE_SUCCESS);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[0], "company_new" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[1], "company 1" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[2], "company 2" ) ,0);
    TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 3);
    TEST_EQUALS(result, orangeSetFoodCompanyForDelivery(orange,"company_new"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 3);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[0], "company_new" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[1], "company 1" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[2], "company 2" ) ,0);
    TEST_EQUALS(result, orangeSetFoodCompanyForDelivery(orange,"company 2"), ORANGE_SUCCESS);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[0], "company 2" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[1], "company_new" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[2], "company 1" ) ,0);
    TEST_EQUALS(result, orange->foodCompanies[3] ,NULL);
    TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 3);
    TEST_EQUALS(result, orangeSetFoodCompanyForDelivery(orange,"company_new 2"), ORANGE_SUCCESS);
    TEST_EQUALS(result, orangeSetFoodCompanyForDelivery(orange,"company_new 3"), ORANGE_SUCCESS);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[0], "company_new 3" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[1], "company_new 2" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[2], "company 2" ) ,0);
    TEST_EQUALS(result, strcmp(orange->foodCompanies[3], "company_new" ) ,0);
    TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 4);
    orangeDestroy(orange);
	return result;
}

int main() {
	RUN_TEST(testOrange1);
	RUN_TEST(testOrangeCreate);
	RUN_TEST(testOrangeCopy);
	RUN_TEST(testOrangeCompareBySize);
	RUN_TEST(testOrangeAddFoodCompany);
	RUN_TEST(testOrangeRemoveFoodCompany);
	RUN_TEST(testOrangeSetFoodCompanyForDelivery);
	return 0;
}


