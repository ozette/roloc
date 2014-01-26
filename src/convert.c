/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/
/* This file contains all convert functionality of the program. */

#include <math.h>
#include "convert.h"

#define HEX       16
#define RGB_MAX   255.0


const char repr[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                     'A', 'B', 'C', 'D', 'E', 'F'};


/*! Converts a pair of hex characters to a floating-point rgb representation
 *  between 0.0 and 1.0. E.g. FF becomes 1.0 */
float hex_to_rgb(char hexchar, char hexchar2)
{
  /* convert charactrs to int */
  int head = hexchar;
  int tail = hexchar2;

  /* if yields A-F, else yields 0-9 (ASCII repr.) */
  if(head > 57) {
    head = head - ('0')-7;
  } else {
    head = head - '0';
  }
  head = head* ((int) pow(HEX, 1));

  if(tail > 57) {
    tail = tail - ('0')-7;
  } else {
    tail = tail - '0';
  }
  tail = tail* ((int) pow(HEX, 0));

  float rgb_component = ((head+tail)/RGB_MAX);

  return rgb_component;
}


/*! Converts a rgb value between 0.0 and 1.0 to its hexadecimal representation.
 *  E.g. 1.0 becomes FF */
char *rgb_to_hex(float value)
{
  int val = (int) (value*RGB_MAX);

  char hex_component[3];

  hex_component[0] = repr[((val-(val%HEX))/HEX)]; 
  hex_component[1] = repr[(val%HEX)];
  hex_component[2] = '\0';

  return hex_component;
}


/*! Converts a HSV representation to its rgb equivalent. */
roloc_rgb hsv_to_rgb(HSV object)
{
  roloc_rgb color = {0.0, 0.0, 0.0};

  /* grayscale color */
  if(object.saturation == 0) {
    color.red = color.green = color.blue = object.value;
    return color; 
  }

  object.hue = object.hue/60;

  int i = floor(object.hue);
  float factorial = object.hue - i;

  float p = object.value * (1 - object.saturation);
  float q = object.value * (1 - object.saturation * factorial);
  float t = object.value * (1 - object.saturation * (1 - factorial)); 

  switch(i) {
    case 0: color = (roloc_rgb) {object.value, t, p}; break;
    case 1: color = (roloc_rgb) {q, object.value, p}; break;
    case 2: color = (roloc_rgb) {p, object.value, t}; break;
    case 3: color = (roloc_rgb) {p, q, object.value}; break;
    case 4: color = (roloc_rgb) {t, p, object.value}; break;
    case 5: color = (roloc_rgb) {object.value, p, q}; break;
  }

  color.red   = color.red*255;
  color.green = color.green*255;
  color.blue  = color.blue*255;

  printf("rgb( %d, %d, %d)\n", (int) color.red, (int) color.green,
                               (int) color.blue);

  return color;
}


/*! Converts a rgb value to its HSV representation
 *  red, green and blue arguments must each be in the range of 0 and 1 */
HSV rgb_to_hsv(float red, float green, float blue)
{ 
  HSV computed;
  float minimum;
  float maximum;

  minimum = red;
  if(green < red) {
    minimum = green;
    if(green > blue ) {
      minimum = blue;
    }
  }
  if(blue < red) {
    minimum = blue;
  }

  maximum = red;
  if(green > red) {
    maximum = green;
    if(blue > green) {
      maximum = blue;
    }
  } 
  if(blue > red) {
    maximum = blue;
  }

  computed.value = maximum;
  float delta = maximum - minimum;

  /* grayscale color */
  if(maximum == minimum) {
    computed.hue = 0;
    computed.saturation = 0;

    printf("hsv(%d, %f, %f)\n", (int) computed.hue, computed.saturation,
                                computed.value);
    return computed;
  }

  computed.saturation = delta/maximum;

  if(red == maximum) {
    computed.hue = (green - blue) / delta;
  } else if(green == maximum) {
    computed.hue = 2 + (blue - green) / delta;
  } else {
    computed.hue = 4 + (red - blue) / delta;
  }

  computed.hue = computed.hue * 60;
  if(computed.hue < 0) {
    computed.hue = computed.hue + 360;
  }

  printf("hsv(%d, %f, %f)\n", (int) computed.hue, computed.saturation,
                            computed.value);
  return computed;
}
