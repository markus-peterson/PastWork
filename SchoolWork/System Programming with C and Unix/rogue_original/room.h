/* room.h ---------
 *
 * Filename: room.h
 * Description:
 * Author: Bryce Himebaugh
 * Maintainer:
 * Created: Tue Sep  6 14:10:53 2017
 * Last-Updated: June 4 11:00 2019
 *           By: Matthew Fulford
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

#define WELL_WIDTH 50
#define WELL_HEIGHT 25

typedef struct room
{
  int upper_left_x;
  int upper_left_y;
  int width;
  int height;
  char draw_char;
  char color[3];
} room_t;

room_t *init_room(int, int, int, int);
room_t *changeRoomSize(int upper_left_x, int upper_left_y, int width, int height, room_t *);

void undraw_room(room_t *);
void draw_room(room_t *);
