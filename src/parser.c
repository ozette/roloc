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

    if(token && (strcmp(token, "to") == 0)) {

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

          if(token && (strcmp(token, "img") == 0)) {

            int horizontal = 0;
            int width      = 200;
            int height     = 200;
            char *filename = "gradient.png";

            token = strtok(NULL, " ");
            if(token && (strcmp(token, "h") == 0)) {
              horizontal = 1;
              token = strtok(NULL, " ");
            }

            if(token && atoi(token)) {
              width = atoi(token);
              token = strtok(NULL, " ");
            }

            if(token && atoi(token)) {
              height = atoi(token);
              token = strtok(NULL, " ");
            }

            if(token) {
              filename = token;
            }

            cairo_surface_t *surface = cairo_image_surface_create(
              CAIRO_FORMAT_ARGB32, width, height
            );

            cairo_t *cr = cairo_create(surface);

            cairo_pattern_t *pat;

            if(horizontal) {
              pat = cairo_pattern_create_linear(0.0, 0.0, 0.0, height);
            } else {
              pat = cairo_pattern_create_linear(0.0, 0.0, width, 0.0);
            }

            double coverage = (1.0/(amount+2));

            int itr;
            for(itr = 0; itr < amount+2; itr++) {
              cairo_pattern_add_color_stop_rgba(
                pat,
                coverage*itr,
                store[itr].red,
                store[itr].green,
                store[itr].blue,
                1
              );
            }

            cairo_rectangle(cr, 0, 0, width, height);
            cairo_set_source(cr, pat);
            cairo_fill(cr);
            cairo_destroy(pat);
            cairo_destroy(cr);

            cairo_surface_write_to_png(surface, filename);
            cairo_surface_destroy(surface);
          }
        }
      }
    }
  }

  return NULL;
}
