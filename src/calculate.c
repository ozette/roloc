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


/*! Create a gradient between two colors. The amount of colors in the gradient
 *  is determined by the value of amount. 
 */
roloc_rgb *calculate_gradient(char *color1, char *color2, int amount)
{
  int listsize = amount+2;

  roloc_rgb *store = malloc(sizeof(roloc_rgb)*listsize);

  store[0] = (roloc_rgb) {
    hex_to_rgb(color1[0], color1[1]),
    hex_to_rgb(color1[2], color1[3]),
    hex_to_rgb(color1[4], color1[5])
  };

  store[listsize-1] = (roloc_rgb) {
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


/*! Calculate the complementary color of the provided keycolor 
 *  char *keycolor must be a hexidecimal value e.g FF0000
 */
roloc_hsv calculate_complement(char *keycolor)
{
  float r = hex_to_rgb(keycolor[0], keycolor[1]);
  float g = hex_to_rgb(keycolor[2], keycolor[3]);
  float b = hex_to_rgb(keycolor[4], keycolor[5]);

  roloc_hsv key_color = rgb_to_hsv(r, g, b);

  roloc_hsv complementary_color = {
    key_color.hue+180 > 360 ? key_color.hue+180-360 : key_color.hue+180,
    key_color.saturation,
    key_color.value
  };

  return complementary_color;
}


/*! \brief Calculate the split-complement of the provided key color.
 *
 *  \param keycolor must be a hexidecimal value e.g FF0000.
 *  \param offset must be an integer value between 0 and 360.
 *  \param dirn the direction of the triad based on the offset from the
 *         keycolor. 1 is left and 0 is right. 
 */
roloc_hsv calculate_splitcomplement(char *keycolor, int offset, int dirn)
{
  float r = hex_to_rgb(keycolor[0], keycolor[1]);
  float g = hex_to_rgb(keycolor[2], keycolor[3]);
  float b = hex_to_rgb(keycolor[4], keycolor[5]);

  roloc_hsv key_color = rgb_to_hsv(r, g, b);

  roloc_hsv split_color;

  if(dirn) {
    split_color = (roloc_hsv) {
      key_color.hue+offset > 360 ? key_color.hue+offset-360 :
                                   key_color.hue+offset,
      key_color.saturation,
      key_color.value
    };
  } else {
    split_color = (roloc_hsv) {
      key_color.hue-offset < 0 ? key_color.hue-offset+360 :
                                 key_color.hue-offset,
      key_color.saturation,
      key_color.value
    };
  }
  return split_color;
}
