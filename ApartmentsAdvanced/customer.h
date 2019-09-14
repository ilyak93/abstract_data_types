/*
 * customer_user.h
 *
 *  Created on: 27 באפר 2016
 *      Author: batel
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

 /** Type for defining the customer*/
typedef struct customer_t* Customer;

/** Type used for returning result codes from customer functions */
typedef enum {
	CUSTOMER_OUT_OF_MEMORY,
	CUSTOMER_INVALID_INPUT,
	CUSTOMER_PURCHASE_WRONG_PROPERTIES,
	CUSTOMER_REQUEST_WRONG_PROPERTIES,
	CUSTOMER_ALREADY_REQUESTED,
	CUSTOMER_NOT_REQUESTED,
	CUSTOMER_REQUEST_ILLOGICAL_PRICE,
	CUSTOMER_SUCCESS
} CustomerResult;


/**
* Creates a new customer: allocates and fullfills the
* customer by given data:
* each customer has a mail and valid properties of apartments to purchase:
* minimum area, minimum count of rooms, maximum price of the apartments.
*
* @param
	min_area - minimum area desired by the client
	min_room - minimum amount of room have to be in the apartment
*   max_price - maximum price of the apartment
*
* @return
* 	NULL email is NULL or if minimum area/minimum room/maximum price
* 	are not positive,or if email is invalid.
* 	Result contains the relevant result:
* 	CUSTOMER_INVALID_INPUT if minimum area/minimum room/maximum price
* 	are not positive,or if email is invalid.
* 	CUSTOMER_OUT_OF_MEMORY in case of memory allocation failure.
*   CUSTOMER_SUCCESS if new customer successfully created.
*
*   Valid Email contains exactly one character of @
* 	A new allocated apartment otherwise.
*
* 	Result must not be NULL.
**/
Customer customerCreate(int min_area,int min_room,
		int max_price,char*email, CustomerResult*result);

/**
* Frees all memory allocated for the given  customer.
*@param
*	customer - customer to destroy
*
*This function can receive NULL.
*
* @return
*	none
*/
void customerDestroy(Customer customer);

/**
* Creates a newly allocated copy of a given customer.
*
* @param
*	customer - source to copy from
*
* @return
* A newly allocated copy of the original customer.
* NULL if the customer is NULL or in any case of memory allocation failure
*/
Customer customerCopy(Customer customer);

/**
* Return the minimum area desired by the customer
*
* @param
*	customer - customer to get from the area
*
* @return
* 	the minimum area of the desired apartment.
*   -1 if customer argument NULL.
*/
int customerGetMinArea(Customer customer);


/**
* Return the minimum rooms desired by the customer
*
* @param
*	customer - customer to get from the amount of rooms
*
* @return
* 	the minimum rooms of the desired apartment.
*   -1 if customer argument NULL.
*/
int customerGetMinRooms(Customer customer);

/**
* Return the maximum price customer can afford
*
* @param
*	customer - customer to get from the price
*
* @return
* 	the maximum price of the desired apartment.
*   -1 if customer argument NULL.
*/
int customerGetMaxPrice(Customer customer);

/**
*   Creates a copy of customer email.
*
*	@param
*		customer - customer to get from email
*
*	@return
* 		the email string of customer.
*		NULL if customer param is NULL or in case of
*		memory allocation failure
*/
char*customerGetEmail(Customer customer);

/**
* The amount of housing prices the same customer purchased
*
* @param
*	customer - customer to get from the number of purchases
*
* @return
* 	the muber of purchased apartments.
*   -1 if customer argument NULL.
*/
int customerGetAmountPurchases(Customer customer);

/**
*  This function checks if an offer from customer was allready made
*	to realtor.
*
* @param
*	customer - customer made/not made offer
*   realtor_email - realtor offered to him an offer
*
* @return
* CUSTOMER_INVALID_INPUT if realtor/customer null args
* or if email is invalid.
* Valid Email contains exactly one character of @.
* CUSTOMER_ALREADY_REQUESTED if an offer from customer was made
* to this realtor.
* CUSTOMER_NOT_REQUESTED if no proposal was sent from customer to realtor.
*
*/
CustomerResult customerCheckIfOfferExist(Customer customer,
		char*realtor_email);

/**
* Return the result of comparing 2 emails lexicographically
*
* @param
*	customer1 - first to compare
*   customer2 - second to compare
*
* @return
*	negative result if first lexicographically lesser then second
*   zero if the identical
*   positive result if first greater lexicographically from second
*/
int customerLexicographicalEmail(Customer customer1,Customer customer2);

/**
*  This function removes an offer was made by client to realtor.
*
* @param
*	customer - customer made the offer
*   realtor - realtor was offered an offer
*
* @return
* @return
* CUSTOMER_INVALID_INPUT if realtor email/customer null args or if
* email is invalid.
* Valid Email contains exactly one character of @.
* CUSTOMER_SUCCESS offer removed succefully or offer does not exist.
*
*/
CustomerResult customerRemoveOfferAccordingEmail(Customer customer,
		char* realtor_email);

/**
*  This function removes all offers are made by customer which
*  made to apartment of given service.
*
* @param
*	customer - customer made the offers
*   realtor_email - email of realtor offered offers
*	service_name - used to find offers include apartments
*	which were offered to purchase from realtor of that service.
*
* @return
* CUSTOMER_INVALID_INPUT if realtor email/customer/service name null
* args or if email is invalid.
* Valid Email contains exactly one character of @.
* CUSTOMER_SUCCESS offer removed succefully or offer does not exist.
*
*/
CustomerResult customerRemoveOfferAccordingService(Customer customer,
		char*realtor_email,char*service_name);

/**
*  This function removes all exist offers are made by customer which
*  made to apartment of given id.
*
* @param
*	customer - customer made the offers
*   realtor_email - email of realtor offered offers
*	apartment_id - used to find offers which referennses
*	the apartment of that apartment_id.
*
* @return
* CUSTOMER_INVALID_INPUT if realtor email/customer/service namr null
* args or if apartment id is negative, or if email is invalid.
* Valid Email contains exactly one character of @.
* CUSTOMER_SUCCESS offer removed succefully or offer does not exist.
*
*/
CustomerResult customerRemoveOfferAccordingId(Customer customer,
		char*realtor_email,char*service_name,int id);

/**
*  This function receives an customer which makes an offer to the recieved
*	realtor. It stored and belongs to customer, so the offers he made are
*	supported by other function of customer.
*
* @param
*	customer - customer wich makes the offer
*   realtor_email - rialtor which the offer is made
*	service_name - service from which the customer wants to purchase the
*	apartment.
*	price - the price of the apartment the customer is making an offer to.
*	 must be positive
*	new_price - a price which has to be smaller then
*	then the original apartment's price with including rialtor's tax.
*	apartment_id - the id of the desired appartment
*	area - the area of desired apartment. must be non-negative.
*	num_of_rooms - number of rooms of desired apartment. must be
*	non-negative.
*
* @return
* CUSTOMER_INVALID_INPUT if rialtor email/customer/service name null
*  	args or number of rooms/minimal area/price isn't positive or
*   apartment id is negative.
* CUSTOMER_ALREADY_REQUESTED if an offer from the same customer already
* 	sent to this realtor an offer.
* CUSTOMER_REQUEST_WRONG_PROPERTIES if realtors's number of rooms lesser
* 	then the customer's demand or are is lesser then minimal area or price
* 	is greater then customer can afford.
* CUSTOMER_OUT_OF_MEMORY in case of memory allocation failure.
* CUSTOMER_SUCCESS otherwise.
*
*/
CustomerResult customerMakeOffer(Customer customer,char*realtor_email,
		char*service_name,int num_of_rooms, int area, int price,
		int new_price,int apartment_id);

/**
* Return the new price of an offer made by customer to realtor.
*
* @param
*	customer - customer to get from the new price
*   rialtor_email - email of realtor which was offered to.
*   price - integer address which return the price
*
* @return
*	CUSTOMER_INVALID_INPUT if rialtor mail/customer null args
*	CUSTOMER_NOT_REQUESTED if an offer from the customer was not
*	requsted from the rialtor.
*	CUSTOMER_SUCCESS and the new price was offered
*/
CustomerResult customerOfferGetPrice(Customer customer,
		char*realtor_email,int* price);

/**
* Return a copy of the service name of an offer made by customer to realtor.
*
* @param
*	customer - customer to get from the service name
*   rialtor_email - email of realtor which was offered to.
*   result - an address in which will be returned error code.
*   cannot be NULL;
*
* @return
*	CUSTOMER_INVALID_INPUT if rialtor/customer null args
*	CUSTOMER_NOT_REQUESTED if an offer from the customer was not
*	requsted from the rialtor.
*	CUSTOMER_SUCCESS and the service name
*
*/
char* customerOfferGetServiceName(Customer customer,
		char* realtor_email,CustomerResult* result);

/**
* Return the apartment id of an offer made by customer to realtor.
*
* @param
*	customer - customer to get from the apartment id
*   rialtor_email - email of realtor which was offered to.
*   apartment_id - an address in which will be returned apartment id.
*
* @return
*	CUSTOMER_INVALID_INPUT if rialtor/customer null args
*	CUSTOMER_NOT_REQUESTED if an offer from the customer was not
*	requsted from the rialtor.
*	CUSTOMER_SUCCESS and the apartment's id
*/
CustomerResult customerOfferGetId(Customer customer,char* realtor_email,
		int* apartment_id);
/**
*  This function complete the offer making function by making the purchase
*  which belong to certain offer.  By result, offer is removed and the
*  customer paying a price he offered without any taxes. The offer is also
*  removed from customer.
*
* @param
*	customer - customer made the offer and purchases the apartment.
*   realtor_email - email of rialtor offered offer.
*   service_name - service to which offer's apartment belongs.
*   apartment_id - apartment's id.
*
* @return
* CUSTOMER_INVALID_INPUT if rialtor email/customer/service name null args
* apartment id isn't positive.
* CUSTOMER_NOT_REQUESTED if an offer from the customer was not
* requsted from the rialtor.
* CUSTOMER_SUCCESS if purchase succeded.
*
*/
CustomerResult customerMakePurchaseByOffer(char* realtor_email,
		Customer customer,char*service_name,int apartment_id);

/**
*  This function makes the customer to purchase an apartment directly
*  without making an offer, by the apartment's price.
*
* @param
*	customer - customer made the offer and purchases the apartment
*   realtor_email - email of rialtor offered offer
*   service_name - the service from which the apartment is
*   apartment_id - apartment's id
*   num_of_rooms - apartment's number of rooms
*   min_area - apartment's area size
*   price - apartment's price
*
* @return
* CUSTOMER_INVALID_INPUT if rialtor email/customer/service name null
*  	args or number of rooms/area isn't non-negative or price isnt
*  	positive apartment id is negative.
* CUSTOMER_PURCHASE_WRONG_PROPERTIES if realtors's number of rooms lesser
* 	then the customer's demand or are is lesser then minimal area or price
* 	is greater then customer can afford.
* CUSTOMER_SUCCESS if purchase succeded.
*
*/
CustomerResult customerMakePurchase(char* realtor_email,
		Customer customer,char*service_name,int apartment_id,
		int num_of_rooms, int area, int price);


#endif /* CUSTOMER_H_ */
