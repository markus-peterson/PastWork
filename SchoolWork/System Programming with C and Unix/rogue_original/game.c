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

#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "tile.h"
#include "game.h"
#include "room.h"
#include "key.h"


//function the fill the array with tiles to make a floor
void MakeFloor(tile* tiles[100][100], int width, int height, int screen_x_offset, int screen_y_offset){
	int i, j;
	for(i = 0; i < 100; i++){
		for(j = 0; j < 100; j++){
			tiles[i][j] = create_tile(i, j, screen_x_offset + i%width, screen_y_offset + j%height, 0, 0, 0);

			if(j%height == 0 && j != 0 && rand()%height == 1){
				tiles[i][j]->door = 1;
				tiles[i][j-1]->door = 1;
			}else if(i%width == 0 && i != 0 && rand()%width == 1){
				tiles[i][j]->door = 1;
				tiles[i-1][j]->door = 1;
			}
		}
	}

	tiles[rand()%100][rand()%100]->stair = 1;
	tiles[width/2][height/2]->state[CURRENT] = PLAYER;
}

void init_game(void)
{
	int x,y;
}

int game(void)
{
	srand(time(0));  //Seed random numbers

	static int state = INIT;
	struct timespec tim = {0,1000000};  // Each execution of while(1) is approximately 1mS
	struct timespec tim_ret;

	int width = 50;
	int height = 25;

	tile * tiles[100][100];  //tiles that represent the floor
	tile * player;           //pointer to keep track of the player

	room_t *r;

	int x_offset=0, y_offset=0;
	int screen_x_offset, screen_y_offset;
	int x_max, y_max;
	int arrow;
	int move_counter = 0;
	int move_timeout = BASE_TIMEOUT/10;

	int test = 0;

	while(1) {
		switch(state) {
		case INIT:                   // Initialize the game, only run one time


			initscr();
			nodelay(stdscr, TRUE);   // Do not wait for characters using getch.
			noecho();                // Do not echo input characters
			getmaxyx(stdscr, y_max, x_max);  // Get the screen dimensions
			
			curs_set(0);

			screen_x_offset = (x_max / 2) - ((width) / 2);
			screen_y_offset = (y_max / 2) - ((height) / 2);

			MakeFloor(tiles, width, height, screen_x_offset, screen_y_offset);

			tiles[width/2][height/2]->state[CURRENT] = PLAYER;
			player = tiles[width/2][height/2];



			r = init_room(screen_x_offset - 1, screen_y_offset - 1, width+1, height+1);
			draw_room(r);

			state = STEP;
			break;
		case STEP:
			

			if (move_counter > move_timeout) {
				

				
				if(move_enemies(player, x_offset, y_offset, width, height, tiles))
					state = EXIT;

				draw_room(r);
				update_tiles(x_offset, y_offset, width, height, tiles);
				display_tiles(x_offset, y_offset, width, height, tiles);
				
				while(getch() != ERR);  //prevents lag from having getch and sleep in a loop

				move_counter = 0;
			}
			move_counter++;
			break;
		case EXIT:
			endwin();
			return(0);
			break;
		}
		refresh();
		nanosleep(&tim,&tim_ret);
	}
}
