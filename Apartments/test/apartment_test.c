
#include "aux_macros.h"
#include "apartment.h"
#include <assert.h>
#include <stdlib.h>

int line;

bool apartment_combo_test() {
	bool final = true;

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;

	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);

	bool result; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentIsSameRoom(NULL, 0, 1, 0, 1, &result)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	SquareType squareVal; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentGetSquare(apartment, 0, 2, &squareVal)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 1, apartmentTotalArea(apartment)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	int area; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentRoomArea(apartment, 0, 0, &area)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment, 0, 1, &area)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 1, area); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	Apartment split1 = NULL, split2 = NULL; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment, false, 1, &split1, &split2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, 1, apartmentNumOfRooms(apartment)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment, 0, 0, WALL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 2, apartmentGetWidth(apartment)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 10)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_PRICE_NOT_IN_RANGE, apartmentChangePrice(apartment, -3000)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 110, apartmentGetPrice(apartment)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	Apartment copy = apartmentCopy(apartment); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, copy)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	apartmentDestroy(apartment); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	apartmentDestroy(copy); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }


	return final;
}

bool apartment_creation_simple_modifications_and_destruction_test() {
	bool final = true;

	int length = 20, width = 15;
	SquareType** rooms4 = (SquareType**)malloc(sizeof(SquareType*)*length);
	for (int i = 0; i < length; i++) {
		rooms4[i] = (SquareType*)malloc(sizeof(SquareType)*width);
		for (int j = 0; j < width; j++) {
			rooms4[i][j] = EMPTY;
		}
	}
	for (int j = 0; j < length; j++) {
		rooms4[j][width / 2] = WALL;
	}
	for (int j = 0; j < width; j++) {
		rooms4[length / 2][j] = WALL;
	}

	TEST_EQUALS(final, NULL, apartmentCreate(rooms4, 0, width, 1000)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, NULL, apartmentCreate(rooms4, 5, -2, 1000)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, NULL, apartmentCreate(rooms4, -1, width, 1000)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, NULL, apartmentCreate(rooms4, 5, 0, 1000)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, NULL, apartmentCreate(NULL, length, width, 1000)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	Apartment apartment1 = apartmentCreate(rooms4, length, width, 1000); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_DIFFERENT(final, NULL, apartment1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, length, apartmentGetLength(apartment1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, width, apartmentGetWidth(apartment1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_DIFFERENT(final, length - 1, apartmentGetLength(apartment1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_DIFFERENT(final, width - 1, apartmentGetWidth(apartment1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }


	Apartment apartment2 = apartmentCreate(rooms4, length, width, 1000); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	Apartment apartment_copy = apartmentCopy(apartment2); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment1, apartment2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment1, apartment_copy)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment1, 10)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 1100, apartmentGetPrice(apartment1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 1000, apartmentGetPrice(apartment2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 1000, apartmentGetPrice(apartment_copy)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, false, apartmentIsIdentical(apartment1, apartment2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment1, apartment_copy)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_PRICE_NOT_IN_RANGE, apartmentChangePrice(apartment1, -101)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment1, -100)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 0, apartmentGetPrice(apartment1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	apartmentDestroy(apartment1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment2, apartment_copy)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	apartmentDestroy(apartment2); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	apartmentDestroy(apartment_copy); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	for (int i = 0; i < length; i++) {
		free(rooms4[i]);
	}
	free(rooms4);

	return final;
}

bool apartment_more_modifications_test() {
	bool final = true;

	int length = 20, width = 15;
	SquareType** rooms4 = (SquareType**)malloc(sizeof(SquareType*)*length);
	for (int i = 0; i < length; i++) {
		rooms4[i] = (SquareType*)malloc(sizeof(SquareType)*width);
		for (int j = 0; j < width; j++) {
			rooms4[i][j] = EMPTY;
		}
	}
	for (int j = 0; j < length; j++) {
		rooms4[j][width / 2] = WALL;
	}
	for (int j = 0; j < width; j++) {
		rooms4[length / 2][j] = WALL;
	}

	int length2 = 21, width2 = 15;
	SquareType** rooms4_diff_length = (SquareType**)malloc(sizeof(SquareType*)*length2);
	for (int i = 0; i < length2; i++) {
		rooms4_diff_length[i] = (SquareType*)malloc(sizeof(SquareType)*width2);
		for (int j = 0; j < width2; j++) {
			rooms4_diff_length[i][j] = EMPTY;
		}
	}
	for (int j = 0; j < length2; j++) {
		rooms4_diff_length[j][width2 / 2] = WALL;
	}
	for (int j = 0; j < width2; j++) {
		rooms4_diff_length[length2 / 2][j] = WALL;
	}

	int length3 = 20, width3 = 16;
	SquareType** rooms4_diff_width = (SquareType**)malloc(sizeof(SquareType*)*length3);
	for (int i = 0; i < length3; i++) {
		rooms4_diff_width[i] = (SquareType*)malloc(sizeof(SquareType)*width3);
		for (int j = 0; j < width3; j++) {
			rooms4_diff_width[i][j] = EMPTY;
		}
	}
	for (int j = 0; j < length3; j++) {
		rooms4_diff_width[j][width3 / 2] = WALL;
	}
	for (int j = 0; j < width3; j++) {
		rooms4_diff_width[length3 / 2][j] = WALL;
	}

	SquareType** rooms4_diff_square = (SquareType**)malloc(sizeof(SquareType*)*length);
	for (int i = 0; i < length; i++) {
		rooms4_diff_square[i] = (SquareType*)malloc(sizeof(SquareType)*width);
		for (int j = 0; j < width; j++) {
			rooms4_diff_square[i][j] = EMPTY;
		}
	}
	for (int j = 0; j < length; j++) {
		rooms4_diff_square[j][width2 / 2] = WALL;
	}
	for (int j = 0; j < width; j++) {
		rooms4_diff_square[length / 2][j] = WALL;
	}
	rooms4_diff_square[length / 2][width / 2] = EMPTY;


	Apartment apartment = apartmentCreate(rooms4, length, width, 1000);
	Apartment apartment_dif_length = apartmentCreate(rooms4_diff_length, length2, width2, 1000);
	Apartment apartment_dif_width = apartmentCreate(rooms4_diff_width, length3, width3, 1000);
	Apartment apartment_dif_square = apartmentCreate(rooms4_diff_square, length, width, 1000);
	Apartment apartment_dif_price = apartmentCreate(rooms4, length, width, 999);
	Apartment copy = apartmentCopy(apartment);

	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, apartment_dif_length)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, apartment_dif_width)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, apartment_dif_price)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, apartment_dif_square)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, false, apartmentIsIdentical(NULL, apartment)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, NULL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, apartmentIsIdentical(NULL, NULL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentChangePrice(NULL, 50)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentChangePrice(NULL, -100)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	int mid_row = length / 2, mid_col = width / 2; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	int i = 2, j = 14; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	SquareType res; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentGetSquare(NULL, i, j, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentGetSquare(NULL, -3, j, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentGetSquare(NULL, 9, -5, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentGetSquare(apartment, -1, j, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentGetSquare(apartment, 20, j, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentGetSquare(apartment, 9, -1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentGetSquare(apartment, 9, 15, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment, i, j, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, res, EMPTY); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment, i, mid_col, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, res, WALL); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment, mid_row, j, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, res, WALL); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	//TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment, mid_row, mid_col, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	//TEST_EQUALS(final, res, WALL); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSetSquare(NULL, -1, -2, EMPTY)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSetSquare(NULL, mid_row, mid_col, WALL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSetSquare(NULL, mid_row, mid_col, EMPTY)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSetSquare(NULL, mid_row, mid_col, EMPTY)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSetSquare(apartment, length, width, WALL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment, mid_row, mid_col, WALL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, mid_row, mid_col, EMPTY)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment, mid_row, mid_col, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, res, EMPTY); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, length*width - (width + length - 2), apartmentTotalArea(apartment)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, copy)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	apartmentDestroy(apartment); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	apartmentDestroy(apartment_dif_length); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	apartmentDestroy(apartment_dif_width);
	apartmentDestroy(apartment_dif_price);
	apartmentDestroy(apartment_dif_square);
	apartmentDestroy(copy);

	for (int i = 0; i < length; i++) {
		free(rooms4[i]);
	}
	free(rooms4);

	for (int i = 0; i < length2; i++) {
		free(rooms4_diff_length[i]);
	}
	free(rooms4_diff_length);

	for (int i = 0; i < length3; i++) {
		free(rooms4_diff_width[i]);
	}
	free(rooms4_diff_width);

	for (int i = 0; i < length; i++) {
		free(rooms4_diff_square[i]);
	}
	free(rooms4_diff_square);


	return final;
}

bool apartment_complex_funcs_test() {
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
	{
		Apartment rooms4_ap = apartmentCreate(rooms4, length1, width1, 1000); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, length1*width1 - (width1 + length1 - 1), apartmentTotalArea(rooms4_ap)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, 4, apartmentNumOfRooms(rooms4_ap)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		bool res; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(rooms4_ap, 0, 0, length1 - 1, width1 - 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, false, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(rooms4_ap, 0, 0, length1 / 2 + 1, width1 / 2 - 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, false, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(rooms4_ap, 0, 0, length1 / 2 - 1, width1 / 2 + 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(rooms4_ap, 0, 0, length1 / 2 - 1, width1 / 2 - 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, true, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(rooms4_ap, length1 / 2 + 1, width1 / 2 - 1,
				length1 / 2 - 1, width1 / 2 + 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, false, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(rooms4_ap, length1 / 2 + 1, width1 / 2 - 1,
				length1 - 1, width1 - 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, false, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

		int area1;
		int area2;
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(rooms4_ap, 0, 0, &area1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, (length1 / 2)*(width1 / 2), area1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(rooms4_ap, length1 - 1, width1 - 1, &area1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, (length1 / 2 - 1)*(width1 / 2), area1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		apartmentRoomArea(rooms4_ap, length1 / 2 - 1, width1 / 2 - 1, &area1),
			apartmentRoomArea(rooms4_ap, length1 / 2 - 1, width1 / 2 + 1, &area2); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, area1, area2); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		apartmentDestroy(rooms4_ap); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	}
	Apartment wall_empty_ap = apartmentCreate(wall_empty, length2, width2, 500); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, length2 * (width2 / 2), apartmentTotalArea(wall_empty_ap)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, width2 / 2, apartmentNumOfRooms(wall_empty_ap));

	for (int i = 0; i < width2; i++) {
		for (int j = 0; j < length2 - 1; j++) {
			bool res;
			int room_area;
			if (i % 2 == 0) {
				TEST_EQUALS(final, APARTMENT_NO_ROOM,
					apartmentIsSameRoom(wall_empty_ap, j, i, j + 1, i, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NO_ROOM,
					apartmentRoomArea(wall_empty_ap, j, i, &room_area)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NULL_ARG,
					apartmentIsSameRoom(NULL, j, i, j + 1, i, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NULL_ARG,
					apartmentIsSameRoom(wall_empty_ap, j, i, j + 1, i, NULL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

			}
			else {
				TEST_EQUALS(final, APARTMENT_SUCCESS,
					apartmentIsSameRoom(wall_empty_ap, j, i, j + 1, i, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_SUCCESS,
					apartmentRoomArea(wall_empty_ap, j, i, &room_area)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, true, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, length2, room_area); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NULL_ARG,
					apartmentIsSameRoom(NULL, j, i, j + 1, i, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NULL_ARG,
					apartmentIsSameRoom(wall_empty_ap, j, i, j + 1, i, NULL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

			}
		}
	}

	Apartment chess_ap = apartmentCreate(chess, length3, width3, 500); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, (length3 * width3) / 2, apartmentTotalArea(chess_ap)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, (length3 * width3) / 2, apartmentNumOfRooms(chess_ap)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	for (int i = 0; i < length3; i++) {
		for (int j = 0; j < width3; j++) {
			bool res;
			int room_area;
			if (i == length3 - 1 || j == width3 - 1) {
				TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
					apartmentIsSameRoom(chess_ap, i, j, i + 1, j + 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
			}
			else if (i % 2 == j % 2) {
				TEST_EQUALS(final, APARTMENT_SUCCESS,
					apartmentIsSameRoom(chess_ap, i, j, i + 1, j + 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_SUCCESS,
					apartmentRoomArea(chess_ap, i, j, &room_area)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, false, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, 1, room_area); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NULL_ARG,
					apartmentRoomArea(NULL, i, j, &room_area)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NULL_ARG,
					apartmentRoomArea(chess_ap, i, j, NULL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
			}
			else if (i % 2 != j % 2 && i < length3 - 1 && j < width3 - 1) {
				TEST_EQUALS(final, APARTMENT_NO_ROOM,
					apartmentIsSameRoom(chess_ap, i, j, i + 1, j + 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NO_ROOM,
					apartmentRoomArea(chess_ap, i, j, &room_area)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NULL_ARG,
					apartmentRoomArea(NULL, i, j, &room_area)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
				TEST_EQUALS(final, APARTMENT_NULL_ARG,
					apartmentRoomArea(chess_ap, i, j, NULL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
			}
		}
	}
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

	return final;
}

bool apartment_more_complex_funcs_test() {
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


	Apartment rooms4_ap = apartmentCreate(rooms4, length1, width1, 1000);

	Apartment rooms2splited4_1, rooms2splited4_2;
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(rooms4_ap, length1 / 2, width1 / 2, EMPTY)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(rooms4_ap, true, length1 / 2, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(rooms4_ap, false, width1 / 2, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(rooms4_ap, false, length1 / 2, NULL, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(rooms4_ap, false, length1 / 2, &rooms2splited4_1, NULL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(rooms4_ap, false, length1 / 2, &rooms2splited4_1, NULL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(NULL, false, length1 / 2, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(NULL, true, length1, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(NULL, false, width1, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSplit(rooms4_ap, true, length1, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSplit(rooms4_ap, false, width1, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(rooms4_ap, length1 / 2, width1 / 2, WALL)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSplit(rooms4_ap, true, length1 / 2, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, apartmentIsIdentical(rooms2splited4_1, rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, length1 / 2, apartmentGetLength(rooms2splited4_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, length1 / 2 - !(length1 % 2), apartmentGetLength(rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, width1, apartmentGetWidth(rooms2splited4_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, width1, apartmentGetWidth(rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 550, apartmentGetPrice(rooms2splited4_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 500, apartmentGetPrice(rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 2, apartmentNumOfRooms(rooms2splited4_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 2, apartmentNumOfRooms(rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	int len1 = apartmentGetLength(rooms2splited4_1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	for (int i = 0; i < len1; i++) {
		SquareType wall_check1;
		SquareType wall_check2;
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentGetSquare(rooms2splited4_1, i, width1 / 2, &wall_check1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentGetSquare(rooms2splited4_1, i <
				apartmentGetLength(rooms2splited4_1) - 1 ? i : i - 1,
				width1 / 2, &wall_check2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, WALL, wall_check1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
		TEST_EQUALS(final, WALL, wall_check2); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	}
	int area1, area2; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(rooms2splited4_1, 0, 0, &area1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(rooms2splited4_2, 0, 0, &area2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 70, area1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 63, area2); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS,
		apartmentRoomArea(rooms2splited4_1, 0, width1 / 2 + 1, &area2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS,
		apartmentRoomArea(rooms2splited4_2, 0, width1 / 2 + 1, &area1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 70, area2); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 63, area1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 140, apartmentTotalArea(rooms2splited4_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 126, apartmentTotalArea(rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	bool res; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(rooms2splited4_1, 0, 0, 0, width1 / 2 + 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(rooms2splited4_1, 0, 0, 0, width1 / 2 - 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(rooms2splited4_1, 0, 0, 0, width1 / 2, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(rooms2splited4_2, 0, 0, 0, width1 / 2 + 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, false, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(rooms2splited4_2, 0, 0, 0, width1 / 2 - 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, res); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(rooms2splited4_2, 0, 0, 0, width1 / 2, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	// Same tests Split By Collume
	Apartment rooms1splited2_1, rooms1splited2_2; if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSplit(rooms2splited4_1, false, width1 / 2, &rooms1splited2_1, &rooms1splited2_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, apartmentIsIdentical(rooms1splited2_1, rooms1splited2_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, length1 / 2, apartmentGetLength(rooms1splited2_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, width1 / 2, apartmentGetWidth(rooms1splited2_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 293, apartmentGetPrice(rooms1splited2_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 1, apartmentNumOfRooms(rooms1splited2_1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(rooms1splited2_1, 0, 0, &area1)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 70, area1); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, 70, apartmentTotalArea(rooms1splited2_1));

	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(rooms1splited2_1, 0, 0, 0, width1 / 2 + 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(rooms1splited2_1, 0, 0, 0, width1 / 2 - 1, &res)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, true, res);

	int length2 = 30, width2 = 20;
	SquareType** wall_empty_col = (SquareType**)malloc(sizeof(SquareType*)*length2);
	for (int i = 0; i < length2; i++) {
		wall_empty_col[i] = (SquareType*)malloc(sizeof(SquareType)*width2);
		for (int j = 0; j < width2; j++) {
			if (j % 2 == 0) {
				wall_empty_col[i][j] = WALL;
			}
			else {
				wall_empty_col[i][j] = EMPTY;
			}
		}
	}

	Apartment wall_empty_col_ap = apartmentCreate(wall_empty_col, length2, width2, 750);


	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(wall_empty_col_ap, false, 0, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(wall_empty_col_ap, false, width2 - 1, &rooms2splited4_1, &rooms2splited4_2));

	int length3 = 30, width3 = 20;
	SquareType** wall_empty_row = (SquareType**)malloc(sizeof(SquareType*)*length3);
	for (int i = 0; i < length3; i++) {
		wall_empty_row[i] = (SquareType*)malloc(sizeof(SquareType)*width3);
		for (int j = 0; j < width3; j++) {
			if (i % 2 == 0) {
				wall_empty_row[i][j] = WALL;
			}
			else {
				wall_empty_row[i][j] = EMPTY;
			}
		}
	}

	Apartment wall_empty_row_ap = apartmentCreate(wall_empty_row, length3, width3, 750);


	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(wall_empty_row_ap, true, 0, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(wall_empty_row_ap, true, length3 - 1, &rooms2splited4_1, &rooms2splited4_2)); if (!final) { int a = __LINE__; printf("error in &line: %d", a); scanf("%d", &line); exit(0); }

	apartmentDestroy(rooms1splited2_1);
	apartmentDestroy(rooms1splited2_2);
	apartmentDestroy(rooms2splited4_1);
	apartmentDestroy(rooms2splited4_2);
	apartmentDestroy(rooms4_ap);
	apartmentDestroy(wall_empty_col_ap);
	apartmentDestroy(wall_empty_row_ap);

	for (int i = 0; i < length1; i++) {
		free(rooms4[i]);
	}
	free(rooms4);

	for (int i = 0; i < length2; i++) {
		free(wall_empty_col[i]);
	}
	free(wall_empty_col);

	for (int i = 0; i < length3; i++) {
		free(wall_empty_row[i]);
	}
	free(wall_empty_row);

	return final;
}



int main() {
	RUN_TEST(apartment_combo_test);
	RUN_TEST(apartment_creation_simple_modifications_and_destruction_test);
	RUN_TEST(apartment_more_modifications_test);
	RUN_TEST(apartment_complex_funcs_test)
	RUN_TEST(apartment_more_complex_funcs_test)
	return true;
}
