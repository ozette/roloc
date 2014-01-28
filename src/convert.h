/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

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

float hex_to_rgb(char digit, char digit2);
char *rgb_to_hex(float value);

roloc_hsv rgb_to_hsv(float red, float green, float blue);
roloc_rgb hsv_to_rgb(roloc_hsv object);
