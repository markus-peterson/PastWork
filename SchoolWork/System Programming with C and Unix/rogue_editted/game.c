/* game.c ---------
 *
 * Filename: game.c
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
 */

/* Copyright (c) 2018 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tile.h"
#include "game.h"
#include "room.h"
#include "key.h"

//function the fill the array with tiles to make a floor
void MakeFloor(tile* tiles[1000][1000], int width, int height, int screen_x_offset, int screen_y_offset, int fCount){
  int i, j;
  int hFull = 1;
  int vFull = 1;
  int temp = 0;
  int widthUp = 100 + fCount * width;
  int heightUp = 100 + fCount * height;
  for(i = 0; i < widthUp; i++){
    for(j = 0; j < heightUp; j++){
      destroy_tile(tiles[i][j]);
      tiles[i][j] = create_tile(i, j, screen_x_offset + i%width, screen_y_offset + j%height, 0, 0, 0);
    }
  }
  for(i = 0; i < widthUp; i++){
    if (i%width == 0){
      hFull = 1;
    }
    for(j = 0; j < heightUp; j++){
      if(j%height == 0){
        vFull = 1;
      }
      if(j%height == 0 && j != 0 &&  hFull){ //rand()%height <= 2 && hFull){
        int off = rand()%width;
        tiles[i + off][j]->door = 1;
        tiles[i + off][j-1]->door = 1;
        hFull = 0;
      }else if(i%width == 0 && i != 0 && vFull ){ //rand()%width <= 2 && vFull){
        int off = rand()%height;
        tiles[i][j + off]->door = 1;
        tiles[i-1][j + off]->door = 1;
        vFull = 0;
      }
    }
  }

  tiles[rand()%widthUp][rand()%heightUp]->stair = 1;
  tiles[width/2][height/2]->state[CURRENT] = PLAYER;
}

//use malloc and ralloc
void init_game(FILE * file, tile * tiles[1000][1000], int * fCount, int * x_off, int * y_off, tile ** player, int screen_x_offset, int screen_y_offset) {
  int i, j, state, p, l, e;
  int health, points, x, y, count, door, stair;
  int height = 25;
  int width = 50;
  fscanf(file, "%d:%d:%d:%d|", &health, &points, &x, &y, &count);
  plyr.health = health;
  plyr.points = points;
  *x_off = x;
  *y_off = y;
  *fCount = count;
  int widthUp = 100 + *fCount * 50;
  int heightUp = 100 + *fCount * 25;
  for(i = 0; i < widthUp; i++){
    for(j = 0; j < heightUp; j++){
      p = 0;
      l = 0;
      e = 0;
      fscanf(file, "%d %d %d|", &state, &door, &stair);
      tiles[i][j]->door = door;
      tiles[i][j]->stair = stair;
      destroy_tile(tiles[i][j]);
      
      tiles[i][j] = create_tile(i, j, screen_x_offset + i%width, screen_y_offset + j%height, 0, 0, 0);
      tiles[i][j]->door = door;
      tiles[i][j]->stair = stair;
      
      if(state == 0){
        tiles[i][j]->state[NEW] = EMPTY;
      }else if(state == 1){
        tiles[i][j]->state[NEW] = LOOT;
      }else if(state == 2){
        tiles[i][j]->state[NEW] = PLAYER;
	*player = tiles[i][j];
      }else if(state == 3){
	tiles[i][j]->state[NEW] = ENEMY;
      }else if(state == 4){
	tiles[i][j]->state[NEW] = MONSTER;
      }else if(state == 5){
	tiles[i][j]->state[NEW] = FATAL;
      }
    }
  }  
}

void save_game(tile* tiles[1000][1000], int fCount, int x_off, int y_off, char file_name[24]){
  FILE * file;
  if((file = fopen(file_name, "w")) != NULL){
    int i, j, state;
    int widthUp = 100 + fCount * 50;
    int heightUp = 100 + fCount * 25;
    fprintf(file, "%d:%d:%d:%d|", plyr.health, plyr.points, x_off, y_off, fCount);
    for(i = 0; i < widthUp; i++){
      for(j = 0; j < heightUp; j++){
	if(tiles[i][j]->state[CURRENT] == EMPTY){
	  state = 0;
	}else if(tiles[i][j]->state[CURRENT] == LOOT){
	  state = 1;
	}else if(tiles[i][j]->state[CURRENT] == PLAYER){
	  state = 2;
	}else if(tiles[i][j]->state[CURRENT] == ENEMY){
	  state = 3;
	}else if(tiles[i][j]->state[CURRENT] == MONSTER){
	  state = 4;
	}else if(tiles[i][j]->state[CURRENT] == FATAL){
	  state = 5;
	}
	fprintf(file, "%d %d %d|", state, tiles[i][j]->door, tiles[i][j]->stair);
      }
    }
    fclose(file);
  }
}

int game(void) {
  srand(time(0));  //Seed random numbers
  
  static int state = INIT;
  struct timespec tim = {0,1000000};  // Each execution of while(1) is approximately 1mS
  struct timespec tim_ret;
  
  int width = 50;
  int height = 25;
  int fCount = 0;
  tile * tiles[1000][1000];  //tiles that represent the floor

  tile * player;         //pointer to keep track of the player
  plyr.points = 0;
  plyr.health = 100;
  
  room_t *r; //Used to draw the menu

  int x_offset = 0, y_offset = 0;
  int screen_x_offset, screen_y_offset;
  int x_max, y_max;
  int arrow, i;
  int move_counter = 0;
  int move_timeout = BASE_TIMEOUT/10;
  int test = 0;

  /* Pause menu variables section */
  int optionColor = 0; // used to indicate currently highlighted option
  int menuType = 0; // used to indicate which menu to display
  int savesColor = 1; // used for highlighting the currently selected save option

  const char * savesMenu[10];
  char file_name[24];

  for(i = 0; i < 10; i++){
    sprintf(file_name, "./saves/save_%d.game", (i+1));
    savesMenu[i] = file_name;
  }
  /* End of pause menu variable section */

  /* For creating saves folder if it doesn't exist */
  FILE * file;
  if((file = fopen("./saves", "r")) == NULL)
    system("mkdir saves");
  else
    fclose(file);
  /* End ./saves section */

  while(1) {
    switch(state) {
    case INIT:                 // Initialize the game, only run one time
      
      initscr();
      nodelay(stdscr, TRUE);   // Do not wait for characters using getch.
      noecho();                // Do not echo input characters
      getmaxyx(stdscr, y_max, x_max);  // Get the screen dimensions
      
      curs_set(0);
      
      screen_x_offset = (x_max / 2) - ((width) / 2);
      screen_y_offset = ((y_max / 2) - ((height) / 2)) + 1;
      
      MakeFloor(tiles, width, height, screen_x_offset, screen_y_offset, fCount);
      
      tiles[width/2][height/2]->state[CURRENT] = PLAYER;
      player = tiles[width/2][height/2];

      r = init_room(screen_x_offset - 1, screen_y_offset - 1, width+1, height+1);
      draw_room(r);

      mvprintw(0, 0, "Points: %-3d", plyr.points);
      mvprintw(1, 0, "Health: %-3d", plyr.health);
      
      state = STEP;
      break;
    case STEP:
      if (move_counter > move_timeout) {
	arrow = read_escape(&arrow);
	switch (arrow){
	case (UP):
	  test = move_player(&player, player->x, player->y - 1, width, height, tiles);
	  break;
	case (DOWN):
	  test = move_player(&player, player->x, player->y + 1, width, height, tiles);
	  break;
	case (LEFT):
	  test = move_player(&player, player->x - 1, player->y, width, height, tiles);
	  break;
	case (RIGHT):
	  test = move_player(&player, player->x + 1, player->y, width, height, tiles);
	  break;
	case 'p':
	  savesColor = 1;
	  menuType = 0;
	  state = PAUSE;
	  break;
	case 'q':
          state = EXIT;
          break;
	case 's':
	  savesColor = 1;
	  menuType = 2;
	  state = PAUSE;
	  break;
	case 'l':
	  savesColor = 1;
          menuType = 3;
          state = PAUSE;
          break;
	default:
	  break;
	}

	switch(test){
	case 1:
	  plyr.points++;
	  break;
	case 3: // move up through a door
	  y_offset -= height;
	  break;
	case 4: // move down through a door
	  y_offset += height;
	  break;
	case 5: // move left through a door
	  x_offset -= width;
	  break;
	case 6: // move right through a door
	  x_offset += width;
	  break;
	case 7: // go on stairs
	  MakeFloor(tiles, width, height, screen_x_offset, screen_y_offset, fCount);
	  fCount++;
	  tiles[width/2][height/2]->state[CURRENT] = PLAYER;
	  player = tiles[width/2][height/2];
	  x_offset = 0;
	  y_offset = 0;
	  break;
	default:
	  break;
	}
	test = 0;

	switch(move_enemies(player, x_offset, y_offset, width, height, tiles)){
	case 1:
	  if(plyr.health > 10){
            plyr.health -= 10;
	  }else{
            plyr.health = 0;
	    state = EXIT;
	  }
	  break;
	case 2:
	  if(plyr.health > 25){
            plyr.health -= 25;
          }else{
            plyr.health = 0;
            state = EXIT;
          }
	  break;
	case 3:
	  if(plyr.health > 30){
            plyr.health -= 30;
          }else{
            plyr.health = 0;
            state = EXIT;
          }
	  break;
	default:
	  break;
	}

	update_tiles(x_offset, y_offset, width, height, tiles);
	display_tiles(x_offset, y_offset, width, height, tiles);
	mvprintw(0, 0, "Points: %d", plyr.points);
        mvprintw(1, 0, "Health: %-3d", plyr.health);

	while(getch() != ERR);  //prevents lag from having getch and sleep in a loop
	move_counter = 0;
      } 
      move_counter++;
      break;
    case PAUSE: // case for the pause menu also stops the progression of the ingame time      
      if(menuType == 1){
	arrow = read_escape(&arrow);
        if(arrow == 0){
	  draw_help(r, screen_x_offset + 2, screen_y_offset + (height / 2) - 5);
	}else{
	  menuType = 0;
	}
	arrow = 0;
      }else if(menuType == 2){
	draw_saves(r, screen_x_offset + 2, screen_y_offset + (height / 2) - 6, 1, savesColor);
	arrow = read_escape(&arrow);
        switch (arrow){
        case (UP):
          if(savesColor > 1)
            savesColor--;
          break;
        case (DOWN):
          if(savesColor < 11)
            savesColor++;
          break;
	case (ENTER):
	  if(savesColor <= 10){
	    sprintf(file_name, "./saves/save_%d.game", savesColor);
	    save_game(tiles, fCount, x_offset, y_offset, file_name);
	  }
	  savesColor = 1;
	  menuType = 0;
	  break;
	default:
	  break;
	}
      }else if(menuType == 3){
	draw_saves(r, screen_x_offset + 2, screen_y_offset + (height / 2) - 6, 0, savesColor);
        arrow = read_escape(&arrow);
        switch (arrow){
        case (UP):
          if(savesColor > 1)
            savesColor--;
          break;
        case (DOWN):
          if(savesColor < 11)
            savesColor++;
          break;
        case (ENTER):
	  if(savesColor <= 10){
	    sprintf(file_name, "./saves/save_%d.game", savesColor);
	    if((file = fopen(file_name, "r")) != NULL){
	      init_game(file, tiles, &fCount, &x_offset, &y_offset, &player, screen_x_offset, screen_y_offset);
	      update_tiles(x_offset, y_offset, width, height, tiles);
	    }
	  }
	  savesColor = 1;
	  menuType = 0;
          break;
        default:
          break;
        }
      }else if(menuType == 0){
	draw_menu(r, screen_x_offset + 2, screen_y_offset + (height / 2) - 5, optionColor);
	arrow = read_escape(&arrow);
        switch (arrow){
        case (UP):
          if(optionColor > 0)
            optionColor--;
          break;
        case (DOWN):
          if(optionColor < 4)
            optionColor++;
          break;
        case (ENTER):
          if(optionColor == 0){
	    draw_room(r);
	    state = STEP;
	  }else if(optionColor == 1){
	    menuType = 2;
	  }else if(optionColor == 2){
	    menuType = 3;
          }else if(optionColor == 3){
	    menuType = 1;
          }else if(optionColor == 4){
	    state = EXIT;
          }
	  break;
        default:
          break;
        }
      }
      break;
    case EXIT:
      arrow = read_escape(&arrow);
      if(arrow == 0 && plyr.health == 0){
	clear();
	mvprintw((y_max/2), (x_max/2) - 5, "GAME OVER!");
	mvprintw((y_max/2) + 1, (x_max/2) - 5, "Score %d", plyr.points); 
      }else{
	endwin();
	system("clear");
	return(0);
      }
      break;
    }
    refresh();
    nanosleep(&tim,&tim_ret);
  }
}
