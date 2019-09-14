
#include "../realtor.h"
#include "test_utilities.h"
#include <string.h>
#include <stdlib.h>

//***************************Static function******************************

static void freeSquare(SquareType**squares, int length){
	if (squares == NULL) {
		return;
	}
	for (int i = 0; i < length; i++) {
		free(squares[i]);
	}
	free(squares);
}

static SquareType** createSquaresEmpty(int length, int width){
	SquareType**squares =malloc(length * sizeof(*squares));
	if (squares == NULL) {
		return NULL;
	}
	for (int i = 0; i < length; i++) {
		squares[i] =malloc(width * sizeof(**squares));
		if (squares[i] == NULL) {
			freeSquare(squares, i);
			free(squares);
			return NULL;
		}
	}
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			squares[i][j] = EMPTY;
		}
	}
	return squares;
}

/************************* Realtor Tests *******************************/
static bool realtorDestroyTest(){
	realtorDestroy(NULL);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);
	realtorDestroy(realtor);
	return true;
}

static bool realtorCreateTest(){

	RealtorResult result=REALTOR_SUCCESS;

	Realtor realtor= realtorCreate("shsiBar",0,"@b",&result);
	ASSERT_TEST( realtor==NULL && result==REALTOR_INVALID_INPUT);

	realtor= realtorCreate("shsiBar",3,"b",&result);
	ASSERT_TEST( realtor==NULL && result==REALTOR_INVALID_INPUT);

	realtor= realtorCreate("shsiBar",3,"@@j",&result);
	ASSERT_TEST( realtor==NULL && result==REALTOR_INVALID_INPUT);

	realtor= realtorCreate("shsiBar",101,"@b",&result);
	ASSERT_TEST( realtor==NULL && result==REALTOR_INVALID_INPUT);

	realtor= realtorCreate(NULL,5,"@b",&result);
	ASSERT_TEST( realtor==NULL && result==REALTOR_INVALID_INPUT);

	realtor= realtorCreate("shsiBar",5,NULL,&result);
	ASSERT_TEST( realtor==NULL && result==REALTOR_INVALID_INPUT);

	realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	realtorDestroy(realtor);

	return true;
}

static bool realtorCopyTest(){

	Realtor realtor=realtorCopy(NULL);
	ASSERT_TEST( realtor==NULL);

	RealtorResult result=REALTOR_SUCCESS;
	realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);
	ASSERT_TEST(realtorAddApartmentService(realtor,"bbbb",5)
			==REALTOR_SUCCESS);

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);
	ASSERT_TEST(realtorAddApartment(realtor,
			"bbbb",apartment,555)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetRating(realtor)==1100100);

	Realtor realtor_copy=realtorCopy(realtor);
	ASSERT_TEST( realtor!=NULL);

	char*name1=realtorGetCompanyName(realtor);
	char*name2=realtorGetCompanyName(realtor_copy);
	ASSERT_TEST(strcmp(name1,name2)==0);
	free(name1);
	free(name2);

	ASSERT_TEST(realtorGetTaxPercentage(realtor)==
				realtorGetTaxPercentage(realtor_copy));

	ASSERT_TEST(realtorGetAmountApartment(realtor)==
				realtorGetAmountApartment(realtor_copy));

	char*email1=realtorGetEmail(realtor);
	char*email2=realtorGetEmail(realtor_copy);
	ASSERT_TEST(strcmp(email1,email2)==0);
	free(email1);
	free(email2);

	ASSERT_TEST(realtorGetRating(realtor)==
				realtorGetRating(realtor_copy));

	apartmentDestroy(apartment);
	realtorDestroy(realtor);
	realtorDestroy(realtor_copy);
	return true;
}

static bool realtorGetCompanyNameTest(){

	ASSERT_TEST(realtorGetCompanyName(NULL)==NULL);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	char*name=realtorGetCompanyName(realtor);
	ASSERT_TEST(strcmp(name,"shsiBar")==0);
	free(name);
	realtorDestroy(realtor);
	return true;
}
static bool realtorGetTaxPercentageTest(){

	ASSERT_TEST(realtorGetTaxPercentage(NULL)==-1);

	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetTaxPercentage(realtor)==5);
	realtorDestroy(realtor);
	return true;
}

static bool realtorGetAmountApartmentTest(){

	ASSERT_TEST(realtorGetAmountApartment(NULL)==-1);

	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);


	ASSERT_TEST(realtorGetAmountApartment(realtor)==0);

	ASSERT_TEST(realtorAddApartmentService(realtor,"bbbb",5)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorAddApartmentService(realtor,"ggg",5)
			==REALTOR_SUCCESS);

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);
	ASSERT_TEST(realtorAddApartment(realtor,
			"bbbb",apartment,555)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorAddApartment(realtor,
			"bbbb",apartment,777)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorAddApartment(realtor,
				"ggg",apartment,777)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==3);

	apartmentDestroy(apartment);
	realtorDestroy(realtor);
	return true;
}

static bool realtorGetEmailTest(){
	ASSERT_TEST(realtorGetEmail(NULL)==NULL);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	char*email=realtorGetEmail(realtor);
	ASSERT_TEST(strcmp(email,"@b")==0);
	free(email);

	realtorDestroy(realtor);
	return true;
}

static bool realtorGetRatingTest(){

	ASSERT_TEST(realtorGetRating(NULL)==-1);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetRating(realtor)==0);

	ASSERT_TEST(realtorAddApartmentService(realtor,"bbbb",5)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetRating(realtor)==0);

	ASSERT_TEST(realtorAddApartmentService(realtor,"ggg",5)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetRating(realtor)==0);

	ASSERT_TEST(realtorAddApartmentService(realtor,"lll",5)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetRating(realtor)==0);

	SquareType**squares = createSquaresEmpty(4, 4);
	Apartment apartment1 = apartmentCreate(squares, 4, 4, 100);

	ASSERT_TEST(realtorAddApartment(realtor,
			"bbbb",apartment1,555)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetRating(realtor)==2600100);

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment2 = apartmentCreate(&squarePtr, 1, 2, 200);
	ASSERT_TEST(realtorAddApartment(realtor,
				"ggg",apartment2,444)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetRating(realtor)==2800150);

	freeSquare(squares,4);
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	realtorDestroy(realtor);
	return true;
}

static bool realtorServiceNameExistTest(){
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorAddApartmentService(realtor,"bbbb",5)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorAddApartmentService(realtor,"gggg",5)
			==REALTOR_SUCCESS);

	ASSERT_TEST(realtorServiceNameExist(realtor,"bbbb")==true);
	ASSERT_TEST(realtorServiceNameExist(realtor,"gggg")==true);
	ASSERT_TEST(realtorServiceNameExist(realtor,"kkk")==false);

	realtorDestroy(realtor);
	return true;
}

static bool realtorAddApartmentServiceTest(){

	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorAddApartmentService(NULL,"bbbb",5)
			==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorAddApartmentService(realtor,NULL,5)
			==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorAddApartmentService(realtor,"jjj",0)
			==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorAddApartmentService(realtor,"jjj",-1)
			==REALTOR_INVALID_INPUT);

	ASSERT_TEST(realtorAddApartmentService(realtor,"bbbb",5)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorAddApartmentService(realtor,"bbbb",5)
			==REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS);
	ASSERT_TEST(realtorAddApartmentService(realtor,"bbbb",3)
			==REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS);

	realtorDestroy(realtor);
	return true;
}

static bool realtorRemoveApartmentServiceTest(){

	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorRemoveApartmentService(NULL,"bbbb")
			==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorRemoveApartmentService(realtor,NULL)
			==REALTOR_INVALID_INPUT);

	ASSERT_TEST(realtorRemoveApartmentService(realtor,"hhh")
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(realtorAddApartmentService(realtor,"hhhh",5)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorRemoveApartmentService(realtor,"hhhh")
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorRemoveApartmentService(realtor,"hhh")
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	realtorDestroy(realtor);
	return true;

}

static bool realtorAddApartmentTest(){
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorAddApartment(realtor,
			"bbbb",NULL,555)==REALTOR_INVALID_INPUT);

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 200);
	ASSERT_TEST(apartment!=NULL);

	ASSERT_TEST(realtorAddApartment(NULL,
				"bbbb",apartment,555)==REALTOR_INVALID_INPUT);

	ASSERT_TEST(realtorAddApartment(realtor,
				NULL,apartment,555)==REALTOR_INVALID_INPUT);

	ASSERT_TEST(realtorAddApartment(realtor,
				"bbbb",apartment,-10)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorAddApartment(realtor,"bbbb",apartment,0)
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(realtorGetAmountApartment(realtor)==0);
	ASSERT_TEST(realtorAddApartmentService(realtor,"hhhh",2)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==0);
	ASSERT_TEST(realtorAddApartment(realtor,"bbbb",apartment,555)==
			REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==0);
	ASSERT_TEST(realtorAddApartment(realtor,"hhhh",apartment,555)==
			REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==1);
	ASSERT_TEST(realtorAddApartment(realtor,"hhhh",apartment,555)==
			REALTOR_APARTMENT_ALREADY_EXISTS);
	ASSERT_TEST(realtorAddApartment(realtor,"hhhh",apartment,777)==
			REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==2);
	ASSERT_TEST(realtorAddApartment(realtor,"hhhh",apartment,555)==
			REALTOR_APARTMENT_SERVICE_FULL);
	ASSERT_TEST(realtorAddApartment(realtor,"hhhh",apartment,444)==
			REALTOR_APARTMENT_SERVICE_FULL);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==2);

	apartmentDestroy(apartment);
	realtorDestroy(realtor);

	return true;
}

static bool realtorRemoveApartmentTest(){

	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 200);
	ASSERT_TEST(apartment!=NULL);

	ASSERT_TEST(realtorRemoveApartment(realtor,"hhhh",-2)
				==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorRemoveApartment(NULL,"hhhh",2)
			==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorRemoveApartment(realtor,NULL,2)
				==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorRemoveApartment(realtor,"gggg",0)
				==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==0);
	ASSERT_TEST(realtorAddApartmentService(realtor,"hhhh",2)
				==REALTOR_SUCCESS);
	ASSERT_TEST(realtorRemoveApartment(realtor,"gggg",0)
				==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(realtorRemoveApartment(realtor,"hhhh",0)
				==REALTOR_APARTMENT_DOES_NOT_EXIST);
	ASSERT_TEST(realtorAddApartment(realtor,"hhhh",apartment,555)==
				REALTOR_SUCCESS);
	ASSERT_TEST(realtorAddApartment(realtor,"hhhh",apartment,777)==
				REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==2);
	ASSERT_TEST(realtorRemoveApartment(realtor,"hhhh",555)
				==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==1);
	ASSERT_TEST(realtorRemoveApartment(realtor,"hhhh",777)
				==REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetAmountApartment(realtor)==0);

	apartmentDestroy(apartment);
	realtorDestroy(realtor);

	return true;
}


static bool realtorApartmentExistInTheServiceTest(){

	ASSERT_TEST(realtorApartmentExistInTheService(NULL
			,"hhh",888)==REALTOR_INVALID_INPUT);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorApartmentExistInTheService(realtor
			,NULL,888)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorApartmentExistInTheService(realtor
			,"hhh",-1)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorApartmentExistInTheService(realtor
			,"hhh",888)==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(realtorAddApartmentService(realtor,"hhh",2)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorApartmentExistInTheService(realtor
			,"hhjjh",888)==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(realtorApartmentExistInTheService(realtor
			,"hhh",888)==REALTOR_APARTMENT_DOES_NOT_EXIST);

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 200);
	ASSERT_TEST(apartment!=NULL);
	ASSERT_TEST(realtorAddApartment(realtor,"hhh",apartment,555)==
			REALTOR_SUCCESS);
	ASSERT_TEST(realtorApartmentExistInTheService(realtor
			,"hhh",888)==REALTOR_APARTMENT_DOES_NOT_EXIST);
	ASSERT_TEST(realtorApartmentExistInTheService(realtor
			,"hhh",555)==REALTOR_SUCCESS);
	apartmentDestroy(apartment);
	realtorDestroy(realtor);
	return true;

}

static bool realtorGetApartmentDetailsTest(){

	int area=0,rooms=0;
	ASSERT_TEST(realtorGetApartmentDetails(NULL
			,"hhh",888,&area,&rooms)==REALTOR_INVALID_INPUT);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentDetails(realtor
			,NULL,888,&area,&rooms)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorGetApartmentDetails(realtor
			,"hhh",-1,&area,&rooms)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorGetApartmentDetails(realtor
				,"hhh",2,NULL,&rooms)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorGetApartmentDetails(realtor
				,"hhh",2,&area,NULL)==REALTOR_INVALID_INPUT);

	ASSERT_TEST(realtorGetApartmentDetails(realtor
			,"hhh",888,&area,&rooms)
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(realtorAddApartmentService(realtor,"hhh",2)
			==REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentDetails(realtor
			,"hhHHHh",888,&area,&rooms)
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(realtorGetApartmentDetails(realtor
			,"hhh",888,&area,&rooms)
			==REALTOR_APARTMENT_DOES_NOT_EXIST);

	SquareType square[5] = { WALL, EMPTY,EMPTY,WALL,EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 5, 200);
	ASSERT_TEST(apartment!=NULL);
	ASSERT_TEST(realtorAddApartment(realtor,"hhh",apartment,0)==
			REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentDetails(realtor
			,"hhh",888,&area,&rooms)
			==REALTOR_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST(realtorGetApartmentDetails(realtor
			,"hhh",0,&area,&rooms)
			==REALTOR_SUCCESS);

	ASSERT_TEST(area==3 && rooms==2);
	apartmentDestroy(apartment);
	realtorDestroy(realtor);
	return true;

}

static bool realtorGetApartmentPriceTest(){

	int price=0;
	ASSERT_TEST(realtorGetApartmentPrice(NULL
			,"hhh",888,&price)==REALTOR_INVALID_INPUT);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentPrice(realtor
			,NULL,888,&price)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorGetApartmentPrice(realtor
			,"hhh",-1,&price)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorGetApartmentPrice(realtor
				,"hhh",2,NULL)==REALTOR_INVALID_INPUT);

	ASSERT_TEST(realtorGetApartmentPrice(realtor
			,"hhh",888,&price)
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(realtorAddApartmentService(realtor,"hhh",2)
			==REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentPrice(realtor
			,"hhHHHh",888,&price)
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(realtorGetApartmentPrice(realtor
			,"hhh",888,&price)==REALTOR_APARTMENT_DOES_NOT_EXIST);

	SquareType square[5] = { WALL, EMPTY,EMPTY,WALL,EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 5, 200);
	ASSERT_TEST(apartment!=NULL);
	ASSERT_TEST(realtorAddApartment(realtor,"hhh",apartment,0)==
			REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentPrice(realtor
			,"hhh",888,&price)
			==REALTOR_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST(realtorGetApartmentPrice(realtor
			,"hhh",0,&price)
			==REALTOR_SUCCESS);

	ASSERT_TEST(price==200);
	apartmentDestroy(apartment);
	realtorDestroy(realtor);
	return true;

}

static bool realtorGetApartmentPriceWithFeeTest(){

	int price=0;
	ASSERT_TEST(realtorGetApartmentPriceWithFee(NULL
			,"hhh",888,&price)==REALTOR_INVALID_INPUT);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentPriceWithFee(realtor
			,NULL,888,&price)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorGetApartmentPriceWithFee(realtor
			,"hhh",-1,&price)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorGetApartmentPriceWithFee(realtor
				,"hhh",2,NULL)==REALTOR_INVALID_INPUT);

	ASSERT_TEST(realtorGetApartmentPriceWithFee(realtor
			,"hhh",888,&price)
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(realtorAddApartmentService(realtor,"hhh",2)
			==REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentPriceWithFee(realtor
			,"hhHHHh",888,&price)
			==REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST(realtorGetApartmentPriceWithFee(realtor
			,"hhh",888,&price)==REALTOR_APARTMENT_DOES_NOT_EXIST);

	SquareType square[5] = { WALL, EMPTY,EMPTY,WALL,EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 5, 352);
	ASSERT_TEST(apartment!=NULL);
	ASSERT_TEST(realtorAddApartment(realtor,"hhh",apartment,0)==
			REALTOR_SUCCESS);

	ASSERT_TEST(realtorGetApartmentPriceWithFee(realtor
			,"hhh",888,&price)
			==REALTOR_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST(realtorGetApartmentPriceWithFee(realtor
			,"hhh",0,&price)
			==REALTOR_SUCCESS);

	ASSERT_TEST(price==369);
	apartmentDestroy(apartment);
	realtorDestroy(realtor);
	return true;
}

static bool realtorExsistSuitableApartmentTest(){

	ASSERT_TEST(realtorExsistSuitableApartment(NULL
			,2,2,2)==REALTOR_INVALID_INPUT);
	RealtorResult result=REALTOR_SUCCESS;
	Realtor realtor= realtorCreate("shsiBar",5,"@b",&result);
	ASSERT_TEST( realtor!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,-2,2,2)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,2,-2,2)==REALTOR_INVALID_INPUT);
	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,2,2,-2)==REALTOR_INVALID_INPUT);

	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,2,2,2)==REALTOR_NO_RELEVANT_APARTMENT);
	ASSERT_TEST(realtorAddApartmentService(realtor,"hhh",2)
			==REALTOR_SUCCESS);
	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,2,2,2)==REALTOR_NO_RELEVANT_APARTMENT);

	SquareType square[5] = { WALL, EMPTY,EMPTY,WALL,EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 5, 200);
	ASSERT_TEST(apartment!=NULL);
	ASSERT_TEST(realtorAddApartment(realtor,"hhh",apartment,0)==
			REALTOR_SUCCESS);
	apartmentDestroy(apartment);


	SquareType**squares = createSquaresEmpty(4, 4);
	apartment = apartmentCreate(squares, 4, 4, 100);
	ASSERT_TEST(realtorAddApartment(realtor,"hhh",apartment,2)==
			REALTOR_SUCCESS);

	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,2,2,50)==REALTOR_NO_RELEVANT_APARTMENT);
	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,156,2,101)==REALTOR_NO_RELEVANT_APARTMENT);
	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,4,156,100)==REALTOR_NO_RELEVANT_APARTMENT);

	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,16,1,200)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,3,2,500)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,5,1,560)==REALTOR_SUCCESS);
	ASSERT_TEST(realtorExsistSuitableApartment(realtor
			,1,2,301)==REALTOR_SUCCESS);

	apartmentDestroy(apartment);
	realtorDestroy(realtor);
	freeSquare(squares,4);
	return true;
}

static bool realtorLexicographicalEmailTest(){
	RealtorResult result=REALTOR_SUCCESS;

	Realtor realtor1= realtorCreate("shsiBar",5,"a@",&result);
	ASSERT_TEST( realtor1!=NULL && result==REALTOR_SUCCESS);

	Realtor realtor2= realtorCreate("shsiBar",5,"b@",&result);
	ASSERT_TEST(realtor2!=NULL && result==REALTOR_SUCCESS);

	ASSERT_TEST(realtorLexicographicalEmail(realtor1,realtor2)<0);
	ASSERT_TEST(realtorLexicographicalEmail(realtor2,realtor1)>0);
	ASSERT_TEST(realtorLexicographicalEmail(realtor1,realtor1)==0);

	realtorDestroy(realtor1);
	realtorDestroy(realtor2);
	return true;
}

int main(){
   RUN_TEST(realtorDestroyTest);
   RUN_TEST(realtorCreateTest);
   RUN_TEST(realtorCopyTest);
   RUN_TEST(realtorGetCompanyNameTest);
   RUN_TEST(realtorGetTaxPercentageTest);
   RUN_TEST(realtorGetAmountApartmentTest);
   RUN_TEST(realtorGetEmailTest);
   RUN_TEST(realtorGetRatingTest);
   RUN_TEST(realtorServiceNameExistTest);
   RUN_TEST(realtorAddApartmentServiceTest);
   RUN_TEST(realtorRemoveApartmentServiceTest);
   RUN_TEST(realtorAddApartmentTest);
   RUN_TEST(realtorRemoveApartmentTest);
   RUN_TEST(realtorApartmentExistInTheServiceTest);
   RUN_TEST(realtorGetApartmentDetailsTest);
   RUN_TEST(realtorGetApartmentPriceTest);
   RUN_TEST(realtorGetApartmentPriceWithFeeTest);
   RUN_TEST(realtorExsistSuitableApartmentTest);
   RUN_TEST(realtorLexicographicalEmailTest);

   return 0;
}

