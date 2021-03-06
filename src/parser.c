/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/
/* This file contains line parse functionality. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"
#include "command.h"
#include "color.h"
#include "convert.h"

roloc_hsv calculate_complement(char *keycolor);
roloc_hsv calculate_splitcomplement(char *keycolor, int offset, int dirn);

/*! Find arithmetic in a provided line and call the appropriate blend function.
 *  Variable names in the line are sought for in color.txt.
 *  Conversion happens between hex to rgb with the final result being a hex
 *  value.
*/
char *find_arithmetic(char *line)
{

  char linecopy[strlen(line)+1];
  strcpy(linecopy, line);

  char *nosupport = strpbrk(linecopy, "`~!@#$%^&*()-_=[{]}\\|';:'\",<.>/?");

  if(nosupport) {
    return NULL;
  } else if(strpbrk(linecopy, "+")) {

    /* additive blend functionality */

    int  count = 0;

    float red   = 0.0;
    float green = 0.0;
    float blue  = 0.0;


    char *token = strtok(linecopy, "+ ");

    while(token) {

      char *color = find_color(token, RLC_PATH, 1);

      if(color == NULL) {
        token = strtok(NULL, "+ ");
      } else {
 
        red   = red + hex_to_rgb(color[0], color[1]);
        green = green + hex_to_rgb(color[2], color[3]);
        blue  = blue + hex_to_rgb(color[4], color[5]);

        count = count+1;
        token = strtok(NULL, "+ ");
      }
    }
 
    red = red ? red/count : 0.0;
    green = green ? green/count : 0.0;
    blue = blue ? blue/count : 0.0;


    char result[7];

    result[0] = '\0';
    strcat(result, rgb_to_hex(red));
    strcat(result, rgb_to_hex(green));
    strcat(result, rgb_to_hex(blue));

    printf("%s\n", result);

    roloc_rgb val;

    val.red = red;
    val.green = green;
    val.blue = blue;

    r_set_last_color(&val, 1);

    return result;
  }

  return NULL;
}


/*! Determine if line is a program request and call the appropriate
 *  request handle function.
 *  Possible request lines are "red to blue 5" or "repr rgb".
 */
void *find_request(char *line)
{

  char linecopy[strlen(line)+1];
  strcpy(linecopy, line);

  /* color to color gradient functionality */

  char *color   = NULL;
  char *color_1 = NULL;
  char *color_2 = NULL;

  char *token = strtok(linecopy, " ");

  if(token && (color = find_color(token, RLC_PATH, 1))) {

    color_1 = malloc(strlen(color)+1);
    strcpy(color_1, color);
 
    token = strtok(NULL, " ");

    if(token && (strcmp(token, "to") == 0)) {

      token = strtok(NULL, " ");

      if(token && (color = find_color(token, RLC_PATH, 1))) {

        color_2 = malloc(strlen(color)+1);
        strcpy(color_2, color);

        token = strtok(NULL, " ");

        if(token && atoi(token)) {

          roloc_rgb *store;
          store = calculate_gradient(color_1, color_2, atoi(token));

          free(color_1);
          free(color_2);
          color_1 = NULL;
          color_2 = NULL;

          int amount = atoi(token);
          amount = amount + 2;

          token = strtok(NULL, " ");

          r_set_last_color(store, amount);

          if(token && (strcmp(token, "img") == 0)) {
            export_linear_gradient_image(store, amount, token);
          }
        }
      }
    }
  } else if(token) {
    /* rgb repr. functionality */
    if(strcmp(token, "rgb") == 0) {
      token = strtok(NULL, " ");
      if(token && (color = find_color(token, RLC_PATH, 1))) {

          float red   = hex_to_rgb(color[0], color[1]);
          float green = hex_to_rgb(color[2], color[3]);
          float blue  = hex_to_rgb(color[4], color[5]);

        printf("rgb(%.0f, %.0f, %.0f)\n", red*255, green*255, blue*255); 
      }
    }
    /* hsv repr. functionality */
    if(strcmp(token, "hsv") == 0) {
      token = strtok(NULL, " ");
      if(token && (color = find_color(token, RLC_PATH, 1))) {

        float red   = hex_to_rgb(color[0], color[1]);
        float green = hex_to_rgb(color[2], color[3]);
        float blue  = hex_to_rgb(color[4], color[5]);

        roloc_hsv o = rgb_to_hsv(red, green, blue);

        printf("hsv(%d, %.2f, %.2f)\n", (int) o.hue, o.saturation, o.value);
     }
   } else if(strcmp(token, "complement") == 0 ||
             strcmp(token, "complementary") == 0) {
      /* complement color calculation functionality */
      token = strtok(NULL, " ");
      if(token && (color = find_color(token, RLC_PATH, 1))) {
        roloc_rgb c = hsv_to_rgb(calculate_complement(color));

        char hex[7];
        hex[0] = '\0';
        strcat(hex, rgb_to_hex(c.red/255));
        strcat(hex, rgb_to_hex(c.green/255));
        strcat(hex, rgb_to_hex(c.blue/255));

        printf("%s\n", hex);
        r_set_last_color(&c, 1);
      }
    } else if(strcmp(token, "split") == 0) {
      /* triad color calculation functionality */
      token = strtok(NULL, " ");
      if(token && (color = find_color(token, RLC_PATH, 1))) {

        roloc_rgb c  = hsv_to_rgb(calculate_splitcomplement(color, 120, 0));
        roloc_rgb c2 = hsv_to_rgb(calculate_splitcomplement(color, 120, 1));

        char hex[7];
        hex[0] = '\0';
        strcat(hex, rgb_to_hex(c.red/255));
        strcat(hex, rgb_to_hex(c.green/255));
        strcat(hex, rgb_to_hex(c.blue/255));

        char hex2[7];
        hex2[0] = '\0';
        strcat(hex2, rgb_to_hex(c2.red/255));
        strcat(hex2, rgb_to_hex(c2.green/255));
        strcat(hex2, rgb_to_hex(c2.blue/255));

        printf("%s %s\n", hex, hex2);

        roloc_rgb rgb_array[2];
        rgb_array[0] = c;
        rgb_array[1] = c2;
        r_set_last_color(rgb_array, 2);
      }
    } else if(strcmp(token, "cmy") == 0) {
      token = strtok(NULL, " ");
      if(token && (color = find_color(token, RLC_PATH, 1))) {

        roloc_rgb o;

        o.red   = hex_to_rgb(color[0], color[1]);
        o.green = hex_to_rgb(color[2], color[3]);
        o.blue  = hex_to_rgb(color[4], color[5]);

        roloc_cmy result = rgb_to_cmy(o);

        printf("%.2f%, %.2f%, %.2f%\n", result.c*100, result.m*100,
                                         result.y*100);

        r_set_last_color(&o, 1);
      }
    }
  }

  return NULL;
}
