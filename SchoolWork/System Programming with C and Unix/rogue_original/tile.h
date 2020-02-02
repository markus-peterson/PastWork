/* tile.h ---------
 *
 * Filename: cell.h
 * Description:
 * Author: Dmitrii Galantsev
 * Maintainer:
 * Created:      May 12 11:28 2018
 * Last-Updated: June 4 11:00 2019
 *           By: Matthew Fulford 
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Copyright (c) 2018 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

/* Code: */

typedef struct tile
{
  int x;
  int y;

  int draw_x;
  int draw_y;

  int door;
  int stair;

  /* State explanation
   * state[0] represents previous state
   * state[1] is the current state
   * state[2] is the next step
   */
  int state[3];
} tile;

enum {PLAYER, LOOT, EMPTY, ENEMY, UNCHANGED};  // States of each tile
enum {OLD, CURRENT, NEW};   // Enumerations for each step
enum {SUCCESS, FAIL};       // Status of functions

tile* create_tile (int init_x, int init_y, int d_x, int d_y, int p, int l, int e);
int destroy_tile (tile*);
void update_tiles(int x_off, int y_off, int x, int y, tile* t[x][y]);
void display_tile (tile *);
void display_tiles (int x_off, int y_off, int width, int height, tile* c[100][100]);
int move_enemies(tile* player, int x_off, int y_off, int x, int y, tile* t[100][100]);
int move_player (tile** player, int new_x, int new_y, int width, int height, tile* t[100][100]);
void test_cell (void);

/* tile.h ends here */
