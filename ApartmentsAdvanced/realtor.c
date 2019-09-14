/*
 * realtor_user.c
 *
 *  Created on: 27 באפר 2016
 *      Author: batel
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "realtor.h"
#include "map.h"

#define TEXT_PERCENTAGE_MAX 100
#define TEXT_PERCENTAGE_MIN 1

#define EMAIL_SIGN '@'
#define EMAIL_LEGAL 1

typedef int NumberOfApartments;
typedef Map MapService;
struct realtor_t {
	char*email;
	char* company_name;
	int rating;
	int tax_percentage;
	NumberOfApartments amount_apartment;
	MapService service_map;
};


//************************************************************************
//***************************Static function******************************
//************************************************************************

static bool taxValid(int tax){
	return tax<=TEXT_PERCENTAGE_MAX && tax>=TEXT_PERCENTAGE_MIN;
}

static RealtorResult realtorGetApartment(Realtor realtor
		,char* service_name, int id, Apartment* apartment) {
	if(!(service_name && realtor && apartment && id>=0)){
			return REALTOR_INVALID_INPUT;
	} else if (!realtorServiceNameExist(realtor, service_name)) {
			return REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST;
	}
	ApartmentService service =
			mapGet(realtor->service_map, service_name);
	assert(service);
	ApartmentServiceResult result=
			serviceGetById(service, id,apartment);
	if(result==APARTMENT_SERVICE_OUT_OF_MEM){
			return REALTOR_OUT_OF_MEMORY;
	} else if( result==APARTMENT_SERVICE_EMPTY ||
			result==APARTMENT_SERVICE_NO_FIT){
			return REALTOR_APARTMENT_DOES_NOT_EXIST;
	}
	assert(result == APARTMENT_SERVICE_SUCCESS);
	return REALTOR_SUCCESS;
}

static char* copyString(char* str) {
	assert(str);
	char* str_copy = malloc(strlen(str) + 1);
	return  str_copy ? strcpy(str_copy, str) : NULL;
}

static bool validEmail(char* email) {
	assert(email);
	int counter = 0;
	for (char* temp = email; *temp != '\0'; temp++) {
		if (*temp == EMAIL_SIGN) {
			counter++;
		}
	}
	return counter == EMAIL_LEGAL;
}

static void realtorGetPriceAndAreaMedian(Realtor realtor,
		int* price, int*area,int* number_of_services) {
	assert(realtor && price && area );
	*price = 0;
	*area = 0;
	*number_of_services=0;
	char* service_name = mapGetFirst(realtor->service_map);
	assert(service_name);
	while (service_name) {
		int counter_price = 0, counter_area = 0;
		ApartmentService service =
				mapGet(realtor->service_map, service_name);
		assert(service);
		ApartmentServiceResult result = servicePriceMedian(service,
				&counter_price);
		serviceAreaMedian(service, &counter_area);
		if (result == APARTMENT_SERVICE_EMPTY) {
			service_name = mapGetNext(realtor->service_map);
			continue;
		}
		(*number_of_services)++;
		(*price) += counter_price;
		(*area) += counter_area;
		service_name = mapGetNext(realtor->service_map);
	}
}

static void realtorUpdateRating(Realtor realtor){
    assert(realtor);
	int number_of_services = mapGetSize(realtor->service_map);
	if (!number_of_services) {
		realtor->rating=realtor->amount_apartment;
		return;
	}
	int area_median=0;
	int price_median=0;
	number_of_services=0;
	realtorGetPriceAndAreaMedian(realtor, &price_median,
			&area_median, &number_of_services);
	if(number_of_services==0){
		realtor->rating=realtor->amount_apartment;
		return;
	}
	area_median = (int) (area_median / number_of_services);
	price_median = (int) (price_median / number_of_services);
	realtor->rating= (1000000 * (realtor->amount_apartment)
			+ price_median + 100000 * (area_median));
}

static RealtorResult realtorExistSuitableApartmentInService
       (Realtor realtor,int min_area, int min_rooms,
		int max_price,char* service_name){
	if(!(service_name && realtor && min_rooms>0 && max_price>0
			&& min_area>0)){
		return REALTOR_INVALID_INPUT;
	} else if (!realtorServiceNameExist(realtor, service_name)) {
		return REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST;
	}
	ApartmentService service = mapGet(realtor->service_map,
			service_name);
	assert(service);
    Apartment apartment=NULL;
	ApartmentServiceResult result = serviceSearch(service,
			min_area, min_rooms,max_price, &apartment);
	if(result ==APARTMENT_SERVICE_OUT_OF_MEM){
		return REALTOR_OUT_OF_MEMORY;
	}else if(result !=APARTMENT_SERVICE_SUCCESS){
		return REALTOR_NO_RELEVANT_APARTMENT;
	}
	assert(result == APARTMENT_SERVICE_SUCCESS && apartment);

	int price=apartmentGetPrice(apartment);
	int price_whith_fee=price_whith_fee =
			(int)( ((price * (realtor->tax_percentage)) / 100) + price );
	if(max_price>=price_whith_fee){
		apartmentDestroy(apartment);
		return REALTOR_SUCCESS;
	}
	apartmentDestroy(apartment);
	return REALTOR_NO_RELEVANT_APARTMENT;
}

//************************************************************************
//*********************Static function for map ADT************************
//************************************************************************

static void serviceElementFree(MapDataElement service){
	serviceDestroy((ApartmentService) (service));
}

static MapDataElement serviceElementCopy(constMapDataElement service){
	return serviceCopy((ApartmentService) (service));
}

static MapKeyElement strCopy(constMapKeyElement str){
	return copyString((char*) (str));
}

static void strFree(MapKeyElement str){
	free((char*) (str));
}

static int compareStr(constMapKeyElement str1, constMapKeyElement str2){
	return strcmp((char*) (str1), (char*) (str2));
}

//************************************************************************
//***************************Realtor function*****************************
//************************************************************************

void realtorDestroy(Realtor realtor){
	if (realtor){
		free(realtor->email);
		free(realtor->company_name);
		mapDestroy(realtor->service_map);
		free(realtor);
	}
}

Realtor realtorCreate(char* company_name, int tax_percentage,
		char*email,RealtorResult* result){
	assert(result);
	if(!company_name||!taxValid(tax_percentage)||!email||!validEmail(email)){
        *result=REALTOR_INVALID_INPUT;
		return NULL;
	}
	Realtor new_realtor = malloc(sizeof(*new_realtor));
	if (!new_realtor) {
		*result=REALTOR_OUT_OF_MEMORY;
		return NULL;
	}
	new_realtor->email=copyString(email);
	if (!new_realtor->email) {
			realtorDestroy(new_realtor);
			*result=REALTOR_OUT_OF_MEMORY;
			return NULL;
	}
	new_realtor->company_name = copyString(company_name);
	if (!new_realtor->company_name) {
		realtorDestroy(new_realtor);
		*result=REALTOR_OUT_OF_MEMORY;
		return NULL;
	}
	new_realtor->service_map = mapCreate(serviceElementCopy,
			strCopy, serviceElementFree, strFree,compareStr);
	if (!(new_realtor->service_map)) {
		realtorDestroy(new_realtor);
		*result=REALTOR_OUT_OF_MEMORY;
		return NULL;
	}
	new_realtor->amount_apartment = 0;
	new_realtor->rating=0;
	new_realtor->tax_percentage = tax_percentage;
	*result=REALTOR_SUCCESS;
	return new_realtor;
}

Realtor realtorCopy(Realtor realtor) {
	if (!realtor) {
		return NULL;
	}
	Realtor realtor_copy = malloc(sizeof(*realtor_copy));
	if (!realtor_copy) {
		return NULL;
	}
	realtor_copy->company_name = copyString(realtor->company_name);
	if (!realtor_copy->company_name) {
		realtorDestroy(realtor_copy);
		return NULL;
	}
	realtor_copy->email = copyString(realtor->email);
	if (!realtor_copy->email) {
		realtorDestroy(realtor_copy);
		return NULL;
	}
	realtor_copy->service_map = mapCopy(realtor->service_map);
	if (!(realtor_copy->service_map)) {
		realtorDestroy(realtor_copy);
		return NULL;
	}
	realtor_copy->tax_percentage = realtor->tax_percentage;
	realtor_copy->rating=realtor->rating;
	realtor_copy->amount_apartment = realtor->amount_apartment;
	return realtor_copy;
}

char* realtorGetCompanyName(Realtor realtor) {
	return realtor ? copyString(realtor->company_name) : NULL;
}

int realtorGetTaxPercentage(Realtor realtor) {
	return realtor ? realtor->tax_percentage : -1;
}

int realtorGetAmountApartment(Realtor realtor) {
	return realtor ? realtor->amount_apartment : -1;
}

char* realtorGetEmail(Realtor realtor) {
	return realtor ? copyString(realtor->email) : NULL;
}

int realtorGetRating(Realtor realtor) {
	return realtor ? realtor->rating : -1;
}

bool realtorServiceNameExist(Realtor realtor, char* service_name) {
	assert(realtor && service_name);
	return mapContains(realtor->service_map, service_name);
}

RealtorResult realtorApartmentExistInTheService(Realtor realtor
		,char* service_name, int id) {
	Apartment apartment=NULL;
	RealtorResult result= realtorGetApartment(realtor
			,service_name,id,&apartment);
	if(result!=REALTOR_SUCCESS){
		return result;
	}
	apartmentDestroy(apartment);
	return REALTOR_SUCCESS;
}

RealtorResult realtorGetApartmentDetails(Realtor realtor
		,char* service_name,int id,int*area,int*rooms){
	if(!(area && rooms)){
			return REALTOR_INVALID_INPUT;
	}
	Apartment apartment=NULL;
	RealtorResult result= realtorGetApartment(realtor
			,service_name,id,&apartment);
	if(result!=REALTOR_SUCCESS){
		return result;
	}
	*area=apartmentTotalArea(apartment);
	*rooms=apartmentNumOfRooms(apartment);
	apartmentDestroy(apartment);
	return REALTOR_SUCCESS;
}

RealtorResult realtorExsistSuitableApartment(Realtor realtor,
		int min_area, int min_rooms,int max_price) {
	if(!(realtor && min_rooms>0 && max_price>0 && min_area>0)){
		return REALTOR_INVALID_INPUT;
	}
	char* service_name = mapGetFirst(realtor->service_map);
	if (!service_name) {
		return REALTOR_NO_RELEVANT_APARTMENT;
	}
	while (service_name) {
		RealtorResult result=realtorExistSuitableApartmentInService
				(realtor,min_area,min_rooms,max_price,service_name);
		if(result==REALTOR_OUT_OF_MEMORY){
			return REALTOR_OUT_OF_MEMORY;
		} else if(result==REALTOR_SUCCESS){
			return REALTOR_SUCCESS;
		}
		assert(result==REALTOR_NO_RELEVANT_APARTMENT);
		service_name = mapGetNext(realtor->service_map);
	}
	return REALTOR_NO_RELEVANT_APARTMENT;
}

int realtorLexicographicalEmail(Realtor realtor1,Realtor realtor2) {
	assert(realtor1 && realtor2);
	return strcmp(realtor1->email,realtor2->email);
}

RealtorResult realtorAddApartmentService(Realtor realtor ,
		char* service_name, int max_apartments) {
	if(!(service_name && realtor && max_apartments>0)){
		return REALTOR_INVALID_INPUT;
	} else if (realtorServiceNameExist(realtor, service_name)) {
		return REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS;
	}
	ApartmentService service = serviceCreate(max_apartments);
	if (!service) {
		return REALTOR_OUT_OF_MEMORY;
	}
	MapResult map_result = mapPut(realtor->service_map,
			service_name, service);
	if (map_result == MAP_OUT_OF_MEMORY) {
		serviceDestroy(service);
		return REALTOR_OUT_OF_MEMORY;
	}
	assert(map_result == MAP_SUCCESS);
	serviceDestroy(service);
	realtorUpdateRating(realtor);
	return REALTOR_SUCCESS;
}

RealtorResult realtorRemoveApartmentService(Realtor realtor,
		char* service_name) {
	if(!(realtor && service_name)){
		return REALTOR_INVALID_INPUT;
	} else if (!realtorServiceNameExist(realtor, service_name)) {
		return REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST;
	}
#ifndef NDEBUG
	MapResult map_result =
#endif
			mapRemove(realtor->service_map, service_name);
	assert(map_result == MAP_SUCCESS);
	realtorUpdateRating(realtor);
	return REALTOR_SUCCESS;
}

RealtorResult realtorAddApartment(Realtor realtor,
		char* service_name,Apartment apartment,int apartment_id) {
	if(!(realtor && service_name &&  apartment && apartment_id>=0)){
		return REALTOR_INVALID_INPUT;
	} else if (!realtorServiceNameExist(realtor, service_name)) {
		return REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST;
	}
	ApartmentService service = mapGet(realtor->service_map,
			service_name);
	assert(service);
	ApartmentServiceResult service_result =
			serviceAddApartment(service,apartment, apartment_id);
	if (service_result == APARTMENT_SERVICE_OUT_OF_MEM) {
		return REALTOR_OUT_OF_MEMORY;
	} else if (service_result == APARTMENT_SERVICE_FULL) {
		return REALTOR_APARTMENT_SERVICE_FULL;
	} else if(service_result == APARTMENT_SERVICE_ALREADY_EXISTS){
		return REALTOR_APARTMENT_ALREADY_EXISTS;
	}
	assert(service_result == APARTMENT_SERVICE_SUCCESS);
	realtor->amount_apartment++;
	realtorUpdateRating(realtor);
	return REALTOR_SUCCESS;
}

RealtorResult realtorRemoveApartment(Realtor realtor,
		char* service_name,int id) {
	if (id < 0 || !service_name || !realtor) {
		return REALTOR_INVALID_INPUT;
	} else if (!realtorServiceNameExist(realtor, service_name)) {
		return REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST;
	}
	ApartmentService service = mapGet(realtor->service_map,
			service_name);
	assert(service);
	ApartmentServiceResult result= serviceDeleteById(service, id);
	if( result==APARTMENT_SERVICE_EMPTY ||
			result==APARTMENT_SERVICE_NO_FIT){
		return REALTOR_APARTMENT_DOES_NOT_EXIST;
	}
	assert(result == APARTMENT_SERVICE_SUCCESS);
	realtor->amount_apartment--;
	realtorUpdateRating(realtor);
	return REALTOR_SUCCESS;
}

RealtorResult realtorGetApartmentPriceWithFee(Realtor realtor
		,char* service_name,int id,int*price_whith_fee) {
	if(!(price_whith_fee)){
			return REALTOR_INVALID_INPUT;
	}
	int price=0;
	RealtorResult result=realtorGetApartmentPrice(realtor,
			service_name,id,&price);
	if(result!=REALTOR_SUCCESS){
		return result;
	}
	int precentage = realtor->tax_percentage;
	*price_whith_fee =(int)( ((price * precentage) / 100) + price );
	return REALTOR_SUCCESS;
}

RealtorResult realtorGetApartmentPrice(Realtor realtor
		,char* service_name,int id,int*price) {
	if(!(price)){
			return REALTOR_INVALID_INPUT;
	}
	Apartment apartment=NULL;
	RealtorResult result= realtorGetApartment(realtor
			,service_name,id,&apartment);
	if(result!=REALTOR_SUCCESS){
		return result;
	}
	*price = apartmentGetPrice(apartment);
	apartmentDestroy(apartment);
	return REALTOR_SUCCESS;
}
