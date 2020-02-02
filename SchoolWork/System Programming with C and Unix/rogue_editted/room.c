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
#include <string.h>
#include <ncurses.h>
#include "room.h"

room_t *init_room(int upper_left_x, int upper_left_y, int width, int height) {
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

void draw_room(room_t *r) {
  int row_counter, column_counter;
  if(has_colors() == TRUE){
    start_color();
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    attron(COLOR_PAIR(6));
  }
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
  if(has_colors() == TRUE){
    attroff(COLOR_PAIR(6));
  }
}

room_t *changeRoomSize(int upper_left_x, int upper_left_y, int width, int height, room_t *r) {
  r->upper_left_x = upper_left_x;
  r->upper_left_y = upper_left_y;
  
  if (width < 10) {
    r->width = 10;
  } else {
    r->width = width;
  }
  if (height < 10) {
    r->height = 10;
  } else {
    r->height = height;
  }
  return (r);
}

void undraw_room(room_t *r) {
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

void draw_menu(room_t * r, int x, int y, int optionColor){
  int row_counter, column_counter, i;
  const char * options[5] = {"Resume", "Save Game", "Load Game", "Help Menu", "Quit Game"};

  for (row_counter = r->upper_left_x;
       row_counter <= (r->upper_left_x + r->width);
       row_counter++) {
    for (column_counter = r->upper_left_y;
	 column_counter<=(r->upper_left_y + r->height);
	 column_counter++) {
      mvprintw(column_counter, row_counter, " ");
    }
  }
    start_color();
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    init_pair(7, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(6));
  for (column_counter = r->upper_left_y;
       column_counter <= (r->upper_left_y + r->height);
       column_counter++) {
    mvprintw(column_counter, r->upper_left_x, "%c", r->draw_char);
  }
  if(has_colors() == TRUE){
    attroff(COLOR_PAIR(6));
  }

  for(i = 0; i < 5; i++){
    if(optionColor == i)
      attron(COLOR_PAIR(6));
    else
      attron(COLOR_PAIR(7));

    mvprintw(y + i, x, options[i]);

    if(optionColor == i)
      attroff(COLOR_PAIR(6));
    else
      attroff(COLOR_PAIR(7));
  }
}

void draw_help(room_t * r, int x, int y){
  int i;
  init_pair(8, COLOR_WHITE, COLOR_BLACK);
  const char * helpMenu[8] = {"Move Up    -    Up Arrow",
                              "Move Down  -  Down Arrow",
                              "Move Left  -  Left Arrow",
                              "Move Right - Right Arrow",
                              "Pause Game -      p or P",
                              "Save Game  -      s or S",
			      "Loas Game  -      l or L",
                              "Quite Game -      q or Q"};

  attron(COLOR_PAIR(8));
  mvprintw(y - 2, x, "Help Menu");
  mvprintw(y - 1, x, "------------------------");
  attroff(COLOR_PAIR(8));
  attron(COLOR_PAIR(7));
  for(i = 0; i < 8; i++){
    mvprintw(y + i, x, helpMenu[i]);
  }
  attroff(COLOR_PAIR(7));
}
void draw_saves(room_t * r, int x, int y, int saveOrLoad, int optionColor){
  int row_counter, column_counter, i;
  char fileName[8];

  init_pair(6, COLOR_BLACK, COLOR_WHITE);
  init_pair(7, COLOR_GREEN, COLOR_BLACK);  
  init_pair(8, COLOR_WHITE, COLOR_BLACK);
  
  for (row_counter = r->upper_left_x + 1;
       row_counter <= (r->upper_left_x + r->width);
       row_counter++) {
    for (column_counter = r->upper_left_y;
         column_counter<=(r->upper_left_y + r->height);
         column_counter++) {
      mvprintw(column_counter, row_counter, " ");
    }
  }

  attron(COLOR_PAIR(8));
  if(saveOrLoad)
    mvprintw(y - 1, x, "Save Game");
  else
    mvprintw(y - 1, x, "Load Game");
  mvprintw(y - 0, x, "---------");
  attroff(COLOR_PAIR(8));

  for(i = 1; i <= 11; i++){
    if(optionColor == i)
      attron(COLOR_PAIR(6));
    else
      attron(COLOR_PAIR(7));
    if(i <= 10){
      sprintf(fileName, "Save %-2d", i);
      mvprintw(y + i, x, fileName);
    }else{
      mvprintw(y + 1 + i, x, "Cancel");
    }
    
    if(optionColor == i)
      attroff(COLOR_PAIR(6));
    else
      attroff(COLOR_PAIR(7));
  }
}

/* room.c ends here */
