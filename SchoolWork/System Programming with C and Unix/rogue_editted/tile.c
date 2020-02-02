/* tile.c ---------
 *
 * Filename: cell.c
 * Description:
 * Author: Dmitrii Galantsev
 * Maintainer:
 * Created:      May 12 11:28 2018
 * Last-Updated: June 4 11:00 2019
 *           By: Matthew Fulford
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include "tile.h"

tile * create_tile(int init_x, int init_y, int d_x, int d_y, int p, int l, int e) {

  // create the tile
  tile * t = malloc(sizeof(tile));
  t->x = init_x;
  t->y = init_y;
  
  t->draw_x = d_x;
  t->draw_y = d_y;
  
  t->door = 0;
  t->stair = 0;
  
  if(p){
    t->state[CURRENT] = PLAYER;
  }else if(l){
    t->state[CURRENT] = LOOT;
  }else if(e){
    t->state[CURRENT] = EMPTY;
  }else if(rand()%100 == 1){
    t->state[CURRENT] = LOOT;
  }else if(rand()%500 == 1){
    int randomE = rand()%3;
    switch(randomE){
    case 1:
      t->state[CURRENT] = MONSTER;
      break;
    case 2:
      t->state[CURRENT] = ENEMY;
      break;
    default:
      t->state[CURRENT] = FATAL;
      break;
    }
  }else{
    t->state[CURRENT] = EMPTY;
  }
  t->state[NEW] = UNCHANGED;
  
  return(t);
}

int destroy_tile (tile * t) {
  free(t);
  
  return SUCCESS;
}

int update_tile (tile * t){
  if(t->state[NEW] != UNCHANGED){
    t->state[OLD] = t->state[CURRENT];
    t->state[CURRENT] = t->state[NEW];
    t->state[NEW] = UNCHANGED;
  }
  return SUCCESS;
}

void update_tiles(int x_off, int y_off, int x, int y, tile* t[1000][1000]){
  int i, j;
  for(i=0;i<x;i++)
    for(j=0;j<y;j++)
      update_tile(t[i+x_off][j+y_off]);
}

void display_tile (tile * t) {
  if(has_colors() == TRUE){
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK); /* Player color */
    init_pair(2, COLOR_GREEN, COLOR_BLACK); /* Loot color */
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); /* Enemy color */
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK); /* Monster color */
    init_pair(5, COLOR_RED, COLOR_BLACK); /* Fatal color */

    if(t->state[CURRENT] == PLAYER){
      attron(COLOR_PAIR(1));
      mvprintw(t->draw_y, t->draw_x, "@");
      attroff(COLOR_PAIR(1));
    }else if(t->state[CURRENT] == LOOT){
      attron(COLOR_PAIR(2));
      mvprintw(t->draw_y, t->draw_x, "$");
      attroff(COLOR_PAIR(2));
    }else if(t->state[CURRENT] == ENEMY){
      attron(COLOR_PAIR(3));
      mvprintw(t->draw_y, t->draw_x, "E");
      attroff(COLOR_PAIR(3));
    }else if(t->state[CURRENT] == MONSTER){
      attron(COLOR_PAIR(4));
      mvprintw(t->draw_y, t->draw_x, "M");
      attroff(COLOR_PAIR(4));
    }else if(t->state[CURRENT] == FATAL){
      attron(COLOR_PAIR(5));
      mvprintw(t->draw_y, t->draw_x, "F");
      attroff(COLOR_PAIR(5));
    }else if(t->door){
      mvprintw(t->draw_y, t->draw_x, "D");
    }else if(t->stair){
      mvprintw(t->draw_y, t->draw_x, "H");
    }else{
      mvprintw(t->draw_y, t->draw_x, " ");
    }
  } else {
    if(t->state[CURRENT] == PLAYER){
      mvprintw(t->draw_y, t->draw_x, "@");
    }else if(t->state[CURRENT] == LOOT){
      mvprintw(t->draw_y, t->draw_x, "$");
    }else if(t->state[CURRENT] == ENEMY){
      mvprintw(t->draw_y, t->draw_x, "E");
    }else if(t->state[CURRENT] == MONSTER){
      mvprintw(t->draw_y, t->draw_x, "M");
    }else if(t->state[CURRENT] == FATAL){
      mvprintw(t->draw_y, t->draw_x, "F");
    }else if(t->door){
      mvprintw(t->draw_y, t->draw_x, "D");
    }else if(t->stair){
      mvprintw(t->draw_y, t->draw_x, "H");
    }else{
      mvprintw(t->draw_y, t->draw_x, " ");
    }
  }
}


void display_tiles (int x_off, int y_off, int width, int height, tile * t[1000][1000]) {
  int x, y;
  for (x = 0; x < width; x++)
    for (y = 0; y < height; y++)
      display_tile(t[x+x_off][y+y_off]);
}

//function for moving all the enemies
int move_enemies(tile* player, int x_off, int y_off, int x, int y, tile* t[1000][1000]){
  static count;
  
  if(count != 5){
    count++;
    return 0;
  }
  
  int i, j, k;
  for(i=0;i<x;i++)
    for(j=0;j<y;j++){
      int x_diff = abs(player->x - (i+x_off));
      int y_diff = abs(player->y - (j+y_off));
      if(t[i+x_off][j+y_off]->state[CURRENT] == ENEMY){ // Condition for moving the weakest enemy
	if(x_diff <= 2 && y_diff <= 2 && x_diff != 0 && y_diff != 0){
	  t[i+x_off][j+y_off]->state[CURRENT] = EMPTY;
	  return 1;
	}else if(x_diff == 0 && y_diff == 0){
	  t[i+x_off][j+y_off]->state[CURRENT] = PLAYER;
          return 1;
	}else if(x_diff > y_diff){
	  int new_x = i+x_off + (3 * (player->x - (i+x_off)))/x_diff;
	  t[i+x_off][j+y_off]->state[NEW] = EMPTY;
	  t[new_x][j+y_off]->state[NEW] = ENEMY;
	}else{
	  int new_y = j+y_off + (3 * (player->y - (j+y_off)))/y_diff;
	  t[i+x_off][j+y_off]->state[NEW] = EMPTY;
	  t[i+x_off][new_y]->state[NEW] = ENEMY;
	}
      }else if(t[i+x_off][j+y_off]->state[CURRENT] == MONSTER){ // moving the medium strength enemy
        if(x_diff <= 1 && y_diff <= 1 && x_diff != 0 && y_diff != 0){
          t[i+x_off][j+y_off]->state[CURRENT] = EMPTY;
          return 2;
        }else if(x_diff == 0 && y_diff == 0){
          t[i+x_off][j+y_off]->state[CURRENT] = PLAYER;
          return 2;
        }else if(x_diff > y_diff){
          int new_x = i+x_off + (2 * (player->x - (i+x_off)))/x_diff;
	  t[i+x_off][j+y_off]->state[NEW] = EMPTY;
          t[new_x][j+y_off]->state[NEW] = MONSTER;
	}else{
	  int new_y = j+y_off + (2 * (player->y - (j+y_off)))/y_diff;
	  t[i+x_off][j+y_off]->state[NEW] = EMPTY;
          t[i+x_off][new_y]->state[NEW] = MONSTER;
        }
      }else if(t[i+x_off][j+y_off]->state[CURRENT] == FATAL){ // moving the strongest enemy
        if(x_diff == 0 && y_diff == 0){
          t[i+x_off][j+y_off]->state[CURRENT] = PLAYER;
          return 3;
        }else if(x_diff > y_diff){
          int new_x = i+x_off + (player->x - (i+x_off))/x_diff;
          t[i+x_off][j+y_off]->state[NEW] = EMPTY;
          t[new_x][j+y_off]->state[NEW] = FATAL;
        }else{
          int new_y = j+y_off + (player->y - (j+y_off))/y_diff;
          t[i+x_off][j+y_off]->state[NEW] = EMPTY;
          t[i+x_off][new_y]->state[NEW] = FATAL;
        }
      }
    }
  count = 0;
  return 0;
}

//A function for moving the player
int move_player (tile** player, int new_x, int new_y, int width, int height, tile * t[1000][1000]) {	
  int down,up,left,right;
  
  //decide enemie's direction of movement
  if(new_x >= 0 && new_x < (*player)->x && new_x%width < (*player)->x%width)
    down = 1;
  else 
    down = 0;
  if(new_x >= 0 && new_x > (*player)->x && new_x%width > (*player)->x%width)
    up = 1;
  else 
    up = 0;
  if(new_y >= 0 && new_y < (*player)->y && new_y%height < (*player)->y%height)
    left = 1;
  else 
    left = 0;
  if(new_y >= 0 && new_y > (*player)->y && new_y%height > (*player)->y%height)
    right = 1;
  else
    right = 0;
  
  if(down || up || left || right || ((*player)->door && t[new_x][new_y]->door)){
    if(t[new_x][new_y]->state[CURRENT] == LOOT){
      t[new_x][new_y]->state[NEW] = PLAYER;
      t[(*player)->x][(*player)->y]->state[NEW] = EMPTY;
      
      *player = t[new_x][new_y];
      return 1;    //returns 1 when the player picks u pgold
    }else if(t[new_x][new_y]->state[CURRENT] == ENEMY){
      t[new_x][new_y]->state[NEW] = PLAYER;
      t[(*player)->x][(*player)->y]->state[NEW] = EMPTY;
      *player = t[new_x][new_y];
      return -1;    //returns -1 when the player walks into a monster
    }else if(t[new_x][new_y]->state[CURRENT] == MONSTER){
      t[new_x][new_y]->state[NEW] = PLAYER;
      t[(*player)->x][(*player)->y]->state[NEW] = EMPTY;
      *player = t[new_x][new_y];
      return -2;    //returns -2 when the player walks into a monster
    }else if(t[new_x][new_y]->state[CURRENT] == FATAL){
      t[new_x][new_y]->state[NEW] = PLAYER;
      t[(*player)->x][(*player)->y]->state[NEW] = EMPTY;
      *player = t[new_x][new_y];
      return -3;    //returns -3 when the player walks into a monster
    }else if(((*player)->door) && t[new_x][new_y]->door){
      int out;
      if((*player)->y > new_y){
	out = 3; // up
      }else if((*player)->y < new_y){
	out = 4; // down
      }else if((*player)->x > new_x){
	out = 5; // left
      }else if((*player)->x < new_x){
        out = 6; // right
      }
      t[new_x][new_y]->state[NEW] = PLAYER;
      t[(*player)->x][(*player)->y]->state[NEW] = EMPTY;
      update_tile(t[(*player)->x][(*player)->y]);
      *player = t[new_x][new_y];
      return out;
    }else if((*player)->stair){
      return 7;
    }else{
      t[new_x][new_y]->state[NEW] = PLAYER;
      t[(*player)->x][(*player)->y]->state[NEW] = EMPTY;
      
      *player = t[new_x][new_y];
      return 2;    //returns 2 when the player walks into an empty tile
    }
  }else{
    return 0;    //returns 0 if the player isn't allowed to move
  }
}


