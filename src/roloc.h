/*
* roloc: a color cook program, color calculator and palette generator.
*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#ifndef ROLOC_H_
#define ROLOC_H_
typedef struct roloc_rgb {
  float red;
  float green;
  float blue;
} roloc_rgb;

typedef struct roloc_hsv {
  float hue;
  float saturation;
  float value; 
} roloc_hsv;
#endif
