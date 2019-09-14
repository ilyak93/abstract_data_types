/*
 * yad3_test.c
 *
 *  Created on: 28 באפר 2016
 *      Author: batel
 */

#include "../yad3.h"
#include "test_utilities.h"
#include "../apartment_service.h"
#include <string.h>
#include <stdlib.h>
#include "../mtm_ex2.h"


static bool systemUsersCreateForTest(SystemUser* system){
	*system=systemUsersCreate();
	Yad3ErrorCode error;
	ASSERT_TEST(systemUserAddRealtor(*system,"farok@","soda", 2,&error));
	ASSERT_TEST(systemUserAddRealtor(*system,"rinat@","carmona",5,&error));
	ASSERT_TEST(systemUserAddRealtor(*system,"or@","karmisi",15,&error));

	ASSERT_TEST(systemUserAddCustomer(*system,"@JUMP",1,1,300,&error));
	ASSERT_TEST(systemUserAddCustomer(*system,"@NIKOL",2,5,300,&error));
	ASSERT_TEST(systemUserAddCustomer(*system,"@arike",2,5,300,&error));
	ASSERT_TEST(systemUserAddCustomer(*system,"@afi",7,2,300,&error));
	ASSERT_TEST(systemUserAddCustomer(*system,"@Alon",2,8,200,&error));
	ASSERT_TEST(systemUserAddCustomer(*system,"@loli",1,1,50,&error));
	ASSERT_TEST(systemUserAddCustomer(*system,"@liron",3,2,150,&error));
	return true;
}


static bool systemUsersCreateAndDestroyTest() {
	SystemUser system=systemUsersCreate();
	systemUsersDestroy(system);
	systemUsersDestroy(NULL);
	return true;
}
static bool systemUserAddRealtorTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));

	Yad3ErrorCode error;
	ASSERT_TEST(!systemUserAddRealtor(system,"fa@rok@","soda", 7,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddRealtor(system,"fa","soda", 7,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddRealtor(system,"farok@","soda",101,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddRealtor(system,"f@a","soda", 0,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddRealtor(system,"f@a","soda",-1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserAddRealtor(system,"farok@","soda",1,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_ALREADY_EXISTS);
	ASSERT_TEST(!systemUserAddRealtor(system,"farok@","soda",100,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_ALREADY_EXISTS);

	ASSERT_TEST(systemUserAddRealtor(system,"dani@","soda",100,&error));

	ASSERT_TEST(!systemUserAddRealtor(system,"dani@","soda",100,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_ALREADY_EXISTS);
	systemUsersDestroy(system);
	return true;
}
static bool systemUserAddCustomerTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserAddCustomer(NULL,"@JUMP",7,2,0,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,NULL,7,2,0,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserAddCustomer(system,"@JUMP",7,2,0,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,"@JUMP",-7,2,1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,"@JUMP",0,2,1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,"@JUMP",7,-2,1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,"@JUMP",7,0,1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,"@JUMP",7,2,-1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,"@JUM@P",7,2,1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,"",7,2,1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddCustomer(system,"j",7,2,1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);


	ASSERT_TEST(!systemUserAddCustomer(system,"@JUMP",7,2,1,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_ALREADY_EXISTS);

	ASSERT_TEST(systemUserAddCustomer(system,"@dani",7,2,1,&error));
	ASSERT_TEST(systemUserAddCustomer(system,"@",7,2,1,&error));
	ASSERT_TEST(systemUserAddCustomer(system,"@ ",7,2,1,&error));

	ASSERT_TEST(!systemUserAddCustomer(system,"@dani",7,2,1,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_ALREADY_EXISTS);

	systemUsersDestroy(system);
	return true;
}
static bool systemUserRemoveUserTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserRemoveUser(NULL,"@J@UMP",CUSTOMER_USER,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserRemoveUser(system,NULL,CUSTOMER_USER,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserRemoveUser(system,
			"@J@UMP",CUSTOMER_USER,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserRemoveUser(system,"@MP",CUSTOMER_USER,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserRemoveUser(system,
			"farok@",CUSTOMER_USER,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(systemUserRemoveUser(system,"@JUMP",CUSTOMER_USER,&error));
	ASSERT_TEST(systemUserAddCustomer(system,"@JUMP",7,2,1,&error));

	ASSERT_TEST(!systemUserRemoveUser(system,"@J@UMP",REALTOR_USER,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserRemoveUser(system,"@MP",REALTOR_USER,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserRemoveUser(system,"@JUMP",REALTOR_USER,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(systemUserRemoveUser(system,"farok@",REALTOR_USER,&error));
	ASSERT_TEST(systemUserAddRealtor(system,"farok@","soda",100,&error));

	systemUsersDestroy(system);
	return true;
}

static bool systemUserAddToRealtorApartmentServiceTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;
	ASSERT_TEST(!systemUserAddToRealtorApartmentService(NULL,"@batel",
			"sode",0,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,NULL,
			"sode",0,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,"@batel",
			NULL,0,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,"@batel",
			"sode",0,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,"@batel",
			"sode",-1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,"@ba@tel",
			"sode",3,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,"batel",
			"sode",3,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,"",
			"sode",3,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,"@batel_c",
			"sode",3,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserAddToRealtorApartmentService(system,"@JUMP",
			"sode",3,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(!systemUserAddRealtor(system,"farok@","soda",1,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_ALREADY_EXISTS);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board1",3,&error));
	ASSERT_TEST(!systemUserAddToRealtorApartmentService
				(system,"farok@","board1",3,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_ALREADY_EXISTS);

	systemUsersDestroy(system);
	return true;
}
static bool systemUserRemoveApartmentServiceFromeRealtorTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(NULL,"E@@",
			"bbbService",&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,NULL,
			"bbbService",&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,"E@@",
			NULL,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board1",2,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board2",3,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",5,300,2, 2, "ewww",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",4,300,2, 2, "ewww",&error));

	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,
			"E@@","bbbService",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,"E",
			"bbbService",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,"E@",
			"bbbService",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,
			"@JUMP","bbbService",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,
			"farok@","bbbService",&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);


	ASSERT_TEST(systemUserRemoveApartmentServiceFromeRealtor(system,
			"farok@","board2",&error));
	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,
			"farok@","board2",&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserRemoveApartmentServiceFromeRealtor(system,
			"farok@","board1",&error));
	ASSERT_TEST(!systemUserRemoveApartmentServiceFromeRealtor(system,
			"farok@","board1",&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	systemUsersDestroy(system);
	return true;
}
static bool systemUserAddApartmentToRealtorTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(NULL,"farok","soda",5,300,2, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,NULL,"soda",5,300,2, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok",NULL,5,300,2, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok","soda",5,300,2, 2, NULL,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok","soda",5,300,2, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"far@ok@","soda",5,300,2, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",-5,300,2, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,-300,2, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,-7, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,2,- 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,7, 2, "eeww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,2, 2, "eeeww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,2, 2, "eEww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,2, 2, "e ww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,2, 2, "eEwew",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,301,2, 2, "ewww",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"dana@","soda",5,300,2, 2, "ewww",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"@JUMP","soda",5,300,2, 2, "ewww",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,2, 2, "ewww",&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board1",2,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board2",3,&error));

	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","soda",5,300,2, 2, "ewww",&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",5,300,2, 2, "ewww",&error));
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","board1",5,300,2, 2, "ewww",&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_ALREADY_EXISTS);
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",4,300,2, 2, "ewww",&error));
	ASSERT_TEST(!systemUserAddApartmentToRealtor
			(system,"farok@","board1",5,300,2, 2, "ewww",&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_FULL);

	systemUsersDestroy(system);
	return true;
}
static bool systemUserRemoveApartmentFromeRealtorTest() {


	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(NULL,"WWW","hhh",0, &error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,NULL,"hhh",0, &error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"WWW",NULL,0, &error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"WWW","hhh",0, &error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"W@@WW","hhh",0, &error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"WWW","hhh",-1, &error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"W@WW","hhh",0, &error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"@JUMP","hhh",0, &error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"farok@","hhh",0, &error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board1",2,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board2",3,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",5,300,2, 2, "ewww",&error));

	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"farok@","hhh",0, &error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"farok@","board1",0, &error));
	ASSERT_TEST(error==YAD_3_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserRemoveApartmentFromeRealtor
			(system,"farok@","board1",5, &error));
	ASSERT_TEST(!systemUserRemoveApartmentFromeRealtor
			(system,"farok@","board1",5, &error));
	ASSERT_TEST(error==YAD_3_APARTMENT_DOES_NOT_EXIST);

	systemUsersDestroy(system);
	return true;
}
static bool systemUserCustomerMakeOfferTest() {
	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);
	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserCustomerMakeOffer(NULL,"@customer@",
			"realtor@","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,NULL,
			"realtor@","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@customer@",
		NULL,"board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@customer@",
			"realtor@",NULL,5,555,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@customer@",
			"realtor@","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"customer@",
			"r@ealtor@","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"customer@",
			"realtor@","board1",-1,555,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"customer@",
			"realtor@","board1",5,0,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"customer@",
			"realtor@","board1",5,-555,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"customer@",
			"realtor@","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
			"realtor@","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"customer@",
			"farok@","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"farok@",
			"@JUMP","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"farok@",
			"farok@","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
			"@JUMP","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"customer@",
			"@JUMP","board1",5,555,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board1",2,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board2",3,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"rinat@","board1",2,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",5,200,2, 2, "ewew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",0,200,4, 3,
						"eeweeeweeewe",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",1,200,2, 5,
						"ewweewweew",&error));

	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@JUMP",
			"farok@","board1",5,100,&error));

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
			"farok@","board1",5,100,&error));
	ASSERT_TEST(error==YAD_3_ALREADY_REQUESTED);


	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@NIKOL",
			"rinat@","board1",1,100,&error));
	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@afi",
			"rinat@","board1",0,100,&error));

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
			"farok@","board3",5,100,&error));
	ASSERT_TEST(error==YAD_3_ALREADY_REQUESTED);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@afi",
			"farok@","board3",5,100,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@afi",
			"farok@","board1",55,100,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@afi",
			"farok@","board1",5,50,&error));//worng area
	ASSERT_TEST(error==YAD_3_REQUEST_WRONG_PROPERTIES);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@Alon",
			"rinat@","board1",1,50,&error));//worng rooms
	ASSERT_TEST(error==YAD_3_REQUEST_WRONG_PROPERTIES);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@loli",
			"farok@","board1",5,70,&error));//worng price
	ASSERT_TEST(error==YAD_3_REQUEST_WRONG_PROPERTIES);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
			"rinat@","board1",1,210,&error));//worng price
	ASSERT_TEST(error==YAD_3_REQUEST_ILLOGICAL_PRICE);
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
			"rinat@","board1",1,211,&error));//worng price
	ASSERT_TEST(error==YAD_3_REQUEST_ILLOGICAL_PRICE);
	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@JUMP",
			"rinat@","board1",1,209,&error));
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
			"rinat@","board1",0,100,&error));
	ASSERT_TEST(error==YAD_3_ALREADY_REQUESTED);

	ASSERT_TEST(systemUserRemoveUser(system,"farok@",REALTOR_USER,&error));
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
			"farok@","board1",5,100,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@JUMP",
				"farok@","board1",5,100,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserRemoveApartmentServiceFromeRealtor(system,
			"rinat@","board1",&error));

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@NIKOL",
				"rinat@","board1",5,100,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@afi",
				"rinat@","board1",5,100,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"rinat@","board1",2,&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",0,200,4, 3,
						"eeweeeweeewe",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",1,200,2, 5,
						"ewweewweew",&error));

	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@NIKOL",
			"rinat@","board1",1,100,&error));
	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@afi",
			"rinat@","board1",0,100,&error));

	ASSERT_TEST(systemUserRemoveApartmentFromeRealtor
			(system,"rinat@","board1",0, &error));
	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@afi",
				"rinat@","board1",5,100,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_DOES_NOT_EXIST);


	systemUsersDestroy(system);
	return true;
}
static bool systemUserCustomerPurchaseTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);
	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserCustomerPurchase(NULL,"customer@","@realtor@",
					"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerPurchase(system,NULL,"@realtor@",
					"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"customer@",NULL,
					"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"customer@","@realtor@",
					NULL,2,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserCustomerPurchase(system,"customer@","@realtor@",
					"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"@customer@","realtor@",
					"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"customer@","realtor",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"customer","realtor@",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"customer@","realtor@",
						"apartmentBBb",-1,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserCustomerPurchase(system,"customer@","farok@",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"@JUMP","realtor@",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserCustomerPurchase(system,"@JUMP","@afi",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"farok@","@JUMP",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"farok@","or@",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(!systemUserCustomerPurchase(system,"@JUMP","farok@",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"farok@","board1",2,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"farok@","board2",3,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"rinat@","board1",2,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",5,500,2, 2, "ewww",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"farok@","board1",0,200,2, 2, "ewew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",0,200,2, 3, "ewweew",&error));

	ASSERT_TEST(!systemUserCustomerPurchase(system,"@JUMP","farok@",
						"apartmentBBb",2,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(!systemUserCustomerPurchase(system,"@JUMP","farok@",
						"board1",2,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserCustomerPurchase(system,"@JUMP","farok@",
						"board1",5,&error));
	ASSERT_TEST(error==YAD_3_PURCHASE_WRONG_PROPERTIES);//worng price
	ASSERT_TEST(!systemUserCustomerPurchase(system,"@NIKOL","farok@",
						"board1",0,&error));
	ASSERT_TEST(error==YAD_3_PURCHASE_WRONG_PROPERTIES);//worng min rooms
	ASSERT_TEST(!systemUserCustomerPurchase(system,"@afi","rinat@",
						"board1",0,&error));
	ASSERT_TEST(error==YAD_3_PURCHASE_WRONG_PROPERTIES);//worng min area

	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",1,200,2, 5, "ewweewweew",&error));
	ASSERT_TEST(systemUserCustomerPurchase(system,"@NIKOL","rinat@",
						"board1",1,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"rinat@","board1",1,500,2, 2, "ewww",&error));

	bool flag=systemUserReportMostPayingCustomers(system,100,stdout,&error);
	ASSERT_TEST(flag);

	systemUsersDestroy(system);
	return true;
}
static bool systemUserRealtorRespondToOfferTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserRealtorRespondToOffer(NULL,
			"realtor@","@customer@","accept",&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,NULL,"@customer@",
			"accept",&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"realtor@",NULL,
			"accept",&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,
			"realtor@","@customer@",NULL,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,
			"realtor@","@customer@","accept",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,
			"realtor","customer@","accept",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,
			"realtor@","customer@","achhhcept",&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"realtor@",
			"customer@","accept",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"realtor@",
			"@JUMP","accept",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"farok@",
			"customer@","accept",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"@afi","@JUMP",
			"accept",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"@JUMP","farok@",
			"accept",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"or@","farok@",
			"accept",&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"or@","@JUMP",
			"accept",&error));
	ASSERT_TEST(error==YAD_3_NOT_REQUESTED);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board1",2,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board2",3,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"rinat@","board1",5,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",5,200,2, 2, "ewew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",0,200,4, 3,
						"eeweeeweeewe",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",1,200,2, 5,
						"ewweewweew",&error));

	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@NIKOL",
			"rinat@","board1",1,100,&error));
	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@arike",
			"rinat@","board1",1,100,&error));
	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@afi",
			"rinat@","board1",0,100,&error));

	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"farok@","@NIKOL",
			"accept",&error));
	ASSERT_TEST(error==YAD_3_NOT_REQUESTED);
	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"farok@","@NIKOL",
			"decline",&error));
	ASSERT_TEST(error==YAD_3_NOT_REQUESTED);

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@arike",
			"rinat@","board1",0,100,&error));
	ASSERT_TEST(error==YAD_3_ALREADY_REQUESTED);

	ASSERT_TEST(systemUserRealtorRespondToOffer(system,"rinat@","@NIKOL",
			"accept",&error));

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@arike",
			"rinat@","board1",1,100,&error));
	ASSERT_TEST(error==YAD_3_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"rinat@","@NIKOL",
			"decline",&error));
	ASSERT_TEST(error==YAD_3_NOT_REQUESTED);

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"rinat@","board1",1,200,2, 2, "ewew",&error));

	ASSERT_TEST(!systemUserRealtorRespondToOffer(system,"rinat@","@arike",
			"decline",&error));
	ASSERT_TEST(error==YAD_3_NOT_REQUESTED);

	bool flag=systemUserReportMostPayingCustomers(system,12,stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserRealtorRespondToOffer(system,"rinat@","@afi",
			"decline",&error));

	ASSERT_TEST(!systemUserCustomerMakeOffer(system,"@afi",
			"rinat@","board1",1,100,&error));
	ASSERT_TEST(error==YAD_3_REQUEST_WRONG_PROPERTIES);

	ASSERT_TEST(!systemUserAddApartmentToRealtor
				(system,"rinat@","board1",0,200,2, 5, "ewweewweew",&error));
    ASSERT_TEST(error==YAD_3_APARTMENT_ALREADY_EXISTS);

	systemUsersDestroy(system);
	return true;
}

static bool systemUseReportRelevantRealtorsTest() {
	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUseReportRelevantRealtors(NULL," ",
			stdout,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUseReportRelevantRealtors(system,NULL,
			stdout,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUseReportRelevantRealtors(system," ",
			NULL,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUseReportRelevantRealtors(system," ",
			stdout,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUseReportRelevantRealtors(system," @@",
			stdout,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUseReportRelevantRealtors(system,"l @@",
			stdout,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUseReportRelevantRealtors(system,"lll@",
			stdout,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(!systemUseReportRelevantRealtors(system,"farok@",
			stdout,&error));
	ASSERT_TEST(error==YAD_3_EMAIL_WRONG_ACCOUNT_TYPE);

	printf("\ntest00\n");
	bool flag=systemUseReportRelevantRealtors(system,"@liron",
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board1",200,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
			(system,"farok@","board2",200,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",0,200,2, 2,"eewe",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",1,100,2, 2, "ewwe",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board2",0,100,2, 2, "eeee",&error));
	printf("\ntest01\n");
	flag=systemUseReportRelevantRealtors(system,"@liron",
			stdout,&error);
	ASSERT_TEST(flag);
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board2",1,100,2,3, "ewweew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board2",2,100,2,3, "ewweew",&error));
	printf("\ntest1\n");
	flag=systemUseReportRelevantRealtors(system,"@liron",
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"or@","board2",200,&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"or@","board2",2,20000,2,3, "ewweew",&error));
	printf("\ntest2\n");
	flag=systemUseReportRelevantRealtors(system,"@liron",
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"or@","board2",0,100,2,3, "ewweew",&error));
	printf("\ntest3\n");
	flag=systemUseReportRelevantRealtors(system,"@liron",
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"rinat@","board2",200,&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"rinat@","board2",0,100,2,3, "ewweew",&error));
	printf("\ntest4\n");
	flag=systemUseReportRelevantRealtors(system,"@liron",
			stdout,&error);
	ASSERT_TEST(flag);

	systemUsersDestroy(system);
	return true;
}
static bool systemUserReportSignificantRealtorsTest() {
	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserReportSignificantRealtors(NULL,0,
			stdout,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserReportSignificantRealtors(system,0,
			NULL,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	ASSERT_TEST(!systemUserReportSignificantRealtors(system,0,
			stdout,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserReportSignificantRealtors(system,-3,
			stdout,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	printf("\ntest00\n");
	bool flag=systemUserReportSignificantRealtors(system,12,
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"farok@","board2",300,&error));

	printf("\ntest01\n");
	flag=systemUserReportSignificantRealtors(system,12,
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"rinat@","board2",300,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"rinat@","board1",300,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"or@","board2",300,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"or@","board1",300,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"rinat@","board1",1,1000,2, 3, "ewweew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"rinat@","board2",2,1000,2, 3, "ewweew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"rinat@","board2",3,200,2, 3, "ewweew",&error));

	printf("\ntest1\n");
	flag=systemUserReportSignificantRealtors(system,12,
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"or@","board1",1,1000,2, 3, "ewweew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"or@","board2",2,1000,2, 3, "ewweew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"or@","board2",3,200,2, 3, "ewweew",&error));

	printf("\ntest2\n");
	flag=systemUserReportSignificantRealtors(system,12,
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"rinat@","board2",4,1000000,2, 3, "eeeeee",&error));

	printf("\ntest3\n");
	flag=systemUserReportSignificantRealtors(system,12,
			stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board2",4,100,2, 3, "ewweew",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board2",5,100,2, 3, "ewweew",&error));
	printf("\ntest4\n");
	flag=systemUserReportSignificantRealtors(system,12,
			stdout,&error);
	ASSERT_TEST(flag);

	systemUsersDestroy(system);
	return true;
}
static bool systemUserReportMostPayingCustomersTest() {

	SystemUser system=NULL;
	ASSERT_TEST(systemUsersCreateForTest(&system));
	ASSERT_TEST(system!=NULL);

	Yad3ErrorCode error;

	ASSERT_TEST(!systemUserReportMostPayingCustomers(NULL,0,stdout,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);
	ASSERT_TEST(!systemUserReportMostPayingCustomers(system,0,NULL,&error));
	ASSERT_TEST(error==YAD_3_NULL_PARAMETERS);

	printf("\ntest00\n");
	bool flag=systemUserReportMostPayingCustomers(system,12,stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(!systemUserReportMostPayingCustomers(system,0,
			stdout,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);
	ASSERT_TEST(!systemUserReportMostPayingCustomers(system,
			-1,stdout,&error));
	ASSERT_TEST(error==YAD_3_INVALID_PARAMETERS);

	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"farok@","board1",300,&error));
	ASSERT_TEST(systemUserAddToRealtorApartmentService
				(system,"rinat@","board1",300,&error));

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",1,100,2, 3, "ewweew",&error));
	printf("\ntest01\n");
	flag=systemUserReportMostPayingCustomers(system,12,stdout,&error);
	ASSERT_TEST(flag);
	ASSERT_TEST(systemUserCustomerPurchase(system,"@liron","farok@",
						"board1",1,&error));
	printf("\ntest1\n");
	flag=systemUserReportMostPayingCustomers(system,2,stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"farok@","board1",1,200,2,6,
						"ewweewweewwe",&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
				(system,"rinat@","board1",1,1000,2,6,
						"ewweewweewwe",&error));
	ASSERT_TEST(systemUserCustomerPurchase(system,"@JUMP","farok@",
						"board1",1,&error));
	ASSERT_TEST(systemUserAddCustomer(system,"@rich",2,2,1050,&error));
	ASSERT_TEST(systemUserCustomerPurchase(system,"@rich","rinat@",
						"board1",1,&error));
	printf("\ntest2\n");
	flag=systemUserReportMostPayingCustomers(system,2,stdout,&error);
	ASSERT_TEST(flag);

	printf("\ntest3\n");
	flag=systemUserReportMostPayingCustomers(system,12,stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",1,100,2,6, "ewweewweewwe",&error));
	ASSERT_TEST(systemUserCustomerPurchase(system,"@NIKOL","farok@",
						"board1",1,&error));
	printf("\ntest4\n");
	flag=systemUserReportMostPayingCustomers(system,12,stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",1,100,2,6, "ewweewweewwe",&error));
	ASSERT_TEST(systemUserAddCustomer(system,"@NAch",2,2,1050,&error));
	ASSERT_TEST(systemUserCustomerPurchase(system,"@NAch","farok@",
						"board1",1,&error));
	printf("\ntest5\n");
	flag=systemUserReportMostPayingCustomers(system,12,stdout,&error);
	ASSERT_TEST(flag);

	ASSERT_TEST(systemUserAddCustomer(system,"@Zero",2,2,50,&error));
	ASSERT_TEST(systemUserAddApartmentToRealtor
			(system,"farok@","board1",1,100,2,6, "ewweewweewwe",&error));
	ASSERT_TEST(systemUserCustomerMakeOffer(system,"@Zero",
			"farok@","board1",1,50,&error));
	ASSERT_TEST(systemUserRealtorRespondToOffer(system,"farok@","@Zero",
			"accept",&error));
	printf("\ntest6\n");
	flag=systemUserReportMostPayingCustomers(system,12,stdout,&error);
	ASSERT_TEST(flag);

	printf("\ntest7\n");
	flag=systemUserReportMostPayingCustomers(system,2,stdout,&error);
	ASSERT_TEST(flag);

	systemUsersDestroy(system);
	return true;
}

int main() {
	RUN_TEST(systemUsersCreateAndDestroyTest);
	RUN_TEST(systemUserRemoveUserTest);
	RUN_TEST(systemUserAddRealtorTest);
	RUN_TEST(systemUserAddToRealtorApartmentServiceTest);
	RUN_TEST(systemUserRemoveApartmentServiceFromeRealtorTest);
	RUN_TEST(systemUserAddCustomerTest);
	RUN_TEST(systemUserAddApartmentToRealtorTest);
	RUN_TEST(systemUserRemoveApartmentFromeRealtorTest);
	RUN_TEST(systemUserCustomerMakeOfferTest);
	RUN_TEST(systemUserCustomerPurchaseTest);
	RUN_TEST(systemUserRealtorRespondToOfferTest);
	RUN_TEST(systemUserRealtorRespondToOfferTest);
	RUN_TEST(systemUserReportMostPayingCustomersTest);
	RUN_TEST(systemUserReportSignificantRealtorsTest);
	RUN_TEST(systemUseReportRelevantRealtorsTest);


	return 0;
}
