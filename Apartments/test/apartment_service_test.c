
#include "aux_macros.h"
#include "apartment_service.h"
#include <assert.h>
#include <stdlib.h>

bool service_test_combo() {
	bool final = true;

	ApartmentService service = serviceCreate(2);
	ApartmentService service2 = serviceCopy(service);
	SquareType square = EMPTY;
	SquareType* squarePtr = &square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 1, 100);
	int intResult = -1;
	Apartment apartmentRes = NULL;

	int id = 1000;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAreaMedian(NULL, &intResult));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceDeleteApartment(service, apartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, apartment, id));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service, &intResult));
	TEST_EQUALS(final, 100, intResult);
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceSearch(service, 50, 50, 50, &apartmentRes));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceGetById(service, 2000, &apartmentRes));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, id));

	apartmentDestroy(apartment);
	serviceDestroy(service);
	serviceDestroy(service2);

	return final;
}
bool service_creation_copy_destruction_test() {
	bool final = true;

	SquareType square = EMPTY;
	SquareType* squarePtr = &square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 1, 100);
	TEST_EQUALS(final, NULL, serviceCreate(0));
	TEST_EQUALS(final, NULL, serviceCreate(-1));
	ApartmentService service = serviceCreate(2);
	TEST_EQUALS(final, NULL, serviceCopy(NULL));
	ApartmentService copy = serviceCopy(service);
	int id = 1000;
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, apartment, id));
	Apartment apartmentRes = NULL;
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceGetById(copy, 1000, &apartmentRes));

	apartmentDestroy(apartment);
	serviceDestroy(service);
	serviceDestroy(copy);

	return final;
}


bool add_delete_test() {
	bool final = true;

	int length1 = 20, width1 = 15;
	SquareType** rooms4 = (SquareType**)malloc(sizeof(SquareType*)*length1);
	for (int i = 0; i < length1; i++) {
		rooms4[i] = (SquareType*)malloc(sizeof(SquareType)*width1);
		for (int j = 0; j < width1; j++) {
			rooms4[i][j] = EMPTY;
		}
	}
	for (int j = 0; j < length1; j++) {
		rooms4[j][width1 / 2] = WALL;
	}
	for (int j = 0; j < width1; j++) {
		rooms4[length1 / 2][j] = WALL;
	}

	int length2 = 30, width2 = 20;
	SquareType** wall_empty = (SquareType**)malloc(sizeof(SquareType*)*length2);
	for (int i = 0; i < length2; i++) {
		wall_empty[i] = (SquareType*)malloc(sizeof(SquareType)*width2);
		for (int j = 0; j < width2; j++) {
			if (j % 2 == 0) {
				wall_empty[i][j] = WALL;
			}
			else {
				wall_empty[i][j] = EMPTY;
			}
		}
	}

	Apartment rooms4_ap = apartmentCreate(rooms4, length1, width1, 1000);
	Apartment wall_empty_ap = apartmentCreate(wall_empty, length2, width2, 500);
	Apartment rooms4_ap_copy = apartmentCopy(rooms4_ap);

	ApartmentService service = serviceCreate(1);
	int id1 = 1001, id2 = 563, id3 = 0;

	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAddApartment(NULL, rooms4_ap, id1));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAddApartment(service, NULL, id1));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAddApartment(NULL, rooms4_ap, -1));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceAddApartment(service, rooms4_ap, -1));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceDeleteApartment(service, rooms4_ap));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceDeleteById(service, 0));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceDeleteById(service, -1));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, rooms4_ap, id3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceDeleteById(service, -1));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceDeleteById(NULL, id2));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteById(service, id2));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteApartment(service, wall_empty_ap));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, 0));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, rooms4_ap, id3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_FULL, serviceAddApartment(service, rooms4_ap, id3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceAddApartment(service, rooms4_ap, -3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAddApartment(NULL, rooms4_ap, -3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAddApartment(service, NULL, -3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_FULL, serviceAddApartment(service, rooms4_ap, id1));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceDeleteApartment(NULL, rooms4_ap_copy));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceDeleteApartment(service, NULL));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service, rooms4_ap_copy));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service));
	serviceDestroy(service);

	ApartmentService service2 = serviceCreate(2);
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service2));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
		serviceAddApartment(service2, rooms4_ap, id3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service2));
	TEST_EQUALS(final, APARTMENT_SERVICE_ALREADY_EXISTS,
		serviceAddApartment(service2, rooms4_ap, id3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service2));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
		serviceAddApartment(NULL, rooms4_ap, id3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service2));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
		serviceAddApartment(service2, NULL, id3));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service2));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
		serviceAddApartment(service2, rooms4_ap, id2));
	TEST_EQUALS(final, 2, serviceNumberOfApatments(service2));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service2, rooms4_ap_copy));
	Apartment ap_id3 = NULL;
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service2, id3, &ap_id3));
	apartmentDestroy(ap_id3);
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceGetById(service2, id2, &ap_id3));

	apartmentDestroy(rooms4_ap);
	apartmentDestroy(wall_empty_ap);
	apartmentDestroy(rooms4_ap_copy);

	for (int i = 0; i < length1; i++) {
		free(rooms4[i]);
	}
	free(rooms4);

	for (int i = 0; i < length2; i++) {
		free(wall_empty[i]);
	}
	free(wall_empty);

	serviceDestroy(service2);

	return final;
}

bool search_get_test() {
	bool final = true;

	int length1 = 20, width1 = 15;
	SquareType** rooms4 = (SquareType**)malloc(sizeof(SquareType*)*length1);
	for (int i = 0; i < length1; i++) {
		rooms4[i] = (SquareType*)malloc(sizeof(SquareType)*width1);
		for (int j = 0; j < width1; j++) {
			rooms4[i][j] = EMPTY;
		}
	}
	for (int j = 0; j < length1; j++) {
		rooms4[j][width1 / 2] = WALL;
	}
	for (int j = 0; j < width1; j++) {
		rooms4[length1 / 2][j] = WALL;
	}

	int length2 = 30, width2 = 20;
	SquareType** wall_empty = (SquareType**)malloc(sizeof(SquareType*)*length2);
	for (int i = 0; i < length2; i++) {
		wall_empty[i] = (SquareType*)malloc(sizeof(SquareType)*width2);
		for (int j = 0; j < width2; j++) {
			if (j % 2 == 0) {
				wall_empty[i][j] = WALL;
			}
			else {
				wall_empty[i][j] = EMPTY;
			}
		}
	}

	int length3 = 40, width3 = 25;
	SquareType** chess = (SquareType**)malloc(sizeof(SquareType*)*length3);
	for (int i = 0; i < length3; i++) {
		chess[i] = (SquareType*)malloc(sizeof(SquareType)*width3);
		for (int j = 0; j < width3; j++) {
			if (i % 2 == j % 2) {
				chess[i][j] = EMPTY;
			}
			else {
				chess[i][j] = WALL;
			}
		}
	}

	Apartment rooms4_ap = apartmentCreate(rooms4, length1, width1, 500);
	Apartment wall_empty_ap = apartmentCreate(wall_empty, length2, width2, 750);
	Apartment chess_ap = apartmentCreate(chess, length3, width3, 1000);

	ApartmentService service = serviceCreate(5);
	int id1 = 1001, id2 = 563, id3 = 0;
	Apartment by_id = NULL;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceGetById(NULL, -1, &by_id));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceGetById(service, -1, &by_id));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceGetById(service, id1, &by_id));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, rooms4_ap, id1));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, wall_empty_ap, id2));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, chess_ap, id3));
	TEST_EQUALS(final, 3, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, id3, &by_id));
	TEST_EQUALS(final, true, apartmentIsIdentical(chess_ap, by_id));
	apartmentDestroy(by_id);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, id2, &by_id));
	TEST_EQUALS(final, true, apartmentIsIdentical(wall_empty_ap, by_id));
	apartmentDestroy(by_id);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, id1, &by_id));
	TEST_EQUALS(final, true, apartmentIsIdentical(rooms4_ap, by_id));
	apartmentDestroy(by_id);
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceGetById(NULL, id1, &by_id));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceGetById(service, id1, NULL));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceGetById(service, -1, NULL));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceGetById(service, -1, &by_id));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceGetById(service, 54, &by_id));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceGetById(NULL, 54, &by_id));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, id1));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceGetById(service, id1, &by_id));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, id2));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, id3));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceGetById(service, id3, &by_id));

	//Search
	Apartment found = NULL;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceSearch(NULL, 100, 10, 1000, &found));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceSearch(service, 100, 10, 1000, NULL));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceSearch(service, -1, -1, -1, NULL));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service, -1, 10, 1000, &found));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service, 100, -1, 1000, &found));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service, 100, 10, -1, &found));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceSearch(service, 100, 10, 1000, &found));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, rooms4_ap, id1));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, wall_empty_ap, id2));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, chess_ap, id3));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service, 100, 4, 1000, &found));
	TEST_EQUALS(final, true, apartmentIsIdentical(chess_ap, found));
	apartmentDestroy(found);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service, 100, 4, 750, &found));
	TEST_EQUALS(final, true, apartmentIsIdentical(wall_empty_ap, found));
	apartmentDestroy(found);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service, 266, 4, 500, &found));
	TEST_EQUALS(final, true, apartmentIsIdentical(rooms4_ap, found));
	apartmentDestroy(found);
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceSearch(service, 1000, 4, 500, &found));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service, rooms4_ap));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceSearch(service, 266, 4, 500, &found));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, rooms4_ap, id1));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service, 100, 4, 1000, &found));
	TEST_EQUALS(final, true, apartmentIsIdentical(rooms4_ap, found));
	apartmentDestroy(found);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service, chess_ap));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service, 100, 4, 1000, &found));
	TEST_EQUALS(final, true, apartmentIsIdentical(rooms4_ap, found));
	apartmentDestroy(found);
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceSearch(NULL, 266, 4, 500, &found));

	apartmentDestroy(rooms4_ap);
	apartmentDestroy(wall_empty_ap);
	apartmentDestroy(chess_ap);

	for (int i = 0; i < length1; i++) {
		free(rooms4[i]);
	}
	free(rooms4);

	for (int i = 0; i < length2; i++) {
		free(wall_empty[i]);
	}
	free(wall_empty);

	for (int i = 0; i < length3; i++) {
		free(chess[i]);
	}
	free(chess);

	serviceDestroy(service);

	return final;
}

bool medians_test() {
	bool final = true;

	int length1 = 20, width1 = 15;
	SquareType** rooms4 = (SquareType**)malloc(sizeof(SquareType*)*length1);
	for (int i = 0; i < length1; i++) {
		rooms4[i] = (SquareType*)malloc(sizeof(SquareType)*width1);
		for (int j = 0; j < width1; j++) {
			rooms4[i][j] = EMPTY;
		}
	}
	for (int j = 0; j < length1; j++) {
		rooms4[j][width1 / 2] = WALL;
	}
	for (int j = 0; j < width1; j++) {
		rooms4[length1 / 2][j] = WALL;
	}

	int length2 = 30, width2 = 20;
	SquareType** wall_empty = (SquareType**)malloc(sizeof(SquareType*)*length2);
	for (int i = 0; i < length2; i++) {
		wall_empty[i] = (SquareType*)malloc(sizeof(SquareType)*width2);
		for (int j = 0; j < width2; j++) {
			if (j % 2 == 0) {
				wall_empty[i][j] = WALL;
			}
			else {
				wall_empty[i][j] = EMPTY;
			}
		}
	}

	int length3 = 40, width3 = 25;
	SquareType** chess = (SquareType**)malloc(sizeof(SquareType*)*length3);
	for (int i = 0; i < length3; i++) {
		chess[i] = (SquareType*)malloc(sizeof(SquareType)*width3);
		for (int j = 0; j < width3; j++) {
			if (i % 2 == j % 2) {
				chess[i][j] = EMPTY;
			}
			else {
				chess[i][j] = WALL;
			}
		}
	}

	Apartment rooms4_ap = apartmentCreate(rooms4, length1, width1, 500);
	Apartment wall_empty_ap = apartmentCreate(wall_empty, length2, width2, 750);
	Apartment chess_ap = apartmentCreate(chess, length3, width3, 1000);

	ApartmentService service = serviceCreate(5);
	int id1 = 1001, id2 = 563, id3 = 0;
	int median = -1;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAreaMedian(NULL, &median));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, servicePriceMedian(NULL, &median));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAreaMedian(service, NULL));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, servicePriceMedian(service, NULL));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceAreaMedian(service, &median));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, servicePriceMedian(service, &median));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, rooms4_ap, id1));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service, &median));
	TEST_EQUALS(final, apartmentTotalArea(rooms4_ap), median);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service, &median));
	TEST_EQUALS(final, apartmentGetPrice(rooms4_ap), median);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, wall_empty_ap, id2));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service, &median));
	TEST_EQUALS(final, (apartmentTotalArea(rooms4_ap) + apartmentTotalArea(wall_empty_ap)) / 2, median);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service, &median));
	TEST_EQUALS(final, (apartmentGetPrice(rooms4_ap) + apartmentGetPrice(wall_empty_ap)) / 2, median);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, chess_ap, id3));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service, &median));
	TEST_EQUALS(final, 300, median);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service, &median));
	TEST_EQUALS(final, 750, median);
	apartmentDestroy(rooms4_ap);
	apartmentDestroy(wall_empty_ap);
	apartmentDestroy(chess_ap);

	for (int i = 0; i < length1; i++) {
		free(rooms4[i]);
	}
	free(rooms4);

	for (int i = 0; i < length2; i++) {
		free(wall_empty[i]);
	}
	free(wall_empty);

	for (int i = 0; i < length3; i++) {
		free(chess[i]);
	}
	free(chess);

	serviceDestroy(service);

	return final;
}

bool test_serviceGetById() {
	bool final = true;
	SquareType square0[] = { EMPTY, WALL, WALL };
	SquareType square1[] = { EMPTY, WALL, WALL };
	SquareType square2[] = { WALL, WALL, EMPTY };
	SquareType* squares1[3];
	squares1[0] = square0;
	squares1[1] = square1;
	squares1[2] = square2;
	Apartment apartment1 = apartmentCreate(squares1, 3, 3, 300);
	Apartment apartment2 = apartmentCreate(squares1, 3, 3, 100);
	ApartmentService service = serviceCreate(5);
	ApartmentService serivceNULL = NULL;
	Apartment getapartment = NULL;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceGetById(serivceNULL, 1, &getapartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceGetById(service, 0, &getapartment));
	serviceAddApartment(service, apartment1, 0);
	serviceAddApartment(service, apartment2, 1);
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceGetById(service, -5, &getapartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, 0, &getapartment));
	final = (apartmentIsIdentical((service->aprts)[0], getapartment)) ? final : false;
	apartmentDestroy(getapartment);
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceGetById(service, 2, &getapartment));

	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	serviceDestroy(service);
	return final;
}


int main() {
	RUN_TEST(service_test_combo);
	RUN_TEST(service_creation_copy_destruction_test);
	RUN_TEST(add_delete_test);
	RUN_TEST(search_get_test);
	RUN_TEST(medians_test);
	// TODO: add more tests:
	// RUN_TEST(...);
	return 0;
}
