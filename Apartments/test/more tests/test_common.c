#include "test_common.h"

#include <stdlib.h>

SquareType** allocate_one_room(int length, int width) {
	SquareType** room = malloc(length * sizeof(SquareType*));
	for (int i = 0; i < length; i++) {
		room[i] = malloc(width * sizeof(SquareType));
	}
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			room[i][j] = EMPTY;
		}
	}
	return room;
}

SquareType** allocate_two_room(int length, int width) {
	SquareType** room = allocate_one_room(length, width);
	for (int i = 0; i < length; i++) {
		room[i][width / 2] = WALL;
	}

	return room;
}

void delete_apartment(SquareType** squares, int length) {
	if (squares == NULL) {
		return;
	}
	for (int i = 0; i < length; i++) {
		free(squares[i]);
	}
	free(squares);
}
