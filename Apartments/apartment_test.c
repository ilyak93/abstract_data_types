#include "test.h"
#include "test_common.h"
#include "aux_macros.h"

#include <stdlib.h>

/*
static char squareTypeToChar(SquareType square) {
	switch (square)
	{
	case WALL:
		return 'W';
	case EMPTY:
		return 'E';
	}

	return '-';
}

static void print_floor(SquareType** apartment, int length, int width) {
	printf("-------------------\n");
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			printf("%c ", squareTypeToChar(apartment[i][j]));
		}
		printf("\n");
	}
	printf("-------------------\n");
}
*/
bool apartment_test_create() {
	bool final = true;
	SquareType** room = allocate_one_room(4,3);
	TEST_EQUALS(final, NULL, apartmentCreate(room, 4, 0, 100));
	TEST_EQUALS(final, NULL, apartmentCreate(room, 4, -1, 100));
	TEST_EQUALS(final, NULL, apartmentCreate(room, 4, -2, 100));
	TEST_EQUALS(final, NULL, apartmentCreate(room, 0, 3, 100));
	TEST_EQUALS(final, NULL, apartmentCreate(room, -1, 3, 100));
	TEST_EQUALS(final, NULL, apartmentCreate(room, -2, 3, 100));
	TEST_EQUALS(final, NULL, apartmentCreate(room, 4, 3, -1));
	TEST_EQUALS(final, NULL, apartmentCreate(room, 4, 3, -2));
	Apartment apartment = apartmentCreate(room, 4, 3, 100);
	Apartment apartment2 = apartmentCreate(room, 3, 3, 200);
	delete_apartment(room, 4);
	TEST_DIFFERENT(final, NULL, apartment);
	TEST_DIFFERENT(final, NULL, apartment2);

	apartmentDestroy(apartment);
	apartmentDestroy(apartment2);

	return final;
}

bool apartment_test_same_room() {
	bool final = true;

	SquareType** room = allocate_one_room(3,5);
	SquareType** room2 = allocate_two_room(3, 5);
	bool result = false;
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentIsSameRoom(NULL,0,0,0,0, &result));
	Apartment apartment = apartmentCreate(room, 2, 5, 100);
	Apartment apartment2 = apartmentCreate(room2, 3, 5, 100);
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentIsSameRoom(apartment, 0, 0, 0, 0, NULL));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, -1, 0, 0, 0, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 0, -1, 0, 0, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 0, 0, -1, 0, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 0, 0, 0, -1, &result));
	
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 0, 5, 0, 0, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 2, 0, 0, 0, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 3, 0, 0, 0, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 0, 0, 0, 5, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 0, 0, 2, 0, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentIsSameRoom(apartment, 0, 0, 3, 0, &result));

	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(apartment2, 0, 0, 0, 2, &result));
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(apartment2, 1, 1, 1, 2, &result));
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(apartment2, 2, 3, 2, 2, &result));
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(apartment2, 0, 2, 1, 4, &result));
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(apartment2, 1, 2, 0, 3, &result));
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(apartment2, 2, 2, 1, 1, &result));
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentIsSameRoom(apartment2, 1, 2, 1, 2, &result));

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(apartment, 0, 0, 0, 3, &result));
	TEST_EQUALS(final, true, result);

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(apartment2, 0, 0, 0, 3, &result));
	TEST_EQUALS(final, false, result);

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(apartment2, 0, 0, 2, 3, &result));
	TEST_EQUALS(final, false, result);

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(apartment2, 2, 0, 1, 0, &result));
	TEST_EQUALS(final, true, result);

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(apartment2, 0, 0, 0, 0, &result));
	TEST_EQUALS(final, true, result);

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(apartment2, 1, 1, 1, 1, &result));
	TEST_EQUALS(final, true, result);

	for (int i1 = 0; i1 < 3; i1++) {
		for (int j1 = 0; j1 < 5; j1++) {
			for (int i2 = 0; i2 < 3; i2++) {
				for (int j2 = 0; j2 < 5; j2++) {
					if (i1 < 2 && i2 < 2) {
						TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(apartment, i1, j1, i2, j2, &result));
						TEST_EQUALS(final, true, result);

						if (j1 == 2 || j2 == 2) {
							continue;
						}
						bool expected = ((j1 < 2 && j2 < 2) || (j1 > 2 && j2 > 2));
						TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentIsSameRoom(apartment2, i1, j1, i2, j2, &result));
						TEST_EQUALS(final, expected, result);
					}
				}
			}
		}
	}

	delete_apartment(room, 3);
	delete_apartment(room2, 3);

	apartmentDestroy(apartment);
	apartmentDestroy(apartment2);

	return final;
}

bool apartment_test_total_area() {
	bool final = true;
	
	SquareType** square = allocate_one_room(1, 1);
	SquareType** room = allocate_one_room(3, 5);
	SquareType** room2 = allocate_two_room(3, 5);

	Apartment apartmentSquare = apartmentCreate(square, 1, 1, 100);

	square[0][0] = WALL;
	Apartment apartmentWall = apartmentCreate(square, 1, 1, 100);
	Apartment apartment = apartmentCreate(room, 3, 5, 100);
	Apartment apartment2 = apartmentCreate(room2, 3, 5, 100);

	delete_apartment(square, 1);
	delete_apartment(room, 3);
	delete_apartment(room2, 3);
	
	TEST_EQUALS(final, 0, apartmentTotalArea(apartmentWall));
	TEST_EQUALS(final, 1, apartmentTotalArea(apartmentSquare));
	TEST_EQUALS(final, 15, apartmentTotalArea(apartment));
	TEST_EQUALS(final, 12, apartmentTotalArea(apartment2));

	apartmentDestroy(apartmentWall);
	apartmentDestroy(apartmentSquare);
	apartmentDestroy(apartment);
	apartmentDestroy(apartment2);

	return final;
}

bool apartment_test_room_area() {
	bool final = true;
	
	SquareType** square = allocate_one_room(1, 1);
	SquareType** room = allocate_one_room(5, 5);
	SquareType** room2 = allocate_two_room(5, 5);

	Apartment apartmentSquare = apartmentCreate(room, 1, 1, 100);
	square[0][0] = WALL;
	Apartment apartmentWall = apartmentCreate(square, 1, 1, 100);
	Apartment apartment = apartmentCreate(room, 5, 5, 100);
	Apartment apartment2 = apartmentCreate(room2, 5, 5, 100);

	delete_apartment(square, 1);
	delete_apartment(room, 5);
	delete_apartment(room2, 5);

	int result = -1;
	
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentRoomArea(NULL, 0, 0, &result));
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentRoomArea(apartment, 0, 0, NULL));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentRoomArea(apartmentSquare, 0, 1, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentRoomArea(apartmentSquare, 1, 0, &result));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentRoomArea(apartmentSquare, 1, 1, &result));
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentRoomArea(apartmentWall, 0, 0, &result));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartmentSquare, 0, 0, &result));
	TEST_EQUALS(final, result, 1);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment, i, j, &result));
			TEST_EQUALS(final, result, 25);

			if (j == 2) {
				TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentRoomArea(apartment2, i, j, &result));
			} else {
				TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment2, i, j, &result));
				TEST_EQUALS(final, result, 10);
			}
		}
	}

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 2, 1, WALL));
	
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentRoomArea(apartment2, 2, 1, &result));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment2, 1, 1, &result));
	TEST_EQUALS(final, result, 9);
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment2, 2, 3, &result));
	TEST_EQUALS(final, result, 10);

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 2, 0, WALL));
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentRoomArea(apartment2, 2, 0, &result));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment2, 1, 1, &result));
	TEST_EQUALS(final, result, 4);
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment2, 3, 0, &result));
	TEST_EQUALS(final, result, 4);
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment2, 4, 4, &result));
	TEST_EQUALS(final, result, 10);

	apartmentDestroy(apartmentWall);
	apartmentDestroy(apartmentSquare);
	apartmentDestroy(apartment);
	apartmentDestroy(apartment2);
	
	return final;
}

bool apartment_test_split() {
	bool final = true;

	SquareType** square = allocate_one_room(1, 1);
	SquareType** room = allocate_one_room(6, 6);
	SquareType** room2 = allocate_two_room(5, 5);

	Apartment apartmentSquare = apartmentCreate(room, 1, 1, 100);
	square[0][0] = WALL;
	Apartment apartmentWall = apartmentCreate(square, 1, 1, 100);
	Apartment apartment = apartmentCreate(room, 5, 4, 100);
	Apartment apartment2 = apartmentCreate(room2, 5, 4, 100);

	delete_apartment(square, 1);
	delete_apartment(room, 6);
	delete_apartment(room2, 5);

	Apartment result1 = NULL;
	Apartment result2 = NULL;
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(NULL, true, 1, &result1, &result2));
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(apartmentSquare, true, 0, NULL, &result2));
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSplit(apartmentSquare, true, 0, &result1, NULL));

	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSplit(apartment, true, -1, &result1, &result2));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSplit(apartment, true, 5, &result1, &result2));

	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSplit(apartment, false, -1, &result1, &result2));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSplit(apartment, false, 4, &result1, &result2));

	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment, true, 0, &result1, &result2));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment, true, 4, &result1, &result2));

	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment, false, 0, &result1, &result2));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment, false, 3, &result1, &result2));
	
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment, true, 2, &result1, &result2));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment, false, 2, &result1, &result2));

	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment2, true, 2, &result1, &result2));
	
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 2, 2, EMPTY));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment2, false, 2, &result1, &result2));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 2, 2, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 3, 1, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSplit(apartment2, false, 2, &result1, &result2));
	TEST_DIFFERENT(final, result1, NULL);
	TEST_DIFFERENT(final, result2, NULL);

	SquareType** left = allocate_one_room(5, 2);
	left[3][1] = WALL;
	SquareType** right = allocate_one_room(5, 1);

	for (int i = 0; i < 5; i++) {
		SquareType value;
		for (int j = 0; j < 2; j++) {
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(result1, i, j, &value));
			TEST_EQUALS(final, value, left[i][j]);
		}
		TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(result2, i, 0, &value));
		TEST_EQUALS(final, value, right[i][0])
	}

	delete_apartment(left, 5);
	delete_apartment(right, 5);

	apartmentDestroy(result1);
	apartmentDestroy(result2);

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 3, 0, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 3, 3, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 0, 2, EMPTY));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSplit(apartment2, true, 3, &result1, &result2));

	TEST_DIFFERENT(final, result1, NULL);
	TEST_DIFFERENT(final, result2, NULL);

	int area;
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(result1, 0, 0, &area));
	TEST_EQUALS(final, 10, area);
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(result2, 0, 0, &area));
	TEST_EQUALS(final, 2, area);
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(result2, 0, 3, &area));
	TEST_EQUALS(final, 1, area);

	apartmentDestroy(result1);
	apartmentDestroy(result2);

	apartmentDestroy(apartmentWall);
	apartmentDestroy(apartmentSquare);
	apartmentDestroy(apartment);
	apartmentDestroy(apartment2);

	return final;
}

bool apartment_test_num_of_rooms() {
	bool final = true;

	SquareType** square = allocate_one_room(1, 1);
	SquareType** room = allocate_one_room(3, 3);
	SquareType** room2 = allocate_two_room(7, 5);

	Apartment apartmentSquare = apartmentCreate(room, 1, 1, 100);
	square[0][0] = WALL;
	Apartment apartmentWall = apartmentCreate(square, 1, 1, 100);
	Apartment apartment = apartmentCreate(room, 3, 3, 100);
	Apartment apartment2 = apartmentCreate(room2, 7, 5, 100);

	TEST_EQUALS(final, 1, apartmentNumOfRooms(apartmentSquare));
	TEST_EQUALS(final, 0, apartmentNumOfRooms(apartmentWall));
	TEST_EQUALS(final, 1, apartmentNumOfRooms(apartment));
	TEST_EQUALS(final, 2, apartmentNumOfRooms(apartment2));

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 4, 2, EMPTY));
	TEST_EQUALS(final, 1, apartmentNumOfRooms(apartment2));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 4, 0, WALL));
	TEST_EQUALS(final, 1, apartmentNumOfRooms(apartment2));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 4, 1, WALL));
	TEST_EQUALS(final, 3, apartmentNumOfRooms(apartment2));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 4, 3, WALL));
	TEST_EQUALS(final, 4, apartmentNumOfRooms(apartment2));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 4, 4, WALL));
	TEST_EQUALS(final, 5, apartmentNumOfRooms(apartment2));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 0, 3, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 1, 3, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 2, 3, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 3, 3, WALL));
	TEST_EQUALS(final, 5, apartmentNumOfRooms(apartment2));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 1, 4, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 2, 4, WALL));
	TEST_EQUALS(final, 6, apartmentNumOfRooms(apartment2));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 4, 2, WALL));
	TEST_EQUALS(final, 5, apartmentNumOfRooms(apartment2));

	delete_apartment(square, 1);
	delete_apartment(room, 3);
	delete_apartment(room2, 7);

	apartmentDestroy(apartmentWall);
	apartmentDestroy(apartmentSquare);
	apartmentDestroy(apartment);
	apartmentDestroy(apartment2);

	return final;
}

bool apartment_get_and_set_test() {
	bool final = true;

	SquareType value = WALL;
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSetSquare(NULL, 0, 0, value));
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentSetSquare(NULL, 1, 1, value));
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentGetSquare(NULL, 0, 0, &value));
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentGetSquare(NULL, 1, 1, &value));

	SquareType** square = allocate_one_room(1, 1);
	SquareType** room = allocate_one_room(4, 4);
	SquareType** room2 = allocate_two_room(3, 5);

	Apartment apartmentSquare = apartmentCreate(room, 1, 1, 100);
	square[0][0] = WALL;
	Apartment apartmentWall = apartmentCreate(square, 1, 1, 100);
	Apartment apartment = apartmentCreate(room, 4, 4, 100);
	Apartment apartment2 = apartmentCreate(room2, 3, 5, 100);

	for (int i = -1; i <= 4; i++) {
		for (int j = -1; j <= 4; j++) {
			if (i == -1 || i == 4 || j == -1 || j == 4) {
				TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentGetSquare(apartment, i, j, &value));
				TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSetSquare(apartment, i, j, EMPTY));
			}
			else {
				TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment, i, j, &value));
				TEST_EQUALS(final, EMPTY, value);
				TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment, i, j, EMPTY));
				TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, i, j, WALL));
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment, i, j, &value));
			TEST_EQUALS(final, WALL, value);
			TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment, i, j, WALL));
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment, i, j, EMPTY));
		}
	}

	for (int i = -1; i <= 3; i++) {
		for (int j = -1; j <= 5; j++) {
			if (i == -1 || i == 3 || j == -1 || j == 5) {
				TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentGetSquare(apartment2, i, j, &value));
				TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentSetSquare(apartment2, i, j, EMPTY));
			}
			else {
				TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment2, i, j, &value));
				SquareType expected = (j == 2) ? WALL : EMPTY;
				TEST_EQUALS(final, expected, value);
				TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment2, i, j, value));

				if (i == 2 && j != 2) {
					TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, i, j, WALL));
					TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment2, i, j, WALL));
				}
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment2, i, j, &value));
			value = (value == WALL) ? EMPTY : WALL;
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, i, j, value));
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment2, i, j, &value));
			SquareType expected = (j == 2 || i == 2) ?  EMPTY : WALL;
			TEST_EQUALS(final, expected, value);
			TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment2, i, j, value));

			if (i == 2 && j != 2) {
				TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, i, j, WALL));
				TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment2, i, j, WALL));
			}
		}
	}

	delete_apartment(square, 1);
	delete_apartment(room, 4);
	delete_apartment(room2, 3);

	apartmentDestroy(apartmentWall);
	apartmentDestroy(apartmentSquare);
	apartmentDestroy(apartment);
	apartmentDestroy(apartment2);

	return final;
}

bool apartment_compare_test() {
	bool final = true;

	TEST_EQUALS(final, true, apartmentIsIdentical(NULL, NULL));

	SquareType** square = allocate_one_room(1, 1);
	SquareType** room = allocate_one_room(4, 4);
	SquareType** room2 = allocate_two_room(4, 5);

	Apartment apartmentSquare = apartmentCreate(room, 1, 1, 100);
	square[0][0] = WALL;
	Apartment apartmentWall = apartmentCreate(square, 1, 1, 100);
	Apartment apartment = apartmentCreate(room, 4, 4, 100);
	Apartment apartment1Copy = apartmentCreate(room, 4, 4, 100);
	Apartment apartmentSmall = apartmentCreate(room, 4, 3, 100);
	Apartment apartment2 = apartmentCreate(room2, 4, 5, 100);
	Apartment apartment2copy = apartmentCreate(room2, 4, 5, 100);
	Apartment apartment2small = apartmentCreate(room2, 3, 5, 100);

	TEST_EQUALS(final, false, apartmentIsIdentical(NULL, apartment));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, NULL));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, apartmentSmall));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment2, apartment2small));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, apartment));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment2, apartment2));

	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, apartment1Copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment1Copy, 0, 0, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, apartment1Copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment1Copy, 0, 0, EMPTY));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, apartment1Copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment1Copy, 3, 3, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, apartment1Copy));

	TEST_EQUALS(final, true, apartmentIsIdentical(apartment2, apartment2copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2copy, 0, 0, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment2, apartment2copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2copy, 0, 0, EMPTY));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment2, apartment2copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2copy, 3, 4, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment2, apartment2copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2copy, 3, 4, EMPTY));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment2, apartment2copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2copy, 3, 2, EMPTY));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment2, apartment2copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2copy, 3, 2, WALL));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment2, apartment2copy));

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment2, 100));
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment2, apartment2copy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment2, -50));
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment2, apartment2copy));

	delete_apartment(square, 1);
	delete_apartment(room, 4);
	delete_apartment(room2, 4);

	apartmentDestroy(apartmentWall);
	apartmentDestroy(apartmentSquare);
	apartmentDestroy(apartment);
	apartmentDestroy(apartmentSmall);
	apartmentDestroy(apartment1Copy);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment2small);
	apartmentDestroy(apartment2copy);

	return final;
}

bool apartment_copy_test() {
	bool final = true;

	SquareType** room = allocate_one_room(4, 4);
	SquareType** room2 = allocate_two_room(3, 5);

	Apartment apartment = apartmentCreate(room, 4, 4, 100);
	Apartment apartmentDuplicate = apartmentCopy(apartment);
	Apartment apartment2 = apartmentCreate(room2, 3, 5, 200);
	Apartment apartment2duplicate = apartmentCopy(apartment2);

	TEST_EQUALS(final, NULL, apartmentCopy(NULL));

	TEST_EQUALS(final, apartmentGetPrice(apartment2), apartmentGetPrice(apartment2duplicate));
	TEST_EQUALS(final, 200, apartmentGetPrice(apartment2duplicate));
	TEST_EQUALS(final, apartmentGetLength(apartment2), apartmentGetLength(apartment2duplicate));
	TEST_EQUALS(final, 3, apartmentGetLength(apartment2duplicate));
	TEST_EQUALS(final, apartmentGetWidth(apartment2), apartmentGetWidth(apartment2duplicate));
	TEST_EQUALS(final, 5, apartmentGetWidth(apartment2duplicate));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			SquareType value1;
			SquareType value2;
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment2, i, j, &value1));
			TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment2duplicate, i, j, &value2));
			TEST_EQUALS(final, value1, value2);
		}
	}

	TEST_EQUALS(final, true, apartmentIsIdentical(apartment2, apartment2duplicate));

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 1, 0, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(apartment2, 1, 1, WALL));

	TEST_EQUALS(final, false, apartmentIsIdentical(apartment2, apartment2duplicate));

	SquareType value;
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment2, 1, 0, &value));
	TEST_EQUALS(final, WALL, value);
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentGetSquare(apartment2duplicate, 1, 0, &value));
	TEST_EQUALS(final, EMPTY, value);
	TEST_EQUALS(final, 3, apartmentNumOfRooms(apartment2));
	TEST_EQUALS(final, 2, apartmentNumOfRooms(apartment2duplicate));

	apartmentDestroy(apartment);
	apartmentDestroy(apartmentDuplicate);
	apartmentDestroy(apartment2duplicate);
	apartmentDestroy(apartment2);
	delete_apartment(room, 4);
	delete_apartment(room2, 3);

	return final;
}

bool apartment_destroy_test() {
	apartmentDestroy(NULL);

	SquareType** room2 = allocate_two_room(3, 5);
	Apartment apartment = apartmentCreate(room2, 3, 5, 100);
	Apartment apartment2 = apartmentCreate(room2, 3, 5, 100);

	apartmentDestroy(apartment);
	delete_apartment(room2, 3);
	apartmentDestroy(apartment2);

	return true;
}

bool apartment_length_width_test() {
	bool final = true;

	SquareType** room2 = allocate_two_room(4, 6);
	Apartment apartment = apartmentCreate(room2, 4, 6, 100);

	TEST_EQUALS(final, 4, apartmentGetLength(apartment));
	TEST_EQUALS(final, 6, apartmentGetWidth(apartment));
	TEST_EQUALS(final, 4, apartmentGetLength(apartment));
	TEST_EQUALS(final, 6, apartmentGetWidth(apartment));

	Apartment result1 = NULL, result2 = NULL;
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSplit(apartment,false,3,&result1, &result2));
	TEST_DIFFERENT(final, NULL, result1);
	TEST_DIFFERENT(final, NULL, result2);
	TEST_EQUALS(final, 4, apartmentGetLength(result1));
	TEST_EQUALS(final, 3, apartmentGetWidth(result1));
	TEST_EQUALS(final, 4, apartmentGetLength(result2));
	TEST_EQUALS(final, 2, apartmentGetWidth(result2));

	apartmentDestroy(result1);

	Apartment result3 = NULL, result4 = NULL;
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(result2, 1, 0, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(result2, 1, 1, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSplit(result2, true, 1, &result3, &result4));
	apartmentDestroy(result2);

	TEST_DIFFERENT(final, NULL, result3);
	TEST_DIFFERENT(final, NULL, result4);
	TEST_EQUALS(final, 1, apartmentGetLength(result3));
	TEST_EQUALS(final, 2, apartmentGetWidth(result3));
	TEST_EQUALS(final, 2, apartmentGetLength(result4));
	TEST_EQUALS(final, 2, apartmentGetWidth(result4));

	apartmentDestroy(result3);
	apartmentDestroy(result4);

	delete_apartment(room2, 4);
	apartmentDestroy(apartment);

	return final;
}

bool apartment_price_test() {
	bool final = true;

	SquareType square = WALL;
	SquareType* squarePtr = &square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 0);
	TEST_DIFFERENT(final, NULL, apartment);
	apartmentDestroy(apartment);
	apartment = apartmentCreate(&squarePtr, 1, 2, 200);
	TEST_EQUALS(final, 200, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentChangePrice(NULL, 50));
	TEST_EQUALS(final, APARTMENT_PRICE_NOT_IN_RANGE, apartmentChangePrice(apartment, -101));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 200));
	TEST_EQUALS(final, 600, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 300, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 150, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 225, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 113, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 169, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 85, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 127, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 64, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 96, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 48, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 72, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 36, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 54, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 27, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 40, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 20, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 30, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 15, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 22, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 11, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 16, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 8, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 12, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 6, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 9, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 5, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 7, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 4, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 6, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 3, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 4, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 2, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 3, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 2, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 20));
	TEST_EQUALS(final, 2, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -60));
	TEST_EQUALS(final, 1, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -60));
	TEST_EQUALS(final, 1, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -99));
	TEST_EQUALS(final, 1, apartmentGetPrice(apartment));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -100));
	TEST_EQUALS(final, 0, apartmentGetPrice(apartment));

	apartmentDestroy(apartment);

	SquareType** room2 = allocate_two_room(4, 6);
	apartment = apartmentCreate(room2, 4, 6, 100);

	Apartment result1 = NULL, result2 = NULL;
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSplit(apartment, false, 3, &result1, &result2));
	TEST_DIFFERENT(final, NULL, result1);
	TEST_DIFFERENT(final, NULL, result2);
	TEST_EQUALS(final, 66, apartmentGetPrice(result1));
	TEST_EQUALS(final, 50, apartmentGetPrice(result2));

	apartmentDestroy(result2);

	Apartment result3 = NULL, result4 = NULL;
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(result1, 1, 0, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(result1, 1, 1, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(result1, 1, 2, WALL));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSplit(result1, true, 1, &result3, &result4));
	apartmentDestroy(result1);

	TEST_DIFFERENT(final, NULL, result3);
	TEST_DIFFERENT(final, NULL, result4);
	TEST_EQUALS(final, 33, apartmentGetPrice(result3));
	TEST_EQUALS(final, 49, apartmentGetPrice(result4));

	apartmentDestroy(result3);
	apartmentDestroy(result4);

	delete_apartment(room2, 4);
	apartmentDestroy(apartment);

	return final;
}

bool apartment_combo_test() {
	bool final = true;

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;

	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);

	bool result;
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentIsSameRoom(NULL, 0, 1, 0, 1, &result));
	SquareType squareVal;
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS, apartmentGetSquare(apartment, 0, 2, &squareVal));
	TEST_EQUALS(final, 1, apartmentTotalArea(apartment));
	int area;
	TEST_EQUALS(final, APARTMENT_NO_ROOM, apartmentRoomArea(apartment, 0, 0, &area));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentRoomArea(apartment, 0, 1, &area));
	TEST_EQUALS(final, 1, area);
	Apartment split1 = NULL, split2 = NULL;
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT, apartmentSplit(apartment, false, 1, &split1, &split2));

	TEST_EQUALS(final, 1, apartmentNumOfRooms(apartment));
	TEST_EQUALS(final, APARTMENT_OLD_VALUE, apartmentSetSquare(apartment, 0, 0, WALL));
	TEST_EQUALS(final, 2, apartmentGetWidth(apartment));

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 10));
	TEST_EQUALS(final, 110, apartmentGetPrice(apartment));

	Apartment copy = apartmentCopy(apartment);
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, copy));

	apartmentDestroy(apartment);
	apartmentDestroy(copy);

	return final;
}

bool apartment_test_main(int testNum) {
	switch (testNum) {
	case 0:
		RUN_TEST(apartment_test_create);
		RUN_TEST(apartment_test_same_room);
		RUN_TEST(apartment_test_total_area);
		RUN_TEST(apartment_test_room_area);
		RUN_TEST(apartment_test_split);
		RUN_TEST(apartment_test_num_of_rooms);
		RUN_TEST(apartment_get_and_set_test);
		RUN_TEST(apartment_compare_test);
		RUN_TEST(apartment_copy_test);
		RUN_TEST(apartment_destroy_test);
		RUN_TEST(apartment_length_width_test);
		RUN_TEST(apartment_price_test);
		RUN_TEST(apartment_combo_test);
		break;
	case 1:
		RUN_TEST(apartment_test_create);
		break;
	case 2:
		RUN_TEST(apartment_test_same_room);
		break;
	case 3:
		RUN_TEST(apartment_test_total_area);
		break;
	case 4:
		RUN_TEST(apartment_test_room_area);
		break;
	case 5:
		RUN_TEST(apartment_test_split);
		break;
	case 6:
		RUN_TEST(apartment_test_num_of_rooms);
		break;
	case 7:
		RUN_TEST(apartment_get_and_set_test);
		break;
	case 8:
		RUN_TEST(apartment_compare_test);
		break;
	case 9:
		RUN_TEST(apartment_copy_test);
		break;
	case 10:
		RUN_TEST(apartment_destroy_test);
		break;
	case 11:
		RUN_TEST(apartment_length_width_test);
		break;
	case 12:
		RUN_TEST(apartment_price_test);
		break;
	case 13:
		RUN_TEST(apartment_combo_test);
	default:
		return false;
	}

	return true;
}
