/* key.c ---------
 *
 * Filename: key.c
 * Description: Controls all key inputs
 * Author: Bryce Himebaugh
 * Maintainer:
 * Created: Thu Sep 15 16:35:07 2017
 * Last-Updated: September 23 15:38 2018
 *           By: Dmitrii Galantsev
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

#include <ncurses.h>
#include "key.h"

int read_escape(int *read_char) {
  int c;
  if ((c = getch()) == ERR) {
    return (NOCHAR);
  } else if (c==0x1b) {
    if ((c = getch()) == '[') {
      c=getch();
      switch (c) {
      case 'A':
	return (UP);
	break;
      case 'B':
	return (DOWN);
	break;
      case 'C':
	return (RIGHT);
	break;
      case 'D':
	return (LEFT);
	break;
      default:
	return (BADESC);
      }
    }
  } else if(c==0xA){
    return (ENTER);
  }else {
    *read_char = c;
    switch(c){
    case 'p':
      c = 'p';
      break;
    case 'P':
      c = 'P';
      break;
    case 'l':
      c = 'l';
      break;
    case 'L':
      c = 'l';
      break;
    case 'q':
      c = 'q';
      break;
    case 'Q':
      c = 'q';
      break;
    case 's':
      c = 's';
      break;
    case 'S':
      c = 's';
      break;
    default:
      c = (REGCHAR);
      break;
    }
    return c;
  }
}
