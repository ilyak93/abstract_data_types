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
	TEST_EQUALS(result, orangeGetNumberOfFoodCompanies(orange), 3);
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

	// Complete your Code here ...

	return result;
}

static bool testOrangeCopy(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

static bool testOrangeCompareBySize(){
	bool result = true;

	// Complete your Code here ...

	return result;
}

static bool testOrangeAddFoodCompany(){
	bool result = true;

	// Complete your Code here ...

	return result;

}

static bool testOrangeRemoveFoodCompany(){
	bool result = true;

	// Complete your Code here ...

	return result;
}


static bool testOrangeSetFoodCompanyForDelivery(){
	bool result = true;

	// Complete your Code here ...

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
