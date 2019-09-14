#ifndef ORANGE_H_
#define ORANGE_H_

#include<stdbool.h>

#define ORANGE_MAX_SIZE 256

typedef struct orange_t* Orange;

/*
 * Date in months
 */
typedef enum {
	JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC,
} Month;


/**
 * Results and errors codes
 */
typedef enum {
	ORANGE_SUCCESS,
	ORANGE_OUT_OF_MEMORY,
	ORANGE_ILLEGAL_ARGUMENT,
	ORANGE_NO_PLACE_FOR_COMPANY,
	ORANGE_ALREADY_INSIDE,
	ORANGE_COMPANY_ALREADY_EXISTS,
	ORANGE_COMPANY_DOESNT_EXISTS
} OrangeResult;

struct orange_t {

	short size;
	Month expirationMonth;
	char** foodCompanies;
	int maxNumberOfFoodCompanies;
	int sellingPrice;
};

/**
 * Creates a new Orange.
 *
 *
 * @return
 * 	A new allocated Orange,
 * 	or NULL in case of error.
 */
Orange orangeCreate(short size, Month expirationMonth, int maxNumberOfFoodCompanies, int sellingPrice);

/**
 * Copies an Orange.
 *
 * @return
 * 	A new allocated copy of the Orange,
 * 	or NULL in case of error.
 */
Orange orangeCopy(const Orange src);


/**
 * Compare between 2 oranges.
 *
 * @return
 * 	1 - if size of orange1 is bigger then size of orange2,
 * 	-1 - if size of orange2 is bigger then size of orange1,
 * 	0 - if the sizes of the oranges are equal.
 */
int orangeComparebySize(Orange orange1, Orange orange2);

/**
 * Adds an optional food company to the orange.
 *
 * @return
 * 	Result code.
 */
OrangeResult orangeAddFoodCompany(Orange orange,const char* foodCompany);


/**
 * Removes an optional food company from the orange.
 *
 * @return
 * 	Result code.
 */
OrangeResult orangeRemoveFoodCompany(Orange orange, const char* foodCompany);

/**
 * Sets a foodCompany to be the chosen food company which the orange
 * will be sent.
 *
 * @return
 * 	Result code.
 */
OrangeResult orangeSetFoodCompanyForDelivery(Orange orange, const char* foodCompany);

/**
 * Returns the Price of the orange.
 *
 * @return
 * 	The price.
 */
unsigned int orangeGetPrice(const Orange orange);

/**
 * Returns the Weight of the orange.
 *
 * @return
 * 	The weight.
 */
unsigned int orangeGetSize(const Orange orange);


/**
 * Returns the food companies of the orange.
 *
 * @return
 * 	The food companies.
 */
char* const * orangeGetFoodCompanies(const Orange orange);

/**
 * Returns the expiration date of the orange.
 *
 * @return
 * 	The expiration date.
 */
Month orangeGetExpirationMonth(const Orange orange);

/**
 * Returns the the number of food companies for the orange.
 *
 * @return
 * 	The expiration date.
 */
unsigned int orangeGetNumberOfFoodCompanies(const Orange org);
/**
 * Destroys an orange - frees its memory.
 */
void orangeDestroy(Orange orange);



#endif /* ORANGE_H_ */
