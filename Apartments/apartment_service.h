#ifndef APARTMENT_SERVICE_H
#define APARTMENT_SERVICE_H

#include "apartment.h"

typedef struct apartment_service_t* ApartmentService;

struct apartment_service_t {
	Apartment* aprts;
	int* ids;
	int* serials;
	int num_of_aprts;
	int max_aprts;
	int serial;
};

typedef enum {
	APARTMENT_SERVICE_SUCCESS,
	APARTMENT_SERVICE_OUT_OF_MEM,
	APARTMENT_SERVICE_NULL_ARG,
	APARTMENT_SERVICE_FULL,
	APARTMENT_SERVICE_NO_FIT,
	APARTMENT_SERVICE_OUT_OF_BOUNDS,
	APARTMENT_SERVICE_EMPTY,
	APARTMENT_SERVICE_ALREADY_EXISTS
} ApartmentServiceResult;

/* This function receives an integer maxNumOfApartments and
 * creates a new, empty apartment service which can contain
 * at most maxNumOfApartments apartments.
 *
 * @return:
 * 	A new allocated apartment service.
 * 	If the input is invalid (maxNumOfApartments<=0) or a memory
 * 	allocation failure has occured, the function returns NULL.
 */
ApartmentService serviceCreate(int maxNumOfApartments);

/* This function creates a newly allocated copy of a given apartment
 * service. The apartments appearing in the new apartment service
 * will also be copies of the apartments appearing in the given
 * apartment service.
 *
 * @return:
 * 	A new copy of the given apartment service.
 * 	If the given apartment service is NULL or a memory allocation
 * 	failure has occured, the function returns NULL.
 */
ApartmentService serviceCopy(ApartmentService service);

/* This function frees all memory allocated for the given apartment service.
 * This function can receive NULL.
 */
void serviceDestroy(ApartmentService service);

/* This function receives an apartment service and returns the number
 * of apartments currently in the apartment service.
 * This function asserts (service != NULL)
 */
int serviceNumberOfApatments(ApartmentService service);

/* This function receives an apartment service, a new apartment and a
 * positive integer id, and adds a copy of the new apartment to the given
 * apartment service, which has the id given (which is a unique identifier for
 * in the service).
 *
 * @return:
 * 	A copy of apartment with the identifier id is added to service.
 *	APARTMENT_SERVICE_NULL_ARG if apartment==NULL.
 * 	APARTMENT_SERVICE_OUT_OF_BOUNDS if id<0.
 * 	APARTMENT_SERVICE_FULL if there is no room in service to contain
 * 	new apartments.
 * 	APARTMENT_SERVICE_ALREADY_EXISTS if an apartment with the given id
 * 	already exists in service.
 */
ApartmentServiceResult serviceAddApartment(ApartmentService service, 									   
										   Apartment apartment, int id);

/* This function receives an apartment service and computes
 * the median price of apartments in this service.
 *
 * @return:
 * 	*outResult contains the median price of apartments in service.
 * 	APARTMENT_SERVICE_EMPTY if there are no apartments in the service.
 */
ApartmentServiceResult servicePriceMedian(ApartmentService service,
										  int* outResult);

/* This function receives an apartment service and computes
 * the median area of apartments in this service.
 *
 * @return:
 * 	*outResult contains the median area of apartments in service.
 * 	APARTMENT_SERVICE_EMPTY if there are no apartments in the service.
 */
ApartmentServiceResult serviceAreaMedian(ApartmentService service, 
										 int* outResult);
/* This function receives an apartment service and an apartment
 * and deletes the most recently-added apartment in the service
 * which is identical in all its parameters to the given apartment.
 *
 * @return:
 * 	The apartment in service which is identical in all parameters
 * 	to the given apartment and was added most recently is deleted
 * 	from the service.
 * 	APARTMENT_SERVICE_NULL_ARG if apartment==NULL.
 * 	APARTMENT_SERVICE_EMPTY if there are no apartments in the service.
 * 	APARTMENT_SERVICE_NO_FIT if there is no apartment in the service with
 * 	the parameters of the apartment input.
 */
ApartmentServiceResult serviceDeleteApartment(ApartmentService service,
											  Apartment apartment);

/* This function receives an apartment service and an identifier id and
 * removes the apartment with this id from the given apartment service.
 *
 * @return:
 * 	The apartment with the identifier id is deleted from service.
 * 	APARTMENT_SERVICE_OUT_OF_BOUNDS if id<0.
 * 	APARTMENT_SERVICE_EMPTY if there are no apartments in the service.
 * 	APARTMENT_SERVICE_NO_FIT if there is no apartment in the service with
 * 	the given id.
 */
ApartmentServiceResult serviceDeleteById(ApartmentService service, int id);

/* This function receives an apartment service and an identifier id and
 * returns an apartment in the service with the given id as identifier.
 *
 * @return:
 * 	*outApartment contains a new copy of the apartment in the service with
 * 	the identifier id.
 * 	APARTMENT_SERVICE_OUT_OF_BOUNDS if id<0.
 * 	APARTMENT_SERVICE_EMPTY if there are no apartments in the service.
 * 	APARTMENT_SERVICE_NO_FIT if there is no apartment in the service with
 * 	the given id.
 */
ApartmentServiceResult serviceGetById(ApartmentService service, int id,
									  Apartment* outApartment);

/* This function receives an apartment service and three integer parameters
 * representing the area, number of rooms and price of a potential apartment.
 * It returns the most recently-added apartment to the service which has area
 * and number of rooms at least as much as the given area and number of rooms
 * parameters (respectively) and a price at most as much as the give
 *  price parameter.
 *
 *  @return:
 *  	*outApartment contains a new copy of the most recently-added apartment
 *  	to the service with:
 *  	1. Area at least 'area'.
 *  	2. Number of rooms at least 'rooms'.
 *  	3. Price at most 'price'.
 *  	APARTMENT_SERVICE_OUT_OF_BOUNDS if one of the integer parameters is
 *  	negative.
 * 		APARTMENT_SERVICE_EMPTY if there are no apartments in the service.
 * 		APARTMENT_SERVICE_NO_FIT if there is no apartment in the service which
 * 		satisfies the requirements above.
 */
ApartmentServiceResult serviceSearch(ApartmentService service, int area,
									 int rooms, int price, 
									 Apartment* outApartment);

#endif /* APARTMENT_SERVICE_H */
