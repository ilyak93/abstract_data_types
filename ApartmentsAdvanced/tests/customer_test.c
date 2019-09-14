
#include "../customer.h"
#include "test_utilities.h"
#include <string.h>
#include <stdlib.h>

/************************* Customer Tests *******************************/

static bool customerCreateTest(){
	CustomerResult result=CUSTOMER_SUCCESS;

	ASSERT_TEST(customerCreate(8,8,80,"jjj",&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerCreate(8,8,0,"j@jj",&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerCreate(8,0,80,"j@jj",&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerCreate(0,8,80,"j@jj",&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_INVALID_INPUT);

	Customer customer= customerCreate(8,8,80,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);
	customerDestroy(customer);

	return true;
}

static bool customerDestroyTest(){

	customerDestroy(NULL);
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(8,8,80,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);
	customerDestroy(customer);

	return true;
}

static bool customerCopyTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(1,1,300,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerMakeOffer(customer,"@ll",
			"batel",2,2,150,120,0)==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerGetMinArea(customer)==1 &&
	            customerGetMinRooms(customer)==1 &&
	            customerGetMaxPrice(customer)==300&&
				customerGetAmountPurchases(customer)==0);

	Customer customer_copy=customerCopy(customer);
	char*email=customerGetEmail(customer);
	ASSERT_TEST(strcmp(email,"j@jj")==0);


	ASSERT_TEST(customerGetMinArea(customer)==
			    customerGetMinArea(customer_copy));
	ASSERT_TEST(customerGetMinRooms(customer)==
			customerGetMinRooms(customer_copy));
	ASSERT_TEST(customerGetMaxPrice(customer)==
			customerGetMaxPrice(customer_copy));

	char*email_copy=customerGetEmail(customer_copy);
	ASSERT_TEST(strcmp(email_copy,email)==0);
	free(email);
	free(email_copy);

	ASSERT_TEST(customerGetAmountPurchases(customer)==
			customerGetAmountPurchases(customer_copy));

	ASSERT_TEST(customerMakeOffer(customer_copy,"@ll",
			"hhh",5,5, 100, 90,2)==CUSTOMER_ALREADY_REQUESTED);


	customerDestroy(customer);
	customerDestroy(customer_copy);

	return true;
}

static bool customerGetMinAreaTest(){
	ASSERT_TEST(customerGetMinArea(NULL)==-1);

	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(9,8,80,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerGetMinArea(customer)==9);
	customerDestroy(customer);

	return true;
}

static bool customerGetMinRoomsTest(){
	ASSERT_TEST(customerGetMinRooms(NULL)==-1);

	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(9,8,80,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerGetMinRooms(customer)==8);
	customerDestroy(customer);

	return true;
}

static bool customerGetMaxPriceTest(){
	ASSERT_TEST(customerGetMaxPrice(NULL)==-1);
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(9,8,80,"j@jj",&result);

	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerGetMaxPrice(customer)==80);

	customerDestroy(customer);

	return true;
}

static bool customerGetEmailTest(){
	ASSERT_TEST(customerGetEmail(NULL)==NULL);

	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(9,8,80,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	char*email=customerGetEmail(customer);
	ASSERT_TEST(strcmp(email,"j@jj")==0);
	free(email);

	customerDestroy(customer);

	return true;
}

static bool customerGetAmountPurchasesTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(9,8,80,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerMakePurchase("@gg",
			customer,"hhh",5,8,9,80)==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerMakePurchase("@gg",
			customer,"hhh",5,8,9,30)==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerGetAmountPurchases(customer)==110);

	ASSERT_TEST(customerGetAmountPurchases(NULL)==-1);

	customerDestroy(customer);
	return true;
}

static bool customerMakeOfferTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(9,8,80,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerMakeOffer(NULL,"@jj",
		"hhh",1, 1, 80, 1,0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,NULL,
		"hhh",1, 1, 80, 1,0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"jj",
		"hhh",1, 1, 80, 1,0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		NULL, 1, 1, 80, 1, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",1, 1, 0, 1,0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",1, 1, -1, 0, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",1, 1, 1, 0, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",1, 1, 1, -1, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",1, 1, 1, 80, -1)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh", -1, 1, 1, 80, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh", 1, -2, 1, 80, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(NULL,"@jj",
		"hhh", 1, 1, 0, 80, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,NULL,
		"hhh", 1, 1, 80, 100, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(NULL,"jj@",
		"hhh", 1, 1, 80, 80, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(NULL,"jj@",
		"hhh", 7, 9, 80, 79, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,NULL,
		"hhh", 8, 8, 80, 79, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",8, 9, 79, 78, 2)==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"@jj")==
		CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"jjj",10, 20, 60, 80,2)==CUSTOMER_ALREADY_REQUESTED);

	ASSERT_TEST(customerMakeOffer(NULL,"@jj",
		"hhh",1, 1, 78, 80,0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer, "@jj",
		NULL,1, 1, 78, 80,0)==CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh", -5, 1, 78, 80, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh", 1, -10, 78, 80, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh", 1, 1, 0, 78, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh", 1, 1, 50, 0, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh", 1, 1, 82, 0, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(NULL,"@jj",
		"hhh", 1, 1, 80, 78, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		NULL, 1, 1, 79, 78, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(customer,NULL,
		"hhh", 7, 9, 79, 78, 0)==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakeOffer(NULL,"@jj",
		"hhh", 8, 8, 80, 78, 0)==CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",7, 9, 80, 78,0)==CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",8, 8, 78, 76,0)==CUSTOMER_ALREADY_REQUESTED);

	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",8, 9, 82, 78,0)==CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
		"hhh",8, 9, 82, 83,0)==CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerMakeOffer(customer,"@jj",
			"hhh",8, 9, 83, 78,0)==CUSTOMER_ALREADY_REQUESTED);

	ASSERT_TEST(customerMakeOffer(customer,"j@j",
		"hhh",1, 1, 83, 82, 0)==CUSTOMER_REQUEST_WRONG_PROPERTIES);
	ASSERT_TEST(customerMakeOffer(customer,"j@j",
		"hhh",8, 8, 82, 78, 0)==CUSTOMER_REQUEST_WRONG_PROPERTIES);
	ASSERT_TEST(customerMakeOffer(customer,"j@j",
			"hhh",8, 8, 80, 81, 0)==CUSTOMER_REQUEST_WRONG_PROPERTIES);

	ASSERT_TEST(customerMakeOffer(customer,"j@j",
		"hhh",8, 9, 76, 79,0)== CUSTOMER_REQUEST_ILLOGICAL_PRICE);



	customerDestroy(customer);
	return true;
}

static bool customerCheckIfOfferExistTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(9,8,80,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerMakeOffer(customer,"@ll", "batel",8,9,78,50,0)
			==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"@ll")
				==CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerCheckIfOfferExist(NULL,"@ll")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"ll")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerCheckIfOfferExist(customer,NULL)
			==CUSTOMER_INVALID_INPUT);

	customerDestroy(customer);
	return true;
}

static bool customerLexicographicalEmailTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer1= customerCreate(9,8,80,"a@jj",&result);
	ASSERT_TEST(customer1!=NULL && result==CUSTOMER_SUCCESS);

	Customer customer2= customerCreate(9,8,80,"b@jj",&result);
	ASSERT_TEST(customer2!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerLexicographicalEmail(customer1,customer2)<0);
	ASSERT_TEST(customerLexicographicalEmail(customer2,customer1)>0);
	ASSERT_TEST(customerLexicographicalEmail(customer1,customer1)==0);

	customerDestroy(customer2);
	customerDestroy(customer1);
	return true;
}

static bool customerRemoveOfferAccordingEmailTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(1,1,300,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerRemoveOfferAccordingEmail(customer,"g")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingEmail(customer,"g@@")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingEmail(NULL,"@g")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingEmail(customer,NULL)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingEmail(customer,"@J")
			==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerMakeOffer(customer,"j@jj",
			"batel",1,1,100,80,0)==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"j@jj")
				==CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerRemoveOfferAccordingEmail(customer,"j@jj")
			==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"j@jj")
				==CUSTOMER_NOT_REQUESTED);

	customerDestroy(customer);
	return true;
}
static bool customerRemoveOfferAccordingServiceTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(1,1,300,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerRemoveOfferAccordingService(customer,"g","GGG")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingService(customer,"g@@","GGG")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingService(NULL,"@g","GGG")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingService(customer,NULL,"GGG")
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingService(customer,"@g",NULL)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingService(customer,"@g","LLL")
			==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerMakeOffer(customer,"@G",
			"LLL",10,20, 40,20,0)==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"@G")
				==CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerRemoveOfferAccordingService(customer,"@G","LLL")
			==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"@G")
				==CUSTOMER_NOT_REQUESTED);

	customerDestroy(customer);
	return true;
}

static bool customerRemoveOfferAccordingIdTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(1,1,300,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerRemoveOfferAccordingId(customer,"g","GGG",8)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingId(customer,"g@@","GGG",8)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingId(NULL,"@g","GGG",8)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingId(customer,NULL,"GGG",8)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingId(customer,"@g",NULL,8)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingId(customer,"@g","JJ",-1)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerRemoveOfferAccordingId(customer,"@g","LLL",0)
			==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerMakeOffer(customer,"@G",
			"LLL",50, 50, 70, 1,0)==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"@G")
				==CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerRemoveOfferAccordingId(customer,"@G","LLL",0)
			==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer,"@G")
				==CUSTOMER_NOT_REQUESTED);

	customerDestroy(customer);
	return true;
}

static bool customerMakePurchaseTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(5,5,300,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerMakePurchase(NULL,customer,"hh",0, 5,5,  5)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchase("@hhh",NULL,"hh",0, 5, 5, 5)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchase("@hhh",customer,NULL,0, 5, 5, 5)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchase("hhh",customer,"hh",0, 5, 5, 5)
			==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchase("@hhh",customer,"hh",-1, 5, 5, 5)
				==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchase("@hhh",customer,"hh",0, 5, 5, 0)
				==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchase("@hhh",customer,"hh",0, 5, 5, -1)
				==CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerMakePurchase("@hhh", customer, "hh", 3, 2, 5, 1)
		== CUSTOMER_PURCHASE_WRONG_PROPERTIES);
	ASSERT_TEST(customerMakePurchase("@hhh", customer, "hh", 2, 2, 5, 3)
		== CUSTOMER_PURCHASE_WRONG_PROPERTIES);
	ASSERT_TEST(customerMakePurchase("@hhh", customer, "hh", 2, 5, 5, 400)
		== CUSTOMER_PURCHASE_WRONG_PROPERTIES);

	ASSERT_TEST(customerMakePurchase(NULL, customer, "hh", 3, 2, 5, 1)
		== CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchase("@hhh", NULL, "hh", 2, 2, 5, 3)
		== CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchase("@hhh", customer, NULL, 2, 5, 5, 400)
		== CUSTOMER_INVALID_INPUT);


	ASSERT_TEST(customerMakePurchase("@hhh",customer,"hh",0, 5, 5, 200)
				==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerMakePurchase("@hhh",customer,"hh",0, 5, 5, 300)
				==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerGetAmountPurchases(customer)==500);

	customerDestroy(customer);
	return true;
}

static bool customerMakePurchaseByOfferTest() {
	CustomerResult result = CUSTOMER_SUCCESS;
	Customer customer = customerCreate(5, 5, 300, "@jjj", &result);
	ASSERT_TEST(customer != NULL && result == CUSTOMER_SUCCESS);

	ASSERT_TEST(customerMakePurchaseByOffer(NULL, customer, "hhh", 0)
		== CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchaseByOffer("@jjj", NULL, "hhh", 0)
		== CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchaseByOffer("@jjj", customer, NULL, 0)
		== CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchaseByOffer("@jjj", customer, "hhh", -1)
		== CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerMakePurchaseByOffer("jjj", customer, "hhh", 0)
		== CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerMakeOffer(customer, "@jjj",
		"hhh", 10, 10, 200, 100, 0) == CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer, "@jjj") ==
			CUSTOMER_ALREADY_REQUESTED);
	ASSERT_TEST(customerMakePurchaseByOffer("@jjj", customer, "hhh", 0)
		== CUSTOMER_SUCCESS);
	ASSERT_TEST(customerCheckIfOfferExist(customer, "@jjj") ==
			CUSTOMER_NOT_REQUESTED);


	customerDestroy(customer);
	return true;
}

static bool customerOfferGetPriceTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(1,1,300,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);
	int price=0;
	ASSERT_TEST(customerOfferGetPrice(NULL,"@l",&price)
			 ==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerOfferGetPrice(customer,"@l",NULL)
			 ==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerOfferGetPrice(customer,NULL,&price)
			 ==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerOfferGetPrice(customer,"@@l",&price)
			 ==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerOfferGetPrice(customer,"l",&price)
			 ==CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerMakeOffer(customer,"@G",
			"LLL", 5, 10, 250, 120,0)==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerOfferGetPrice(customer,"@G",&price)
			==CUSTOMER_SUCCESS);
	ASSERT_TEST(price==120);


	customerDestroy(customer);
	return true;
}

static bool customerOfferGetServiceNameTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(1,1,300,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	ASSERT_TEST(customerOfferGetServiceName(NULL,"hh@",&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerOfferGetServiceName(customer,NULL,&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerOfferGetServiceName(customer,"hh",&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerOfferGetServiceName(customer,"h@h@",&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_INVALID_INPUT);
	ASSERT_TEST(customerOfferGetServiceName(customer,"h@",&result)==NULL);
	ASSERT_TEST(result==CUSTOMER_NOT_REQUESTED);

	ASSERT_TEST(customerMakeOffer(customer,"@G",
			"LLL",3,5,150,120,0)==CUSTOMER_SUCCESS);
	char* name=customerOfferGetServiceName(customer,"@G",&result);
	ASSERT_TEST(result==CUSTOMER_SUCCESS&& name);

	ASSERT_TEST(strcmp(name,"LLL")==0);
	free(name);

	customerDestroy(customer);
	return true;
}

static bool customerOfferGetIdTest(){
	CustomerResult result=CUSTOMER_SUCCESS;
	Customer customer= customerCreate(1,1,300,"j@jj",&result);
	ASSERT_TEST(customer!=NULL && result==CUSTOMER_SUCCESS);

	 int id=0;
	 ASSERT_TEST(customerOfferGetId(NULL,"@l",&id)
			 ==CUSTOMER_INVALID_INPUT);
	 ASSERT_TEST(customerOfferGetId(customer,NULL,&id)
			 ==CUSTOMER_INVALID_INPUT);
	 ASSERT_TEST(customerOfferGetId(customer,"@l",NULL)
			 ==CUSTOMER_INVALID_INPUT);
	 ASSERT_TEST(customerOfferGetId(customer,"@@l",&id)
			 ==CUSTOMER_INVALID_INPUT);
	 ASSERT_TEST(customerOfferGetId(customer,"l",&id)
			 ==CUSTOMER_INVALID_INPUT);

	ASSERT_TEST(customerMakeOffer(customer,"@G",
			"LLL",3,6,300,120,0)==CUSTOMER_SUCCESS);
	ASSERT_TEST(customerOfferGetId(customer,"@G",&id)
			==CUSTOMER_SUCCESS);
	ASSERT_TEST(id==0);

	customerDestroy(customer);
	return true;
}

int main(){
	RUN_TEST(customerCreateTest);
	RUN_TEST(customerDestroyTest);
	RUN_TEST(customerCopyTest);
	RUN_TEST(customerGetMinAreaTest);
	RUN_TEST(customerGetMinRoomsTest);
	RUN_TEST(customerGetMaxPriceTest);
	RUN_TEST(customerGetEmailTest);
	RUN_TEST(customerGetAmountPurchasesTest);
	RUN_TEST(customerMakeOfferTest);
	RUN_TEST(customerMakePurchaseTest);
	RUN_TEST(customerMakePurchaseByOfferTest);
	RUN_TEST(customerCheckIfOfferExistTest);
	RUN_TEST(customerLexicographicalEmailTest);
	RUN_TEST(customerRemoveOfferAccordingEmailTest);
	RUN_TEST(customerRemoveOfferAccordingServiceTest);
	RUN_TEST(customerRemoveOfferAccordingIdTest);
	RUN_TEST(customerOfferGetPriceTest);
	RUN_TEST(customerOfferGetServiceNameTest);
	RUN_TEST(customerOfferGetIdTest);

	return 0;
}
