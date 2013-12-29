/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/
/* This file contains line parse functionality. */

#include <stdio.h>
#include <matheval.h>
#include <string.h>

#include "parser.h"
#include "command.h"
#include "color.h"
#include "convert.h"


/*! Find arithmetic in a provided line and call the appropriate blend function.
 *  Variable names in the line are sought for in color.txt.
 *  Conversion happens between hex to rgb with the final result being a hex
 *  value.
*/
char* find_arithmetic(char *line)
{
  char *nosupport = strpbrk(line, "`~!@#$%^&*()-_=[{]}\\|';:'\",<.>/?");

  if(nosupport) {

    printf("Currently only additive blending with the + operand is supported: "
           "%s\n", line); 
    return NULL;

  } else if(strpbrk(line, "+")) {

    /* additive blend functionality */

    void *expression;
    expression = evaluator_create(line);

    char **names;
    int  count;
    int  itr;

    evaluator_get_variables(expression, &names, &count);

    float red   = 0.0;
    float green = 0.0;
    float blue  = 0.0;


    for(itr = 0; itr < count; itr++) {
      char* color = find_color(names[itr], RLC_PATH, 1);
 
      red   = red + hex_to_rgb(color[0], color[1]);
      green = green + hex_to_rgb(color[2], color[3]);
      blue  = blue + hex_to_rgb(color[4], color[5]);
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

    evaluator_destroy(expression);

    return result;
  }

  return NULL;
}
