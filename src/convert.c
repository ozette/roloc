/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/
/* This file contains all convert and calculate functionality of the program. */

#include <math.h>
#include "convert.h"

#define HEX       16
#define RGB_MAX   255.0


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
