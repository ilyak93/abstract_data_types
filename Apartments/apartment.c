/*
* apartment->c
*
*  Created on: Mar 25, 2016
*      Author: Ilya
*/

#include <stdio.h>
#include <stdlib.h>
#include "apartment.h"
#include <assert.h>

//the function returns true if there is a path  for (x,y) and (z,w)
// and false otherwise

/* This function receives an apartment and indices representing two
 * EMPTY squares in it and returns true if and only if there is a path
 * of EMPTY squares connecting the two given squares.
 * Here a path is a sequence s_0, s_1, ..., s_n of squares such that
 * each pair s_i,s_{i+1} are adjacent squares, i.e. s_i is directly
 * above, below, to the left or to the right of s_{i+1}. We also consider
 * each square to be connected to itself.
 *
 * @return:
 * 	true if the squares [row1,col1] and [row2,col2] in the given
 * 	apartment are connected by a path of EMPTY squares.
 * 	false otherwise. In particular, invalid indices (not representing
 * 	EMPTY squares in the given apartment) result in the function
 * 	returning false.
 */
static bool recSearch(Apartment apartment, int row1, int col1,
	int row2, int col2) {
	if (row1 < 0 || row1 >= apartment->length || col1 < 0 ||
		col1 >= apartment->width || apartment->squares[row1][col1] == WALL) {
		return false;
	} else if (row1 == row2 && col1 == col2) {
		return true;
	}
	apartment->squares[row1][col1] = WALL;
	return recSearch(apartment, row1 + 1, col1, row2, col2) ||
		recSearch(apartment, row1 - 1, col1, row2, col2) ||
		recSearch(apartment, row1, col1 + 1, row2, col2) ||
		recSearch(apartment, row1, col1 - 1, row2, col2);
}

//the function returns the area of the room by recursive steps on each square
// until we step a WALL
/* This function receives an apartment and indices representing
 * an EMPTY square in it and returns to the area of the room which
 * contains this square, i.e. the number of EMPTY squares that are
 * connected to the given square.
 *
 * @return:
 * The number of EMPTY squares in the given apartment that are connected
 * to the square [row,col].
 * Invalid input (indices not representing an EMPTY square in the
 * apartment) results in the function returning a room area of 0.
 */
int static RoomArea(Apartment apartment, int row, int col) {
	assert(apartment != NULL);
	if (row < 0 || col < 0 || row >= apartment->length ||
		col >= apartment->width || apartment->squares[row][col] == WALL) {
		return 0;
	}
	apartment->squares[row][col] = WALL;
	int count = 1 + RoomArea(apartment, row + 1, col) +
		RoomArea(apartment, row - 1, col) +
		RoomArea(apartment, row, col + 1) +
		RoomArea(apartment, row, col - 1);
	return count;
}

/* This function receives an apartment and indices representing an
 * EMPTY square in it. It fills the room in which this square lies
 * with WALL squares, including the given square.
 *
 * @return:
 * 	The room in which lies the square represented by [row,col] in the given
 * 	apartment is filled with WALL squares.
 * 	Invalid input (illegal indices or a square representing a WALL) result
 * 	in the function doing nothing.
 */
static void FillRoom(Apartment apartment, int row, int col) {
	assert(apartment != NULL);
	if (row < 0 || col < 0 || row >= apartment->length ||
		col >= apartment->width || apartment->squares[row][col] == WALL) {
		return;
	}
	apartment->squares[row][col] = WALL;
	FillRoom(apartment, row + 1, col);
	FillRoom(apartment, row - 1, col);
	FillRoom(apartment, row, col + 1);
	FillRoom(apartment, row, col - 1);
}

//The functionc checks if the row or the col are all WALL squares.
/* This function receives an apartment, a boolean parameter and an index
 * and checks whether the row or column in the apartment with this index
 * has only WALL squares. The value of the boolean parameter determines
 * whether the check will be made on a row or a column.
 *
 * @return:
 * 	true if split_by_row==1 and the row with index i in the given apartment
 * 	has only WALLs.
 * 	true if split_by_row==0 and the column with index i in the given apartment
 * 	has only WALLs.
 * 	false otherwise.
 */
static bool rowOrColWallCheck(Apartment apartment, bool split_by_row,
	int index) {
	assert(apartment != NULL || index < 1 || index > apartment->length *
		split_by_row + apartment->width * !split_by_row);
	for (int i = 0; i < apartment->length * !split_by_row +
		apartment->width * split_by_row; i++) {
		if (apartment->squares[index*split_by_row + i*!split_by_row][index *
			!split_by_row + i*split_by_row] != WALL) {
			return false;
		}
	}
	return true;
}

Apartment apartmentCreate(SquareType** squares, int length, int width,
	int price) {
	if (length < 1 || width < 1 || price < 0 || !squares) {
		return NULL;
	}
	Apartment ap = (Apartment)malloc(sizeof(*ap));
	if (!ap) {
		return NULL;
	}
	ap->squares = (SquareType**)malloc(sizeof(SquareType*)*length);
	if (!ap->squares) {
		apartmentDestroy(ap);
		return NULL;
	}
	/*for(int i = 0; i < length; i++){ /// In principle, I could do this, and
	 * 								// as we learned, it is more correct, but
	 * 								// here I can iterate on length and destory
	 * 								// only the relevant squares to destroy
		ap->squares[i] = NULL;
	}*/
	ap->length = 0;
	for (int l = 0; l < length; l++) {
		ap->squares[l] = (SquareType*)malloc(sizeof(SquareType)*width);
		ap->length++;
		if (!ap->squares[l]) {
			apartmentDestroy(ap);
			return NULL;
		} else {
			for (int w = 0; w < width; w++) {
				ap->squares[l][w] = squares[l][w];
			}
		}
	}
	ap->width = width;
	ap->price = price;
	return ap;
}

void apartmentDestroy(Apartment apartment) {
	if (apartment) {
		for (int i = 0; i < apartment->length; i++) {
			free(apartment->squares[i]);
		}
		free(apartment->squares);
		free(apartment);
	}
}

Apartment apartmentCopy(Apartment apartment) {
	if (!apartment) {
		return NULL;
	}
	return apartmentCreate(apartment->squares, apartment->length,
		apartment->width, apartment->price);
}

ApartmentResult apartmentIsSameRoom(Apartment apartment, int row1, int col1,
	int row2, int col2, bool* outResult) {
	if (!apartment || !outResult) {
		return APARTMENT_NULL_ARG;
	} else if (row1 < 0 || row1 >= apartment->length || row2 < 0 ||
		row2 >= apartment->length || col1 < 0 || col1 >= apartment->width ||
		col2 < 0 || col2 >= apartment->width) {
		return APARTMENT_OUT_OF_BOUNDS;
	} else if (apartment->squares[row1][col1] == WALL ||
		apartment->squares[row2][col2] == WALL) {
		return APARTMENT_NO_ROOM;
	}
	Apartment tmp = apartmentCopy(apartment);
	if (!tmp) {
		return APARTMENT_OUT_OF_MEM;
	}
	*outResult = recSearch(tmp, row1, col1, row2, col2);
	apartmentDestroy(tmp);
	return APARTMENT_SUCCESS;
}

int apartmentTotalArea(Apartment apartment) {
	assert(apartment != NULL);
	int empty = 0;
	for (int i = 0; i < apartment->length; i++) {
		for (int j = 0; j < apartment->width; j++) {
			if (apartment->squares[i][j] == EMPTY) {
				empty++;
			}
		}
	}
	return empty;
}

ApartmentResult apartmentRoomArea(Apartment apartment, int row, int col,
	int* outArea) {
	if (!apartment || !outArea) {
		return APARTMENT_NULL_ARG;
	} else if (row < 0 || row >= apartment->length ||
		col < 0 || col >= apartment->width) {
		return APARTMENT_OUT_OF_BOUNDS;
	} else if (apartment->squares[row][col] == WALL) {
		return APARTMENT_NO_ROOM;
	}
	Apartment tmp = apartmentCopy(apartment);
	if (!tmp) {
		return APARTMENT_OUT_OF_MEM;
	}
	*outArea = RoomArea(tmp, row, col);
	apartmentDestroy(tmp);
	return APARTMENT_SUCCESS;
}

//Here I use splitByRow like a switch for the correct parameters to be passed
// for the relevant function. Its very comfortable to see both variation in one
// place. It's 48 lines.

ApartmentResult apartmentSplit(Apartment apartment, bool splitByRow,
	int index, Apartment* first, Apartment* second) {
	if (!apartment || !first || !second) {
		return APARTMENT_NULL_ARG;
	} else if (index < 0 || index >= apartment->length*splitByRow +
		apartment->width*!splitByRow) {
		return APARTMENT_OUT_OF_BOUNDS;
	} else if (index == 0 || index == (apartment->length - 1)*splitByRow +
		(apartment->width - 1)*(!splitByRow) ||
		!rowOrColWallCheck(apartment, splitByRow, index)) {
		return APARTMENT_BAD_SPLIT;
	}
	Apartment new1 = apartmentCreate(apartment->squares,
		apartment->length * !splitByRow + index *splitByRow,
		apartment->width * splitByRow + index * !splitByRow,
		apartment->price *(index + 1) / (apartment->length *
			splitByRow + apartment->width * !splitByRow));
	if (!new1) {
		return APARTMENT_OUT_OF_MEM;
	}
	SquareType** tmp = NULL;
	if (!splitByRow) {
		tmp = malloc(sizeof(SquareType*)*apartment->length);
		if (!tmp) {
			apartmentDestroy(new1);
			return APARTMENT_OUT_OF_MEM;
		}
		for (int i = 0; i < apartment->length; i++) {
			tmp[i] = &apartment->squares[i][index + 1];
		}

	}
	Apartment new2 = apartmentCreate((!splitByRow ? tmp :
		apartment->squares + index + 1), (apartment->length - index - 1) *
		splitByRow + apartment->length * !splitByRow, apartment->width *
		splitByRow + (apartment->width - index - 1) *!splitByRow,
		apartment->price *((apartment->length - index - 1) * splitByRow +
			(apartment->width - index - 1) * !splitByRow + 1) /
		(apartment->length * splitByRow + apartment->width * !splitByRow));
	if (!new2) {
		free(tmp);
		apartmentDestroy(new1);
		return APARTMENT_OUT_OF_MEM;
	}
	*first = new1;
	*second = new2;
	free(tmp);
	return APARTMENT_SUCCESS;
}

int apartmentNumOfRooms(Apartment apartment) {
	assert(apartment != NULL);
	Apartment tmp = apartmentCopy(apartment);
	if (!tmp) {
		return APARTMENT_OUT_OF_MEM;
	}
	int count = 0;
	for (int i = 0; i < apartment->length; i++) {
		for (int j = 0; j < apartment->width; j++) {
			if (tmp->squares[i][j] == EMPTY) {
				FillRoom(tmp, i, j);
				count++;
			}
		}
	}
	apartmentDestroy(tmp);
	return count;
}

ApartmentResult apartmentSetSquare(Apartment apartment, int row, int col,
	SquareType value) {
	if (!apartment) {
		return APARTMENT_NULL_ARG;
	} else if (row < 0 || row >= apartment->length || col < 0 ||
		col >= apartment->width) {
		return APARTMENT_OUT_OF_BOUNDS;
	} else if (apartment->squares[row][col] == value) {
		return APARTMENT_OLD_VALUE;
	}
	apartment->squares[row][col] = value;
	return APARTMENT_SUCCESS;
}

ApartmentResult apartmentGetSquare(Apartment apartment, int row, int col,
	SquareType* outValue) {
	if (!apartment || !outValue) {
		return APARTMENT_NULL_ARG;
	} else if (row < 0 || row >= apartment->length || col < 0 ||
		col >= apartment->width) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	*outValue = apartment->squares[row][col];
	return APARTMENT_SUCCESS;
}

ApartmentResult apartmentChangePrice(Apartment apartment, int percent) {
	if (!apartment) {
		return APARTMENT_NULL_ARG;
	} else if (percent < -100) {
		return APARTMENT_PRICE_NOT_IN_RANGE;
	}
	apartment->price = apartment->price
		+ (int)((percent * apartment->price) / 100);
	return APARTMENT_SUCCESS;
}

int apartmentGetPrice(Apartment apartment) {
	assert(apartment != NULL);
	return apartment->price;
}

int apartmentGetLength(Apartment apartment) {
	assert(apartment != NULL);
	return apartment->length;
}

int apartmentGetWidth(Apartment apartment) {
	assert(apartment != NULL);
	return apartment->width;
}

bool apartmentIsIdentical(Apartment apartment1, Apartment apartment2) {
	if (!apartment1 && !apartment2) {
		return true;
	} else if (apartment1 && apartment2 &&
			apartment1->length == apartment2->length &&
			apartment1->width == apartment2->width &&
		apartment1->price == apartment2->price) {
		for (int i = 0; i < apartment1->length; i++) {
			for (int j = 0; j < apartment1->width; j++) {
				if (apartment1->squares[i][j] != apartment2->squares[i][j]) {
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

