/*
* apartment->c
*
*  Created on: Mar 25, 2016
*      Author: Ilya
*/

#include <stdio.h>
#include <stdlib.h>
#include "apartment_service.h"
#include <assert.h>

typedef int(*GetApartmentValueFunc)(Apartment);

/* Receives an array 'arr' of integers and and its length 'n'
 * and applies "Bubble Sort" to the array.
 *
 * @return:
 * The array is sorted (in ascending order).
 * The function asserts arr!=NULL.
 */
static void bubSort(int* arr, int n) {
	assert(arr != NULL);
	if (n == 1) {
		return;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				int tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

/* This function receives an apartment service and a function of type
 * GetApartmentValueFunc which assigns a value to every apartment in the
 * service. The function returns the median of these values.
 *
 * @return:
 * 	the median value of {GetValue(apartment) : apartment is in 'service'}.
 * 	-1 if a dynamic memory allocation fails.
 * 	The function asserts (service!=NULL) and (getValue!=NULL).
 */
static int Median(ApartmentService service, GetApartmentValueFunc GetValue) {
	assert(service != NULL && GetValue != NULL);
	int* values = (int*)malloc(sizeof(int)*service->num_of_aprts);
	if (!values) {
		return -1;
	}
	int itr = 0;
	for (int i = 0; i < service->max_aprts; i++) {
		if (service->aprts[i]) {
			values[itr++] = GetValue(service->aprts[i]);
		}
	}
	bubSort(values, itr);
	if (itr % 2 == 0) {
		int res = (values[itr / 2 - 1] + values[itr / 2]) / 2;
		free(values);
		return res;
	}
	int res = values[itr / 2];
	free(values);
	return res;
}



ApartmentService serviceCreate(int maxNumOfApartments) {
	if (maxNumOfApartments < 1) {
		return NULL;
	}
	ApartmentService ap_srvc = (ApartmentService)malloc(sizeof(*ap_srvc));
	if (!ap_srvc) {
		return NULL;
	}
	ap_srvc->aprts = NULL;
	ap_srvc->ids = NULL;
	ap_srvc->serials = NULL;
	ap_srvc->ids = (int*)malloc(sizeof(int)*maxNumOfApartments);
	ap_srvc->serials = (int*)malloc(sizeof(int)*maxNumOfApartments);
	if (!ap_srvc->ids || !ap_srvc->serials) {
		serviceDestroy(ap_srvc);
		return NULL;
	}
	ap_srvc->aprts = (Apartment*)malloc(sizeof(Apartment)*maxNumOfApartments);
	if(!ap_srvc->aprts){
		serviceDestroy(ap_srvc);
	}
	for (int i = 0; i < maxNumOfApartments; i++) {
		ap_srvc->aprts[i] = NULL;
	}
	ap_srvc->num_of_aprts = 0;
	ap_srvc->serial = 0;
	ap_srvc->max_aprts = maxNumOfApartments;
	return ap_srvc;
}

void serviceDestroy(ApartmentService service) {
	if (service) {
		if(service->aprts){
			for (int i = 0; i < service->max_aprts; i++) {
				apartmentDestroy(service->aprts[i]);
			}
		}
		free(service->aprts);
		free(service->ids);
		free(service->serials);
		free(service);
	}
}

ApartmentService serviceCopy(ApartmentService service) {
	if (!service) {
		return NULL;
	}
	ApartmentService srvc = serviceCreate(service->max_aprts);
	if (!srvc) {
		return NULL;
	}
	for (int i = 0; i < service->max_aprts; i++) {
		if (service->aprts[i]) {
			srvc->aprts[i] = apartmentCopy(service->aprts[i]);
			if (!srvc->aprts[i]) {
				serviceDestroy(srvc);
				return NULL;
			}
			srvc->ids[i] = service->ids[i];
			srvc->serials[i] = service->serials[i];
		} else {
			srvc->aprts[i] = NULL;
		}
	}
	srvc->max_aprts = service->max_aprts;
	srvc->num_of_aprts = service->num_of_aprts;
	srvc->serial = service->serial;
	return srvc;
}

int serviceNumberOfApatments(ApartmentService service) {
	assert(service != NULL);
	return service->num_of_aprts;
}

ApartmentServiceResult serviceAddApartment(ApartmentService service,
	Apartment apartment, int id) {
	if (!service || !apartment) {
		return APARTMENT_SERVICE_NULL_ARG;
	} else if (id < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	} else if (service->num_of_aprts >= service->max_aprts) {
		return APARTMENT_SERVICE_FULL;
	} else {
		int free_ind;
		for (int i = 0; i < service->max_aprts; i++) {
			if (service->aprts[i] && service->ids[i] == id) {
				return APARTMENT_SERVICE_ALREADY_EXISTS;
			} else if (!service->aprts[i]) {
				free_ind = i;
			}
		}
		Apartment copy = apartmentCopy(apartment);
		if (!copy) {
			return APARTMENT_SERVICE_OUT_OF_MEM;
		}
		service->aprts[free_ind] = copy;
		service->ids[free_ind] = id;
		service->serials[free_ind] = service->serial;
		service->serial++;
		service->num_of_aprts++;
	}
	return APARTMENT_SERVICE_SUCCESS;
}

ApartmentServiceResult serviceDeleteApartment(ApartmentService service,
	Apartment apartment) {
	if (!service || !apartment) {
		return APARTMENT_SERVICE_NULL_ARG;
	} else if (service->num_of_aprts < 1) {
		return APARTMENT_SERVICE_EMPTY;
	}
	int max_i = -1; int max_serial = -1;
	for (int i = 0; i < service->max_aprts; i++) {
		if (service->aprts[i] &&
			apartmentIsIdentical(service->aprts[i], apartment) &&
			service->serials[i] > max_serial) {
			max_serial = service->serials[i];
			max_i = i;
		}
	}
	if (max_i == -1) {
		return APARTMENT_SERVICE_NO_FIT;
	}
	apartmentDestroy(service->aprts[max_i]);
	service->aprts[max_i] = NULL;
	service->num_of_aprts--;
	return APARTMENT_SERVICE_SUCCESS;
}


ApartmentServiceResult serviceDeleteById(ApartmentService service, int id) {
	if (!service) {
		return APARTMENT_SERVICE_NULL_ARG;
	} else if (id < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	} else if (service->num_of_aprts < 1) {
		return APARTMENT_SERVICE_EMPTY;
	} else {
		for (int i = 0; i < service->max_aprts; i++) {
			if (service->aprts[i] && service->ids[i] == id) {
				apartmentDestroy(service->aprts[i]);
				service->aprts[i] = NULL;
				service->num_of_aprts--;
				return APARTMENT_SERVICE_SUCCESS;
			}
		}
	}
	return APARTMENT_SERVICE_NO_FIT;
}

ApartmentServiceResult serviceSearch(ApartmentService service, int area,
	int rooms, int price, Apartment* outApartment) {
	if (!service || !outApartment) {
		return APARTMENT_SERVICE_NULL_ARG;
	} else if (area < 0 || rooms < 0 || price < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	} else if (service->num_of_aprts < 1) {
		return APARTMENT_SERVICE_EMPTY;
	}
	Apartment found = NULL;
	int serial = -1;
	for (int i = 0; i < service->max_aprts; i++) {
		if (service->aprts[i] && service->aprts[i]->price <= price &&
			apartmentTotalArea(service->aprts[i]) >= area &&
			apartmentNumOfRooms(service->aprts[i]) >= rooms &&
			service->serials[i] > serial) {
			found = service->aprts[i];
			serial = service->serials[i];
		}
	}
	if (found) {
		Apartment copy = apartmentCopy(found);
		if (!copy) {
			return APARTMENT_SERVICE_OUT_OF_MEM;
		}
		*outApartment = copy;
		return APARTMENT_SERVICE_SUCCESS;
	}
	return APARTMENT_SERVICE_NO_FIT;
}

ApartmentServiceResult serviceGetById(ApartmentService service, int id, Apartment* outApartment) {
	if (!service || !outApartment) {
		return APARTMENT_SERVICE_NULL_ARG;
	} else if (id < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	} else if (service->num_of_aprts < 1) {
		return APARTMENT_SERVICE_EMPTY;
	}
	for (int i = 0; i < service->max_aprts; i++) {
		if (service->aprts[i] && service->ids[i] == id) {
			Apartment copy = apartmentCopy(service->aprts[i]);
			if (!copy) {
				return APARTMENT_SERVICE_OUT_OF_MEM;
			}
			*outApartment = copy;
			return APARTMENT_SERVICE_SUCCESS;
		}
	}
	return APARTMENT_SERVICE_NO_FIT;
}

ApartmentServiceResult serviceAreaMedian(ApartmentService service, int* outResult) {
	if (!service || !outResult) {
		return APARTMENT_SERVICE_NULL_ARG;
	} else if (service->num_of_aprts < 1) {
		return APARTMENT_SERVICE_EMPTY;
	}
	*outResult = Median(service, apartmentTotalArea);
	if (*outResult == -1) {
		return APARTMENT_OUT_OF_MEM;
	}
	return APARTMENT_SERVICE_SUCCESS;
}

ApartmentServiceResult servicePriceMedian(ApartmentService service,
	int* outResult) {
	if (!service || !outResult) {
		return APARTMENT_SERVICE_NULL_ARG;
	} else if (service->num_of_aprts < 1) {
		return APARTMENT_SERVICE_EMPTY;
	}
	*outResult = Median(service, apartmentGetPrice);
	if (*outResult == -1) {
		return APARTMENT_OUT_OF_MEM;
	}
	return APARTMENT_SERVICE_SUCCESS;
}
