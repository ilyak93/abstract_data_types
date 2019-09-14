/*
 * customer_user.c
 *
 *  Created on: 27 באפר 2016
 *      Author: batel
 */

#include "map.h"
#include "customer.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define EMAIL_SIGN '@'
#define EMAIL_LEGAL 1

typedef struct offer_t* Offer;
typedef Map OfferMap;
typedef void* Element;

struct customer_t {
	char*email;
	int min_area;
	int min_room;
	int max_price;
	int amount_purchases;
	OfferMap offer_map;
};

struct offer_t{
	int price;
	int apartment_id;
	char* service_name;
};

//************************************************************************
//***************************Static function******************************
//************************************************************************

static bool validEmail(char* email){
	assert(email);
	int counter = 0;
	for (char* temp = email; *temp != '\0'; temp++) {
		if (*temp == EMAIL_SIGN) {
			counter++;
		}
	}
	return (counter == EMAIL_LEGAL);
}

static char* copyString(char* str){
	assert(str);
    char* str_copy = malloc(strlen(str) + 1);
    return str_copy ? strcpy(str_copy, str) : NULL;
}

static Offer createOffer(int price , int apartment_id, char* service_name)
{
	assert( service_name);
    Offer new_offer=malloc(sizeof(*new_offer));
    if(!new_offer){
        return NULL;
    }

    new_offer->service_name= copyString(service_name);
    if(!new_offer->service_name){
        return NULL;
    }
    new_offer->price=price;
    new_offer->apartment_id=apartment_id;
    return new_offer;
}

static Offer offerCopy(Offer offer){
    assert(offer);
    return createOffer(offer->price,offer->apartment_id,offer->service_name);
}

static void offerDestroy(Offer offer){
    if(offer){
    	free(offer->service_name);
    	free(offer);
    }
}

static void removeOffer(Customer customer,
		char*realtor_email,char*service_name,int id) {
	assert(realtor_email && service_name && customer);
	Offer offer=mapGet(customer->offer_map,realtor_email);
	if(offer && strcmp(offer->service_name,service_name)==0
			 && offer->apartment_id==id){
#ifndef NDEBUG
	MapResult result=
#endif
	      mapRemove(customer->offer_map,realtor_email);
	      assert(result!=MAP_NULL_ARGUMENT);
	}
}

//************************************************************************
//*********************Static function for map ADT************************
//************************************************************************

static MapDataElement offerElementCopy(constMapDataElement offer){
	return offerCopy((Offer)(offer));
}

static void offerElementFree(MapDataElement offer){
   offerDestroy(offer);
}

static MapKeyElement strCopy(constMapKeyElement str){
	return copyString((char*)(str));
}

static void strFree(MapKeyElement str){
	free((char*)(str));
}

static int compareStr(constMapKeyElement str1, constMapKeyElement str2){
	return strcmp((char*)(str1), (char*)(str2));
}


//************************************************************************
//***************************Customer function****************************
//************************************************************************
void customerDestroy(Customer customer){
	if (customer) {
		free(customer->email);
		mapDestroy(customer->offer_map);
		free(customer);
	}
}

Customer customerCreate(int min_area,int min_room,
		int max_price,char*email, CustomerResult*result){
	assert(result);
	if(!(min_area>0 && min_room>0 && max_price>0 &&
			email && validEmail(email))){
		*result=CUSTOMER_INVALID_INPUT;
		return NULL;
	}
	Customer new_customer = malloc(sizeof(*new_customer));
	if (!new_customer) {
		*result=CUSTOMER_OUT_OF_MEMORY;
		return NULL;
	}
	new_customer->amount_purchases=0;
	new_customer->max_price=max_price;
	new_customer->min_area=min_area;
	new_customer->min_room=min_room;
	new_customer->email=copyString(email);
	if(!(new_customer->email)){
		customerDestroy(new_customer);
		*result=CUSTOMER_OUT_OF_MEMORY;
		return NULL;
	}
	new_customer->offer_map=mapCreate(offerElementCopy, strCopy,
			offerElementFree, strFree, compareStr);
	if(!(new_customer->offer_map)){
		customerDestroy(new_customer);
		*result=CUSTOMER_OUT_OF_MEMORY;
		return NULL;
	}
	*result=CUSTOMER_SUCCESS;
	return new_customer;
}

Customer customerCopy(Customer customer){
	if (!customer) {
		return NULL;
	}
	Customer new_customer = malloc(sizeof(*new_customer));
	if (!new_customer) {
		return NULL;
	}
	new_customer->email=copyString(customer->email);
	if(!(new_customer->email)){
		customerDestroy(new_customer);
		return NULL;
	}
	new_customer->offer_map=mapCopy(customer->offer_map);
	if(!(new_customer->offer_map)){
		customerDestroy(customer);
		return NULL;
	}
	new_customer->amount_purchases=customer->amount_purchases;
	new_customer->max_price=customer->max_price;
	new_customer->min_area=customer->min_area;
	new_customer->min_room=customer->min_room;
	return new_customer;
}

int customerGetMinArea(Customer customer) {
	return customer ? customer->min_area : -1;
}

int customerGetMinRooms(Customer customer){
	return customer ? customer->min_room : -1;
}

int customerGetMaxPrice(Customer customer){
	return customer ? customer->max_price : -1;
}

char*customerGetEmail(Customer customer){
	return customer ? copyString(customer->email) : NULL;
}

int customerGetAmountPurchases(Customer customer) {
	return customer ? customer->amount_purchases : -1;
}

CustomerResult customerCheckIfOfferExist(Customer customer,
		char*realtor_email) {
	if(!customer || !realtor_email || !validEmail(realtor_email)){
		return CUSTOMER_INVALID_INPUT;
	}
    assert(customer->offer_map);
    return mapContains(customer->offer_map,realtor_email) ?
    		CUSTOMER_ALREADY_REQUESTED : CUSTOMER_NOT_REQUESTED;
}

CustomerResult customerOfferGetPrice(Customer customer,
		char*realtor_email,int*price) {
	if(!(customer && price && realtor_email &&
			validEmail(realtor_email))){
		 return CUSTOMER_INVALID_INPUT;
	} else if(customerCheckIfOfferExist(customer,realtor_email)
				==CUSTOMER_NOT_REQUESTED){
		return CUSTOMER_NOT_REQUESTED;
	}
	Offer offer=mapGet(customer->offer_map,realtor_email);
	assert(offer);
	*price= offer->price;
	return CUSTOMER_SUCCESS;
}

char* customerOfferGetServiceName(Customer customer,
		char* realtor_email,CustomerResult* result) {
	assert(result);
	if(!(customer && realtor_email && validEmail(realtor_email))){
		*result=CUSTOMER_INVALID_INPUT;
		return NULL;
	} else if(customerCheckIfOfferExist(customer,realtor_email)
				==CUSTOMER_NOT_REQUESTED){
		*result=CUSTOMER_NOT_REQUESTED;
		return NULL;
	}
	Offer offer=mapGet(customer->offer_map,realtor_email);
	assert(offer);
	char* name= copyString(offer->service_name);
	if(!name){
		*result=CUSTOMER_OUT_OF_MEMORY;
		return NULL;
	}
	*result=CUSTOMER_SUCCESS;
	return name;
}

CustomerResult customerOfferGetId(Customer customer,
		char* realtor_email,int* apartment_id) {
	if(!(customer&&realtor_email&&validEmail(realtor_email)&&apartment_id)){
		return CUSTOMER_INVALID_INPUT;
	} else if(customerCheckIfOfferExist(customer,realtor_email)
				==CUSTOMER_NOT_REQUESTED){
		return CUSTOMER_NOT_REQUESTED;
	}
	Offer offer=mapGet(customer->offer_map,realtor_email);
	assert(offer);
	*apartment_id=offer->apartment_id;
	return CUSTOMER_SUCCESS;
}

CustomerResult customerMakeOffer(Customer customer,char*realtor_email,
		char*service_name,int num_of_rooms, int area, int price,
		int new_price,int apartment_id) {
	if(!(new_price>0 && customer && realtor_email &&
			service_name && apartment_id>=0 && num_of_rooms >= 0 &&
			area >= 0 && price > 0 && validEmail(realtor_email))){
		return CUSTOMER_INVALID_INPUT;
	} else if(mapContains(customer->offer_map,realtor_email)){
		return CUSTOMER_ALREADY_REQUESTED;
	} else if(num_of_rooms < customer->min_room ||
			  area < customer->min_area ||
			  new_price > customer->max_price){
		return CUSTOMER_REQUEST_WRONG_PROPERTIES;
	} else if(new_price >= price){
		return CUSTOMER_REQUEST_ILLOGICAL_PRICE;
	}
	Offer new_offer=createOffer(new_price,apartment_id,service_name);
	if(!new_offer){
		return CUSTOMER_OUT_OF_MEMORY;
	}
	MapResult map_result=mapPut(customer->offer_map, realtor_email,
			new_offer);
	offerDestroy(new_offer);
	if(map_result==MAP_OUT_OF_MEMORY){
		return CUSTOMER_OUT_OF_MEMORY;
	}
	assert(map_result==MAP_SUCCESS);
	return CUSTOMER_SUCCESS;
}

CustomerResult customerRemoveOfferAccordingEmail(Customer customer,
		char* realtor_email) {
	if(!customer || !realtor_email || !validEmail(realtor_email)){
		return CUSTOMER_INVALID_INPUT;
	}
	if(mapContains(customer->offer_map,realtor_email)){
#ifndef NDEBUG
	   MapResult result=
#endif
	   mapRemove(customer->offer_map,realtor_email);
	   assert(result!=MAP_NULL_ARGUMENT);
	}
	return CUSTOMER_SUCCESS;
}

CustomerResult customerRemoveOfferAccordingService(Customer customer,
		char*realtor_email,char*service_name) {

	if(!customer || !realtor_email || !service_name
			|| !validEmail(realtor_email)){
		return CUSTOMER_INVALID_INPUT;
	}
	Offer offer=mapGet(customer->offer_map,realtor_email);
	if(offer && strcmp(offer->service_name,service_name)==0){
#ifndef NDEBUG
	MapResult result=
#endif
	     mapRemove(customer->offer_map,realtor_email);
	     assert(result!=MAP_NULL_ARGUMENT);
	}
	return CUSTOMER_SUCCESS;
}

CustomerResult customerRemoveOfferAccordingId(Customer customer,
		char*realtor_email,char*service_name,int id) {
	if(!customer || !realtor_email || !service_name || id<0
			|| !validEmail(realtor_email)){
		return CUSTOMER_INVALID_INPUT;
	}
	removeOffer(customer,realtor_email,service_name,id);
	return CUSTOMER_SUCCESS;
}

int customerLexicographicalEmail(Customer customer1,Customer customer2){
	return strcmp(customer1->email,customer2->email);
}

CustomerResult customerMakePurchaseByOffer(char* realtor_email,
		Customer customer,char*service_name,int apartment_id) {
	if(!(customer && realtor_email && service_name &&
				apartment_id>=0 && validEmail(realtor_email))){
			return CUSTOMER_INVALID_INPUT;
	} else if(!mapContains(customer->offer_map, realtor_email)){
		return CUSTOMER_NOT_REQUESTED;
	}
	int price=0;
#ifndef NDEBUG
	CustomerResult result=
#endif
			customerOfferGetPrice(customer,realtor_email,&price);
	        assert(result==CUSTOMER_SUCCESS);
	customer->amount_purchases += price;
	removeOffer(customer,realtor_email,service_name,apartment_id);
	return CUSTOMER_SUCCESS;
}

CustomerResult customerMakePurchase(char* realtor_email,
		Customer customer,char*service_name,int apartment_id,
		int num_of_rooms, int area, int price) {
	if(!(customer && realtor_email && service_name &&
			apartment_id>=0 && num_of_rooms >= 0 && area >= 0 &&
			price>0 && validEmail(realtor_email))){
		return CUSTOMER_INVALID_INPUT;
	} else if(num_of_rooms < customer->min_room ||
	   area < customer->min_area ||
	   price > customer->max_price){
		return CUSTOMER_PURCHASE_WRONG_PROPERTIES;
	}
	customer->amount_purchases+=price;
	removeOffer(customer,realtor_email,service_name,apartment_id);
	return CUSTOMER_SUCCESS;
}
