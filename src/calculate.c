/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/
/* This file contains calculate functionality of the program in conjunction
 * with convert.c */

#include <stdio.h>
#include <stdlib.h>

#include "calculate.h"
#include "convert.h"


/*! Create a gradient between two colors. The amount of colors in the gradient
 *  is determined by the value of amount. 
 */
roloc_color_t* gradient(char *color1, char *color2, int amount)
{
  int listsize = amount+2;

  roloc_color_t *store = malloc(sizeof(roloc_color_t)*listsize);

  store[0] = (roloc_color_t) {
    hex_to_rgb(color1[0], color1[1]),
    hex_to_rgb(color1[2], color1[3]),
    hex_to_rgb(color1[4], color1[5])
  };

  store[listsize-1] = (roloc_color_t) {
    hex_to_rgb(color2[0], color2[1]),
    hex_to_rgb(color2[2], color2[3]),
    hex_to_rgb(color2[4], color2[5])
  };
 
  double full = 100.0;
  double divide = ((full/listsize) / 100); /*!< percentage per color */


  char firstvalue[7];
  firstvalue[0] = '\0';
  strcat(firstvalue, rgb_to_hex(store[0].red));
  strcat(firstvalue, rgb_to_hex(store[0].green));
  strcat(firstvalue, rgb_to_hex(store[0].blue));

  printf("%s ", firstvalue);

  int pos = 1;
  int itr;
  for(itr = amount; itr > 0; itr--) {

    store[pos].red = (store[0].red * (divide*(itr)) )
                   + (store[listsize-1].red *(1 - (divide*(itr))));

    store[pos].green = (store[0].green * (divide*(itr)) )
                     + (store[listsize-1].green *(1 - (divide*(itr))));

    store[pos].blue = (store[0].blue * (divide*(itr)) )
                    + (store[listsize-1].blue *(1 - (divide*(itr))));

    char value[7];
    value[0] = '\0';
    strcat(value, rgb_to_hex(store[pos].red));
    strcat(value, rgb_to_hex(store[pos].green));
    strcat(value, rgb_to_hex(store[pos].blue));

    printf("%s ", value);
    pos = pos+1;
  }

  char lastvalue[7];
  lastvalue[0] = '\0';
  strcat(lastvalue, rgb_to_hex(store[listsize-1].red));
  strcat(lastvalue, rgb_to_hex(store[listsize-1].green));
  strcat(lastvalue, rgb_to_hex(store[listsize-1].blue));

  printf("%s\n", lastvalue);

  return store;
}
