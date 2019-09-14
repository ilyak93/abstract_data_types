#include "test.h"
#include "test_common.h"
#include "aux_macros.h"

bool service_test_createAndAdd() {
	bool final = true;
	TEST_EQUALS(final, serviceCreate(0), NULL);
	TEST_EQUALS(final, serviceCreate(-1), NULL);
	TEST_EQUALS(final, serviceCreate(-2), NULL);
	ApartmentService service = serviceCreate(3);
	
	TEST_EQUALS(final, serviceNumberOfApatments(service), 0);
	SquareType square = EMPTY;
	SquareType* squarePtr = &square;
	Apartment singleApartment = apartmentCreate(&squarePtr, 1, 1, 400);

	SquareType** room2 = allocate_two_room(5, 3);

	Apartment apartment = apartmentCreate(room2, 5, 3, 190);

	int id = 1000;

	
	TEST_EQUALS(final, serviceAddApartment(NULL, singleApartment, id++), APARTMENT_SERVICE_NULL_ARG);
	TEST_EQUALS(final, serviceAddApartment(service, NULL, id++), APARTMENT_SERVICE_NULL_ARG);
	TEST_EQUALS(final, serviceAddApartment(NULL, apartment, id++), APARTMENT_SERVICE_NULL_ARG);
	TEST_EQUALS(final, serviceAddApartment(service, singleApartment, id++), APARTMENT_SERVICE_SUCCESS);
	TEST_EQUALS(final, serviceAddApartment(service, apartment, id++), APARTMENT_SERVICE_SUCCESS);
	TEST_EQUALS(final, serviceNumberOfApatments(service), 2);
	TEST_EQUALS(final, serviceAddApartment(service, singleApartment, id++), APARTMENT_SERVICE_SUCCESS);
	TEST_EQUALS(final, serviceAddApartment(service, singleApartment, id++), APARTMENT_SERVICE_FULL);
	TEST_EQUALS(final, serviceAddApartment(service, apartment, id++), APARTMENT_SERVICE_FULL);
	TEST_EQUALS(final, serviceNumberOfApatments(service), 3);

	ApartmentService service2 = serviceCreate(1);
	TEST_EQUALS(final, serviceAddApartment(service2, apartment, id++), APARTMENT_SERVICE_SUCCESS);
	TEST_EQUALS(final, serviceAddApartment(service2, singleApartment, id++), APARTMENT_SERVICE_FULL);

	delete_apartment(room2, 5);
	apartmentDestroy(apartment);
	serviceDestroy(NULL);
	serviceDestroy(service);
	serviceDestroy(service2);
	apartmentDestroy(singleApartment);
	
	return final;
}

bool service_test_median() {
	bool final = true;

	int resultVar;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAreaMedian(NULL, &resultVar));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, servicePriceMedian(NULL, &resultVar));

	ApartmentService service1 = serviceCreate(10);
	ApartmentService service2 = serviceCreate(10);

	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceAreaMedian(service1, NULL));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, servicePriceMedian(service2, NULL));

	TEST_EQUALS(final, serviceAreaMedian(service1, &resultVar), APARTMENT_SERVICE_EMPTY);
	TEST_EQUALS(final, servicePriceMedian(service2, &resultVar), APARTMENT_SERVICE_EMPTY);

	SquareType** room2 = allocate_one_room(10, 10);

	Apartment apartment101 = apartmentCreate(room2, 2, 2, 101);
	Apartment apartment102 = apartmentCreate(room2, 2, 2, 102);
	Apartment apartment104 = apartmentCreate(room2, 2, 2, 104);
	Apartment apartment105 = apartmentCreate(room2, 2, 2, 105);

	Apartment apartment81 = apartmentCreate(room2, 9, 9, 100);
	Apartment apartment64 = apartmentCreate(room2, 8, 8, 100);
	Apartment apartment49 = apartmentCreate(room2, 7, 7, 100);
	Apartment apartment36 = apartmentCreate(room2, 6, 6, 100);
	Apartment apartment25 = apartmentCreate(room2, 5, 5, 100);

	int id = 1000;

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment101, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment102, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service1, &resultVar));
	TEST_EQUALS(final, 4, resultVar);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service1, &resultVar));
	TEST_EQUALS(final, 101, resultVar);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment104, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment104, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service1, &resultVar));
	TEST_EQUALS(final, 103, resultVar);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service1, &resultVar));
	TEST_EQUALS(final, 4, resultVar);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment105, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment105, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment105, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment105, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service1, &resultVar));
	TEST_EQUALS(final, 104, resultVar);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment105, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service1, &resultVar));
	TEST_EQUALS(final, 105, resultVar);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service1, &resultVar));
	TEST_EQUALS(final, 4, resultVar);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service2, apartment81, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service2, &resultVar));
	TEST_EQUALS(final, 81, resultVar);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service2, apartment49, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service2, &resultVar));
	TEST_EQUALS(final, 65, resultVar);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service2, apartment25, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service2, &resultVar));
	TEST_EQUALS(final, 49, resultVar);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service2, apartment64, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service2, &resultVar));
	TEST_EQUALS(final, 56, resultVar);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service2, apartment36, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAreaMedian(service2, &resultVar));
	TEST_EQUALS(final, 49, resultVar);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service2, &resultVar));
	TEST_EQUALS(final, 100, resultVar);

	delete_apartment(room2, 10);

	apartmentDestroy(apartment101);
	apartmentDestroy(apartment102);
	apartmentDestroy(apartment104);
	apartmentDestroy(apartment105);

	apartmentDestroy(apartment25);
	apartmentDestroy(apartment36);
	apartmentDestroy(apartment49);
	apartmentDestroy(apartment64);
	apartmentDestroy(apartment81);
	
	serviceDestroy(service1);
	serviceDestroy(service2);

	return final;
}

bool service_test_delete() {
	bool final = true;

	ApartmentService service1 = serviceCreate(100);
	ApartmentService service2 = serviceCreate(2);

	SquareType square = WALL;
	SquareType* squarePtr = &square;
	Apartment singleApartment = apartmentCreate(&squarePtr, 1, 1, 400);

	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceDeleteApartment(NULL, singleApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceDeleteApartment(service1, NULL));

	SquareType** room2 = allocate_one_room(10, 10);

	Apartment apartment101 = apartmentCreate(room2, 2, 2, 101);
	Apartment apartment101copy = apartmentCopy(apartment101);
	Apartment apartment102 = apartmentCreate(room2, 2, 2, 102);
	Apartment apartment103 = apartmentCreate(room2, 2, 2, 103);

	Apartment apartment64 = apartmentCreate(room2, 8, 8, 101);
	Apartment apartment81 = apartmentCreate(room2, 9, 9, 102);

	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceDeleteApartment(service1, apartment103));

	int id = 1000;

	for (int i = 0; i < 3; i++) {
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment102, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment101, id++));
	}
	for (int i = 0; i < 2; i++) {
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment64, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment81, id++));
	}

	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteApartment(service1, apartment103));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service1, apartment101));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service1, apartment101copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment101copy, 0, 0, WALL));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteApartment(service1, apartment101copy));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service1, apartment101));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteApartment(service1, apartment101));
	TEST_EQUALS(final, 7, serviceNumberOfApatments(service1));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service2, apartment64, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service2, apartment102, id++));
	TEST_EQUALS(final, 2, serviceNumberOfApatments(service2));
	TEST_EQUALS(final, APARTMENT_SERVICE_FULL, serviceAddApartment(service2, apartment64, id++));
	TEST_EQUALS(final, 2, serviceNumberOfApatments(service2));

	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteApartment(service2, apartment103));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service2, apartment102));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service2, apartment64));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceDeleteApartment(service2, apartment64));
	TEST_EQUALS(final, 0, serviceNumberOfApatments(service2));

	delete_apartment(room2, 10);

	apartmentDestroy(singleApartment);
	apartmentDestroy(apartment101);
	apartmentDestroy(apartment102);
	apartmentDestroy(apartment103);
	apartmentDestroy(apartment64);
	apartmentDestroy(apartment81);
	apartmentDestroy(apartment101copy);

	serviceDestroy(service2);
	serviceDestroy(service1);

	return final;
}

bool service_test_copy() {
	bool final = true;

	TEST_EQUALS(final, NULL, serviceCopy(NULL));

	ApartmentService service1 = serviceCreate(100);
	ApartmentService service2 = serviceCopy(service1);

	TEST_DIFFERENT(final, NULL, service1);
	TEST_DIFFERENT(final, NULL, service2);

	SquareType** room2 = allocate_one_room(10, 10);

	Apartment apartment101 = apartmentCreate(room2, 2, 2, 101);
	Apartment apartment22 = apartmentCreate(room2, 2, 2, 22);
	Apartment apartment103 = apartmentCreate(room2, 2, 2, 103);

	Apartment apartment64 = apartmentCreate(room2, 8, 8, 264);
	Apartment apartment81 = apartmentCreate(room2, 9, 9, 381);

	int id = 1000;

	for (int i = 0; i < 3; i++) {
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment22, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment101, id++));
	}
	for (int i = 0; i < 2; i++) {
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment64, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment81, id++));
	}

	ApartmentService service3 = serviceCopy(service1);
	TEST_EQUALS(final, serviceNumberOfApatments(service2), 0);
	TEST_EQUALS(final, serviceNumberOfApatments(service3), 10);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service3, apartment101));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service3, apartment64));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service3, apartment101));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service3, apartment64));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service3, apartment101));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteApartment(service3, apartment64));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteApartment(service3, apartment101));

	int median;
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service3, &median));
	TEST_EQUALS(final, 22, median);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service1, &median));
	TEST_EQUALS(final, 101, median);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service1, apartment101));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service1, apartment101));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service1, &median));
	TEST_EQUALS(final, 182, median);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service3, &median));
	TEST_EQUALS(final, 22, median);
	TEST_EQUALS(final, serviceNumberOfApatments(service3), 5);
	TEST_EQUALS(final, serviceNumberOfApatments(service1), 8);

	delete_apartment(room2, 10);

	apartmentDestroy(apartment101);
	apartmentDestroy(apartment22);
	apartmentDestroy(apartment103);
	apartmentDestroy(apartment64);
	apartmentDestroy(apartment81);

	serviceDestroy(service2);
	serviceDestroy(service1);
	serviceDestroy(service3);

	return final;
}

bool service_test_search() {
	bool final = true;

	ApartmentService service1 = serviceCreate(100);
	Apartment resApartment = NULL;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceSearch(NULL, 0, 0, 0, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceSearch(service1, 0, 0, 0, NULL));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service1, -1, 0, 0, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service1, -2, 0, 0, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service1, 0, -1, 0, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service1, 0, -2, 0, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service1, 0, 0, -1, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceSearch(service1, 0, 0, -2, &resApartment));

	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceSearch(service1, 0, 0, 0, &resApartment));

	SquareType** room2 = allocate_two_room(10, 10);

	Apartment apartment101 = apartmentCreate(room2, 3, 3, 101);
	Apartment apartment102 = apartmentCreate(room2, 3, 3, 102);
	Apartment apartment103 = apartmentCreate(room2, 3, 3, 103);
	room2[1][0] = room2[1][2] = room2[0][1] = room2[2][1] = WALL;
	Apartment apartment104 = apartmentCreate(room2, 3, 3, 104);
	Apartment apartment105 = apartmentCreate(room2, 3, 3, 105);
	room2[1][0] = room2[1][2] = room2[0][1] = room2[2][1] = EMPTY;

	Apartment apartment81 = apartmentCreate(room2, 9, 9, 810);
	Apartment apartment64 = apartmentCreate(room2, 8, 8, 640);
	Apartment apartment49 = apartmentCreate(room2, 7, 7, 490);
	Apartment apartment36 = apartmentCreate(room2, 6, 6, 360);
	Apartment apartment25 = apartmentCreate(room2, 5, 5, 250);

	int id = 1000;

	for (int i = 0; i < 3; i++) {
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment105, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment104, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment103, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment102, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment101, id++));
	}
	for (int i = 0; i < 2; i++) {
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment25, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment36, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment49, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment64, id++));
		TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service1, apartment81, id++));
	}

	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceSearch(service1, 73, 0, 1000, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceSearch(service1, 0, 6, 1000, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceSearch(service1, 0, 0, 100, &resApartment));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service1, 72, 2, 810, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(resApartment, apartment81));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment81,0,0,WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(resApartment, apartment81));
	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service1, 0, 0, 200, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(resApartment, apartment101));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment101, 0, 0, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(resApartment, apartment101));
	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment101, 0, 0, EMPTY));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteApartment(service1, apartment101));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service1, 0, 0, 200, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(resApartment, apartment102));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment102, 0, 0, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(resApartment, apartment102));
	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service1, 0, 2, 500, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(resApartment, apartment49));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment49, 1, 0, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(resApartment, apartment49));
	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service1, 50, 1, 700, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(resApartment, apartment64));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment64, 1, 0, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(resApartment, apartment64));
	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceSearch(service1, 0, 5, 1000, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(resApartment, apartment104));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment104, 1, 0, EMPTY));
	TEST_EQUALS(final, false, apartmentIsIdentical(resApartment, apartment104));
	apartmentDestroy(resApartment);

	delete_apartment(room2, 10);

	apartmentDestroy(apartment101);
	apartmentDestroy(apartment102);
	apartmentDestroy(apartment103);
	apartmentDestroy(apartment104);
	apartmentDestroy(apartment105);

	apartmentDestroy(apartment25);
	apartmentDestroy(apartment36);
	apartmentDestroy(apartment49);
	apartmentDestroy(apartment64);
	apartmentDestroy(apartment81);
	
	
	serviceDestroy(service1);

	return final;
}

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
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, apartment, id++));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, servicePriceMedian(service, &intResult));
	TEST_EQUALS(final, 100, intResult);
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceSearch(service, 50,50,50, &apartmentRes));
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));

	apartmentDestroy(apartment);
	serviceDestroy(service);
	serviceDestroy(service2);

	return final;
}

bool service_test_id() {
	bool final = true;

	Apartment resApartment = NULL;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceDeleteById(NULL, 1));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceGetById(NULL, 1, &resApartment));

	ApartmentService service = serviceCreate(100);
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG, serviceGetById(service, 1, NULL));
	
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceGetById(service, -1, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceDeleteById(service, -1));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceGetById(service, -2, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceDeleteById(service, -2));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceGetById(service, -10, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS, serviceDeleteById(service, -10));

	SquareType sqaure = EMPTY;
	SquareType* sqaurePtr = &sqaure;
	Apartment apartment = apartmentCreate(&sqaurePtr, 1, 1, 100);

	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceGetById(service, 0, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceDeleteById(service, 0));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceGetById(service, 1, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceDeleteById(service, 1));
	
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, apartment, 0));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteById(service, 1));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceGetById(service, 1, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_ALREADY_EXISTS, serviceAddApartment(service, apartment, 0));
	
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, 0, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, resApartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment,0,0,WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, resApartment));

	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, 0));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceGetById(service, 0, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY, serviceDeleteById(service, 0));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, apartment, 0));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, apartment, 1));
	TEST_EQUALS(final, APARTMENT_SERVICE_ALREADY_EXISTS, serviceAddApartment(service, apartment, 0));
	TEST_EQUALS(final, APARTMENT_SERVICE_ALREADY_EXISTS, serviceAddApartment(service, apartment, 1));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, 0, 0, EMPTY));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceAddApartment(service, apartment, 2));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, 0, &resApartment));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, resApartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, 0, 0, WALL));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, resApartment));

	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, 2, &resApartment));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, resApartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, 0, 0, EMPTY));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, resApartment));

	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, 1, &resApartment));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, resApartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, 0, 0, WALL));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, resApartment));

	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_ALREADY_EXISTS, serviceAddApartment(service, apartment, 2));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, 1));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceGetById(service, 1, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteById(service, 1));

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, 0, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, resApartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, 0, 0, EMPTY));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, resApartment));

	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceGetById(service, 2, &resApartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, resApartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, 0, 0, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, resApartment));

	apartmentDestroy(resApartment);

	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, 0));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceGetById(service, 0, &resApartment));
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT, serviceDeleteById(service, 0));


	apartmentDestroy(apartment);
	serviceDestroy(service);

	return final;
}

bool apartment_service_test_main(int testNum) {
	switch (testNum) {
	case 0:
		RUN_TEST(service_test_createAndAdd);
		RUN_TEST(service_test_median);
		RUN_TEST(service_test_delete);
		RUN_TEST(service_test_copy);
		RUN_TEST(service_test_search);
		RUN_TEST(service_test_id);
		RUN_TEST(service_test_combo);
		break;
	case 14:
		RUN_TEST(service_test_createAndAdd);
		break;
	case 15:
		RUN_TEST(service_test_median);
		break;
	case 16:
		RUN_TEST(service_test_delete);
		break;
	case 17:
		RUN_TEST(service_test_copy);
		break;
	case 18:
		RUN_TEST(service_test_search);
		break;
	case 19:
		RUN_TEST(service_test_id);
		break;
	case 20:
		RUN_TEST(service_test_combo);
	default:
		return false;
	}

	return true;
}
