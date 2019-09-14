/*
 * yad3.h
 *
 *  Created on: 27 ׳‘׳�׳₪׳¨ 2016
 *      Author: batel
 */

#ifndef YAD3_H_
#define YAD3_H_

#include <stdio.h>
#include <stdbool.h>


/** Type for defining the yad3*/
typedef struct system_Users_t* SystemUser;

/** Type used for returning error codes from yad3 functions */
typedef enum {
	YAD_3_NULL_PARAMETERS,
	YAD_3_OUT_OF_MEMORY,
	YAD_3_INVALID_PARAMETERS,
	YAD_3_EMAIL_ALREADY_EXISTS,
	YAD_3_EMAIL_DOES_NOT_EXIST,
	YAD_3_EMAIL_WRONG_ACCOUNT_TYPE,
	YAD_3_ALREADY_REQUESTED,
	YAD_3_NOT_REQUESTED,
	YAD_3_APARTMENT_SERVICE_ALREADY_EXISTS,
	YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST,
	YAD_3_APARTMENT_SERVICE_FULL,
	YAD_3_APARTMENT_ALREADY_EXISTS,
	YAD_3_APARTMENT_DOES_NOT_EXIST,
	YAD_3_PURCHASE_WRONG_PROPERTIES,
	YAD_3_REQUEST_WRONG_PROPERTIES,
	YAD_3_REQUEST_ILLOGICAL_PRICE,
} Yad3ErrorCode;

/** Type used for define the types of users */
typedef enum {
	CUSTOMER_USER,
	REALTOR_USER,
} UserStatus;


/**
* Creates a new system users.
* @return
* 	A new allocated system users.
* 	NULL in case of memory allocation failure.
**/
SystemUser systemUsersCreate();

/**
* Frees all memory allocated for the given system users.
* This function can receive NULL.
*/
void systemUsersDestroy(SystemUser system_users);


/**
* Deletes an user from the system users.
* The system users is given by system_users, the email of
* user is given by email, the type of the user is given by type.
*
* @return
* True- if user has been removed from system users.
* False- otherwise,in this case error_code contains the relevant error:
*   YAD_3_INVALID_PARAMETERS if email is invalid,
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_ALREADY_EXISTS if the given email exists in the system.
**/
bool systemUserRemoveUser(SystemUser system,char* email,UserStatus type,
		Yad3ErrorCode* error_code);

/**
* Adds a new realtor user to system.
* The system users is given by system_users,the realtor email is given by
* email,the company name is given by company_name and the share of profit
* of the realtor is given by tax_tax_percentage.
*
* @return
* True- if realtor has been added to system users.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if email is invalid,
*   or if tax_percentage is not positive.
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the email belongs to the customer user.
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
**/
bool systemUserAddRealtor(SystemUser system,char* email,
		char* company_name, int tax_percentage,Yad3ErrorCode*error_code);


/**
* Adds a new customer user to system.
* The system users is given by system_users,the customer email is given by
* email,the minimum number of rooms in apartment that customer is ready to buy
* is given by min_rooms,the minimum area of apartment that customer
* is ready to buy is given by min_area,
* the maximum price the customer is willing to pay for apartment is
* given by max_price.

* @return
* True- if customer has been added to system users.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if email is invalid,
*   or if min_area/max_price/min_rooms are not positive.
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_ALREADY_EXISTS if the given email exists in the system.
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
**/
bool systemUserAddCustomer(SystemUser system,
		char* email,int min_area,int min_rooms,int max_price,
		Yad3ErrorCode*error_code);

/**
* Adds a new service to realtor user.
* The system users is given by system_users,the realtor email is given by
* email,the service name is given by service_name
* and the largest number of apartments that service can accommodateid
* is given by max_apartments.
*
* @return
* True- if service has been added to realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if email is invalid,
*   or if max_apartment not positive.
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the email belongs to the customer user.
*   YAD_3_APARTMENT_SERVICE_ALREADY_EXISTS if service name already
*   exist in the realtor user.
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
*
* */
bool systemUserAddToRealtorApartmentService(SystemUser system,char* email,
		char* service_name,int max_apartments, Yad3ErrorCode*error_code);


/**
* Deletes an service from the realtor user.
* The system users is given by system_users,the realtor email is given by
* email,the service name is given by service_name.
*
* @return
* True- if service has been removed from realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if email is invalid,
*   or if max_apartment not positive.
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the email belongs to the customer user.
*   YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
*   exist in in the realtor user.
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
*
* */
bool systemUserRemoveApartmentServiceFromeRealtor(SystemUser system,
		char* email, char* service_name,Yad3ErrorCode*error_code);

/**
* Adds an apartment to the realtor user.
* The system users is given by system_users, the realtor email is given by
* email,the service name is given by service_name , the apartment id is
* given by apartment id, the price is given by price, the width is
* gien by width, the height is given by height, matrix of char representing
* apartment space of wall or empty tiles by matrix string of "w" or "e"
* chars.
*
* @return
* True- if aparment has been added to service of realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if email/service name is invalid,
*   or if max_apartment/id/price/width/height not positive or
*   matrix includes a char differs from 'w' or 'e'.
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the email belongs to the customer user.
*   YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
*   exist in in the realtor user.
*   YAD_3_APARTMENT_SERVICE_FULL if service is full
	YAD_3_APARTMENT_ALREADY_EXISTS - if the apartment is already in service
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
*
* */
bool systemUserAddApartmentToRealtor(SystemUser system,char* email,
	char* service_name, int id,int price,int width, int height, char*matrix,
	Yad3ErrorCode*error_code);

/**
* Removes an apartment from the realtor user.
* The system users is given by system_users, the realtor email is given by
* email,the service name is given by service_name , the apartment id is
* given by apartment id.
*
* @return
* True- if service has been removed from service of realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if email/service name is invalid,
*   or if id not positive.
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the email belongs to the customer user.
*   YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
*   exist in in the realtor user.
	YAD_3_APARTMENT_DOES_NOT_EXISTS - if the apartment is already in service
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
*
* */
bool systemUserRemoveApartmentFromeRealtor(SystemUser system,char* email,
	 char* service_name, int id,Yad3ErrorCode*error_code);


/**
 * Customer purchases from realtor by regular apartment's price including
 * realtor's tax, which is hold in his services.
* The system users is given by system_users, the realtor email is given by
* email, the customer's email is given by customers_email, the service name
* is given by service_name, apartment's id is given by apartment_id.
*
* @return
* True- if apartment has been purchased from realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if realtors_email/customer's_email invalid
*   or choice differs from "accept"/"decline".
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the email belongs to the customer user.
*   YAD_3_ALREADY_REQUESTED - offer from customer to realtor allready exists.
*   YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
*   exist in in the realtor user.
*   YAD_3_APARTMENT_DOES_NOT_EXISTS - if the apartment is already in service.
*   YAD_3_PURCHASE_WRONG_PROPERTIES if the apartment's properties: number of
*   rooms, area or price doesn't fit client demands.
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
*
* */
bool systemUserCustomerPurchase(SystemUser system,char* email,
		char* realtor_email, char*service_name, int apartment_id,
		Yad3ErrorCode* error_code);


/**
 * Customer makes offer to realtor to purchase by cheaper price an apartment
 * realtor holds in one of his services.
*  The offer is data which is: apartments id to purchase, service name from
*  to buy and a better price. The offer exist in the system until the
*  apartment is in it's realtor owner and removed if realtors answer it or
*  the apartment removed. Only one offer at a time can be hold in the system
*  netween a customer and a realtor.
*
* The system users is given by system_users, the realtor email is given by
* email, the customer's email is given by customers_email, the realtors
* choice is given by choice.
*
* @return
* True- if offer has been made from customer to realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if realtors_email/customer's_email invalid
*   or choice differs from "accept"/"decline".
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the email belongs to the customer user.
*   YAD_3_ALREADY_REQUESTED - offer from customer to realtor allready exists.
*   YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
*   exist in in the realtor user.
*   YAD_3_APARTMENT_DOES_NOT_EXISTS - if the apartment is already in service.
*   YAD_3_REQUEST_WRONG_PROPERTIES if the apartment's properties: number of
*   rooms, area or price doesn't fit client demands.
	YAD_3_REQUEST_ILLOGICAL_PRICE if a regular purchase is cheaper or same
	cost as by the offer.
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
*
* */
bool systemUserCustomerMakeOffer(SystemUser system,char* email,
		char* realtor_email,char*service_name, int apartment_id,
		int new_price,Yad3ErrorCode* error_code);

/**
* Reports all relevant realtors to customer. Relevance defined by the
* condtion: if a realtor holds at least one apartment which fits the
* customers demands: at least customer's number of rooms, area and price
* non greater the he can afford. The report made by lexicographical order.
*
* @return
* True- if service has been removed from realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments(including FILE stream)
* 	 is null except error_code
*   YAD_3_INVALID_PARAMETERS if realtors_email/customer's_email invalid
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if one of the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the one of the emails
*   belongs to the other user's type.
*
* */
bool systemUseReportRelevantRealtors(SystemUser system,char* customer_email,
		FILE* output,Yad3ErrorCode* error_code);

/**
* Reports  the most significant realtors of the system. Significance
* factor including realtors total amount of apartment, average of medians
* of services by apartments price and average of medians of services by area.
* The reporat is made by lexicographical order by emails. Realtors whithout
* apartments will not be printed.
*
* The system users is given by system_users, the amount of realtors to
*  print given by count, stream of output is given by output.
*
* @return
* True- if service has been removed from realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments(including FILE stream)
* 	 is null except error_code.
*   YAD_3_INVALID_PARAMETERS if count is negative.
* */
bool systemUserReportSignificantRealtors(SystemUser system,int count,
		FILE* output,Yad3ErrorCode* error_code);

/**
* Reports  the most paying customers of the system.
* The reporat is made by lexicographical order by emails. Customers
*  whithout expenses will not be printed.
*
* The system users is given by system_users, the amount of customers to
*  print given by count, stream of output is given by output.
*
* @return
* True- if service has been removed from realtor user.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments(including FILE stream)
* 	 is null except error_code.
*   YAD_3_INVALID_PARAMETERS if count is negative.
* */
bool systemUserReportMostPayingCustomers(SystemUser system,int count,
		FILE* output,Yad3ErrorCode* error_code);

/**
 * Realtor answers to a offer was made by customer.
* Removes an apartment from the realtor user if his choice is "accpet" and
* adds it to customers purchased apartments by special offer's price.
* and does not anything if "decline". The offer is removed from the system.
* The system users is given by system_users, the realtor email is given by
* email, the customer's email is given by customers_email, the realtors
* choice is given by choice.
*
* @return
* True- if a response of accpet od decline has been made succecfully
* from realtor user to customer.
* False- otherwise,in this case error_code contains the relevant error:
* 	YAD_3_NULL_PARAMETERS if one of arguments is null except error_code
*   YAD_3_INVALID_PARAMETERS if realtors_email/customer's_email invalid
*   or choice differs from "accept"/"decline".
*   valid Email contains exactly one character of @.
*   YAD_3_EMAIL_DOES_NOT_EXIST if the given email does not
*   exist in the system.
*   YAD_3_EMAIL_WRONG_ACCOUNT_TYPE if the email belongs to the customer user.
*   YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST if service name does not
*   exist in in the realtor user.
*   YAD_3_NOT_REQUESTED if an offer from customer to realtor wasn't made.
*   YAD_3_OUT_OF_MEMORY in case of memory allocation failure.
*
* */
bool systemUserRealtorRespondToOffer(SystemUser system,char* email,
		char*customer_email, char* choice,Yad3ErrorCode* error_code);

#endif /* YAD3_H_ */
