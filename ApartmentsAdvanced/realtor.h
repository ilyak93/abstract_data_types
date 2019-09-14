/*
 * realtor_user.h
 *
 *  Created on: 27 באפר 2016
 *      Author: batel
 */

#ifndef REALTOR_H_
#define REALTOR_H_


#include "apartment_service.h"

/** Type for defining the realtor*/
typedef struct realtor_t* Realtor;

/** Type used for returning result codes from realtor functions */
typedef enum {
	REALTOR_OUT_OF_MEMORY,
	REALTOR_INVALID_INPUT,
	REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS,
	REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST,
	REALTOR_APARTMENT_SERVICE_FULL,
	REALTOR_APARTMENT_ALREADY_EXISTS,
	REALTOR_APARTMENT_DOES_NOT_EXIST,
	REALTOR_NO_RELEVANT_APARTMENT,
	REALTOR_SUCCESS
} RealtorResult;


/**
* Frees all memory allocated for the given  realtor.
* This function can receive NULL.
*/
void realtorDestroy(Realtor realtor);

/**
* Creates a new realtor of type image:
* The company name is given by company_name,the tax_percentage
* is given by tax percentage and the realtor email is given by
* the email.
*
* @return
* 	A new allocated apartment.
* 	NULL if email/company name are null, if tax percentage less
* 	than 100, if tax percentage is not positive or if email
* 	is invalid. Result contains the relevant result:
*   REALTOR_INVALID_INPUT if email or company name is null, if
*   tax percentage less than 100,if tax percentage is not positive
*   or if email is invalid.
*   REALTOR_OUT_OF_MEMORY in case of memory allocation failure.
*   REALTOR_SUCCESS if new realtor successfully created.
*
*   Valid Email contains exactly one character of @.
*
*   Result must not be NULL.
*
**/
Realtor realtorCreate(char* company_name, int tax_percentage,
		char*email,RealtorResult* result);

/**
* Creates a newly allocated copy of a given realtor.
* The realtor is given by realtor.
*
* @return
* A newly allocated copy of the original realtor.
* NULL if the realtor is NULL or in any case of memory allocation
* failure
*/
Realtor realtorCopy(Realtor realtor);

/**
*   Creates a copy of realtor company name.
*   The realtor is given by realtor.
*   @return
* 	the company name of the realtor.
*	NULL if realtor NULL or in case of memory allocation failure
*/
char* realtorGetCompanyName(Realtor realtor);

/**
* @return
* 	the tax percentage of the realtor.
*   -1 if realtor NULL.
*/
int realtorGetTaxPercentage(Realtor realtor);


/**
* @return
* 	the number of apartments that own realtor.
*   -1 if realtor NULL.
*/
int realtorGetAmountApartment(Realtor realtor);

/**
*   Creates a copy of realtor email.
*   The realtor is given by realtor.
*   @return
* 	the eamil of realtor.
*	NULL if realtor NULL or in case of memory allocation failure
*/
char* realtorGetEmail(Realtor realtor);

/**
* @return
* 	the realtor rating.
*   -1 if realtor NULL.
*/
int realtorGetRating(Realtor realtor);

/**
 * Checks if an service name given exists in realtor user.
 * The realtor is given by realtor,the service name is given by
 * service_name.
* @return
* 	true if service name exist
* 	false if service name exist does not exist
*   realtor and service_name must not be NULL
*/
bool realtorServiceNameExist(Realtor realtor, char* service_name);

/**
 * Checks if an apartment given exists in realtor service.
 * The realtor is given by realtor,the service name is given by
 * service_name and the apartment id is given by id.
 *
 * @return
 * REALTOR_SUCCESS If the apartment exists.
 * REALTOR_INVALID_INPUT if service name or realtor are NULL
 * or if id is negative.
 * REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
 * exist in the realtor user.
 * REALTOR_APARTMENT_DOES_NOT_EXIST if apartment does not
 * exist in the realtor user.
 * REALTOR_OUT_OF_MEMORY in any case of memory allocation failure.
 */
RealtorResult realtorApartmentExistInTheService(Realtor realtor
		,char* service_name, int id);

/**
*  This function receives an realtor,service name and id of apartment
*  and return the apartment area and the number of rooms in apartment.
*  apartment area should be stored in area.
*  number of rooms in apartment should be stored in rooms.
*  The realtor is given by realtor,the service name is given by
*  service_name and the apartment id is given by id.

* @return
 * REALTOR_INVALID_INPUT if service name/realtor/area/rooms are NULL
 * or if id is negative.
 * REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
 * exist in the realtor user.
 * REALTOR_APARTMENT_DOES_NOT_EXIST if apartment does not
 * exist in the realtor user.
 * REALTOR_OUT_OF_MEMORY in case of memory allocation failure.
 * REALTOR_SUCCESS otherwise.
*
*/
RealtorResult realtorGetApartmentDetails(Realtor realtor
		,char* service_name,int id,int*area,int*rooms);

/**
*  This function receives an realtor,service name and id of apartment
*  and return the apartment price.
*  apartment price should be stored in price.
*  The realtor is given by realtor,the service name is given by
*  service_name and the apartment id is given by id.

* @return
 * REALTOR_INVALID_INPUT if service name/realtor/price are NULL
 * or if id is negative.
 * REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
 * exist in the realtor user.
 * REALTOR_APARTMENT_DOES_NOT_EXIST if apartment does not
 * exist in the realtor user.
 * REALTOR_OUT_OF_MEMORY in case of memory allocation failure.
 * REALTOR_SUCCESS otherwise.
*
*/
RealtorResult realtorGetApartmentPrice(Realtor realtor
		,char* service_name,int id,int*price);

/**
*  This function receives an realtor,service name and id of apartment
*  and return the apartment price whith brokerage fees.
*  apartment price whith brokerage fees should be stored in
*  price_whith_fee.
*  The realtor is given by realtor,the service name is given by
*  service_name and the apartment id is given by id.

* @return
 * REALTOR_INVALID_INPUT if service name/realtor/price_whith_fee
 * are NULL or if id is negative.
 * REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
 * exist in the realtor user.
 * REALTOR_APARTMENT_DOES_NOT_EXIST if apartment does not
 * exist in the realtor user.
 * REALTOR_OUT_OF_MEMORY in case of memory allocation failure.
 * REALTOR_SUCCESS otherwise.
*
*/
RealtorResult realtorGetApartmentPriceWithFee(Realtor realtor
		,char* service_name,int id,int*price_whith_fee);


/**
* This function receives two realtors and compares ther email.
* The realtors are given by realtor1 and realtor2.
*
* @return
* A negative value if realtor1 email is lexicographically
* smaller than realtor2 email.
* A positive value if realtor2 email is lexicographically
* smaller than realtor1 email.
* Zero if realtor1 email is lexicographically equal to realtor2 email.
* realtor1 and realtor2 must not be NULL
*/
int realtorLexicographicalEmail(Realtor realtor1,Realtor realtor2);

/**
* This function receives minimum area size, minimum number of rooms,
* and maximum price, and check if an apartment at the realtor is suits
* them. The minimum area required is given by min_area, the minimum
* number of rooms required is given by min_rooms, and the maximum
* price required is given by max_price,the realtor is given by realtor.
*
* @return
* REALTOR_SUCCESS If an apartment fitting the criteria above exists.
* REALTOR_NO_RELEVANT_APARTMENT if no suitable apartment.
* REALTOR_INVALID_INPUT if realtor is NULL,
* or if either area, rooms or price are not positive.
* REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
* exist in the realtor user.
* REALTOR_OUT_OF_MEMORY in case of memory allocation failure.
*
* */
RealtorResult realtorExsistSuitableApartment(Realtor realtor,
		int min_area, int min_rooms,int max_price);

/**
* Adds a new service to realtor user.
* The realtor is given by realtor,the service name is given
* by service_name and the largest number of apartments that service
* can accommodateid is given by max_apartments.
* REALTOR_INVALID_INPUT if service name or realtor are NULL
 * or if max_apartments is not positive.
* REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS if service name already
* exist in the realtor user.
* REALTOR_OUT_OF_MEMORY in case of memory allocation failure.
* REALTOR_SUCCESS service has been added to realtor user.
*
* */
RealtorResult realtorAddApartmentService(Realtor realtor ,
		char* service_name, int max_apartments);

/**
* Deletes an service from the realtor user.
* The name of the service is given by service name and the realtor
* is given by realtor.
*
* REALTOR_INVALID_INPUT if service name or realtor are NULL.
* REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
* exist in the realtor user.
* REALTOR_OUT_OF_MEMORY in case of memory allocation failure
* REALTOR_SUCCESS service has been removed from realtor user.
*
* */
RealtorResult realtorRemoveApartmentService(Realtor realtor,
		char* service_name);

/**
* Adds a new apartment to realtor service.
* The realtor is given by realtor,the service name is given by
* service_name,the apartment id is given by apartment_id and
* the apartment is given by apartment.
*
* REALTOR_INVALID_INPUT if service name/realtor/apartment are NULL
* or if id is positive.
* REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
* exist in the realtor user.
* REALTOR_APARTMENT_SERVICE_FULL if realtor service full.
* REALTOR_APARTMENT_ALREADY_EXISTS if the apartment already
* exist in the realtor service.
* REALTOR_OUT_OF_MEMORY in case of memory allocation failure
* REALTOR_SUCCESS apartment has been added to realtor service.
*
* */
RealtorResult realtorAddApartment(Realtor realtor,
		char* service_name,Apartment apartment,int apartment_id);

/**
* Deletes an apartment from the realtor service.
* The name of the service is given by service name and the realtor
* is given by realtor,the apartment id is given by apartment_id.
*
* REALTOR_INVALID_INPUT if service name or realtor are NULL, or if
* id is positive.
* REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
* exist in the realtor user.
* REALTOR_APARTMENT_DOES_NOT_EXIST if the apartment does not
* exist in the realtor service.
* REALTOR_OUT_OF_MEMORY in case of memory allocation failure.
* REALTOR_SUCCESS apartment has been removed from realtor service.
*
* */
RealtorResult realtorRemoveApartment(Realtor realtor,
		char* service_name, int id);

#endif /* REALTOR_H_ */
