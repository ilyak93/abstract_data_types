/*
 * yad3.c
 *
 *  Created on: 27 באפר 2016
 *      Author: batel
 */


#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "yad3.h"
#include "mtm_ex2.h"
#include "customer.h"
#include "realtor.h"
#include "list.h"
#include "map.h"


#define EMAIL_SIGN '@'
#define EMAIL_LEGAL 1
#define TEXT_PERCENTAGE_MAX 100
#define TEXT_PERCENTAGE_MIN 1
#define ACCEPT "accept"
#define DECLINE "decline"

typedef Map RealtorMap;
typedef Map CustomerMap;
struct system_Users_t {
	RealtorMap realtor_map;
	CustomerMap customer_map;
};

//************************************************************************
//***************************Static function******************************
//************************************************************************

static void conventFromeRealtorResultToMtmErrorCode(RealtorResult result,
		Yad3ErrorCode*error){
	assert(error);
	assert(result!=REALTOR_SUCCESS);
	assert(result!=REALTOR_INVALID_INPUT);
	assert(result!=REALTOR_NO_RELEVANT_APARTMENT);
	switch (result) {
		case REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS:
			*error= YAD_3_APARTMENT_SERVICE_ALREADY_EXISTS;
			break;
		case REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST:
			*error= YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST;
			break;
		case REALTOR_APARTMENT_SERVICE_FULL:
			*error= YAD_3_APARTMENT_SERVICE_FULL;
			break;
		case REALTOR_APARTMENT_ALREADY_EXISTS:
			*error= YAD_3_APARTMENT_ALREADY_EXISTS;
			break;
		case REALTOR_APARTMENT_DOES_NOT_EXIST:
			*error= YAD_3_APARTMENT_DOES_NOT_EXIST;
			break;
		case REALTOR_OUT_OF_MEMORY:
			*error= YAD_3_OUT_OF_MEMORY;
			break;
		default:
			assert(false);
	}
}

static void conventFromeCustomerResultToMtmErrorCode(CustomerResult result,
		Yad3ErrorCode*error){
	assert(error);
	assert(result!=CUSTOMER_SUCCESS);
	assert(result!=CUSTOMER_INVALID_INPUT);
	switch (result) {
		case CUSTOMER_PURCHASE_WRONG_PROPERTIES:
			*error= YAD_3_PURCHASE_WRONG_PROPERTIES;
			break;
		case CUSTOMER_REQUEST_WRONG_PROPERTIES:
			*error= YAD_3_REQUEST_WRONG_PROPERTIES;
			break;
		case CUSTOMER_OUT_OF_MEMORY:
			*error= YAD_3_OUT_OF_MEMORY;
			break;
		case CUSTOMER_REQUEST_ILLOGICAL_PRICE:
			*error= YAD_3_REQUEST_ILLOGICAL_PRICE;
			break;
		case CUSTOMER_ALREADY_REQUESTED:
			*error= YAD_3_ALREADY_REQUESTED;
			break;
		case CUSTOMER_NOT_REQUESTED:
			*error= YAD_3_NOT_REQUESTED;
			break;
		default:
			assert(false);
	}
}

//for list
static ListElement customerElementCopyList(ListElement customer){
	return customerCopy((Customer) (customer));
}

static ListElement realtorElementCopyList(ListElement realtor){
	return realtorCopy((Realtor) (realtor));
}

static void customerElementFreeList(ListElement customer) {
	customerDestroy((Customer) (customer));
	return;
}

static void realtorElementFreeList(ListElement realtor) {
	realtorDestroy((Realtor) (realtor));
	return;
}

static bool conventFromeMapToListsSuccess(Map map,Yad3ErrorCode*error,
	   List* list,CopyListElement copyElement,FreeListElement freeElement){
	assert(map);
	 char*email = mapGetFirst(map);
	 if(!email){
		 *error =YAD_3_EMAIL_DOES_NOT_EXIST;
		 return false;
	 }
	 (*list)=listCreate(copyElement, freeElement);
	 if(!(*list)){
		 *error= YAD_3_OUT_OF_MEMORY;
		 return false;
	 }
	 while(email){
		 MapDataElement element=mapGet(map,email);
		 assert(element);
		 ListResult result= listInsertFirst(*list,element);
		 if(result==LIST_OUT_OF_MEMORY){
			 listDestroy(*list);
			 *error= YAD_3_OUT_OF_MEMORY;
			 return false;
		 }
		 assert(result==LIST_SUCCESS);
		 email=mapGetNext(map);
	 }
	 return true;
}

static bool systemUsersWrongId(int id, Yad3ErrorCode*error_code ){
	assert(error_code);
	if (id<0){
		*error_code=YAD_3_INVALID_PARAMETERS;
		return true;
	}
	return false;
}

static bool systemUsersValidEmail(char* email,Yad3ErrorCode*error) {
	assert(email && error);
	int counter = 0;

	for (char* temp = email; *temp != '\0'; temp++) {
		if (*temp == EMAIL_SIGN) {
			counter++;
		}
	}
	if (counter != EMAIL_LEGAL) {
		(*error)= YAD_3_INVALID_PARAMETERS;
		return false;
	}
	return true;
}

static bool systemUsersUserAlreadyExists(SystemUser system,
		char* email,Yad3ErrorCode*error) {
	assert(system && email && error);
	if (mapContains(system->customer_map, email)
			|| mapContains(system->realtor_map, email)) {
		*error=YAD_3_EMAIL_ALREADY_EXISTS;
		return true;
	}
	*error= YAD_3_EMAIL_DOES_NOT_EXIST;
	return false;
}

static bool systemUsersUserWorngType(SystemUser system, char* email,
		UserStatus type,Yad3ErrorCode*error) {
	assert(system && email && error);
	if (mapContains(system->customer_map, email) && type == REALTOR_USER) {
		*error= YAD_3_EMAIL_WRONG_ACCOUNT_TYPE;
		return true;
	} else if (mapContains(system->realtor_map, email)&&type==CUSTOMER_USER){
		*error= YAD_3_EMAIL_WRONG_ACCOUNT_TYPE;
		return true;
	}
	return false;
}

static bool legalAccount(SystemUser system, char* email,
		UserStatus type,Yad3ErrorCode*error) {
	assert(email && error && system);
	if (!systemUsersValidEmail(email,error)) {
		return false;
	} else if (!systemUsersUserAlreadyExists(system, email,error)) {
		return false;
	} else if (systemUsersUserWorngType(system, email, type,error)) {
		return false;
	}
	return true;
}

static bool llegalApartmentData(char* service_name, int id, int price,
		int width, int height, char* matrix,Yad3ErrorCode*error) {
	assert(service_name && matrix && error);
	if (height <= 0 || width <= 0 || price <= 0 || id < 0 || (price % 100)
			|| (strlen(matrix) != (width * height))) {
		*error= YAD_3_INVALID_PARAMETERS;
		return false;
	}
	for (char* temp = matrix; *temp != '\0'; temp++) {
		if (*temp != 'e' && *temp != 'w') {
			*error= YAD_3_INVALID_PARAMETERS;
			return false;
		}
	}
	return true;
}

static void freeSquare(SquareType**squares, int length) {
	if (squares == NULL) {
		return;
	}
	for (int i = 0; i < length; i++) {
		free(squares[i]);
	}
	free(squares);
}

static SquareType** createSquare(int length, int width) {
	assert(width>0 &&  length>0);
	SquareType**squares=(SquareType**)malloc(length*sizeof(*squares));
	if (squares == NULL) {
		return NULL;
	}
	for (int i = 0; i < length; i++) {
		squares[i] = (SquareType*) malloc(width * sizeof(**squares));
		if (squares[i] == NULL) {
			freeSquare(squares,i);
			free(squares);
			return NULL;
		}
	}
	return squares;
}

static SquareType** createSquareFromMatrix(char*matrix,int width,int height){
	assert(matrix && width>0 &&  height>0);
	SquareType** square = createSquare(height, width);
	if (!square){
		return NULL;
	}
	int index = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			square[i][j] = matrix[index] == 'e' ? EMPTY : WALL;
			index++;
		}
	}
	return square;
}

static char* copyString(char* str) {
	assert(str);
    char* str_copy = malloc(strlen(str) + 1);
    return str_copy ? strcpy(str_copy, str) : NULL;
}

static void systemUserRemoveOfferAccordingRealtor(SystemUser system,
		char* realtor_email){
	assert(realtor_email && system);
	char* email_customer=mapGetFirst(system->customer_map);
	if(!email_customer){
		return;
	}
	while(email_customer){
	Customer customer=mapGet(system->customer_map,email_customer);
	assert(customer);
#ifndef NDEBUG
	CustomerResult customer_result=
#endif
	  customerRemoveOfferAccordingEmail(customer,realtor_email);
	  assert(customer_result==CUSTOMER_SUCCESS);
	  email_customer= mapGetNext(system->customer_map);
	}
}

static void systemUserRemoveOfferIfServiceRemove(SystemUser system,
		char* realtor_eamil,char*service_name){
	assert(system && realtor_eamil && service_name);
	char* email_customer=mapGetFirst(system->customer_map);
	if(!email_customer){
		return;
	}
	while(email_customer){
	Customer customer=mapGet(system->customer_map,email_customer);
	assert(customer);
#ifndef NDEBUG
	CustomerResult customer_result=
#endif
	customerRemoveOfferAccordingService(customer,realtor_eamil,service_name);
	assert(customer_result==CUSTOMER_SUCCESS);
	email_customer= mapGetNext(system->customer_map);
	}
}

static void systemUserRemoveOfferIfApartmentRemove(SystemUser system,
		char* realtor_email,char*service_name,int id){
	assert(system && realtor_email && service_name && id>=0);
	char* email_customer=mapGetFirst(system->customer_map);
	if(!email_customer){
		return;
	}
	while(email_customer){
	Customer customer=mapGet(system->customer_map,email_customer);
	assert(customer);
#ifndef NDEBUG
	CustomerResult customer_result=
#endif
	customerRemoveOfferAccordingId(customer,
					realtor_email,service_name,id);
	assert(customer_result==CUSTOMER_SUCCESS);
	email_customer= mapGetNext(system->customer_map);
	}
}

static bool creatListOfRelevantRealtorsSuccess(SystemUser system,
		char* customer_email,List* list,
		Yad3ErrorCode* error_code){
	assert(customer_email && system && error_code);
	if (!legalAccount(system, customer_email,CUSTOMER_USER,error_code)) {
				return false;
	}
	bool success=conventFromeMapToListsSuccess(system->realtor_map,
			error_code,list,realtorElementCopyList, realtorElementFreeList);
	if(!success && *error_code==YAD_3_EMAIL_DOES_NOT_EXIST){
			return true;//map empty
	} else if(!success){
			return false;
	}
	assert(*list);//list not empty
	Customer customer =mapGet(system->customer_map,customer_email);
	Realtor realtor=listGetFirst(*list);
	assert(realtor && customer);
	int min_area = customerGetMinArea(customer);
	int min_rooms = customerGetMinRooms(customer);
	int max_price = customerGetMaxPrice(customer);
	assert(min_area!=-1 && min_rooms !=-1 && max_price!=-1);
	while(realtor){
		RealtorResult result=realtorExsistSuitableApartment
				(realtor,min_area,min_rooms,max_price);

		if(result==REALTOR_OUT_OF_MEMORY){
			*error_code=YAD_3_OUT_OF_MEMORY;
			listDestroy(*list);
			return false;
		}
		if(result!=REALTOR_SUCCESS){
	#ifndef NDEBUG
			ListResult result=
	#endif
	        listRemoveCurrent(*list);
			assert(result==LIST_SUCCESS);
			realtor=listGetFirst(*list);//reset iterator
			assert(*list);
			continue;
		}
		realtor=listGetNext(*list);
	}
		return true;
}

static int lexicographicalSortEmailRealtor(ListElement element1,
										   ListElement element2) {
     return realtorLexicographicalEmail(((Realtor)(element1)),
    		 	 	 	 	 	 	 	((Realtor)(element2)));
}

static int lexicographicalSortEmailCustomer(ListElement element1,
											ListElement element2) {
	return customerLexicographicalEmail(((Customer)(element1)),
										((Customer)(element2)));
}

static int significantRealtorSort(ListElement element1, ListElement element2){
	int rating1=realtorGetRating((Realtor)(element1));
	int rating2=realtorGetRating((Realtor)(element2));
	if(	rating1>rating2){
		return -1;
	} else if(rating1==rating2){
		return lexicographicalSortEmailRealtor(element1,element2);
	}
	return 1;
}

static int amountPurchasesSort(ListElement element1, ListElement element2){
	int purchases1=customerGetAmountPurchases((Customer)(element1));
	int purchases2=customerGetAmountPurchases((Customer)(element2));
	if(	purchases1>purchases2){
		return -1;
	} else if(purchases1==purchases2){
		return lexicographicalSortEmailCustomer(element1,element2);
	}
	return 1;
}

static void getApartemntDetails(SystemUser system,Realtor realtor,
		char*service_name, int apartment_id,
		int* area,int* rooms, int* price_whith_fee){
#ifndef NDEBUG
	RealtorResult realtor_result =
#endif
	realtorGetApartmentDetails(realtor
				,service_name,apartment_id,area,rooms);
	assert(realtor_result==REALTOR_SUCCESS);
#ifndef NDEBUG
	realtor_result =
#endif
	realtorGetApartmentPriceWithFee(realtor,
				service_name,apartment_id,price_whith_fee);
	assert(realtor_result==REALTOR_SUCCESS);
	return;
}

static bool makeOfferToRealtor(SystemUser system,char*realtor_email,
	  char* customer_email,char*service_name,int apartment_id,int new_price,
	  Yad3ErrorCode*error_code){
	Realtor realtor =mapGet(system->realtor_map,realtor_email);
	assert(realtor);
	int area=0,rooms=0, price_whith_fee=0;
	getApartemntDetails(system,realtor,
			service_name,apartment_id,&area,&rooms, &price_whith_fee);
	Customer customer =mapGet(system->customer_map,customer_email);
	assert(customer);
	CustomerResult cusromer_result =
			customerMakeOffer(customer,realtor_email,
					service_name,rooms,area,price_whith_fee,
					new_price,apartment_id);
	if(cusromer_result!=CUSTOMER_SUCCESS){
		 conventFromeCustomerResultToMtmErrorCode
			                   (cusromer_result,error_code);
		 return false;
	}
	return true;
}

static bool makePurchaseFromRealtor(SystemUser system,char*realtor_email,
		char* customer_email,char*service_name, int apartment_id,
		Yad3ErrorCode*error_code){
	Realtor realtor =mapGet(system->realtor_map,realtor_email);
	assert(realtor);
	int area=0,rooms=0, price_whith_fee=0;
	getApartemntDetails(system,realtor,
			service_name,apartment_id,&area,&rooms, &price_whith_fee);
	Customer customer =mapGet(system->customer_map,customer_email);
	assert(customer);
	CustomerResult cusromer_result =
		    customerMakePurchase(realtor_email,
			customer,service_name,apartment_id,
			rooms, area,price_whith_fee);
	if(cusromer_result!=CUSTOMER_SUCCESS){
		 conventFromeCustomerResultToMtmErrorCode
			                   (cusromer_result,error_code);
		  return false;
	}
#ifndef NDEBUG
	RealtorResult realtor_result =
#endif
	realtorRemoveApartment(realtor,
        		 service_name,apartment_id);
	assert(realtor_result == REALTOR_SUCCESS);
	cusromer_result=customerRemoveOfferAccordingId(customer,
				realtor_email,service_name,apartment_id);
	assert(cusromer_result==CUSTOMER_SUCCESS);
	return true;
}

static bool illegalUsers(SystemUser system,char* email,char*customer_email,
		Yad3ErrorCode* error_code){
	if (!systemUsersValidEmail(customer_email,error_code) ||
			!systemUsersValidEmail(email,error_code)) {
		return true;
	}else if(!systemUsersUserAlreadyExists(system,customer_email,error_code)||
			!systemUsersUserAlreadyExists(system,email,error_code)) {
		return true;
	}else if (systemUsersUserWorngType(system, customer_email,
		CUSTOMER_USER,error_code) ||
		systemUsersUserWorngType(system,email, REALTOR_USER,error_code)){
		return true;
	}
	return false;
}

//************************************************************************
//*********************Static function for map ADT************************
//************************************************************************
//for key
static MapKeyElement strCopy(constMapKeyElement str) {
	return copyString((char*)(str));
}

static void strFree(MapKeyElement str) {
	free((char*) (str));
	return;
}

static int compareStr(constMapKeyElement str1, constMapKeyElement str2) {
	return strcmp((char*) (str1), (char*) (str2));
}

//for realtor data
static void realtorElementFree(MapDataElement realtor) {
	realtorDestroy((Realtor) (realtor));
	return;
}

static MapDataElement realtorElementCopy(constMapDataElement realtor) {
	return realtorCopy((Realtor) (realtor));
}

//for customer data
static void customerElementFree(MapDataElement customer) {
	customerDestroy((Customer) (customer));
	return;
}

static MapDataElement customerElementCopy(constMapDataElement customer){
	return customerCopy((Customer)(customer));
}

//************************************************************************
//******************************Yad3 function*****************************
//************************************************************************
SystemUser systemUsersCreate() {
	SystemUser system_users = malloc(sizeof(*system_users));
	if (!system_users) {
		return NULL;
	}
	system_users->realtor_map =mapCreate(realtorElementCopy,
			strCopy, realtorElementFree, strFree,compareStr);
	if (!system_users->realtor_map) {
		systemUsersDestroy(system_users);
		return NULL;
	}
	system_users->customer_map = mapCreate(customerElementCopy,
			strCopy, customerElementFree, strFree,compareStr);
	if (!system_users->customer_map) {
		systemUsersDestroy(system_users);
		return NULL;
	}
	return system_users;
}

void systemUsersDestroy(SystemUser system_users) {
	if (system_users) {
		mapDestroy(system_users->customer_map);
		mapDestroy(system_users->realtor_map);
		free(system_users);
	}
}

bool systemUserRemoveUser(SystemUser system,char* email,UserStatus type,
		Yad3ErrorCode* error_code){
	assert(error_code);
	if(!(email && system)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if (!legalAccount(system, email,type,error_code)) {
			return false;
	} else if(type == REALTOR_USER){
	    systemUserRemoveOfferAccordingRealtor(system,email);
	}
	Map user_map = ((type == REALTOR_USER) ?
			system->realtor_map : system->customer_map);
#ifndef NDEBUG
	MapResult map_result =
#endif
			mapRemove(user_map, email);
	assert(map_result == MAP_SUCCESS);
	mapGetFirst(user_map);
	return true;
}

bool systemUserAddRealtor(SystemUser system,char* email,
		char* company_name, int tax_percentage,
		Yad3ErrorCode*error_code){
    assert(error_code);
	if(!(company_name && email && system )){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if(!systemUsersValidEmail(email,error_code) ||
		tax_percentage>TEXT_PERCENTAGE_MAX ||
		tax_percentage<TEXT_PERCENTAGE_MIN){
		*error_code=YAD_3_INVALID_PARAMETERS;
		return false;
	} else if (systemUsersUserAlreadyExists(system, email,error_code)){
		return false;
	}
	RealtorResult realtor_result=REALTOR_SUCCESS;
	Realtor realtor = realtorCreate(company_name,
			tax_percentage,email,&realtor_result);
	if(realtor_result==REALTOR_OUT_OF_MEMORY){
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	assert(realtor_result==REALTOR_SUCCESS);
	MapResult result = mapPut(system->realtor_map, email,realtor);
	if (result == MAP_OUT_OF_MEMORY) {
		realtorDestroy(realtor);
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	assert(result == MAP_SUCCESS);
	realtorDestroy(realtor);
	return true;
}

bool systemUserAddCustomer(SystemUser system, char* email,int min_area,
		int min_rooms,int max_price,Yad3ErrorCode*error_code){
    assert(error_code);
	if(!(email && system)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if(min_rooms<=0 || max_price<=0 || min_area<=0){
		*error_code=YAD_3_INVALID_PARAMETERS;
		return false;
	} else if(!systemUsersValidEmail(email,error_code)){
		return false;
	} else if (systemUsersUserAlreadyExists(system, email,error_code)) {
		return false;
	}
	CustomerResult customer_result=CUSTOMER_SUCCESS;
	Customer customer = customerCreate(min_area, min_rooms,
			max_price,email,&customer_result);
	if(customer_result==CUSTOMER_OUT_OF_MEMORY){
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	assert(customer_result=CUSTOMER_SUCCESS);
	MapResult result = mapPut(system->customer_map, email,customer);
	if (result == MAP_OUT_OF_MEMORY) {
		*error_code=YAD_3_OUT_OF_MEMORY;
		customerDestroy(customer);
		return false;
	}
	assert(result == MAP_SUCCESS);
	customerDestroy(customer);
	return true;
}

bool systemUserAddToRealtorApartmentService(SystemUser system,char* email,
		char* service_name,int max_apartments, Yad3ErrorCode*error_code){
    assert(error_code);
	if(!(email && system && service_name)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if (max_apartments <= 0 ) {
		*error_code=YAD_3_INVALID_PARAMETERS;
		return false;
	}
	if (!legalAccount(system, email,REALTOR_USER,error_code)) {
		return false;
	}
	Realtor realtor=mapGet(system->realtor_map, email);
	assert(realtor);
	RealtorResult result= realtorAddApartmentService(realtor,
			service_name,max_apartments);
	if(result!=REALTOR_SUCCESS){
	  conventFromeRealtorResultToMtmErrorCode(result,error_code);
	  return false;
	}
	return true;
}

bool systemUserRemoveApartmentServiceFromeRealtor(SystemUser system,
		char* email,char* service_name,Yad3ErrorCode*error_code){
    assert(error_code);
	if(!(email && system && service_name)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if (!legalAccount(system, email,REALTOR_USER,error_code)) {
		return false;
	}
	Realtor realtor=mapGet(system->realtor_map, email);
	assert(realtor);
	RealtorResult result=
			realtorRemoveApartmentService(realtor,service_name);
	if(result!=REALTOR_SUCCESS){
	  conventFromeRealtorResultToMtmErrorCode(result,error_code);
	  return false;
	}
	systemUserRemoveOfferIfServiceRemove(system,email,service_name);
	return true;
}

bool systemUserAddApartmentToRealtor(SystemUser system,char* email,
		char* service_name, int id,int price,
		int width, int height, char*matrix, Yad3ErrorCode*error_code){
    assert(error_code);
	if(!(email && system && service_name && matrix)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	}else if (!llegalApartmentData(service_name, id, price, width, height,
			matrix,error_code)) {
		return false;
	}
	if (!legalAccount(system, email,REALTOR_USER,error_code)) {
		return false;
	}
	SquareType** square = createSquareFromMatrix(matrix, width, height);
	if (!square) {
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	Apartment apartment = apartmentCreate(square, height, width, price);
	freeSquare(square, height);
	if (!apartment) {
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	Realtor realtor=mapGet(system->realtor_map, email);
	assert(realtor);
	RealtorResult result=realtorAddApartment(realtor,
			service_name,apartment,id);
	apartmentDestroy(apartment);
	if(result!=REALTOR_SUCCESS){
	  conventFromeRealtorResultToMtmErrorCode(result,error_code);
	  return false;
	}
	return true;
}

bool systemUserRemoveApartmentFromeRealtor(SystemUser system,char* email,
		char* service_name, int id,Yad3ErrorCode*error_code){
    assert(error_code);
	if(!(email && system && service_name)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if (systemUsersWrongId(id,error_code )) {
			return false;
	} else if (!legalAccount(system, email,REALTOR_USER,error_code)) {
		return false;
	}
	Realtor realtor=mapGet(system->realtor_map, email);
	assert(realtor);
	RealtorResult result=realtorRemoveApartment(realtor,service_name,id);
	assert(result!=REALTOR_INVALID_INPUT);
	if(result!=REALTOR_SUCCESS){
	  conventFromeRealtorResultToMtmErrorCode(result,error_code);
	  return false;
	}
	systemUserRemoveOfferIfApartmentRemove(system,email,service_name,id);
	return true;
}

bool systemUserCustomerPurchase(SystemUser system,char* email,
		char* realtor_email,char*service_name, int apartment_id,
				Yad3ErrorCode* error_code){
    assert(error_code);
	if(!(email && system && service_name && realtor_email)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if (systemUsersWrongId(apartment_id,error_code )) {
			return false;
	} else if (illegalUsers(system,realtor_email,email,error_code)){
		return false;
	}
	Realtor realtor=mapGet(system->realtor_map, realtor_email);
	assert(realtor);
	RealtorResult realtor_result=realtorApartmentExistInTheService
			(realtor,service_name,apartment_id);
	if(realtor_result!=REALTOR_SUCCESS){
		conventFromeRealtorResultToMtmErrorCode(realtor_result,error_code);
		return false;
	} else if(!makePurchaseFromRealtor(system,realtor_email,
			email,service_name,apartment_id,error_code)){
		return false;
	}
	systemUserRemoveOfferIfApartmentRemove(system,realtor_email,
			service_name,apartment_id);
	return true;
}

bool systemUserCustomerMakeOffer(SystemUser system,char* email,
		char* realtor_email,char*service_name, int apartment_id,
		int new_price,Yad3ErrorCode* error_code){
    assert(error_code);
	if(!(email && system && service_name && realtor_email)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if (new_price<=0 || apartment_id<0) {
			*error_code=YAD_3_INVALID_PARAMETERS;
			return false;
	} else if (illegalUsers(system,realtor_email,email,error_code)){
		return false;
	}
	Customer customer =mapGet(system->customer_map,email);
	assert(customer);
	CustomerResult result=customerCheckIfOfferExist(customer,
			realtor_email);
	if(result==CUSTOMER_ALREADY_REQUESTED){
		*error_code=YAD_3_ALREADY_REQUESTED;
		return false;
	}
	Realtor realtor=mapGet(system->realtor_map, realtor_email);
	assert(realtor);
	RealtorResult realtor_result=realtorApartmentExistInTheService
			(realtor,service_name,apartment_id);
	if(realtor_result!=REALTOR_SUCCESS){
		conventFromeRealtorResultToMtmErrorCode(realtor_result,error_code);
		return false;
	} else if(!makeOfferToRealtor(system,realtor_email,
			email,service_name,apartment_id,new_price,
			error_code)){
		return false;
	}
    return true;
}

bool systemUserRealtorRespondToOffer(SystemUser system,char* email,
		char*customer_email,char* choice,Yad3ErrorCode* error_code){
    assert(error_code);
	if(!(email && system && customer_email && choice)){
		*error_code=YAD_3_NULL_PARAMETERS;
		return false;
	} else if(strcmp(choice, ACCEPT)!=0 && strcmp(choice, DECLINE)!=0) {
		*error_code= YAD_3_INVALID_PARAMETERS;
		return false;
	} else if (illegalUsers(system,email,customer_email,error_code)){
		return false;
	}
	Customer customer =mapGet(system->customer_map,customer_email);
	assert(customer);
	if(customerCheckIfOfferExist(customer,email)
				==CUSTOMER_NOT_REQUESTED){
		*error_code= YAD_3_NOT_REQUESTED;
		return false;
	}
	CustomerResult result=CUSTOMER_SUCCESS;
	char* service_name=customerOfferGetServiceName(customer,email,&result);
	if(!service_name){
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	int apartment_id=-1;
	result=customerOfferGetId(customer,email,&apartment_id);
	assert(apartment_id!=-1 && result==CUSTOMER_SUCCESS);
	if(strcmp(choice, ACCEPT)==0){
		result=customerMakePurchaseByOffer(email,
				customer,service_name,apartment_id);
		assert(result==CUSTOMER_SUCCESS);
		Realtor realtor=mapGet(system->realtor_map,email);
		assert(realtor);
        #ifndef NDEBUG
	         RealtorResult realtor_result =
         #endif
	                  realtorRemoveApartment(realtor,
        		                  service_name,apartment_id);
	        assert(realtor_result == REALTOR_SUCCESS);
	    	systemUserRemoveOfferIfApartmentRemove(system,email,
	    			service_name,apartment_id);
	}
	customerRemoveOfferAccordingEmail(customer,
			email);
	free(service_name);
	return true;
}

bool systemUseReportRelevantRealtors(SystemUser system,char* customer_email,
		FILE* output,Yad3ErrorCode* error_code){
	 assert(error_code);
	if(!(system && output && customer_email)){
			*error_code=YAD_3_NULL_PARAMETERS;
			return false;
	}
	List list=NULL;
	if(!creatListOfRelevantRealtorsSuccess(system,
		customer_email,&list,error_code)){
		return false;
	}
	assert(list);
	if(listGetSize(list)==0){
		listDestroy(list);
		return true;
	}
	ListResult result= listSort(list,lexicographicalSortEmailRealtor);
	if(result==LIST_OUT_OF_MEMORY){
		listDestroy(list);
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	assert(result==LIST_SUCCESS);
	Realtor realtor=listGetFirst(list);//reset iterator, list not empty
	while(realtor){
		char*email=realtorGetEmail(realtor);
		if(!email ){
			*error_code=YAD_3_OUT_OF_MEMORY;
			listDestroy(list);
			return false;
		}
		char*company_name=realtorGetCompanyName(realtor);
		if(!company_name){
			*error_code=YAD_3_OUT_OF_MEMORY;
			free(email);
			listDestroy(list);
			return false;
		}
		assert(email && company_name);
		mtmPrintRealtor(output,email,company_name);
		free(company_name);
		free(email);
		realtor=listGetNext(list);
	}
	listDestroy(list);
	return true;
}

bool systemUserReportSignificantRealtors(SystemUser system,int count,
		FILE* output,Yad3ErrorCode* error_code){
	assert(error_code);
	if(!(system && output)){
			*error_code=YAD_3_NULL_PARAMETERS;
			return false;
	} else if(count<=0){
		*error_code=YAD_3_INVALID_PARAMETERS;
		return false;
	}
	List list=NULL;
	bool success=conventFromeMapToListsSuccess(system->realtor_map,
		 error_code,&list,realtorElementCopyList, realtorElementFreeList);
	if(!success && *error_code==YAD_3_EMAIL_DOES_NOT_EXIST){
		return true;//map empty
	} else if(!success){
		return false;
	}
	assert(list);//list not empty
	ListResult result= listSort(list,significantRealtorSort);
	if(result==LIST_OUT_OF_MEMORY){
		listDestroy(list);
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	assert(result==LIST_SUCCESS);
	Realtor realtor=listGetFirst(list);//reset iterator, list not empty
	while(realtor && count){
		char*email=realtorGetEmail(realtor);
		char*company_name=realtorGetCompanyName(realtor);
		if(!email && !company_name){
			*error_code=YAD_3_OUT_OF_MEMORY;
			free(company_name);
			free(email);
			listDestroy(list);
			return false;
		}
		if(realtorGetRating(realtor)>0){
			mtmPrintRealtor(output,email,company_name);
		}
		free(company_name);
		free(email);
		realtor=listGetNext(list);
		count--;
	}
	listDestroy(list);
	return true;
}

bool systemUserReportMostPayingCustomers(SystemUser system,int count,
		FILE* output,Yad3ErrorCode* error_code){
	assert(error_code);
	if(!(system && output)){
			*error_code=YAD_3_NULL_PARAMETERS;
			return false;
	} else if(count<=0){
		*error_code=YAD_3_INVALID_PARAMETERS;
		return false;
	}
	List list=NULL;
	bool success=conventFromeMapToListsSuccess(system->customer_map,
		 error_code,&list,customerElementCopyList, customerElementFreeList);
	if(!success && *error_code==YAD_3_EMAIL_DOES_NOT_EXIST){
		return true;//map empty
	} else if(!success){
		return false;
	}
	assert(list);//list not empty
	ListResult result= listSort(list,amountPurchasesSort);
	if(result==LIST_OUT_OF_MEMORY){
		listDestroy(list);
		*error_code=YAD_3_OUT_OF_MEMORY;
		return false;
	}
	assert(result==LIST_SUCCESS);
	Customer customer=listGetFirst(list);//reset iterator, list not empty
	while(customer && count){
		char*email=customerGetEmail(customer);
		if(!email){
			*error_code=YAD_3_OUT_OF_MEMORY;
			listDestroy(list);
			return false;
		}
		int purchases=customerGetAmountPurchases(customer);
		assert(email);
		if(purchases>0){
		mtmPrintCustomer(output,email,purchases);
		}
		free(email);
		customer=listGetNext(list);
		count--;
	}
	listDestroy(list);
	return true;
}
