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

#include "parser.h"
#include "command.h"
#include "color.h"
#include "convert.h"
#include "calculate.h"
#include "graphics.h"


/*! Find arithmetic in a provided line and call the appropriate blend function.
 *  Variable names in the line are sought for in color.txt.
 *  Conversion happens between hex to rgb with the final result being a hex
 *  value.
*/
char* find_arithmetic(char *line)
{

  char linecopy[strlen(line)+1];
  strcpy(linecopy, line);

  char *nosupport = strpbrk(linecopy, "`~!@#$%^&*()-_=[{]}\\|';:'\",<.>/?");

  if(nosupport) {

    printf("Currently only additive blending with the + operand is supported: "
           "%s\n", linecopy); 
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
 
    red = red/count;
    green = green/count;
    blue = blue/count;


    char result[7];

    result[0] = '\0';
    strcat(result, rgb_to_hex(red));
    strcat(result, rgb_to_hex(green));
    strcat(result, rgb_to_hex(blue));

    printf("%s\n", result);

    set_last_color(result);

    return result;
  }

  return NULL;
}


/*! Determine if line is a program request and call the appropriate
 *  request handle function.
 *  Possible request lines are "red to blue 5" or "repr rgb".
 */
void* find_request(char *line)
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

    if(token && (strcmp(token, "TO") == 0)) {

      token = strtok(NULL, " ");

      if(token && (color = find_color(token, RLC_PATH, 1))) {

        color_2 = malloc(strlen(color)+1);
        strcpy(color_2, color);

        token = strtok(NULL, " ");

        if(token && atoi(token)) {

          GradientColor *store;
          store = gradient(color_1, color_2, atoi(token));

          free(color_1);
          free(color_2);
          color_1 = NULL;
          color_2 = NULL;

          int amount = atoi(token);

          token = strtok(NULL, " ");

          if(token && (strcmp(token, "IMG") == 0)) {

            token = strtok(NULL, " ");
            int width = (token && atoi(token)) ? width = atoi(token) : 200;

            token = strtok(NULL, " ");
            int height = (token && atoi(token)) ? height = atoi(token) : 200;


            cairo_surface_t *surface = cairo_image_surface_create(
              CAIRO_FORMAT_ARGB32, width, height
            );

            cairo_t *cr = cairo_create(surface);
            cairo_scale(cr, width, height);

            double coverage = (1.0/(amount+2));
            int itr;
            for(itr = 0; itr < amount+2; itr++) {

              cairo_set_source_rgb(
                cr,
                store[itr].red,
                store[itr].green,
                store[itr].blue
              );

              cairo_rectangle(cr, 0.0+(coverage*itr), 0.0, coverage, 1.0);
              cairo_fill(cr);
            }

            cairo_destroy(cr);

            token = strtok(NULL, " ");
            char *filename = (token) ? token : "gradient.png";

            cairo_surface_write_to_png(surface, filename);
            cairo_surface_destroy(surface);
          }
        }
      }
    }
  }

  return NULL;
}
