/* room.c -------
 *
 * Filename: room.c
 * Description:
 * Author: Bryce Himebaugh
 * Maintainer:
 * Created: Tue Sep  6 14:10:06 2017
 * Last-Updated: June 4 11:00 2019
 *           By: Matthew Fulford
 *     Update #: 0
 * Keywords:
 * Compatibility:
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 *
 *
 */

/* Copyright (c) 2016 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

/* Code: */

#include <stdlib.h>
#include <ncurses.h>
#include "room.h"

room_t *init_room(int upper_left_x, int upper_left_y, int width, int height)
{
	room_t *r;
	r = malloc(sizeof(room_t));
	r->upper_left_x = upper_left_x;
	r->upper_left_y = upper_left_y;
	r->width = width;
	r->height = height;
	r->draw_char = '#';
	r->color[0] = 0;
	r->color[1] = 0;
	r->color[2] = 255;
	return (r);
}

void draw_room(room_t *r)
{
	int row_counter, column_counter;

	// Draw Top of room
	for (row_counter = r->upper_left_x;
		 row_counter <= (r->upper_left_x + r->width);
		 row_counter++) {
		mvprintw(r->upper_left_y, row_counter, "%c", r->draw_char);
	}

	// Draw left side of room
	for (column_counter = r->upper_left_y;
		 column_counter <= (r->upper_left_y + r->height);
		 column_counter++) {
		mvprintw(column_counter, r->upper_left_x, "%c", r->draw_char);
	}

	// Draw right side of room
	for (column_counter = r->upper_left_y;
		 column_counter <= (r->upper_left_y + r->height);
		 column_counter++) {
		mvprintw(column_counter, (r->upper_left_x + r->width), "%c", r->draw_char);
	}

	// Draw Bottom of room
	for (row_counter = r->upper_left_x;
		 row_counter <= (r->upper_left_x + r->width);
		 row_counter++) {
		mvprintw(r->upper_left_y + r->height, row_counter, "%c", r->draw_char);
	}
}

room_t *changeRoomSize(int upper_left_x, int upper_left_y, int width, int height, room_t *r)
{
	r->upper_left_x = upper_left_x;
	r->upper_left_y = upper_left_y;

	if (width < 10)
		r->width = 10;
	else
		r->width = width;

	if (height < 10)
		r->height = 10;
	else
		r->height = height;

	return (r);
}

void undraw_room(room_t *r)
{
	int row_counter, column_counter;

	// Undraw Top of room
	for (row_counter = r->upper_left_x;
		 row_counter <= (r->upper_left_x + r->width);
		 row_counter++) {
		mvprintw(r->upper_left_y, row_counter, " ");
	}

	// Undraw left side of room
	for (column_counter = r->upper_left_y;
		 column_counter<=(r->upper_left_y + r->height);
		 column_counter++) {
		mvprintw(column_counter, r->upper_left_x, " ");
	}

	// Undraw right side of room
	for (column_counter = r->upper_left_y;
		 column_counter <= (r->upper_left_y + r->height);
		 column_counter++) {
		mvprintw(column_counter, (r->upper_left_x + r->width), " ");
	}

	// Undraw Bottom of room
	for (row_counter = r->upper_left_x;
		 row_counter <= (r->upper_left_x + r->width);
		 row_counter++) {
		mvprintw(r->upper_left_y + r->height, row_counter, " ");
	}
}
/* room.c ends here */
